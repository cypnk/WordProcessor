#ifndef HXTYPES_H
#define HXTYPES_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

// Some ideas to explore:
// https://stackoverflow.com/questions/8365013/reading-line-from-text-file-and-putting-the-strings-into-a-vector


/**
 *  Keyboard map type
 */
#define	MAP_QWERTY	0x0000	// Only QWERTY for now


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


/**
 *  Formatting type
 */
#define	F_NORMAL	0x0100;
#define	F_BOLD		0x0101;
#define	F_ITALIC	0x0102;
#define	F_UNDER		0x0103;
#define	F_SUP		0x0104;
#define	F_SUB		0x0105;

/**
 *  File types
 */
#define	FILE_UNKOWN	0x0000;
#define	FILE_HUXLEY	0x0001;
#define FILE_TEXT	0x0002;


/**
 *  Helpers
 */
#define ARRAY_SIZE( a ) \
	( sizeof( a ) / sizeof( *( a ) ) )

// Columns per screen
#define COL_SIZE	72

// Lines per page
#define PG_SIZE		62

// End markers
#define END_MKR		"~!$%,.;*?()[]{}+-=/。"

// Idea borrowed from Salvatore Sanfilippo's ( antirez ) Kilo editor
// https://github.com/antirez/kilo
inline bool IS_BREAK( int c ) {
	return
	c == '\0' || isspace( c ) || strchr( END_MKR, c ) != NULL;
}

// Word separator ( for languages that use spaces )
inline bool IS_SPACE( const char* c ) {
	return strchr( c, ' ' ) != NULL;
}

// https://stackoverflow.com/a/744822
inline int ENDS_WITH( const char* str, const char* suffix ) {
	if ( !str || !suffix ) {
		return 0;
	}
	
	std::size_t len_str	= strlen( str );
	std::size_t len_sfx	= strlen( suffix );
	
	if ( len_sfx > len_str ) {
		return 0;
	}
	
	return 
	strncmp( str + len_str - len_sfx, suffix, len_sfx ) == 0;
}

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

/**
 *  Command key format
 */
struct command {
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

#endif
