#pragma once
#include "elements/standalone/canvas/canvas.h"

extern SDL_Point windowSize;
extern float windowScale;

extern void loadFrames(SDL_Renderer* renderer);
extern shared_ptr<elementObj> elementsList;