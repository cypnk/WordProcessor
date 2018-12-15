#ifndef FILE_H
#define FILE_H

#include <iterator>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>

// Cursor position on document (not on screen)
struct
HX_CURSOR {
	int		column;
	int		line;
};

// Key press history
struct
HX_HISTORY {
	int		column	= 0;
	int		line	= 0;
	const char*	input;
};

// Line formatting
struct
HX_FORMAT {
	int			start	= 0;
	int			end	= 0;
	unsigned char		type	= 0x0000;
};

struct
HX_LINE {
	std::size_t		chk;
	std::string		line;
	std::vector<HX_FORMAT>	format;
};

struct
HX_FILE {
	std::size_t		chk;
	std::vector<HX_LINE>	data;
};

void
openFile( std::string const fname, HX_FILE& dest );

#endif
