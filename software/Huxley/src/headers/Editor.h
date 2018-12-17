#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include "HXTypes.h"

/**
 *  Text input
 */
extern char* input;
extern char* composition;

class Editor {
	private:
	
		// Current keyboard map
		command *KEY_MAP;
		
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
