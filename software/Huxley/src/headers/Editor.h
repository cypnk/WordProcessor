#ifndef EDITOR_H
#define EDITOR_H

#include <SDL2/SDL.h>

/**
 *  Text input
 */
extern char* input;
extern char* composition;

extern Sint32 cursor;
extern Sint32 selection;

void
sendCombo( int ctrl, int shift, const char* c );

#endif

