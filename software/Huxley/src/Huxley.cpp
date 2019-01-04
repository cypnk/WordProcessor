#ifndef HUXLEY_CPP
#define HUXLEY_CPP

#include "headers/Huxley.h"

/**
 *  Preliminary sketch ideas
 *  This will be the main program window with editor functions handled 
 *  separately
 */

/**
 *  Setup window with base parameters
 */
Huxley::Huxley( const char* title, int width, int height ) {
	// Start SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		exit( 1 );
	}
	atexit( SDL_Quit );
	
	// Generate window
	WINDOW		= SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		WINDOW_FLAGS
	);
	
	// Something went wrong?
	if ( WINDOW == NULL ) {
		printf( "Could not create window: %s\n", 
			SDL_GetError() );
		end( 1 );
	}
	
	// Set size
	status.w	= WINDOW_WIDTH;
	status.h	= WINDOW_HEIGHT;
	
	// Create renderer
	RENDERER	= SDL_CreateRenderer( WINDOW, -1, 0 );
	
	// Set program defaults
	BACKGROUND	= COLORS.paper;
	setupFont( COLORS.graphite );
	
	// Start text input
	SDL_StartTextInput();
	
	// Render window background
	refresh();
	SDL_RenderClear( RENDERER );
	
	// Cache the cursor and clear symbols at index 0 and 1
	cacheSymbol( CUR_SYMBOL );
	cacheSymbol( CLR_SYMBOL );
	
	// Nothing typed
	modified	= false;
}

/**
 *  Send modification flag
 */
bool
Huxley::isModified() {
	return modified;
}

/**
 *  Startup background
 */
void
Huxley::resetRender( RGB bg_color ) {
	// Set background to defaults
	SDL_SetRenderDrawColor( 
		RENDERER, 
		bg_color.R, bg_color.B, bg_color.G, bg_color.A
	);
	
	// Refresh and display
	SDL_RenderPresent( RENDERER );
}

void
Huxley::cleanWrite( 
	int&		x, 
	int&		y, 
	int&		w,
	int&		h, 
	SDL_Texture	*textarea 
) {	
	// Clean space
	SDL_SetRenderDrawColor(
		RENDERER, 
		BACKGROUND.R, 
		BACKGROUND.B, 
		BACKGROUND.G, 
		255 
	);
	SDL_Rect bg		= { x, y, w, h };
	SDL_RenderFillRect( RENDERER, &bg );
	
	// New text
	SDL_Rect box	= { x, y, w, h };
	SDL_RenderCopy( RENDERER, textarea, NULL, &box );
	SDL_RenderPresent( RENDERER );
}

/**
 *  Cache a given chacter as a symbol
 *  TODO: Handle bold/italic etc... and highlighted text
 */
void
Huxley::cacheSymbol( const char* c ) {
	// Already cached index?
	std::size_t idx;
	if ( CACHED_CHAR( SYMBOLS, c, idx ) ) {
		return;
	}
	
	// Create fresh surface
	SDL_Surface *text	= 
	TTF_RenderUTF8_Blended( FONT, c, COLOR );
	
	// Create cache
	CACHE	item = {
		idx,
		SDL_CreateTextureFromSurface( RENDERER, text )
	};
	SYMBOLS.push_back( item );
	SDL_FreeSurface( text );
}


/**
 *  Context and line sensitive printing
 */
void
Huxley::renderText( 
	SDL_Texture	*textarea,
	std::size_t&	col,
	std::size_t&	ln
) {
	int x, y, w, h, column, line;
	SDL_QueryTexture( textarea, NULL, NULL, &w, &h );
	
	// Workaround for SDL_Rect, which only takes Int
	column	= static_cast<int>( col );
	line	= static_cast<int>( ln );
	
	// Render character based on text direction
	switch( TEXT_DIR ) {
		// Top to bottom, left to right
		case TEXT_TDL: {
			x = h * ( line + 1 );
			y = w * ( column + 1 );
			break;
		}
		
		// Top to bottom, right to left
		case TEXT_TDR: {
			x = COL_SIZE - ( h * ( line + 1 ) );
			y = w * ( column + 1 );
			break;
		}
		
		// Right to left
		case TEXT_RTL: {
			x = COL_SIZE - ( w * ( column + 1 ) ) ;
			y = h * ( line + 1 );
			break;
		}
		
		// Left to right
		default: {
			x = w * ( column + 1 );
			y = h * ( line + 1 );
		}
	}
	
	// printf( "%d, %d\n", column, line );
	cleanWrite( x, y, w, h, textarea );
}

/**
 *  Send cached texture to be rendered on screen
 */
bool
Huxley::printFromCache(
	const char*	c,
	std::size_t&	col,
	std::size_t&	ln
) {
	std::size_t idx = SYMBOL_HASH( c );
	
	for ( 
		std::vector<CACHE>::iterator st = 
			SYMBOLS.begin(); 
		st != SYMBOLS.end();
		++st
	) {
		if ( idx == ( *st ).idx ) {
			// Render input at cursor
			renderText( ( *st ).symbol, col, ln );
			return true;
		}
	}
	
	return false;
}

void
Huxley::renderInput( Editor &editor ) {
	// TODO: Fix editor cursor issue
	std::string	working;
	HX_CURSOR	cursor;
	HX_LINE		line;
	
	editor.getWorking( cursor, working );
	
	// Nothing to render yet
	if ( !working.size() ) {
		return;
	}
	
	// Print working line at the cursor index
	for ( std::size_t i = 0; i < working.length(); ++i ) {
		printFromCache( &working[i], i, cursor.line );
	}
}


/**
 *  Redraw and refresh
 */
void
Huxley::refresh() {
	resetRender( BACKGROUND );
	SDL_UpdateWindowSurface( WINDOW );
}

/**
 *  Notify via title the text was modified
 */
void
Huxley::notifyByTitle() {
	if ( modified ) {
		std::string newtitle( WINDOW_TITLE );
		newtitle.append( "*" );
		SDL_SetWindowTitle( WINDOW, newtitle.c_str() );
	}
}

/**
 *  Startup font (probably move to font array for italic, bold etc...)
 */
void
Huxley::setupFont( RGB fg_color ) {
	if ( !TTF_WasInit() && TTF_Init() == -1 ) {
		printf( "Error initializing TTF: %s\n", TTF_GetError() );
		end( 1 );
	}
	
	FONT		= TTF_OpenFont( FONT_FILE, FONT_SIZE );
	if ( !FONT ) {
		printf( "TTF_OpenFont error: %s\n", TTF_GetError() );
		end( 1 );
	}
	
	// Set foreground
	COLOR	= { fg_color.R, fg_color.B, fg_color.G, 255 };
}

/**
 *  Keyboard events
 */
void
Huxley::handleKeyDown( SDL_Event &event ) {
	
	// Check for special keys
	switch( event.key.keysym.sym ) {
		case SDLK_LCTRL: {
			ctrl_key.left	= 1;
			ctrl_key.any	= 1;
			break;
		}
		
		case SDLK_RCTRL: {
			ctrl_key.right	= 1;
			ctrl_key.any	= 1;
			break;
		}
		
		case SDLK_LALT: {
			alt_key.left	= 1;
			alt_key.left	= 1;
			break;
		}
		
		case SDLK_RALT: {
			alt_key.right	= 1;
			alt_key.any	= 1;
			break;
		}
		
		case SDLK_LSHIFT: {
			shift_key.left	= 1;
			shift_key.any	= 1;
			break;
		}
		
		case SDLK_RSHIFT: {
			shift_key.right	= 1;
			shift_key.any	= 1;
			break;
		}
		
		// Quit on escape
		case SDLK_ESCAPE: {
			event.type = SDL_QUIT;
			SDL_PushEvent( &event );
			break;
		}
	}		
}

// Execute edit commands on key up
void
Huxley::handleKeyUp( SDL_Event &event ) {
	
	// Turn off special keys on key release
	switch( event.key.keysym.sym ) {
		case SDLK_LCTRL: {
			ctrl_key.left	= 0;
			break;
		}
		
		case SDLK_RCTRL: {
			ctrl_key.right 	= 0;
			break;
		}
		
		case SDLK_LALT: {
			alt_key.left	= 0;
			break;
		}
		
		case SDLK_RALT: {
			alt_key.right 	= 0;
			break;
		}
		
		case SDLK_LSHIFT: {
			shift_key.left	= 0;
			break;
		}
		
		case SDLK_RSHIFT: {
			shift_key.right = 0;
			break;
		}
	}
	
	// Turn off "any" flag if both are released
	if ( !ctrl_key.left && !ctrl_key.right ) {
		ctrl_key.any	= 0;
	}
	if ( !alt_key.left && !alt_key.right ) {
		alt_key.any	= 0;
	}
	if ( !shift_key.left && !shift_key.right ) {
		shift_key.any	= 0;
	}	
}

/**
 *  Text input handling. Needs buffer etc...
 */
void
Huxley::handleKeyInput( SDL_Event &event, Editor &editor ) {
	if ( ctrl_key.any || alt_key.any ) {
		return;
	}
	
	// TODO: Handle buffer input E.G. AltGr
	if ( event.type == SDL_TEXTINPUT  ) {
		
		editor.sendInput( event.text.text, 0, 0 );
		cacheSymbol( event.text.text );
		
		modified = true;
		
	} else if ( event.type == SDL_TEXTEDITING ) {
		// Capture edit
		editor.sendInput(
			event.edit.text, 
			event.edit.start, 
			event.edit.length
		);
		cacheSymbol( event.edit.text );
	}
}


bool
Huxley::editKey( SDL_Keycode code ) {
	if (
		code		== SDLK_RETURN		|| 
		code		== SDLK_TAB		|| 
		code		== SDLK_BACKSPACE	|| 
		code		== SDLK_DELETE
	) {
		// Also set modified
		modified	= true;
		return true;
	}
	return false;
}

bool
Huxley::movementKey( SDL_Keycode code ) {
	if (
		code		== SDLK_UP		|| 
		code		== SDLK_DOWN		|| 
		code		== SDLK_LEFT		|| 
		code		== SDLK_RIGHT		|| 
		code		== SDLK_HOME		|| 
		code		== SDLK_END		|| 
		code		== SDLK_PAGEUP		|| 
		code		== SDLK_PAGEDOWN
	) {
		return true;
	}
	
	return false;
}

/**
 *  All keyboard events
 */
void
Huxley::handleKeyEvents( SDL_Event &event, Editor &editor ) {
	// Use keycode for simplicity
	SDL_Keycode code = event.key.keysym.sym;
	
	// Check for keydown events
	if ( event.type == SDL_KEYDOWN ) {
		// Ctrl + Shift already pressed and next key isn't one of them
		if ( ctrl_key.any && shift_key.any ) {
			if ( 
				code != SDLK_LCTRL	&&
				code != SDLK_RCTRL	&&
				code != SDLK_LSHIFT	&&
				code != SDLK_RSHIFT
			) {
				// Handle Ctrl + Shift + Key combo
				editor.sendCombo( 
					ctrl_key.any, 
					shift_key.any, 
					code 
				);
				return;
			}
		// Ctrl already pressed and next key isn't shift
		} else if ( ctrl_key.any ) {
			// Ignore second control press
			if (
				code == SDLK_LCTRL	||
				code == SDLK_RCTRL
			) {
				
				
			// TODO: Something with Alt (special symbols)
			} else if (
				code == SDLK_LALT	||
				code == SDLK_RALT
			) {
				
				
			// Ignore shift press
			} else if ( 
				code == SDLK_LSHIFT	||
				code == SDLK_RSHIFT
			) {
				
			} else { 
				// Handle Ctrl+ Key combo
				editor.sendCombo( 
					ctrl_key.any, 
					shift_key.any,
					code 
				);
				return;
			}
			
		// TODO: Symbol selection with Ctrl + Alt	
		} else if ( alt_key.any ) {
			
		}
		
		// Continue handling
		handleKeyDown( event );
		
		// Both left and right keys were pressed
		if ( shift_key.left && shift_key.right ) {
			editor.capslock();
		}
	}
	
	// Check for key up events
	if ( event.type == SDL_KEYUP ) {
		handleKeyUp( event );
		
	// Navigation or special editing?
	} else if (
		editKey( code ) || movementKey( code )
	) {
		editor.sendCombo( ctrl_key.any, shift_key.any, code );
	
	// Normal text input
	} else if ( 
		event.type	== SDL_TEXTINPUT	|| 
		event.type	== SDL_TEXTEDITING
	) {
		handleKeyInput( event, editor );
	}
}

/**
 *  Window events
 */
void
Huxley::handleWindowEvents( SDL_Event &event ) {
	switch( event.window.event ) {
		
		// Window has been resized
		case SDL_WINDOWEVENT_RESIZED: {
			status.w	= event.window.data1;
			status.h	= event.window.data2;
			refresh();
			break;
		}
		
		// Window has been moved
		case SDL_WINDOWEVENT_MOVED: {
			status.x	= event.window.data1;
			status.y	= event.window.data2;
			refresh();
			break;
		}
		
		// Window changed back from full screen or minimize
		case SDL_WINDOWEVENT_RESTORED: {
			status.win	= WINDOW_SRES;
			refresh();
			break;
		}
		
		case SDL_WINDOWEVENT_EXPOSED: {
			refresh();
			break;
		}
		
		// Window has been minimized to taskbar
		case SDL_WINDOWEVENT_MINIMIZED: {
			status.win	= WINDOW_SMIN;
			break;
		}
		
		// Window maximized to full screen
		case SDL_WINDOWEVENT_MAXIMIZED: {
			status.win	= WINDOW_SMAX;
			break;
		}
		
		// Window closed, send quit
		case SDL_WINDOWEVENT_CLOSE: {
			event.type = SDL_QUIT;
			SDL_PushEvent( &event );
			break;
		}
	}
}


/**
 *  Cleanup and end program
 */
void
Huxley::end( int e ) {
	// End text input
	SDL_StopTextInput();
	
	// Clean symbol cache
	for ( 
		std::vector<CACHE>::iterator it = 
			SYMBOLS.begin(); 
		it != SYMBOLS.end();
		++it
	) {
		SDL_DestroyTexture( ( *it ).symbol );
	}
	
	// Clean font
	TTF_CloseFont( FONT );
	TTF_Quit();
	FONT		= NULL;
	
	SDL_DestroyRenderer( RENDERER );
	SDL_DestroyWindow( WINDOW );
	SDL_Quit();
	
	RENDERER	= NULL;
	WINDOW		= NULL;
	
	// Do something with these (maybe profile settings?)
	//printf( "Last window size %d x %d\n", status.w, status.h );
	//printf( "Last window position %d, %d\n", status.x, status.y );
	
	exit( e );
}

/**
 *  Main event loop
 */
bool
Huxley::eventLoop( Editor &editor ) {
	SDL_Event event;
	static Uint32 windowID = SDL_GetWindowID( WINDOW );
	if ( !windowID ) {
		printf( "WindowID error: %s\n", SDL_GetError() );
		end( 1 );
	}
	
	// Wait for window events
	if ( SDL_WaitEvent( &event ) != 0 ) {
		// Check trigger
		switch( event.type ) {
			// Window related event
			case SDL_WINDOWEVENT: {
				if ( 
					windowID ==
					event.window.windowID 
				) {
					handleWindowEvents( event );
				}
				break;
			}
			
			// Keyboard event
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_TEXTINPUT: {
				handleKeyEvents( event, editor );
				
				// Render any changed input
				renderInput( editor );
				break;
			}
			
			// End program
			case SDL_QUIT: {
				// Exit event loop
				return false;
			}
		}
		
	}
	return true;
}

/**
 *  Generate a random filename
 */
void
Huxley::rndFile( std::string& fname ) {
	std::string pool( "01234567890abcdefghijklmnopqrstuvwxyz");
	std::mt19937 generator( std::random_device{}() );
	
	std::shuffle( pool.begin(), pool.end(), generator );
	
	fname.append( pool.substr( 0, 12 ) );
	fname.append( FILE_EXT ); // Extension
}

/**
 *  Clean out current parameter option/value set
 */
void
Huxley::dumpParams( CMD_PARAM& param ) {
	param.opt.clear();
	param.value.clear();
}

/**
 *  Get command line arguments and push to holding vector
 *  -k Keyboard layout
 *  -f File name to create/edit
 *  -d Text direction rtl, ltr, tdl, tdr
 *  -p Page flip direction td, lr, rl
 *  
 *  Cursor position and internal representation of a "line" is 
 *  orthogonal to the way text internally represented, the way it's 
 *  stored in a file, and the way it is shown to the user
 */
void
Huxley::parseParams( int argc, char* argv[] ) {
	CMD_PARAM param;
	std::string	rawparams;
	for ( int i = 0; i < argc; ++i ) {
		rawparams = std::string( argv[i] );
		
		// If this is an option ( starts with dash )
		if ( rawparams[0] == '-' ) {
			// Not just dash?
			if ( rawparams.size() > 1 ) {
				param.opt = rawparams.substr( 1 );
				
			// Invalid option. Start again with next param
			} else {
				dumpParams( param );
				continue;
			}
		
		// If this is a value
		} else {
			// Need an option to be set first
			if ( param.opt.empty() ) {
				// If not, discard value
				dumpParams( param );
				continue;
			} else {
				param.value = rawparams;
			}
		}
		
		// We got nothing from this round? Clear everything
		if ( param.opt.empty() && param.value.empty() ) {
			dumpParams( param );
			continue;
		
		// We got both parts?
		} else if ( !param.opt.empty() && !param.opt.empty() ) {
			parameters.push_back( param );
			dumpParams( param );
			continue;
		}
	}
}

/**
 *  Set text rendering direction
 */
void
Huxley::setDirection() {
	// Set text direction ( default to "left to right" )
	std::size_t dir = 
	SYMBOL_HASH( CMD_FIND( "d", "ltr", parameters ).c_str() );
	
	switch ( dir ) {
		case SYMBOL_HASH( "tdr" ) : {
			TEXT_DIR	= TEXT_TDR;
			break;
		}
		
		case SYMBOL_HASH( "tdl" ) : {
			TEXT_DIR	= TEXT_TDL;
			break;
		}
		
		case SYMBOL_HASH( "rtl" ): {
			TEXT_DIR	= TEXT_RTL;
			break;
		}
		
		default: {
			TEXT_DIR	= TEXT_LTR;
		}
	}
}

/**
 *  Set the working file
 */
void
Huxley::setFile( Editor& editor ) {
	// Set file to be opened or create a random filename
	std::string fname	= CMD_FIND( "f", "", parameters );
	
	// Working directory
	working_fname		= "samples/";
	
	// Create a random filename if no name was specified
	if ( fname.empty() ) {
		rndFile( working_fname );
	} else {
		// Open existing document
		working_fname.append( fname );
		editor.cmdOpen( working_fname );
		
		// TODO: Render file contents
	}
}

void
Huxley::setKeyMap( Editor& editor ) {
	// TODO: Map the other keyboard types
	std::size_t key = 
	SYMBOL_HASH( CMD_FIND( "k", "qwerty", parameters ).c_str() );
	
	switch ( key ) {
		default: {
			editor.setKeyMap( MAP_QWERTY );
		}
	}
}

/**
 *  Set editing parameters
 */
void
Huxley::setParams( Editor& editor ) {
	setDirection();
	
	setKeyMap( editor );
	setFile( editor );
}

/**
 *  Main program
 */
int
main( int argc, char* argv[] ) {
	
	// Begin
	Huxley hx( WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT );
	if ( argc > 0 ) {
		// Cross-platform argument handling
		hx.parseParams( argc, argv );
	}
	
	Editor editor;
	
	// Set parameters
	hx.setParams( editor );
	
	// Event loop
	while ( hx.eventLoop( editor ) ) {
		SDL_Delay( LOOP_WAIT );
		
		// Notify on input
		hx.notifyByTitle();
	}
	
	// If the file was modified, save it to samples
	if ( hx.isModified() ) {
		// Save currently working document
		editor.sync();
		editor.cmdSave( hx.working_fname );
	}
	
	// End
	hx.end( 0 );
}

#endif
