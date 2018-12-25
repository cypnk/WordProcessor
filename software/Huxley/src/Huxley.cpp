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
	
	// Set program defaults ( paper background, coal foreground )
	BACKGROUND	= COLORS.paper;
	setupFont( COLORS.coal );
	
	// Start text input
	SDL_StartTextInput();
	
	// Render window background
	refresh();
	
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
	SDL_RenderClear( RENDERER );
	SDL_RenderPresent( RENDERER );
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
		modified = false;
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
	
	FONT	= TTF_OpenFont( FONT_FILE, FONT_SIZE );
	if ( !FONT ) {
		printf( "TTF_OpenFont error: %s\n", TTF_GetError() );
		end( 1 );
	}
	
	FOREGROUND = { fg_color.R, fg_color.B, fg_color.G, 255 };
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
		// Capture text input
		editor.sendInput( event.text.text, 0, 0 );
		modified = true;
		
	} else if ( event.type == SDL_TEXTEDITING ) {
		// Capture edit
		editor.sendInput(
			event.edit.text, 
			event.edit.start, 
			event.edit.length
		);
	}
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
		code		== SDLK_UP		|| 
		code		== SDLK_DOWN		|| 
		code		== SDLK_LEFT		|| 
		code		== SDLK_RIGHT		|| 
		code		== SDLK_HOME		|| 
		code		== SDLK_END		|| 
		code		== SDLK_PAGEUP		|| 
		code		== SDLK_PAGEDOWN	|| 
		code		== SDLK_RETURN		|| 
		code		== SDLK_TAB		|| 
		code		== SDLK_BACKSPACE	|| 
		code		== SDLK_DELETE
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
	printf( "Last window size %d x %d\n", status.w, status.h );
	printf( "Last window position %d, %d\n", status.x, status.y );
	
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
 */
void
Huxley::parseParams( int argc, char* argv[] ) {
	CMD_PARAM param;
	std::string	raw;
	for ( int i = 0; i < argc; ++i ) {
		raw = std::string( argv[i] );
		
		// If this is an option ( starts with dash )
		if ( raw[0] == '-' ) {
			// Not just dash?
			if ( raw.size() > 1 ) {
				param.opt = raw;
				
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
				param.value = raw;
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
	
	
	// TODO: Make this user selectable. Default QWERTY
	Editor editor( MAP_QWERTY );
	
	// Event loop
	while( hx.eventLoop( editor ) ) {
		SDL_Delay( LOOP_WAIT );
		
		// Notify on input
		hx.notifyByTitle();
	}
	
	
	// If the file was modified, save it to samples
	if ( hx.isModified() ) {
		// Starting directory
		std::string sname = "samples/";
		hx.rndFile( sname );
		
		// Save currently working document
		editor.sync();
		editor.cmdSave( sname );
	}
	
	// End
	hx.end( 0 );
}

#endif
