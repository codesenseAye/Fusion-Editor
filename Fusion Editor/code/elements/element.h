#pragma once
#include "SDL.h"
#include <functional>

#include <iostream>
using namespace std;

#ifndef OBJECT
#define OBJECT
#include "object/object.h"
#endif

extern int wX;
extern int wY;

extern void captureWindowSize(SDL_Window*window);
extern int getScale(int value, char axis);

extern int mouseX, mouseY;
extern int wheelX, wheelY;

extern bool scrolling;
extern int SCROLL_POWER;

namespace elementTypes {
	extern int button;
	extern int label;
	extern int frame;
	extern int scrollingFrame;
	extern int draggable;
};

namespace mState {
	extern int mDown;
	extern int mUp;
	extern int mNone;
}

class clickable {
public:
	std::function<void()> callback;
	clickable(std::function<void()> passedCallback);
};

class elementObj {
public:
	elementObj(shared_ptr<object> objRef, shared_ptr<elementObj> passedParent, int elementType);
	shared_ptr<object>obj;
	SDL_Rect objSize; // saved original size for animations

	float lastClicked = 0;
	clickable* onClicked;

	shared_ptr<elementObj> parent;
	bool visible = true;
	int scrollOffsetY = 0;

	int dragOffsetX = 0;
	int dragOffsetY = 0;

	int contentMaxY = 0;
	int contentMinY = 0;

	int elementType;
	void update(int* mX = nullptr, int* mY = nullptr, int mState = -1);
	void updateClipping(SDL_Rect &dR, SDL_Rect &sR, int w, int h, int x, int y);

	void getScaledValues(SDL_Rect& dR, SDL_Rect& sR, int*& w, int*& h, int*& x, int*& y, bool*& isVisible);
	bool dragging = false;

	const bool checkMouseOver(SDL_Rect destR, int* mX = nullptr, int* mY = nullptr);
	~elementObj();
};

shared_ptr<elementObj>createElement(SDL_Renderer* renderer, const char* imageFileName, int elementType, shared_ptr<elementObj> parent, float h = 1.0f, float w = 1.0f, float x = 0.0f, float y = 0.0f);
shared_ptr<elementObj>createElement(SDL_Renderer* renderer, string text, int elementType, shared_ptr<elementObj> parent, float h = 1.0f, float w = 1.0f, float x = 0.0f, float y = 0.0f);

void updateButton(elementObj* element, bool clicked = false);
extern vector<shared_ptr<elementObj>> elements;

void updateDraggable(elementObj* element, int mStateValue, bool isMouseOver);
void getScaledElement(SDL_Rect& destR, elementObj* element, bool*& isVisible, bool dontIncludeScroll = false);

void updateScrollingFrame(elementObj* element, bool isMouseOver);