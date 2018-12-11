#include "headers/Editor.h"

// https://wiki.libsdl.org/SDLKeycodeLookup
command KEY_MAP[] = {
	{ SDLK_UP,		0, 0, 0, M_UP },		// 1
	{ SDLK_e,		1, 0, 0, M_UP },		// 2
	
	{ SDLK_DOWN,		0, 0, 0, M_DOWN },		// 3
	{ SDLK_x,		1, 0, 0, M_DOWN },		// 4
	
	{ SDLK_LEFT,		0, 0, 0, M_LEFT },		// 5
	{ SDLK_s,		1, 0, 0, M_LEFT },		// 6
	
	{ SDLK_RIGHT,		0, 0, 0, M_RIGHT },		// 7
	{ SDLK_d,		1, 0, 0, M_RIGHT },		// 8
	
	{ SDLK_HOME,		0, 0, 0, M_LNSTART },		// 9
	{ SDLK_k,		1, 0, 0, M_LNSTART },		// 10
	
	{ SDLK_END,		0, 0, 0, M_LNEND },		// 11
	{ SDLK_l,		1, 0, 0, M_LNEND },		// 12
	
	// Line scroll
	{ SDLK_w,		1, 0, 0, M_SCRLUP },		// 13
	{ SDLK_z,		1, 0, 0, M_SCRLDN },		// 14
	
	// Pagination
	{ SDLK_PAGEUP,		0, 0, 0, M_PGUP },		// 15
	{ SDLK_r,		1, 0, 0, M_PGUP },		// 16
	
	{ SDLK_PAGEDOWN,	0, 0, 0, M_PGDN },		// 17
	{ SDLK_c,		1, 0, 0, M_PGDN },		// 18
	
	{ SDLK_HOME,		1, 0, 0, M_DSTART },		// 19
	{ SDLK_COMMA,		1, 0, 0, M_DSTART },		// 20
	
	{ SDLK_END,		1, 0, 0, M_DEND },		// 21
	{ SDLK_PERIOD,		1, 0, 0, M_DEND },		// 22
	
	// Move cursor to last position
	{ SDLK_j,		1, 0, 0, M_CUR },		// 23
	// Move cursor to last position
	{ SDLK_j,		1, 1, 0, M_LCUR },		// 24
	
	// Selections
	{ SDLK_LEFT,		0, 1, 0, S_LEFT },		// 25
	{ SDLK_a,		1, 0, 0, S_LEFT },		// 26
	
	{ SDLK_RIGHT,		0, 1, 0, S_RIGHT },		// 27
	{ SDLK_f,		1, 0, 0, S_RIGHT },		// 28
	
	// Find
	{ SDLK_q,		1, 0, 0, T_QUERY },		// 29
	
	// Editing
	{ SDLK_DELETE,		0, 0, 0, E_DELL },		// 30
	{ SDLK_g,		1, 0, 0, E_DELL },		// 31
	{ SDLK_h,		1, 0, 0, E_DELR },		// 32
	
	{ SDLK_t,		1, 0, 0, E_DELWD },		// 33
	{ SDLK_y,		1, 0, 0, E_DELLN },		// 34
	
	// Clipboard
	{ SDLK_v,		1, 0, 0, C_CLIP },		// 35
	
	// Insert line break
	{ SDLK_b,		1, 0, 0, E_BREAK },		// 36
	
	// Copy paste
	{ SDLK_LEFTBRACKET, 	1, 0, 0, C_COPY },		// 37
	{ SDLK_RIGHTBRACKET,	1, 0, 0, C_PASTE },		// 38
	
	// Memo / Citation
	{ SDLK_m,		1, 0, 0, P_MEMO },		// 39
	
	// History
	{ SDLK_u,		1, 0, 0, H_UNDO },		// 40
	{ SDLK_u,		1, 1, 0, H_REDO },		// 41
	
	// Document handling
	{ SDLK_n,		1, 0, 0, T_NEW },		// 42
	{ SDLK_o,		1, 0, 0, T_OPEN },		// 43
	
	// Indentation
	{ SDLK_TAB,		0, 0, 0, T_INDENT },		// 44
	{ SDLK_i,		1, 0, 0, T_INDENT },		// 45
	
	{ SDLK_TAB,		1, 0, 0, T_ODENT },		// 46
	{ SDLK_i,		1, 1, 0, T_ODENT },		// 47
	
	// Formatting
	{ SDLK_t,		1, 1, 0, T_SUP },		// 48
	{ SDLK_v,		1, 1, 0, T_SUB },		// 49
	{ SDLK_b,		1, 1, 0, T_BOLD },		// 50
	{ SDLK_y,		1, 1, 0, T_ITALIC },		// 51
	{ SDLK_s,		1, 1, 0, T_UNDER }		// 52
};




/*
char* input;
char* composition;

Sint32 cursor;
Sint32 selection;
*/
/**
 *  Receive command from main window
 */
void
sendCombo( int ctrl, int shift, SDL_Keycode &key ) {
	// Iterate through key map
	for ( int i = 0; i < 52; i++ ) {
		// Check code map
		if ( 
			key	== KEY_MAP[i].code	&& 
			ctrl	== KEY_MAP[i].ctrl	&& 
			shift	== KEY_MAP[i].shift
		) {
			applyCommand( KEY_MAP[i].action );
			break;
		}
	}
}

/**
 *  Massive TODO: Text command selector
 */
void
applyCommand( unsigned char action ) {
	switch( action ) {
		
		// Basic movement
		case M_UP: {
			printf( "Move up\n" );
			break;
		}
		case M_DOWN: {
			printf( "Move down\n" );
			break;
		}
		case M_LEFT: {
			printf( "Move left\n" );
			break;
		}
		case M_RIGHT: {
			printf( "Move right\n" );
			break;
		}
		
		// Line movement
		case M_LNSTART: {
			printf( "Line start\n" );
			break;
		}
		case M_LNEND: {
			printf( "Line end\n" );
			break;
		}
		
		// Line crolling
		case M_SCRLUP: {
			printf( "Scroll up\n" );
			break;
		}
		case M_SCRLDN: {
			printf( "Scroll down\n" );
			break;
		}

		// Pagination
		case M_PGUP: {
			printf( "Page up\n" );
			break;
		}
		case M_PGDN: {
			printf( "Page down\n" );
			break;
		}
		
		// Document start/end
		case M_DSTART: {
			printf( "Document start\n" );
			break;
		}
		case M_DEND: {
			printf( "Document end\n" );
			break;
		}
		
		// Text selections
		case S_LEFT: {
			printf( "Select left\n" );
			break;
		}
		case S_RIGHT: {
			printf( "Select right\n" );
			break;
		}
		
		// Find text
		case T_QUERY: {
			printf( "Query text\n" );
			break;
		}
		
		/**
		 *  Editing commands
		 */
		
		// Delete left/right of cursor
		case E_DELL: {
			printf( "Delete left of cursor\n" );
			break;
		}
		case E_DELR: {
			printf( "Delete right of cursor\n" );
			break;
		}

		// Delete current word/line
		case E_DELWD: {
			printf( "Delete word\n" );
			break;
		}
		case E_DELLN: {
			printf( "Delete line\n" );
			break;
		}
		
		// Insert page break
		case E_BREAK: {
			printf( "Insert break\n" );
			break;
		}
		
		// Copy to cliboard
		case C_COPY: {
			printf( "Copy selection to clipboard\n" );
			break;
		}
		case C_PASTE: {
			printf( "Paste last copied selection\n" );
			break;
		}
		
		// Insert memo
		case P_MEMO: {
			printf( "Insert memo to selection\n" );
			break;
		}
		
		// Create subroutine
		case P_PROG: {
			printf( "Create or view subroutine\n" );
			break;
		}
		
		// History undo/redo
		case H_UNDO: {
			printf( "Undo last action\n" );
			break;
		}
		case H_REDO: {
			printf( "Redo last action\n" );
			break;
		}
		
		// Document handling
		case T_NEW: {
			printf( "Create new document\n" );
			break;
		}
		case T_OPEN: {
			printf( "Open existing document\n" );
			break;
		}
		
		// Check for selection for these
		
		// Indent
		case T_INDENT: {
			printf( "Increase indent text\n" );
			break;
		}
		case T_ODENT: {
			printf( "Decrease indent text\n" );
			break;
		}

		// Superscript/subscript
		case T_SUP: {
			printf( "Superscript text\n" );
			break;
		}
		case T_SUB: {
			printf( "Subscript text\n" );
			break;
		}

		// Basic formatting
		case T_BOLD: {
			printf( "Bold text\n" );
			break;
		}
		case T_ITALIC: {
			printf( "Italic text\n" );
			break;
		}
		case T_UNDER: {
			printf( "Underline text\n" );
			break;
		}
		
		// Return cursor
		case M_CUR: {
			printf( "Move cursor to previous position\n" );
			break;
		}
		case M_LCUR: {
			printf( "Move cursor to last position\n" );
			break;
		}
		
		// Open clipboard ( use number keys to select )
		case C_CLIP: {
			printf( "Open clipboard\n" );
			break;
		}

	}
}

