#ifndef EDITOR_CPP
#define EDITOR_CPP

#include "headers/HXTypes.h"
#include "headers/Keyboard.h"
#include "headers/Editor.h"

/**
 *  Initialize editor
 */
Editor::Editor( unsigned char key_map ) {
	// Fresh working string
	working_str	= "";
	
	// Set start position
	working_cur	= { 0, 0 };
	
	// TODO: Make this user selectable. Use QWERTY as default for now
	switch( key_map ) {
		case MAP_QWERTY:
			working_map	= ARRAY_SIZE( QWERTY_MAP );
			KEY_MAP		= QWERTY_MAP;
			break;
	}
}

/**
 *  Line index constraints
 */
void
Editor::workingLimit( std::size_t& index ) {
	std::size_t sz	= working_doc.data.size();
	
	if ( index > sz ) {
		index	= ( sz == 0 ) ? 0 : sz - 1;
	}
}

/**
 *  Sync current working string to input
 */
void
Editor::sync() {
	syncInput( working_str, true );
}

/**
 *  Add to history
 */
void
Editor::syncInput( std::string& working, bool line ) {
	std::size_t lsize	= working.size();
	
	// TODO: Compile formatting for this line
	std::vector<HX_FORMAT> fmt;
	fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
	
	// TODO: Format sync history if new line
	if ( line ) {
		++working_cur.line;
		workingLimit( working_cur.line );
	}
	
	// Calculate current working string checksum
	std::size_t chk	= toCHK( working );
	
	// New line?
	if ( working_cur.line >= working_doc.data.size() ) {
		working_doc.data.push_back( HX_LINE {
			chk, 
			true, 
			working_cur.line, 
			working, 
			fmt
		} );
		
	// Edited line
	} else {
		working_doc.data.at( working_cur.line ) = 
		HX_LINE {
			chk, 
			true, 
			working_cur.line, 
			working, 
			fmt
		};
	}
	
	if ( lsize > 0 ) {
		// Clear after sync
		working.clear();
	}
}

/**
 *  Break sentence at delimiters
 */
void
Editor::syncLine() {
	std::vector<std::string> segments;
	
	// Nothing to break yet
	if ( working_str.size() < COL_SIZE ) {
		return;
	}
	
	// Break working string into segments
	breakSegments( working_str, segments );
	
	// Sync each segment
	for ( 
		std::vector<std::string>::iterator it = 
			segments.begin(); 
		it != segments.end();
		++it
	) {
		// Each segment is a line
		syncInput( ( *it ), true );
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
	
	// Start a new line if we're at the limit
	syncLine();
}

/**
 *  Receive command from main window
 */
void
Editor::sendCombo( int ctrl, int shift, SDL_Keycode &key ) {
	
	// Iterate through key map
	for ( int i = 0; i < working_map; i++ ) {
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
 *  Update cursor position within the limits of the working document
 */
void
Editor::moveCursor( int x, int y ) {
	std::size_t sz = working_doc.data.size();
	if ( y < 0 ) {
		// Move up to max
		if ( 
			working_cur.line > 0	&& 
			( ( working_cur.line + y ) < sz )
		) {
			working_cur.line += y;
		
		// Set to max
		} else {
			working_cur.line = 0;
		}
	} else if ( y > 0) {
		if ( ( working_cur.line + y ) < sz) {
			working_cur.line += y;
		} else {
			working_cur.line = 0;
		}
	}
	if ( x > 0 ) {
		if ( ( working_cur.column + x ) < COL_SIZE ) {
			working_cur.column += x;
			
		// Wrap down
		} else {
			moveCursor( 0, 1 );
			working_cur.column	= 0;
		}
		
	} else if ( x < 0 ) {
		if ( ( working_cur.column + x ) > 0) {
			working_cur.column += x;
		
		// Wrap up
		} else {
			moveCursor( 0, -1 );
			working_cur.column = COL_SIZE;
		}
	}
}
void
Editor::printCursor() {
	printf( " %zu,%zu\n", 
		working_cur.column, 
		working_cur.line 
	);
				
}

/**
 *  Massive TODO: Text command selector
 */
void
Editor::applyCommand( unsigned char action ) {
	switch( action ) {
		
		// Basic movement
		case M_UP: {
			moveCursor( 0, -1 );
			printf( "Move up" );
			printCursor();
			break;
		}
		case M_DOWN: {
			moveCursor( 0, 1 );
			printf( "Move down" );
			printCursor();
			break;
		}
		case M_LEFT: {
			moveCursor( -1, 0 );
			printf( "Move left" );
			printCursor();
			break;
		}
		case M_RIGHT: {
			moveCursor( 1, 0 );
			printf( "Move right" );
			printCursor();
			break;
		}
		
		// Line movement
		case M_LNSTART: {
			working_cur.column = 0;
			printf( "Line start" );
			printCursor();
			break;
		}
		case M_LNEND: {
			working_cur.column = COL_SIZE - 1;
			printf( "Line end" );
			printCursor();
			break;
		}
		
		// Line crolling
		case M_SCRLUP: {
			moveCursor( 0, -SCRL_SIZE );
			printf( "Scroll up" );
			printCursor();
			break;
		}
		
		case M_SCRLDN: {
			moveCursor( 0, SCRL_SIZE );
			printf( "Scroll down" );
			printCursor();
			break;
		}

		// Pagination
		case M_PGUP: {
			moveCursor( 0, -PG_SIZE );
			printf( "Page up" );
			printCursor();
			break;
		}
		case M_PGDN: {
			moveCursor( 0, PG_SIZE );
			printf( "Page down" );
			printCursor();
			break;
		}
		
		// Document start/end
		case M_DSTART: {
			working_cur.line	= 0;
			working_cur.column	= 0;
			printf( "Document start" );
			printCursor();
			break;
		}
		case M_DEND: {
			while( 
				working_cur.line < 
				( working_doc.data.size() - 1 ) 
			) {
				moveCursor( COL_SIZE, PG_SIZE );
			}
			printf( "Document end" );
			printCursor();
			break;
		}
		
		// Text selections
		case S_LEFT: {
			printf( "Select left" );
			printCursor();
			break;
		}
		case S_RIGHT: {
			printf( "Select right" );
			printCursor();
			break;
		}
		
		// Find text
		case T_QUERY: {
			printf( "Query text" );
			printCursor();
			break;
		}
		
		/**
		 *  Editing commands
		 */
		
		// Delete left/right of cursor
		case E_DELL: {
			printf( "Delete left of cursor" );
			printCursor();
			break;
		}
		case E_DELR: {
			printf( "Delete right of cursor" );
			printCursor();
			break;
		}
		
		// Delete current word/line
		case E_DELWD: {
			printf( "Delete word" );
			printCursor();
			break;
		}
		case E_DELLN: {
			printf( "Delete line" );
			printCursor();
			break;
		}
		
		// Delete to start/end of line
		case E_DELSL: {
			printf( "Delete to start of this line" );
			printCursor();
			break;
		}
		case E_DELEL: {
			printf( "Delete to end of this line" );
			printCursor();
			break;
		}
		
		// Insert line break
		case T_BREAK: {
			//printf( "Insert break\n" );
			syncInput( working_str, true );
			break;
		}
		// Insert page break
		case E_BREAK: {
			printf( "Insert page break" );
			syncInput( working_str, true );
			printCursor();
			break;
		}
		
		// Copy to cliboard
		case C_COPY: {
			printf( "Copy selection to clipboard" );
			printCursor();
			break;
		}
		case C_PASTE: {
			printf( "Paste last copied selection" );
			printCursor();
			break;
		}
		
		// Insert memo
		case P_MEMO: {
			printf( "Insert memo to selection" );
			printCursor();
			break;
		}
		
		// Create subroutine
		case P_PROG: {
			printf( "Create or view subroutine" );
			printCursor();
			break;
		}
		
		// History undo/redo
		case H_UNDO: {
			printf( "Undo last action" );
			printCursor();
			break;
		}
		case H_REDO: {
			printf( "Redo last action" );
			printCursor();
			break;
		}
		
		// Document handling
		case T_NEW: {
			printf( "Create new document" );
			printCursor();
			break;
		}
		case T_OPEN: {
			printf( "Open existing document" );
			printCursor();
			
			// Test sample document (need to work more)
			//std::string oname = "samples/republic-plato.txt";
			//std::string sname = "samples/sample.hx";
			//cmdOpen( oname );
			//cmdSave( sname );
			break;
		}
		
		// Check for selection for these
		
		// Indent
		case T_INDENT: {
			printf( "Increase indent text" );
			printCursor();
			break;
		}
		case T_ODENT: {
			printf( "Decrease indent text" );
			printCursor();
			break;
		}

		// Superscript/subscript
		case T_SUP: {
			printf( "Superscript text" );
			printCursor();
			break;
		}
		case T_SUB: {
			printf( "Subscript text" );
			printCursor();
			break;
		}

		// Basic formatting
		case T_BOLD: {
			printf( "Bold text" );
			printCursor();
			break;
		}
		case T_ITALIC: {
			printf( "Italic text" );
			printCursor();
			break;
		}
		case T_UNDER: {
			printf( "Underline text" );
			printCursor();
			break;
		}
		
		// Return cursor
		case M_CUR: {
			printf( "Move cursor to previous position" );
			printCursor();
			break;
		}
		case M_LCUR: {
			printf( "Move cursor to last position" );
			printCursor();
			break;
		}
		
		// Open clipboard ( use number keys to select )
		case C_CLIP: {
			printf( "Open clipboard" );
			printCursor();
			break;
		}
	}
	
	syncLine();
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
 *  Client input handling
 */
std::size_t
Editor::toCHK( std::string& line ) {
	std::size_t chk	= 0;
	const char* str		= line.c_str();
	while( *str ) {
		chk = ( chk * CHK_A ) ^ ( str[0] * CHK_B );
		str++;
	}
	return chk % CHK_C;
}

// Copy from string to checksum size_t
void
Editor::fromCHK(
	std::string&		block, 
	std::size_t&		chk
) {
	char tmp[CHK_SIZE];
	
	// Extract chunk
	snprintf( tmp, CHK_SIZE, "%s", block.c_str() );
	
	std::sscanf( tmp, CHK_FORMAT, &chk );
}

// Copy checksum to char array
void 
Editor::copyCHK(
	char*			check,
	std::size_t&		chk
) {
	snprintf( check, CHK_SIZE, CHK_FORMAT, chk );	
}

// Idea borrowed from Salvatore Sanfilippo's ( antirez ) Kilo editor
// https://github.com/antirez/kilo
bool
Editor::isBreak( int c ) {
	return
	c == '\0' || isspace( c ) || strchr( END_MKR, c ) != NULL;
}

// Word separator ( for languages that use spaces )
bool
Editor::isSpace( const char* c ) {
	return strchr( c, ' ' ) != NULL;
}

// https://stackoverflow.com/a/744822
int
Editor::endsWith(
	const char*	str,
	const char*	suffix
) {
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


/** 
 *  Split line into chunks until working string fits to COL_SIZE
 */
void
Editor::breakSegments( 
	std::string&			working, 
	std::vector<std::string>&	segments, 
	bool				append
) {
	// Nothing to break. Append to segments as-is
	if ( working.size() < COL_SIZE ) {
		segments.push_back( working );
		return;
	}
	
	// Placeholders
	std::string remainder;
	std::size_t ending;
	
	// Keep breaking while working size is bigger than COL_SIZE
	while ( working.size() > COL_SIZE ) {
		
		// Find a good break point in the first COL_SIZE chunk
		ending	= 
		working.substr( 0, COL_SIZE - 1 ).find_last_of( END_MKR );
		
		// Break found?
		if ( ending != std::string::npos ) {
			// Capture everything after the break
			remainder = working.substr( ending + 1 );
		
			// Remove up to break
			working.erase( ending + 1 );
		
		// Break word at the COL_SIZE (this is not ideal)
		} else {
			remainder = working.substr( COL_SIZE - 1 );
			working.erase( COL_SIZE );
		}
		
		//printf( "%s\n", working.c_str() );
		
		// Append working string to processed segments
		segments.push_back( working );
		
		// We have a remainder?
		if ( remainder.size() > 0 ) {
			working	= remainder;
			continue;
		}
		
		// No remainder but working size is still too big?
		// Something went wrong
		break;
	}
	
	// Include the last chunk in the segments?
	// I.E. This wasn't being typed
	if ( append ) {
		segments.push_back( working );
	}
}




/**
 *  File handling
 */


/**
 *  Copy from string in file to formatting
 */
void
Editor::extractFormat(
	std::string& block,
	std::vector<HX_FORMAT>& fmt 
) {
	char tmp[FMT_SIZE];	// String chunk size
	char* raw;		// Raw formatting chunk
	
	// Format data
	Sint32		start	= 0;
	std::size_t	length	= 0;
	char		type[1];
	
	// Default, no formatting
	unsigned char	format	= F_NORMAL;
	
	// Extract from chunk
	snprintf( tmp, FMT_SIZE, "%s", block.c_str() );
	
	// Tokenize
	raw = strtok( tmp, FMT_DELIM );
	
	while( raw != NULL ) {
		sscanf( raw, FMT_FORMAT, type, &start, &length );
		
		// Select formatting by type
		switch( type[0] ) {
			case 'b': { // Bold
				format = F_BOLD;
				break;
			}
			case 'i': { // Italic
				format = F_ITALIC;
				break;
			}
			case 'u': { // Underlined
				format = F_UNDER;
				break;
			}
			case 'p': { // Superscript
				format = F_SUP;
				break;
			}
			case 's': { // Subscript
				format = F_SUB;
				break;
			}
			default: { // No formatting
				format = F_NORMAL;
			}
		}
		fmt.push_back( HX_FORMAT { 
			start, length, format
		} );
		raw =  strtok( NULL, FMT_DELIM );
	}
}

void
Editor::saveFormat(
	std::string& block,
	std::vector<HX_FORMAT>& fmt 
) {
	// Formatting chunk
	char tmp[FMT_SIZE];
	char type;
	
	for (
		std::vector<HX_FORMAT>::iterator it = 
			fmt.begin(); 
		it != fmt.end(); 
		++it
	) {
		switch( (*it).type ) {
			case F_BOLD: {
				type = 'b';
				break;
			}
			case F_ITALIC: {
				type = 'i';
				break;
			}
			case F_UNDER: {
				type = 'u';
				break;
			}
			case F_SUP: {
				type = 'p';
				break;
			}
			case F_SUB: {
				type = 's';
				break;
			}
			default: {
				type = 'n';
			}
		}
		
		snprintf( tmp, FMT_SIZE, FMT_FORMAT, 
			type, (*it).start, (*it).length );
		block.append( tmp );
		tmp[0] = '\0';
		
		// Not the end? Add a delimiter
		if ( std::next( it ) != fmt.end() ) {
			block.append( FMT_DELIM );
		}
	}
}

void
Editor::extractLine(
	std::size_t&		chk,
	std::string&		line,
	std::string&		extracted,
	std::vector<HX_FORMAT>& fmt
) {
	std::istringstream str( line );
	std::string block;
	int i = 0;
	
	// Read each line segment
	while( std::getline( str, block ) ) {
		// Huxley lines should not exceed this
		//if ( i > SEG_SIZE ) {
		//	break;
		//}
		
		// Checksum
		if ( i == 0 ) {
			// Copy checksum
			fromCHK( block, chk );
		
		// Copy formatting
		} else if ( i == 1 ) {
			extractFormat( block, fmt );
		} else {
			// Append as-is
			extracted.append( block );
			extracted.append( SEG_DELIM );
		}
		i++;
	}
	// Remove lastly appened space
	if ( extracted.length() > 1 ) {
		extracted.erase( extracted.length() - 1 );
	}
}


void
Editor::saveDoc( std::string& fname, HX_FILE&source ) {
	std::ofstream file( fname.c_str() );
	if ( !file.is_open() || !file.good() ) {
		return;
	}
	
	std::string block;
	// Checksum placeholder (slightly over-provisioned)
	char check[CHK_SIZE];	
	for (
		std::vector<HX_LINE>::iterator it = 
			source.data.begin(); 
		it != source.data.end(); 
		++it
	) {
		// Copy line checksum
		copyCHK( check, (*it).chk );
		
		// Append formatting data
		saveFormat( block, (*it).format );
		
		// Delimited checksum, formatting, and line text
		file << check << SEG_DELIM << block.c_str() << 
			SEG_DELIM << (*it).line.c_str() << LINE_DELIM;
		
		block.clear();
	}
	
	file.close();
}

void
Editor::openDoc( std::string& fname, HX_FILE& dest ) {
	const char* fcmp	= fname.c_str();
	
	// Setup working file
	std::ifstream file( fcmp );
	if ( !file.is_open() || !file.good() ) {
		return;
	}
	
	// Line index
	std::size_t	index	= 0;
	
	std::string line;
	// Detect file type and load Huxley documents differently
	if ( endsWith( fcmp, ".txt" ) ) {
		// Append lines to document block
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, ++index, FILE_TEXT );
			line.clear();
		}
		
	} else if ( endsWith( fcmp, FILE_EXT ) ) {
		// Append Huxley line to document block
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, ++index, FILE_HUXLEY );
			line.clear();
		}
	} else {
		// Shouldn't handle yet
		while ( std::getline( file, line ) ) {
			appendDoc( line, dest, ++index, FILE_UNKOWN );
			line.clear();
		}
		
	}
	
	file.close();
	if ( !file ) {
		// Error
		return;
	}
}


/**
 *  Convert broken text segments to individual lines in a document
 */
void
Editor::segmentsToFile( 
	std::vector<std::string>&	segments, 
	std::size_t&			index,
	std::size_t&			chk,
	std::vector<HX_FORMAT>&		fmt,
	HX_FILE& 			dest
) {
	for ( 
		std::vector<std::string>::iterator it = 
			segments.begin(); 
		it != segments.end();
		++it
	) {
		// Fresh checksum
		chk	= toCHK( ( *it ) );
		
		// Empty formatting
		fmt.push_back( HX_FORMAT{ 0, 0, 0x0000 } );
		
		// Add line
		dest.data.push_back( HX_LINE { 
			chk, true, index, ( *it ), fmt 
		} );
	}
}

void
Editor::appendDoc(
	std::string&	line, 
	HX_FILE& 	dest,
	std::size_t	index,
	unsigned char	ftype
) {
	// Formatting read from the line
	std::vector<HX_FORMAT>	fmt;
	
	// Line checksum placeholder
	std::size_t		chk;
	
	// Integrity check passed?
	bool			passed	= true;
	
	switch( ftype ) {
		
		// Huxley file
		case FILE_HUXLEY: {
			std::string extracted	= "";
			
			// Extract line, checksum, and formatting
			extractLine( chk, line, extracted, fmt );
			
			// Integrity check
			passed			= 
			chk == toCHK( extracted );
			
			// Integrity check failed at any point?
			if ( !passed ) {
				// Global failiure flag
				good = false;
				
				// Append to bad lines list
				bad_lines.push_back( index );
			}
			
			// Add line with any formatting
			// and if given and calculated checksums match
			dest.data.push_back( HX_LINE {
				chk,
				passed,
				index,
				extracted, 
				fmt
			} );
			break;
		}
		
		// Plain text
		default: {
			std::vector<std::string> segments;
			
			// Break raw line into segments
			breakSegments( line, segments, true );
			segmentsToFile( segments, chk, index, fmt, dest );
		}
	}
}


/**
 *  Open document, create lnie checksums
 */
void
Editor::cmdOpen( std::string &fname ) {
	openDoc( fname, working_doc );
	
	// Check integrity
	if ( good ) {
		printf( "Integrity check passed" );
	} else {
		printf( "Integrity check failed on line(s): " );
		for (
			std::vector<std::size_t>::iterator bt = 
				bad_lines.begin(); 
			bt != bad_lines.end();
			++bt
		) {
			printf( "%zu ", ( *bt ) );
		}
	}
}

void
Editor::cmdSave( std::string& fname ) {
	saveDoc( fname, working_doc );
}

#endif
