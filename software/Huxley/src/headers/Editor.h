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
		HX_COMMAND *KEY_MAP;
		
		// Current working document
		HX_FILE	working_doc;
		
		// Current working input
		std::string working_str;
		
		// Last cusror position
		std::vector<HX_CURSOR> cur_history;
		
		// Keyboard input
		std::vector<HX_HISTORY> content;
		
		// Add last input phrase to history
		void	syncInput();
		
		// Open a given filename as a Huxley document
		void	cmdOpen( std::string& fname );
		
		// Save working doument to filename
		void	cmdSave( std::string& fname );
		
		// Apply requested command
		void	applyCommand( unsigned char action );
		
	public:
		Editor( unsigned char key_map );
		
		void	capslock();
		
		// Receive keyboard input
		void
		sendInput( char* edit, Sint32 cursor, Sint32 len );
		
		// Receive keyboard key combination
		void
		sendCombo( int ctrl, int shift, SDL_Keycode &code );
};

#endif
