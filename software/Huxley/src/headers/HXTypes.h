#ifndef HXTYPES_H
#define HXTYPES_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#define HUXLEY_VERSION	"0.0.1"

/**
 *  Window defaults
 */
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#define WINDOW_TITLE	"Huxley"

// Some ideas to explore:
// https://stackoverflow.com/questions/8365013/reading-line-from-text-file-and-putting-the-strings-into-a-vector


/**
 *  Keyboard map type
 */
#define	MAP_QWERTY	0x0000	// Only QWERTY for now
#define MAP_DVORAK	0x0001
#define MAP_COLEMAK	0x0002


/**
 *  Default font settings
 */
#define FONT_FILE	"bin/assets/fonts/NotoSansMono-Regular.ttf"
#define FONT_SIZE	13

/**
 *  Default text direction
 */
#define TEXT_LTR	0x0000	// Left to right
#define TEXT_RTL	0x0001	// Right to left
#define TEXT_TDL	0x0002	// Top to bottom, left to right
#define TEXT_TDR	0x0003	// Top to bottom, right to left

/**
 *  Page turn direction
 */
#define PAGE_TD		0x0000	// Top to down
#define PAGE_LR		0x0001	// Left to right
#define PAGE_RL		0x0002	// Right to left

/**
 *  Main event loop delay
 */
#define LOOP_WAIT	30

/**
 *  Keyboard commands
 */

/**
 *  Move commands
 */

// Basic movement
#define	M_UP		0x0000
#define	M_DOWN		0x0001
#define	M_LEFT		0x0002
#define M_RIGHT		0x0003

// Line movement
#define M_LNSTART	0x0004
#define M_LNEND		0x0005

// Scrolling
#define M_SCRLUP	0x0006
#define M_SCRLDN	0x0007

// Pagination
#define M_PGUP		0x0008
#define M_PGDN		0x0009

// Document start/end
#define M_DSTART	0x0010
#define M_DEND		0x0011

// Text selections
#define S_LEFT		0x0012
#define S_RIGHT		0x0013

// Find text
#define T_QUERY		0x0014

/**
 *  Editing commands
 */

// Delete left/right of cursor
#define	E_DELL		0x0015
#define	E_DELR		0x0016

// Delete current word/line
#define	E_DELWD		0x0017
#define	E_DELLN		0x0018

// Insert page break
#define E_BREAK		0x0019

// Copy to cliboard
#define C_COPY		0x0020
#define C_PASTE		0x0021

// Insert memo
#define P_MEMO		0x0022

// Create subroutine
#define P_PROG		0x0023

// History undo/redo
#define H_UNDO		0x0024
#define H_REDO		0x0025

// Document handling
#define T_NEW		0x0026
#define T_OPEN		0x0027

// Indent
#define T_INDENT	0x0028
#define T_ODENT		0x0029

// Superscript/subscript
#define T_SUP		0x0030
#define T_SUB		0x0031

// Basic formatting
#define T_BOLD		0x0032
#define T_ITALIC	0x0033
#define T_UNDER		0x0034

// Return cursor
#define M_CUR		0x0035
#define M_LCUR		0x0036

// Open clipboard
#define C_CLIP		0x0037

// Insert break
#define T_BREAK		0x0038

// Delete to start/end of line
#define	E_DELSL		0x0039
#define	E_DELEL		0x0040


/**
 *  Formatting type
 */
#define	F_NORMAL	0x0000
#define	F_BOLD		0x0001
#define	F_ITALIC	0x0002
#define	F_UNDER		0x0003
#define	F_SUP		0x0004
#define	F_SUB		0x0005

/**
 *  File types
 */
#define	FILE_UNKOWN	0x0000
#define	FILE_HUXLEY	0x0001
#define FILE_TEXT	0x0002


// Columns per screen
#define COL_SIZE	72

// Lines per page
#define PG_SIZE		62

// Lines per scroll
#define SCRL_SIZE	3

// End markers
#define END_MKR		" ~!$%,.;。*\?+-=\t\r\f\n\\"

// Space markers
#define SPC_MKR		" \t\r\v\f\n"

// Checksum format
#define CHK_FORMAT	"%zx"

// Checksum max size
#define CHK_SIZE	25

// Formatting size
#define	FMT_SIZE	50

// Line formatting stored in file
#define FMT_FORMAT	"%c%i,%zu"

// Formatting delimiter
#define	FMT_DELIM	"\x1D"

// Line segment (checksum, formatting, text) delimiter
#define SEG_DELIM	"\x1E"

#define MARK_DELIM	"\uFFFF"

// Line segment length 
// Checksum + Formatting + Text data + 2 delimiters between each
#define SEG_SIZE	CHK_SIZE + FMT_SIZE + COL_SIZE  + 2

// Line delimiter
#define LINE_DELIM	"\n"

// File extension
#define FILE_EXT	".hx"

/**
 *  Line checksum helpers
 */

// Cursor position on document (not on screen)
struct
HX_CURSOR {
	std::size_t		column;
	std::size_t		line;
};

// Selected text
struct
HX_SELECTION {
	std::size_t		start	= 0;	// Location from cursor
	std::size_t		length	= 0;	// Selection length
	std::string		data;		// Selected text
};

// Line formatting
struct
HX_FORMAT {
	Sint32			start	= 0;	// Position in line
	std::size_t		length	= 0;	// Apply length
	unsigned char		type	= 0x0000;
};

struct
HX_LINE {
	// Given line checksum in file
	std::size_t		chk;
	
	// Calculated and given checksums match
	bool			good;
	
	// Line content
	std::string		line;
	
	// Text formatting
	std::vector<HX_FORMAT>	format;
	
	// This line precedes a page break
	bool			page	= false;
	
	// Last position of the cursor on this line
	std::size_t		cursor	= 0;
};

// Keyboard input (smallest affected text segment)
struct
HX_ACTION {
	Sint32			start	= 0;
	Sint32			length	= 0;
	std::string		text	= "";
	std::vector<HX_FORMAT>	format;
};

// Annotation/memos
struct
HX_MEMO {
	std::size_t		index;		// Line index
	std::size_t		start	= 0;	// Position in line
	std::size_t		length	= 0;	// Highlight length
	std::string		content;	// Memo data
};

// A complete line synced to the file
struct
HX_FILE {
	std::size_t		chk;
	std::vector<HX_LINE>	data;
};

// Undo/Redo history
struct
HX_HISTORY {
	HX_ACTION	do_cmd;
	HX_ACTION	undo_cmd;
	bool		saved		= false;
};


/**
 *  Command key format
 */
struct
HX_COMMAND {
	SDL_Keycode	code	= SDLK_UNKNOWN;
	int		ctrl	= 0;
	int		shift	= 0;
	int		alt	= 0;  // Not used for now
	unsigned char	action	= 0x0000;
};


// Color option
struct
RGB {
	Uint8 R	= 0;
	Uint8 B	= 0;
	Uint8 G	= 0;
	Uint8 A	= 1;
};


/**
 *  Helpers
 */
#define ARRAY_SIZE( a ) \
	( sizeof( a ) / sizeof( *( a ) ) )


static inline void
rtrim( std::string& str ) {
	if ( str.size() == 0 ) {
		return;
	}
	
	// From the end
	std::size_t last = str.find_last_of( SPC_MKR );
	
	// Space found?
	if ( last != std::string::npos ) {
		str.erase( last + 1 );
	}
}

static inline void
ltrim( std::string& str ) {
	std::size_t sz	= str.size();
	
	if ( sz == 0 ) {
		return;
	}
	// From the front
	std::size_t first = str.find_first_of( SPC_MKR );
	
	// Space found?
	if ( first != std::string::npos ) {
		str	= str.substr( first, ( sz - first + 1 ) );
	}
}

static inline void
trim( std::string& str ) {
	rtrim( str );
	ltrim( str );
}


#endif
