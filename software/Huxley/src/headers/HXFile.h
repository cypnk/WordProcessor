#ifndef HXFILE_H
#define HXFILE_H

#include <iterator>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "HXTypes.h"

class HXFile {
	private:
		// Convert from raw string to format stack
		void
		extractFormat(
			std::string&	block,
			std::vector<HX_FORMAT>& fmt 
		);
		
		// Convert from raw string to document line
		void
		extractLine(
			std::size_t&	chk,
			std::string&	line,
			std::string&	extracted,
			std::vector<HX_FORMAT>& fmt
		);
		
		// Convert from format stack to string
		void
		saveFormat(
			std::string&	block,
			std::vector<HX_FORMAT>& fmt 
		);
		
		void
		appendDoc(
			std::string&	line, 
			HX_FILE& 	dest,
			std::size_t	index,
			unsigned char	ftype
		);
		
	public:
		HXFile();
		std::vector<std::size_t> bad_lines;
		
		// Checksum passed
		bool			good	= true;
		
		void
		openDoc( std::string& fname, HX_FILE& dest );
		
		void
		saveDoc( std::string& fname, HX_FILE& source );
};

#endif
