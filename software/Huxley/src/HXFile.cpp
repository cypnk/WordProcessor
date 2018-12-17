#ifndef HXFILE_CPP
#define HXFILE_CPP

#include "headers/HXFile.h"

HXFile::HXFile() { }

bool
HXFile::openFile( std::ifstream &file ) {
	if( file.is_open() && file.good() ) {
		return true;
	}
	
	return false;
}

void
HXFile::openTextDoc( const char* fname, HX_FILE& dest ) {
	// Setup working file
	std::ifstream file( fname );
	if ( !openFile( file ) ) {
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
HXFile::openHuxleyDoc( const char* fname, HX_FILE& dest ) {
	// Setup working file
	std::ifstream file( fname );
	if ( !openFile( file ) ) {
		return;
	}
	
	// TODO: This needs to be handled very differently
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
HXFile::openDoc( const char* fname, HX_FILE& dest ) {
	// Detect file type and load Huxley documents differently
	if ( ENDS_WITH( fname, ".txt" ) ) {
		openTextDoc( fname, dest );
	} else if ( ENDS_WITH( fname, ".hx" ) ) {
		openHuxleyDoc( fname, dest );
	} else {
		openTextDoc( fname, dest );
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

#endif
