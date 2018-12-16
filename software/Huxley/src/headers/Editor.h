#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include "HXTypes.h"

// Idea borrowed from Salvatore Sanfilippo's ( antirez ) Kilo editor
// https://github.com/antirez/kilo
#define IS_BREAK( c ) \
	( c == '\0' || isspace( c ) || strchr( END_MKR, c ) != NULL )

// Word separator ( for languages that use spaces )
#define IS_SPACE( c ) \
	( strchr( " ", c ) != NULL )

/**
 *  Text input
 */
extern char* input;
extern char* composition;

class Editor {
	private:
		
		// Current working document
		HX_FILE	working_doc;
		
		// Last cusror position
		std::vector<HX_CURSOR> cur_history;
		
		// Keyboard input
		std::vector<HX_HISTORY> content;
		
		//	Open a given filename as a Huxley document
		void cmdOpen( const char* fname );
		
	public:
		Editor( unsigned char key_map );
		
		void	capslock();
		
		// Receive keyboard key combination
		void
		sendCombo( int ctrl, int shift, SDL_Keycode &code );
		
		// Apply requested command
		void	applyCommand( unsigned char action );
};

#endif
