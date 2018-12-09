#include <ctype.h>
#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 *  Main window parameters
 */

// Global window
SDL_Window			*WINDOW;
SDL_Renderer		*RENDERER;

const int 
WINDOW_WIDTH		= 800;

const int 
WINDOW_HEIGHT		= 600;

const char* 
WINDOW_TITLE		= "Huxley";

void
end();

