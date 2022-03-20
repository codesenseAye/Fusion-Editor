#include "texture.h"

// texture creator version for loading images
SDL_Texture * getTex(SDL_Renderer*renderer, const char* imageFileName) {
	SDL_RenderClear(renderer);
	SDL_Surface* tempSurface = IMG_Load(imageFileName);

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

// texture creator version for loading text
SDL_Texture* getTex(SDL_Renderer* renderer, string text) {
	SDL_RenderClear(renderer);
	
	static SDL_Color textColor = { 0, 0, 0 };
	static TTF_Font* sans = TTF_OpenFont("Sans.ttf", 50);
	
	SDL_Surface* textSurface = TTF_RenderText_Solid(sans, text.c_str(), textColor);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);

	return texture;
}