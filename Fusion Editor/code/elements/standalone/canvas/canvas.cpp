#include "canvas.h"

vector<shared_ptr<canvasObject>> canvasObjects = {};
shared_ptr<elementObj> canvasElement;

canvasObject::canvasObject(SDL_Renderer* renderer, string className) {
	listObj = createElement(renderer, className, elementTypes::frame, nullptr,
		(80.0f * 0.5f) / (float)windowSize.y,
		(67.0f * 0.5f) / (float)windowSize.x
	);

	elemObj = createElement(renderer, "textures/ui/popups/ADD NEW.png", elementTypes::draggable, canvasElement,
		(150.0f * 0.5f) / (float)windowSize.y,
		(150.0f * 0.5f) / (float)windowSize.x,
		(150.0f * 0.5f) / (float)windowSize.x,
		(150.0f * 0.5f) / (float)windowSize.y
	);

	// create list element
	// create draggable element for the interactive view
	// modify list element position based on interator in vector
	//canvasObjects.insert(canvasObjects, make_shared<canvasObject>(* this))
}


shared_ptr<canvasObject> createCanvasObject(SDL_Renderer* renderer, string classNames) {
	shared_ptr<canvasObject> sharedCanvasObject = make_shared<canvasObject>(renderer, classNames);
	//canvasObjects.insert(canvasObjects.end(), sharedCanvasObject);
	return sharedCanvasObject;
}

void makeCanvas(SDL_Renderer*renderer, SDL_Point wSize) {
	windowSize = wSize;

	canvasElement = createElement(renderer, "textures/ui/frames/ELEMENT VIEW BACKGROUND.png", elementTypes::frame, nullptr,
		(1080.0f * 0.5f) / (float)windowSize.y,
		(1367.0f * 0.5f) / (float)windowSize.x,
		(500.0f * 0.5f) / (float)windowSize.x
	);
}