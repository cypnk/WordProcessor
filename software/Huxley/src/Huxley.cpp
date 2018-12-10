#include "headers/Huxley.h"

/**
 *  Preliminary sketch ideas
 *  This will be the main program window with editor functions handled 
 *  separately
 */

/**
 *  Setup window with base parameters
 */
void
initialize() {
	// Start SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
		exit( 1 );
	}
	atexit( SDL_Quit );
	
	// Generate window
	WINDOW		= SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FLAGS
	);
	// Something went wrong?
	if ( WINDOW == NULL ) {
		printf( "Could not create window: %s\n", 
			SDL_GetError() );
		end( 1 );
	}
	
	// Create renderer
	RENDERER	= SDL_CreateRenderer( WINDOW, -1, 0 );

	// Set program defaults ( paper background, coal foreground )
	resetRender( COLORS.paper );
	setupFont( COLORS.coal );
}

/**
 *  Startup background
 */
void
resetRender( RGB bg_color ) {
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
 *  Startup font (probably move to font array for italic, bold etc...)
 */
void
setupFont( RGB fg_color ) {
	if ( !TTF_WasInit() && TTF_Init() == -1 ) {
		printf("Error initializing TTF: %s\n", TTF_GetError());
		end( 1 );
	}
	
	FONT	= TTF_OpenFont( FONT_FILE, FONT_SIZE );
	
	if ( !FONT ) {
		printf("TTF_OpenFont error: %s\n", TTF_GetError());
		end( 1 );
	}
	
	COLOR.r		= fg_color.R;
	COLOR.b		= fg_color.B;
	COLOR.g		= fg_color.G;
}

/**
 *  Keyboard events
 */
void
handleKeyDown( SDL_Event &event ) {
	
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
handleKeyUp( SDL_Event &event ) {
	
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
handleKeyInput( SDL_Event &event ) {
	switch( event.key.keysym.sym ) {
		case SDLK_LCTRL:
		case SDLK_RCTRL:
		case SDLK_LSHIFT:
		case SDLK_RSHIFT: {
			break;
		}
		default: {
			printf( "%s\n", event.text.text );
		}
	}
}

/**
 *  All keyboard events
 */
void
handleKeyEvents( SDL_Event &event ) {
	
	// Check for keydown events
	if ( event.type == SDL_KEYDOWN ) {
		// Ctrl + Shift already pressed and next key isn't one of them
		if ( ctrl_key.any && shift_key.any ) {
			if ( 
				event.key.keysym.sym != SDLK_LCTRL	&&
				event.key.keysym.sym != SDLK_RCTRL	&&
				event.key.keysym.sym != SDLK_LSHIFT	&&
				event.key.keysym.sym != SDLK_RSHIFT
			) {
				// TODO: Handle Ctrl + Shift + Key combo
				printf( "CTRL+SHIFT+%d\n", event.key.keysym.sym );
			}
		// Ctrl already pressed and next key isn't shift
		} else if ( ctrl_key.any ) {
			if ( 
				event.key.keysym.sym != SDLK_LSHIFT	&&
				event.key.keysym.sym != SDLK_RSHIFT
			) {
				// TODO: Handle Ctrl+ Key combo
				printf( "CTRL+%d\n", event.key.keysym.sym );
			}
		}
		handleKeyDown( event );
		
		// Both left and right keys were pressed
		if ( shift_key.left && shift_key.right ) {
			// TODO: On the prototype, this will enable CapsLock
			// since he prototype keyboard doesn't have that key
			printf( "Capslock enabled\n" );
		}
	}
	
	// Check for key up events
	if ( event.type == SDL_KEYUP ) {
		handleKeyUp( event );
		
	// Normal text input
	} else if ( event.type == SDL_TEXTINPUT ) {
		handleKeyInput( event );
	}
	
}

/**
 *  Window events
 */
void
handleWindowEvents( SDL_Event &event ) {
	switch( event.window.event ) {
		
		// Window has been resized
		case SDL_WINDOWEVENT_SIZE_CHANGED: {
			status.w	= event.window.data1;
			status.h	= event.window.data2;
			break;
		}
		
		// Window has been moved
		case SDL_WINDOWEVENT_MOVED: {
			status.x	= event.window.data1;
			status.y	= event.window.data2;
			break;
		}
		
		// Window changed back from full screen or minimize
		case SDL_WINDOWEVENT_RESTORED: {
			status.win	= WINDOW_SRES;
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
end( int e ) {
	TTF_CloseFont( FONT );
	TTF_Quit();
	FONT	= NULL;
	
	SDL_DestroyRenderer( RENDERER );
	SDL_DestroyWindow( WINDOW );
	SDL_Quit();
	
	// Do something with these (maybe profile settings?)
	printf( "Last window size %d x %d\n", status.w, status.h );
	printf( "Last window position %d, %d\n", status.x, status.y );
	exit( e );
}

int
main() {
	// Begin
	initialize();
	Uint32 windowID = SDL_GetWindowID( WINDOW );
	
	// Event loop
	while( 1 ) {
		SDL_Event event;
		
		// Get window events
		while( SDL_PollEvent( &event ) ) {
			
			// Check trigger
			switch( event.type ) {
				
				// Window related event
				case SDL_WINDOWEVENT: {
					if ( event.window.windowID == windowID ) {
						handleWindowEvents( event );
					}
					break;
				}
				
				// Keyboard event
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				case SDL_TEXTINPUT: {
					handleKeyEvents( event );
					break;
				}
				
				// End program
				case SDL_QUIT: {
					end( 0 );
				}
			}
		}
	}
	
	// Just in case
	end( 0 );
}


