#ifndef HUXLEY_H
#define HUXLEY_H

#include <ctype.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Editor.h"


#define HUXLEY_VERSION "0.0.1"

/**
 *  Special key input states
 */
struct
SPECIAL_KEY {
	int left	= 0;
	int right	= 0;
	int any		= 0;
} ctrl_key, alt_key, shift_key;


/**
 *  Main window parameters
 */
SDL_Window	*WINDOW;
SDL_Renderer	*RENDERER;
TTF_Font	*FONT;
SDL_Color	COLOR;

int 
WINDOW_WIDTH	= 800;

int 
WINDOW_HEIGHT	= 600;

const char* 
WINDOW_TITLE	= "Huxley";

Uint32
WINDOW_FLAGS	= SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

Uint32
LOOP_WAIT	= 10;

int
FONT_SIZE	= 11;

const char*
FONT_FILE	= "bin/assets/fonts/NotoSansMono-Regular.ttf";

// Render 
//std::unordered_map<SDL_Keycode, SDL_Texture>
//SYMBOL_CACHE;


// Color option
struct
RGB {
	Uint8 R	= 0;
	Uint8 B	= 0;
	Uint8 G	= 0;
	Uint8 A	= 1;
};

// Default colors
struct
COLORS {
	// Standard
	RGB black	{ 0, 0, 0, 1 };
	RGB white	{ 255, 255, 255, 1 };
	
	RGB paper	{ 245, 244, 226, 1 };
	RGB coal	{ 122, 122, 113, 1 };
	
	RGB graphite	{ 25, 25, 24, 1 };
	RGB cream	{ 252, 251, 245, 1 };
	
	// Material UI https://materialuicolors.co (300)
	RGB grey	{ 224, 224, 224, 1 };
	RGB bgrey	{ 144, 164, 174, 1 };
	
	RGB cyan	{ 77, 208, 225, 1 };
	RGB blue	{ 100, 181, 246, 1 };
	RGB indigo	{ 121, 134, 203, 1 };
	
	RGB purple	{ 186, 104, 200, 1 };
	RGB dpurple	{ 149, 117, 205, 1 };
	
	RGB yellow	{ 255, 241, 118, 1 };
	RGB green	{ 129, 199, 132, 1 };
	RGB lgreen	{ 174, 213, 129, 1 };
	RGB teal	{ 77, 182, 172, 1 };
	RGB lime	{ 220, 231, 117, 1 };
	
	RGB pink	{ 240, 98, 146, 1 };
	RGB red		{ 229, 115, 115, 1 };
	RGB brown	{ 161, 136, 127, 1 };
} COLORS;



// Size restored (default)
int
WINDOW_SRES	= 0;

// Size minimized
int
WINDOW_SMIN	= 1;

// Size maximized
int
WINDOW_SMAX	= 2;

// Running parameters
struct
CURRENT {
	int	w	= 0;
	int	h	= 0;
	int	x	= 0;
	int	y	= 0;
	int	win;
} status;

/**
 *  Cursor
 */
float
CUR_TIMER	= 1.0f;

float
CUR_BLINK	= 2.0f;

SDL_Rect 
CURSOR		= { 200, 200, 10, 20 };


/**
 *  Base operations
 */
void
initialize();

bool
eventLoop();

void
resetRender( RGB bg_color );

void
setupFont( RGB fb_color );

void
loadSymbols();

void
cacheSymbol( const char* c );

void
end( int e );


/**
 *  Base operations
 */

/**
 *  Window resizing, repositioning, etc...
 */
void
handleWindowEvents( SDL_Event &event );


/**
 *  Keyboard events
 */

// Keyboard text input
void
handleKeyInput( SDL_Event &event );

// Key press event
void
handleKeyDown( SDL_Event &event );

// Key release event
void
handleKeyUp( SDL_Event &event );

// Capture all key events
void
handleKeyEvents( SDL_Event &event );

#endif

