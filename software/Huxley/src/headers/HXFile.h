#ifndef HXFILE_H
#define HXFILE_H

#include <iterator>
#include <algorithm>
#include <fstream>
#include "HXTypes.h"

class HXFile {
	private:
		void	symbolCache();
		bool	openFile( std::ifstream &file );
		
		void	
		openTextDoc( const char* name, HX_FILE& dest );
		
		void	
		openHuxleyDoc( const char* fname, HX_FILE& dest );
		
	public:
		HXFile();
		
		void
		openDoc( const char* fname, HX_FILE& dest );
		
		void
		appendDoc( std::string& line, HX_FILE& dest );

};

#endif
