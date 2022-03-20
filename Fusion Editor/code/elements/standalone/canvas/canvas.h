#pragma once
#include "elements/element.h"

class canvasObject {
public:
	shared_ptr<elementObj> elemObj;
	shared_ptr<elementObj> listObj;

	canvasObject(SDL_Renderer* renderer, string className);
};

extern vector<shared_ptr<canvasObject>> canvasObjects;
shared_ptr<canvasObject> createCanvasObject(SDL_Renderer* renderer, string classNames);

void makeCanvas(SDL_Renderer* renderer, SDL_Point windowSize);
extern shared_ptr<elementObj> canvasElement;

extern SDL_Point windowSize;