#include "headers/Editor.h"
#include "headers/Keyboard.h"

// TODO: Make this user selectable. Use QWERTY as default for now
command* KEY_MAP = QWERTY_MAP;

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
	static int map_size = ARRAY_SIZE( QWERTY_MAP );
	
	// Iterate through key map
	for ( int i = 0; i < map_size; i++ ) {
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

