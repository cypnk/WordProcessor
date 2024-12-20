#ifndef HUXLEY_H
#define HUXLEY_H

#include <stdio.h>
#include <unordered_map>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Editor.h"


// Size restored (default)
int
WINDOW_SRES	= 0;

// Size minimized
int
WINDOW_SMIN	= 1;

// Size maximized
int
WINDOW_SMAX	= 2;


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
	
	// Flat UI https://flatuicolors.com/palette/defo
	RGB turquoise	{ 26, 188, 156, 1 };
	RGB emarald	{ 46, 204, 113, 1 };
	RGB peter_river	{ 52, 152, 219, 1 };
	RGB amethyst	{ 155, 89, 182, 1 };
	RGB wet_asphalt	{ 52, 73, 94, 1 };
	RGB green_sea	{ 22, 160, 133, 1 };
	RGB nephritis	{ 39, 174, 96, 1 };
	RGB beliz_hole	{ 41, 128, 185, 1 };
	RGB wisteria	{ 142, 68, 173, 1 };
	RGB midnight_blue { 44, 62, 80, 1 };
	RGB sunflower	{ 241, 196, 15, 1 };
	RGB carrot	{ 230, 126, 34, 1 };
	RGB alizarin	{ 231, 76, 60, 1 };
	RGB clouds	{ 236, 240, 241, 1 };
	RGB concrete	{ 149, 165, 166, 1 };
	RGB orange	{ 243, 156, 18, 1 };
	RGB pumpkin	{ 211, 84, 0, 1 };
	RGB pomegranate { 192, 57, 43, 1 };
	RGB silver	{ 189, 195, 199, 1 };
	RGB asbestos	{ 127, 140, 141, 1 };
} COLORS;


// Cursor and cursor clear symbols
#define	CUR_SYMBOL		"▋"
#define CLR_SYMBOL		" "

// Symbol cache item
struct
CACHE {
	std::size_t	idx;
	SDL_Texture	*symbol;
};

// Generic hashing scheme for const char*
constexpr std::size_t
SYMBOL_HASH( const char* c ) {
	std::size_t hash	= 0;
	std::size_t i		= 0;
	const std::size_t prime = 3313;
	
	while( c[i] != '\0' ) {
		hash = c[i] + ( hash * prime );
		++i;
	}
	
	return hash;
}

// Check symbols to see if index already exists
inline bool 
CACHED_IDX( 
	std::vector<CACHE>&	symbols, 
	std::size_t&		idx 
) {
	auto it = std::find_if( 
		symbols.begin(), symbols.end(),
		[ = ]( const CACHE& find ) -> bool {
			return find.idx == idx;
		}
	);
	
	// Symbol already is cached
	if( it != symbols.end() ) {
		return true;
	}
	
	return false;
}

// Check symbols to see if character is already cached
inline bool
CACHED_CHAR (
	std::vector<CACHE>&	symbols, 
	const char*		c,
	std::size_t&		idx
) {
	idx = SYMBOL_HASH( c );
	return CACHED_IDX( symbols, idx );
}


// Command line parameter ( -c value format )
struct
CMD_PARAM {
	std::string opt;
	std::string value;
};

// Command line parameter search
inline std::string
CMD_FIND(
	std::string		opt,	// Option to search
	std::string		def,	// Fallback value
	std::vector<CMD_PARAM>&	params	// Search params
) {
	auto it = std::find_if( 
		params.begin(), params.end(),
		[ = ]( const CMD_PARAM& find ) -> bool {
			return 
			find.opt == opt;
		}
	);
	
	if ( it != params.end() ) {
		return ( *it ).value;
	}
	return def;
}

class Huxley {
	private:
		/**
		 *  Main window parameters
		 */
		SDL_Window	*WINDOW;
		SDL_Renderer	*RENDERER;
		TTF_Font	*FONT;
		SDL_Color	COLOR;
		
		// Render cache
		std::vector<CACHE>
		SYMBOLS;
		
		RGB		BACKGROUND;
		
		Uint32 		WINDOW_FLAGS	= 
					SDL_WINDOW_RESIZABLE |
					SDL_WINDOW_ALLOW_HIGHDPI;
		
		
		// Document modified since opening
		bool		modified;
		bool		
		printFromCache( 
			const char* 	c, 
			std::size_t&	col,
			std::size_t&	ln
		);
		
		void		cacheSymbol( const char* c );
		
		/**
		 *  Special key input states
		 */
		struct
		SPECIAL_KEY {
			int left	= 0;
			int right	= 0;
			int any		= 0;
		} ctrl_key, alt_key, shift_key;
		
		// Running parameters
		struct
		CURRENT {
			int	w	= 0;
			int	h	= 0;
			int	x	= 0;
			int	y	= 0;
			int	win;
		} status;
		
		// Parameter holder
		std::vector<CMD_PARAM>	parameters;
		
		// Text direction
		unsigned char
		TEXT_DIR;
		
		// Cursor history
		HX_CURSOR last_cur	= { 0, 0 };
		
		/**
		 *  Base operations
		 */
		void	resetRender( RGB bg_color );
		void	setupFont( RGB fb_color );
		void	
		renderText(
			SDL_Texture	*textarea,
			std::size_t&	col,
			std::size_t&	ln
		);
		void	renderInput( Editor& editor );
		// Clear background and apply texture to main surface
		void
		cleanWrite( 
			int&		x, 
			int&		y, 
			int&		w, 
			int&		h, 
			SDL_Texture	*textarea 
		);
		
		/**
		 *  Determine if key pressed is movement or edit
		 */
		bool	movementKey( SDL_Keycode code );
		bool	editKey( SDL_Keycode code );
		
		/**
		 *  Window resizing, repositioning, etc...
		 */
		void	handleWindowEvents( SDL_Event &event );
		void	refresh();
		
		/**
		 *  Keyboard events
		 */
		
		// Keyboard text input
		void
		handleKeyInput( SDL_Event &event, Editor &editor );

		// Key press event
		void	handleKeyDown( SDL_Event &event );
		
		// Key release event
		void	handleKeyUp( SDL_Event &event );
		
		// Capture all key events
		void
		handleKeyEvents( SDL_Event &event, Editor &editor );
		
		/**
		 *  Set parameters based on command line options
		 */
		void	setDirection();
		void	setKeyMap( Editor& editor );
		void	setFile( Editor& editor );
	
	public:
		// Initialize
		Huxley( const char* title, int width, int height );
		
		// Currently working filename
		std::string		working_fname;
		
		// Parameter parsing
		void	parseParams( int argc, char* argv[] );
		void	setParams( Editor& editor );
		void	dumpParams( CMD_PARAM& param );
		
		/**
		 *  Filename generation
		 */
		void	rndFile( std::string& fname );
		void	genFile( std::string& fname );
		
		
		// Main event loop
		bool	eventLoop( Editor &editor );
		
		// Check if input was modified
		bool	isModified();
		
		// Notify if text has changed
		void	notifyByTitle();
		
		// Cleanup and end execution
		void	end( int e );
};

#endif
