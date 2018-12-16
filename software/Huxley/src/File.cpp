#include "headers/HXFile.h"

HXFile::HXFile() { }

void
HXFile::openDoc( std::string const fname, HX_FILE& dest ) {
	// Setup working file
	std::ifstream file( fname.c_str() );
	
	if( !file.is_open() || !file.good() ) {
		// Error
		return;
	}
	
	std::string line;
	// Append lines to document block
	while ( std::getline( file, line ) ) {
		appendDoc( line, dest );
		line.clear();
	}
	
	file.close();
	if ( !file ) {
		// Error
		return;
	}
}

void
HXFile::appendDoc( std::string& line, HX_FILE& dest ) {
	// TODO: Make this read formatting from the line
	std::vector<HX_FORMAT> fmt;
	
	// Add line formatting
	fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
	
	// Add line with any formatting and its checksum
	dest.data.push_back( 
		HX_LINE {
			std::hash<std::string>{}( line ),
			line,
			fmt
		}
	);	
}
