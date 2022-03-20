#pragma once

#include "SDL.h"
#include "SDL_image.h"

using namespace std;
extern float preservationScale;

#include <iostream> // <- has shared_ptr
#include <vector>

class object {
public: 
	SDL_Rect srcR, destR;
	SDL_Renderer* renderer;

	SDL_Texture *tex;
	shared_ptr<object> parent;

	object(SDL_Renderer *rendererRef, SDL_Texture* tex, float h = 1.0f, float w = 1.0f, float x = 0.0f, float y = 0.0f);
	object();// <- for map to initialize

	virtual ~object();
};

shared_ptr<object>createObject(SDL_Renderer* renderer, SDL_Texture*tex, float h = 1.0f, float w = 1.0f, float x = 0.0f, float y = 0.0f);
