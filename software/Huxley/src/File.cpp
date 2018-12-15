#include "headers/File.h"

void
openFile( std::string const fname, HX_FILE& dest ) {
	// Setup working file
	std::ifstream file( fname.c_str() );
	
	if( !file.is_open() || !file.good() ) {
		// Error
		return;
	}
	
	std::string line;
	// Append lines to document block
	while ( std::getline( file, line ) ) {
		
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
		line.clear();
	}
	
	file.close();
	if ( !file ) {
		// Error
		return;
	}
}
