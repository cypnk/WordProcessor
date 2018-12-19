#ifndef EDITOR_CPP
#define EDITOR_CPP

#include "headers/HXTypes.h"
#include "headers/HXFile.h"
#include "headers/Keyboard.h"
#include "headers/Editor.h"

/**
 *  Initialize editor
 */
Editor::Editor( unsigned char key_map ) {
	// Fresh working string
	working_str	= "";
	
	// TODO: Make this user selectable. Use QWERTY as default for now
	switch( key_map ) {
		case MAP_QWERTY:
			KEY_MAP = QWERTY_MAP;
			break;
	}
}

/**
 *  Line index constraints
 */
void
Editor::workingLimit( std::size_t& index ) {
	std::size_t sz	= working_doc.data.size();
	
	if ( index >= sz ) {
		index	= ( sz == 0 ) ? 0 : sz - 1;
	}
}

/**
 *  Add to history
 */
void
Editor::syncInput( bool line ) {
	std::size_t lsize	= working_str.length();
	
	// TODO: Compile formatting for this line
	std::vector<HX_FORMAT> fmt;
	fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
	
	// TODO: Format sync history if new line
	if ( line ) {
		workingLimit( working_line );
		++working_line;
	}
	
	// Calculate current working string checksum
	std::size_t chk	= TO_CHK( working_str );
	
	// New line?
	if ( working_line >= working_doc.data.size() ) {
		working_doc.data.push_back( HX_LINE {
			chk, 
			true, 
			working_line, 
			working_str, 
			fmt
		} );
		
	// Edited line
	} else {
		working_doc.data.at( working_line ) = 
		HX_LINE {
			chk, 
			true, 
			working_line, 
			working_str, 
			fmt
		};
	}
	
	printf( "Line: %zx\n", working_line );
	
	if ( lsize > 0 ) {
		printf( "%s", working_str.c_str() );
		
		// Clear after sync
		working_str.clear();
	}
}

/**
 *  Receive keyboard input fron main window
 *  Sync to working string
 */
void
Editor::sendInput( char* edit, Sint32 cursor, Sint32 len ) {
	
	// TODO: Grab the cursor, selection, and update composition
	if ( cursor > 0 || len > 0 ) {
		// TODO: Display Unicode input box
		INPUT_ACTIVE = true;
		
	} else {
		INPUT_ACTIVE = false;
		working_str.append( edit );
	}
}

/**
 *  Receive command from main window
 */
void
Editor::sendCombo( int ctrl, int shift, SDL_Keycode &key ) {
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
Editor::applyCommand( unsigned char action ) {
	bool line = false;
	
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
		
		// Delete to start/end of line
		case E_DELSL: {
			printf( "Delete to start of this line\n" );
			break;
		}
		case E_DELEL: {
			printf( "Delete to end of this line\n" );
			break;
		}
		
		// Insert line break
		case T_BREAK: {
			printf( "Insert break\n" );
			line = true;
			break;
		}
		// Insert page break
		case E_BREAK: {
			printf( "Insert page break\n" );
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
			
			// Test sample document
			std::string oname = "samples/republic-plato.txt";
			//std::string sname = "samples/sample.hx";
			cmdOpen( oname );
			//cmdSave( sname );
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
	
	// Sync before continuing after a command
	syncInput( line );
}

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
void
Editor::splitLine( 
	std::string& line,
	std::vector<std::string>& words 
) {
	std::string	delims	= END_MKR;
	std::size_t	end	= 0;
	
	// First non-breaking block
	std::size_t	start	= 
	line.find_first_not_of( delims );
	
	while( 
		( end = line.find_first_of( delims, start ) )
		!= std::string::npos 
	) {
		words.push_back( line.substr( start, end - start ) );
		start	= line.find_first_not_of( delims, end );
	}
	
	if ( start != std::string::npos ) {
		words.push_back( line.substr( start ) );
	}
}

/**
 * TODO: On the prototype, this will enable CapsLock
 * since that keyboard won't have the key
 */
void
Editor::capslock() {
	if ( caps ) {
		caps = false;
		printf( "Capslock Off \n" );
		SDL_SetModState( KMOD_NONE );
		return;
	}
	caps = true;
	printf( "Capslock On \n" );
	SDL_SetModState( KMOD_CAPS );
}

/**
 *  Get line at given index (or last/first line)
 */
void
Editor::lineAt( std::size_t& index, HX_LINE& line ) {
	workingLimit( index );
	line = working_doc.data.at( index );
}

/**
 *  Open document, create lnie checksums
 */
void
Editor::cmdOpen( std::string &fname ) {
	HXFile document;
	document.openDoc( fname, working_doc );
	
	// Check integrity
	if ( document.good ) {
		printf( "Integrity check passed" );
	} else {
		printf( "Integrity check failed on line(s): " );
		for (
			std::vector<std::size_t>::iterator bt = 
				document.bad_lines.begin(); 
			bt != document.bad_lines.end();
			++bt
		) {
			printf( "%zu ", ( *bt ) );
		}
	}
}

void
Editor::cmdSave( std::string& fname ) {
	HXFile document;
	document.saveDoc( fname, working_doc );
}

#endif
