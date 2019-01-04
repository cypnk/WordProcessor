#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <iterator>
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



/**
 *  Client input handling
 */
// Generate checksum from string
constexpr std::size_t
TO_CHK( const char* str ) {
	std::size_t chk	= 0;
	while( *str ) {
		chk = ( chk * CHK_A ) ^ ( str[0] * CHK_B );
		str++;
	}
	return chk % CHK_C;
}

// Copy checksum to char array
constexpr void 
COPY_CHK(
	char*			check,
	std::size_t&		chk
) {
	snprintf( check, CHK_SIZE, CHK_FORMAT, chk );	
}


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
		
		// Current location being edited
		HX_CURSOR working_cur;
		
		// Current working line
		HX_LINE working_line;
		
		// Current working input
		std::string working_str;
		
		
		bool caps				= false;
		
		// Last cusror position
		std::vector<HX_CURSOR> cur_history;
		
		// Keyboard input
		std::vector<HX_HISTORY> content;
		
		// Move cursor within limits
		void
		moveCursor( int x = 0, int y = 0 );
		
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
		 *  Separator detection
		 */
		static bool	isBreak( int c );
		static bool	isSpace( const char* c );
		
		/**
		 *  Content chunking
		 */
		void
		breakSegments( 
			std::string&			working, 
			std::vector<std::string>&	segments,
			bool				append	= false
		);
		
		// Set the working string to the current line
		void		setWorking();
		void		
		syncMoveSet(
			int	x, 
			int	y, 
			bool	line = false 
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
			std::size_t&			chk,
			std::vector<HX_FORMAT>&		fmt,
			HX_FILE& 			dest
		);
		void
		segmentsToFile( 
			std::vector<std::string>&	segments, 
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
		
		// Add last input phrase to history
		void
		syncInput( 
			HX_LINE&	line,
			std::string&	working, 
			bool 		nline	= false, 
			bool		pg	= false
		);
		
		// Select lines from raw input and sync sentence
		void	syncLine();
		
		/**
		 *  Editing functions
		 */
		// Delete left of cursor
		void	delLeft( int x );
		
		// Delete right of cursor
		void	delRight( int x );
		
	public:
		Editor();
		
		void	capslock();
		
		// Append working string to document
		void	sync();
		
		// Get the current working string and cursor position
		void
		getWorking(
			HX_CURSOR&	cur,
			std::string& 	working
		);
		
		// Testing
		void	printCursor();
		
		// Checksum passed
		bool			good	= true;
		
		// Document loaded
		bool			loaded	= false;
		
		// Open a given filename as a Huxley document
		void	cmdOpen( std::string& fname );
		
		// Save working doument to filename
		void	cmdSave( std::string& fname );
		
		// Get the line at given index
		void	lineAt( std::size_t& index, HX_LINE& line );
		
		// Get the current working document
		void	docAt( HX_FILE& doc );
		
		// Set keyboard map
		void	setKeyMap( unsigned char key_map );
		
		// Receive keyboard input
		void
		sendInput( char* edit, Sint32 cursor, Sint32 len );
		
		// Receive keyboard key combination
		void
		sendCombo( int ctrl, int shift, SDL_Keycode &code );
};

#endif
