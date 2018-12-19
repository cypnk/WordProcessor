#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "HXTypes.h"
#include <SDL2/SDL.h>

/**
 *  Keyboard command mapping
 */

// https://wiki.libsdl.org/SDLKeycodeLookup
HX_COMMAND QWERTY_MAP[] = {
	{ SDLK_UP,		0, 0, 0, M_UP },		// 1
	{ SDLK_e,		1, 0, 0, M_UP },		// 2
	
	{ SDLK_DOWN,		0, 0, 0, M_DOWN },		// 3
	{ SDLK_x,		1, 0, 0, M_DOWN },		// 4
	
	{ SDLK_LEFT,		0, 0, 0, M_LEFT },		// 5
	{ SDLK_s,		1, 0, 0, M_LEFT },		// 6
	
	{ SDLK_RIGHT,		0, 0, 0, M_RIGHT },		// 7
	{ SDLK_d,		1, 0, 0, M_RIGHT },		// 8
	
	{ SDLK_HOME,		0, 0, 0, M_LNSTART },		// 9
	{ SDLK_k,		1, 0, 0, M_LNSTART },		// 10
	
	{ SDLK_END,		0, 0, 0, M_LNEND },		// 11
	{ SDLK_l,		1, 0, 0, M_LNEND },		// 12
	
	// Line scroll
	{ SDLK_w,		1, 0, 0, M_SCRLUP },		// 13
	{ SDLK_z,		1, 0, 0, M_SCRLDN },		// 14
	
	// Pagination
	{ SDLK_PAGEUP,		0, 0, 0, M_PGUP },		// 15
	{ SDLK_r,		1, 0, 0, M_PGUP },		// 16
	
	{ SDLK_PAGEDOWN,	0, 0, 0, M_PGDN },		// 17
	{ SDLK_c,		1, 0, 0, M_PGDN },		// 18
	
	{ SDLK_HOME,		1, 0, 0, M_DSTART },		// 19
	{ SDLK_COMMA,		1, 0, 0, M_DSTART },		// 20
	
	{ SDLK_END,		1, 0, 0, M_DEND },		// 21
	{ SDLK_PERIOD,		1, 0, 0, M_DEND },		// 22
	
	// Move cursor to last position
	{ SDLK_j,		1, 0, 0, M_CUR },		// 23
	// Move cursor to last position
	{ SDLK_j,		1, 1, 0, M_LCUR },		// 24
	
	// Selections
	{ SDLK_LEFT,		0, 1, 0, S_LEFT },		// 25
	{ SDLK_a,		1, 0, 0, S_LEFT },		// 26
	
	{ SDLK_RIGHT,		0, 1, 0, S_RIGHT },		// 27
	{ SDLK_f,		1, 0, 0, S_RIGHT },		// 28
	
	// Find
	{ SDLK_q,		1, 0, 0, T_QUERY },		// 29
	
	// Editing
	{ SDLK_DELETE,		0, 0, 0, E_DELR },		// 30
	{ SDLK_g,		1, 0, 0, E_DELL },		// 31
	{ SDLK_h,		1, 0, 0, E_DELR },		// 32
	
	{ SDLK_t,		1, 0, 0, E_DELWD },		// 33
	{ SDLK_y,		1, 0, 0, E_DELLN },		// 34
	
	// Clipboard
	{ SDLK_v,		1, 0, 0, C_CLIP },		// 35
	
	// Insert line break
	{ SDLK_b,		1, 0, 0, E_BREAK },		// 36
	
	// Copy paste
	{ SDLK_LEFTBRACKET, 	1, 0, 0, C_COPY },		// 37
	{ SDLK_RIGHTBRACKET,	1, 0, 0, C_PASTE },		// 38
	
	// Memo / Citation
	{ SDLK_m,		1, 0, 0, P_MEMO },		// 39
	
	// History
	{ SDLK_u,		1, 0, 0, H_UNDO },		// 40
	{ SDLK_u,		1, 1, 0, H_REDO },		// 41
	
	// Document handling
	{ SDLK_n,		1, 0, 0, T_NEW },		// 42
	{ SDLK_o,		1, 0, 0, T_OPEN },		// 43
	
	// Indentation
	{ SDLK_TAB,		0, 0, 0, T_INDENT },		// 44
	{ SDLK_i,		1, 0, 0, T_INDENT },		// 45
	
	{ SDLK_TAB,		1, 0, 0, T_ODENT },		// 46
	{ SDLK_i,		1, 1, 0, T_ODENT },		// 47
	
	// Formatting
	{ SDLK_t,		1, 1, 0, T_SUP },		// 48
	{ SDLK_v,		1, 1, 0, T_SUB },		// 49
	{ SDLK_b,		1, 1, 0, T_BOLD },		// 50
	{ SDLK_y,		1, 1, 0, T_ITALIC },		// 51
	{ SDLK_s,		1, 1, 0, T_UNDER },		// 52
	
	// Text deletion
	{ SDLK_BACKSPACE,	0, 0, 0, E_DELL },		// 53
	
	// Delete to start/end of line
	{ SDLK_BACKSPACE,	1, 0, 0, E_DELSL },		// 54
	{ SDLK_BACKSPACE,	1, 1, 0, E_DELEL },		// 55
	
	// Insert line break
	{ SDLK_RETURN,		0, 0, 0, T_BREAK },		// 56
	{ SDLK_RETURN,		1, 1, 0, E_BREAK }		// 57
};


#endif
