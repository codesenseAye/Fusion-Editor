#pragma once

#include <string>
using namespace std;

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

SDL_Texture* getTex(SDL_Renderer* renderer, const char* imageFilePath);
SDL_Texture* getTex(SDL_Renderer* renderer, string text);