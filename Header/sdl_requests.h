#ifndef SDLREQUESTS_H
#define SDLREQUESTS_H

bool SDLGetIsCloseRequested();
void SDLSetIsCloseRequested(bool value);
void SDLCreateWindow(const char* title, int x, int y, int width, int height, bool fullscreen);
void SDLSetWindowFullscreen(bool value);
void SDLSwapBuffers();
void SDLDestroyWindow();
void SDLSetMousePosition(int x, int y);

#endif // !SDLREQUESTS_H
