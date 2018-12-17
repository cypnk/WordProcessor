#ifndef HXFILE_H
#define HXFILE_H

#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "HXTypes.h"

class HXFile {
	private:
		void	symbolCache();
		bool	openFile( std::ifstream &file );
		
		void
		extractLine(
			std::size_t&	chk,
			std::string&	line,
			std::string&	extracted
		);
		
		void
		appendDoc(
			std::string&	line, 
			HX_FILE& 	dest,
			unsigned char	ftype
		);
		
	public:
		HXFile();
		
		void
		openDoc( std::string& fname, HX_FILE& dest );
};

#endif
