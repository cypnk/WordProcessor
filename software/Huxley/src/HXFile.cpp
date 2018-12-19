#ifndef HXFILE_CPP
#define HXFILE_CPP

#include "headers/HXFile.h"

HXFile::HXFile() { }

void
HXFile::extractLine(
	std::size_t&	chk,
	std::string&	line,
	std::string&	extracted
) {
	std::istringstream str( line );
	std::string block;
	int i = 0;
	
	while( std::getline( str, block, ' ' ) ) {
		// Checksum
		if ( i == 0 ) {
			// Copy checksum
			std::sscanf( block.c_str(), "%zx", &chk );
		
		// TODO: Extract formatting
		//} else if ( i == 1 ) {
			
		} else {
			extracted.append( block + ' ' );
		}
		i++;
	}
	// Remove lastly appened space
	if ( extracted.length() > 1 ) {
		extracted.erase( extracted.length() - 1 );
	}
}


void
HXFile::saveDoc( std::string& fname, HX_FILE&source ) {
	std::ofstream file( fname.c_str() );
	if ( !file.is_open() || !file.good() ) {
		return;
	}
	
	// Checksum placeholder (slightly over-provisioned)
	char check[25];	
	for (
		std::vector<HX_LINE>::iterator it = 
			source.data.begin(); 
		it != source.data.end(); 
		++it
	) {
		snprintf( check, 25, "%zx ", (*it).chk );
		
		// TODO: Append formatting data
		file << check << (*it).line.c_str() << "\n";
	}
	
	file.close();
}

void
HXFile::openDoc( std::string& fname, HX_FILE& dest ) {
	const char* fcmp	= fname.c_str();
	
	// Setup working file
	std::ifstream file( fcmp );
	if ( !file.is_open() || !file.good() ) {
		return;
	}
	
	std::string line;
	// Detect file type and load Huxley documents differently
	if ( ENDS_WITH( fcmp, ".txt" ) ) {
		// Append lines to document block
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, FILE_TEXT );
			line.clear();
		}
		
	} else if ( ENDS_WITH( fcmp, ".hx" ) ) {
		// Append Huxley line to document block
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, FILE_HUXLEY );
			line.clear();
		}
	} else {
		// Shouldn't handle yet
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, FILE_UNKOWN );
			line.clear();
		}
		
	}
	
	file.close();
	if ( !file ) {
		// Error
		return;
	}
}

void
HXFile::appendDoc(
	std::string&	line, 
	HX_FILE& 	dest,
	unsigned char	ftype
) {
	// TODO: Make this read formatting from the line
	std::vector<HX_FORMAT>	fmt;
	std::size_t		chk;
	
	switch( ftype ) {
		case FILE_HUXLEY: {
			std::string extracted	= "";
			
			// Extract line and checksum
			extractLine( chk, line, extracted );
			
			// TODO: Extract line formatting
			fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
			
			// Add line with any formatting
			// and if given and calculated checksums match
			dest.data.push_back( HX_LINE {
				chk,
				chk == std::hash<std::string>{}( extracted ),
				extracted, 
				fmt
			} );
			break;
		} 
		
		default: {
			// Fresh checksum
			chk	= std::hash<std::string>{}( line );
			
			// Empty formatting
			fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
			
			// Add line
			dest.data.push_back( HX_LINE { 
					chk, true, line, fmt 
			} );
			break;
		}
	}
}

#endif
