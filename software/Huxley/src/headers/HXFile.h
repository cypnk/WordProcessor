#ifndef HXFILE_H
#define HXFILE_H

#include <iterator>
#include <algorithm>
#include <fstream>
#include "HXTypes.h"

class HXFile {
	private:
		void	symbolCache();
	
	public:
		HXFile();
		
		void
		openDoc( std::string const fname, HX_FILE& dest );
		
		void
		appendDoc( std::string& line, HX_FILE& dest );

};

#endif
