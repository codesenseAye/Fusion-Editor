#include "frames.h"
#include <string>

SDL_Point windowSize;
float windowScale = 0.5f;

shared_ptr<elementObj> elementsList;

void loadFrames(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 50, 50, 255, 0);
	makeCanvas(renderer, windowSize);

	elementsList = createElement(renderer, "textures/ui/frames/LIST BACK.png", elementTypes::scrollingFrame, nullptr,
		(815.0f * windowScale) / (float)windowSize.y,
		(499.0f * windowScale) / (float)windowSize.x
	);

	shared_ptr<elementObj> propertiesView = createElement(renderer, "textures/ui/frames/PROPERTIES FRAME.png", elementTypes::frame, nullptr,
		(267.0f * 0.5f) / (float)windowSize.y,
		(499.0f * 0.5f) / (float)windowSize.x,
		0,
		(815.0f * 0.5f) / (float)windowSize.y
	);

	createElement(renderer, "textures/ui/frames/CODE VIEW BACKGROUND.png", elementTypes::frame, nullptr,
		(1080.0f * 0.5f) / (float)windowSize.y,
		(867.0f * 0.5f) / (float)windowSize.x,
		(1839.0f * 0.5f) / (float)windowSize.x
	);

	shared_ptr<elementObj> addNewPopup = createElement(renderer, "textures/ui/popups/ADD NEW.png", elementTypes::frame, nullptr,
		(166.0f * 0.5f) / (float)windowSize.y,
		(326.0f * 0.5f) / (float)windowSize.x,
		(119.0f * 0.5f) / (float)windowSize.x,
		(67.0f * 0.5f) / (float)windowSize.y
	);

	addNewPopup->visible = false;

	shared_ptr<elementObj> toggleButton = createElement(renderer, "textures/ui/buttons/TOGGLE NEW.png", elementTypes::button, nullptr,
		(52.0f * 0.5f) / (float)windowSize.y,
		(42.0f * 0.5f) / (float)windowSize.x,
		(399.0f * 0.5f) / (float)windowSize.x,
		(7.0f * 0.5f) / (float)windowSize.y
	);

	clickable* onClicked = new clickable([addNewPopup]() mutable {
		(*addNewPopup).visible = !(*addNewPopup).visible;
		});

	(*toggleButton).onClicked = onClicked;

	shared_ptr<elementObj> addNewInnerScrollFrame = createElement(renderer, "", elementTypes::scrollingFrame, addNewPopup,
		(150.0f * 0.5f) / (float)windowSize.y,
		(310.0f * 0.5f) / (float)windowSize.x,
		(9.0f * 0.5f) / (float)windowSize.x,
		(8.0f * 0.5f) / (float)windowSize.y
	);

	shared_ptr<elementObj> addFrameButton = createElement(renderer, (string)"FRAME", elementTypes::button, addNewInnerScrollFrame,
		(40.0f * 0.5f) / (float)windowSize.y,
		(180.0f * 0.5f) / (float)windowSize.x,
		(65.0f * 0.5f) / (float)windowSize.x,
		(0.0f * 0.5f) / (float)windowSize.y
	);

	clickable* onClickedAddFrame = new clickable([addFrameButton, renderer]() mutable {
		createCanvasObject(renderer, "FRAME");
	});

	(*addFrameButton).onClicked = onClickedAddFrame;

	shared_ptr<elementObj> addTextButton = createElement(renderer, (string)"TEXT BUTTON", elementTypes::button, addNewInnerScrollFrame,
		(40.0f * 0.5f) / (float)windowSize.y,
		(180.0f * 0.5f) / (float)windowSize.x,
		(65.0f * 0.5f) / (float)windowSize.x,
		(40.0f * 0.5f) / (float)windowSize.y
	);

	clickable* onClickedAddTextButton = new clickable([onClickedAddTextButton, renderer]() mutable {
		createCanvasObject(renderer, "IMAGE LABEL");
	});

	(*addTextButton).onClicked = onClickedAddTextButton;

	shared_ptr<elementObj> addImageLabel = createElement(renderer, (string)"IMAGE LABEL", elementTypes::button, addNewInnerScrollFrame,
		(40.0f * 0.5f) / (float)windowSize.y,
		(180.0f * 0.5f) / (float)windowSize.x,
		(65.0f * 0.5f) / (float)windowSize.x,
		(80.0f * 0.5f) / (float)windowSize.y
	);

	clickable* onClickedAddImageLabel = new clickable([onClickedAddTextButton, renderer]() mutable {
		createCanvasObject(renderer, "IMAGE LABEL");
	});

	(*addImageLabel).onClicked = onClickedAddImageLabel;

	shared_ptr<elementObj> addImageButton = createElement(renderer, (string)"IMAGE BUTTON", elementTypes::button, addNewInnerScrollFrame,
		(40.0f * 0.5f) / (float)windowSize.y,
		(180.0f * 0.5f) / (float)windowSize.x,
		(65.0f * 0.5f) / (float)windowSize.x,
		(120.0f * 0.5f) / (float)windowSize.y
	);

	clickable* onClickedAddImageButton = new clickable([onClickedAddTextButton, renderer]() mutable {
		createCanvasObject(renderer, "IMAGE BUTTON");
	});

	(*addImageButton).onClicked = onClickedAddImageButton;
}