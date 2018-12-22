#ifndef EDITOR_H
#define EDITOR_H

#include <cstring>
#include <string>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <SDL2/SDL.h>
#include "HXTypes.h"

// Checksum primes
#define	CHK_A		81667
#define	CHK_B		83311
#define	CHK_C		90173


class Editor {
	private:
		
		// Collection for corrupted lines
		std::vector<std::size_t> bad_lines;
		
		// Current keyboard map
		HX_COMMAND *KEY_MAP;
		
		// Current working document
		HX_FILE	working_doc;
		
		// Size of the current key map
		int working_map				= 0;
		
		// Current line being edited
		std::size_t working_line		= 0;
		
		// Current working input
		std::string working_str;
		
		bool caps				= false;
		
		// Last cusror position
		std::vector<HX_CURSOR> cur_history;
		
		// Keyboard input
		std::vector<HX_HISTORY> content;
		
		// Split a line by tokens
		void
		splitLine( 
			std::string& line, 
			std::vector<std::string>& words 
		);
		
		// Document working limits
		void	workingLimit( std::size_t& index );
		
		/**
		 *  Client input handling
		 */
				// Generate checksum from string
		static std::size_t
		toCHK( std::string& line );
		
		static int 
		endsWith(
			const char*	str,
			const char*	suffix
		);
		
		// Copy from string to checksum size_t
		static void
		fromCHK(
			std::string&		block, 
			std::size_t&		chk
		);
		
		// Copy checksum to char array
		static void 
		copyCHK(
			char*			check,
			std::size_t&		chk
		);
		
		/**
		 *  Separatator detection
		 */
		static bool	isBreak( int c );
		static bool	isSpace( const char* c );
		
		/**
		 *  Content chunking
		 */
		static void
		breakSegments( 
			std::string&			working, 
			std::vector<std::string>&	segments,
			bool				append	= false
		);
		
		
		/**
		 *  Flags
		 */
		// Unicode text input active
		bool	INPUT_ACTIVE	= false;
		
		// Apply requested command
		void	applyCommand( unsigned char action );
		
		// Open file
		void
		openDoc( std::string& fname, HX_FILE& dest );
		
		// Save file
		void
		saveDoc( std::string& fname, HX_FILE& source );
		
		/**
		 *  Data handling
		 */
		
		// Convert from raw string to format stack
		void
		extractFormat(
			std::string&	block,
			std::vector<HX_FORMAT>& fmt 
		);
		
		// Convert from raw string to document line
		void
		extractLine(
			std::size_t&	chk,
			std::string&	line,
			std::string&	extracted,
			std::vector<HX_FORMAT>& fmt
		);
		
		// Process line segments
		void
		segmentsToLine( 
			std::vector<std::string>&	segments, 
			std::size_t&			index,
			std::size_t&			chk,
			std::vector<HX_FORMAT>&		fmt,
			HX_FILE& 			dest
		);
		void
		segmentsToFile( 
			std::vector<std::string>&	segments, 
			std::size_t&			index,
			std::size_t&			chk,
			std::vector<HX_FORMAT>&		fmt,
			HX_FILE& 			dest
		); 
		
		// Convert from format stack to string
		void
		saveFormat(
			std::string&	block,
			std::vector<HX_FORMAT>& fmt 
		);
		
		void
		appendDoc(
			std::string&	line, 
			HX_FILE& 	dest,
			std::size_t	index,
			unsigned char	ftype
		);
	public:
		Editor( unsigned char key_map );
		
		void	capslock();
		
		// Checksum passed
		bool			good	= true;
		
		// Open a given filename as a Huxley document
		void	cmdOpen( std::string& fname );
		
		// Save working doument to filename
		void	cmdSave( std::string& fname );
		
		// Add last input phrase to history
		void	syncInput( bool line = false );
		
		// Select lines from raw input and sync sentence
		void	syncLine();
		
		// Get the line at given index
		void	lineAt( std::size_t& index, HX_LINE& line );
		
		// Receive keyboard input
		void
		sendInput( char* edit, Sint32 cursor, Sint32 len );
		
		// Receive keyboard key combination
		void
		sendCombo( int ctrl, int shift, SDL_Keycode &code );
};

#endif
