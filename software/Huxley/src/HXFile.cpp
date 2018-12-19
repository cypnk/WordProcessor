#ifndef HXFILE_CPP
#define HXFILE_CPP

#include "headers/HXFile.h"

HXFile::HXFile() { }

/**
 *  Copy from string in file to formatting
 */
void
HXFile::extractFormat(
	std::string& block,
	std::vector<HX_FORMAT>& fmt 
) {
	char tmp[FMT_SIZE];	// String chunk size
	char* raw;		// Raw formatting chunk
	
	// Format data
	Sint32		start	= 0;
	std::size_t	length	= 0;
	char		type[1];
	
	// Default, no formatting
	unsigned char	format	= F_NORMAL;
	
	// Extract from chunk
	snprintf( tmp, FMT_SIZE, "%s", block.c_str() );
	
	// Tokenize
	raw = strtok( tmp, FMT_DELIM );
	
	while( raw != NULL ) {
		sscanf( raw, FMT_FORMAT, type, &start, &length );
		
		// Select formatting by type
		switch( type[0] ) {
			case 'b': { // Bold
				format = F_BOLD;
				break;
			}
			case 'i': { // Italic
				format = F_ITALIC;
				break;
			}
			case 'u': { // Underlined
				format = F_UNDER;
				break;
			}
			case 'p': { // Superscript
				format = F_SUP;
				break;
			}
			case 's': { // Subscript
				format = F_SUB;
				break;
			}
		}
		fmt.push_back( HX_FORMAT { 
			start, length, format
		} );
		raw =  strtok( NULL, FMT_DELIM );
	}
}

void
HXFile::saveFormat(
	std::string& block,
	std::vector<HX_FORMAT>& fmt 
) {
	// Formatting chunk
	char tmp[FMT_SIZE];
	char type;
	
	for (
		std::vector<HX_FORMAT>::iterator it = 
			fmt.begin(); 
		it != fmt.end(); 
		++it
	) {
		switch( (*it).type ) {
			case F_BOLD: {
				type = 'b';
				break;
			}
			case F_ITALIC: {
				type = 'i';
				break;
			}
			case F_UNDER: {
				type = 'u';
				break;
			}
			case F_SUP: {
				type = 'p';
				break;
			}
			case F_SUB: {
				type = 's';
				break;
			}
			default: {
				type = 'n';
			}
		}
		
		snprintf( tmp, FMT_SIZE, FMT_FORMAT, 
			type, (*it).start, (*it).length );
		block.append( tmp );
		tmp[0] = '\0';
		
		// Not the end?
		if ( std::next( it ) != fmt.end() ) {
			block.append( "|" );
		}
	}
}

void
HXFile::extractLine(
	std::size_t&		chk,
	std::string&		line,
	std::string&		extracted,
	std::vector<HX_FORMAT>& fmt
) {
	std::istringstream str( line );
	std::string block;
	int i = 0;
	
	while( std::getline( str, block, ' ' ) ) {
		// Checksum
		if ( i == 0 ) {
			// Copy checksum
			FROM_CHK( block, chk );
		
		// Copy formatting
		} else if ( i == 1 ) {
			extractFormat( block, fmt );
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
	
	std::string block;
	// Checksum placeholder (slightly over-provisioned)
	char check[CHK_SIZE];	
	for (
		std::vector<HX_LINE>::iterator it = 
			source.data.begin(); 
		it != source.data.end(); 
		++it
	) {
		// Copy line checksum
		COPY_CHK( check, (*it).chk );
		
		// Append formatting data
		saveFormat( block, (*it).format );
		
		file << check << " " << block.c_str() << " " <<
			(*it).line.c_str() << "\n";
		
		block.clear();
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
	
	// Line index
	std::size_t	index	= 0;
	
	std::string line;
	// Detect file type and load Huxley documents differently
	if ( ENDS_WITH( fcmp, ".txt" ) ) {
		// Append lines to document block
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, ++index, FILE_TEXT );
			line.clear();
		}
		
	} else if ( ENDS_WITH( fcmp, ".hx" ) ) {
		// Append Huxley line to document block
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, ++index, FILE_HUXLEY );
			line.clear();
		}
	} else {
		// Shouldn't handle yet
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, ++index, FILE_UNKOWN );
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
	std::size_t	index,
	unsigned char	ftype
) {
	// TODO: Make this read formatting from the line
	std::vector<HX_FORMAT>	fmt;
	std::size_t		chk;
	bool			passed	= true;
	
	switch( ftype ) {
		
		// Huxley file
		case FILE_HUXLEY: {
			std::string extracted	= "";
			
			// Extract line, checksum, and formatting
			extractLine( chk, line, extracted, fmt );
			
			// Integrity check
			passed			= 
			chk == TO_CHK( extracted );
			
			// Integrity check failed at any point?
			if ( !passed ) {
				good = false;
				bad_lines.push_back( index );
			}
			
			// Add line with any formatting
			// and if given and calculated checksums match
			dest.data.push_back( HX_LINE {
				chk,
				passed,
				index,
				extracted, 
				fmt
			} );
			break;
		} 
		
		default: {
			// Fresh checksum
			chk	= TO_CHK( line );
			
			// Empty formatting
			fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
			
			// Add line
			dest.data.push_back( HX_LINE { 
				chk, true, index, line, fmt 
			} );
			break;
		}
	}
}

#endif
