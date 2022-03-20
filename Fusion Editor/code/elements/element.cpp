#include "element.h"
#include "texture/texture.h"

int wX = 0;
int wY = 0;

#define BUTTON_RESTORE_TIME 300.0f

namespace elementTypes {
	int button = 0;
	int label = 1;
	int frame = 2;
	int scrollingFrame = 3;
	int draggable = 4;
};

namespace mState {
	int mDown = 0;
	int mUp = 1; 
	int mNone = -1;
}

int SCROLL_POWER = 10;

int mouseX = 0, mouseY = 0;
int wheelX = 0, wheelY = 0;

bool scrolling = false;

vector<shared_ptr<elementObj>> elements = {};

void captureWindowSize(SDL_Window*window) {
	SDL_GetWindowSize(window, &wX, &wY);
}

int getScale(int value, char axis) {
	return (int)(
		(value / preservationScale) * (axis == 0 ? (float)wX : (float)wY)
	);
}

void updateButton(elementObj* element, bool clicked) {
	if (clicked) {
		element->lastClicked = clock();

		if (element->onClicked != nullptr) {
			element->onClicked->callback();
		}
	}

	float timeSinceLastClick = clock() - element->lastClicked;
	SDL_Rect newRect = {};

	float timePassed = min(1.0f, 0.5f + (timeSinceLastClick / BUTTON_RESTORE_TIME));

	newRect.h = element->objSize.h * timePassed;
	newRect.w = element->objSize.w * timePassed;

	newRect.x = element->objSize.x - ((newRect.w - element->objSize.w) / 2);
	newRect.y = element->objSize.y - ((newRect.h - element->objSize.h) / 2);

	(*element->obj).destR = newRect;
}

void updateDraggable(elementObj* element, int mStateValue, bool isMouseOver) {
	if (!element->dragging && mStateValue == mState::mDown && !isMouseOver) {
		return;
	}

	bool thisDragging = element->dragging;

	if (mStateValue == mState::mDown) {
		thisDragging = true;

		element->dragOffsetX = (*element->obj).destR.x - mouseX;
		element->dragOffsetY = (*element->obj).destR.y - mouseY;
	}
	else if (mStateValue == mState::mUp) {
		thisDragging = false;
	}

	if (element->dragging && thisDragging == element->dragging) {
		(*element->obj).destR.x = mouseX + element->dragOffsetX;
		(*element->obj).destR.y = mouseY + element->dragOffsetY;
		return;
	}

	element->dragging = thisDragging;
}

void updateScrollingFrame(elementObj* element, bool isMouseOver) {
	if (!scrolling) {
		return;
	}
	
	if (!isMouseOver) {
		return;
	}

	SDL_Rect destR;
	bool* visible;
	getScaledElement(destR, element, visible, true);

	int contentYSize = element->contentMaxY - element->contentMinY;
	int scrollableDistance = contentYSize - destR.h;

	if (scrollableDistance < 0) {
		scrollableDistance = 0;
	}

	cout << scrollableDistance << endl;
	element->scrollOffsetY = max(0, min(element->scrollOffsetY + (wheelY * SCROLL_POWER), scrollableDistance));
}

shared_ptr<elementObj> createElement(SDL_Renderer* renderer, const char* imageFileName, int elementType, shared_ptr<elementObj> parent, float h, float w, float x, float y)
{
	SDL_Texture* imageTex = getTex(renderer, imageFileName);
	shared_ptr<object> obj = createObject(renderer, imageTex, h, w, x, y);
	
	shared_ptr<elementObj> element = make_shared<elementObj>(obj, parent, elementType);
	elements.insert(elements.end(), element);
	
	return element;
}

shared_ptr<elementObj> createElement(SDL_Renderer* renderer, string text, int elementType, shared_ptr<elementObj> parent, float h, float w, float x, float y)
{
	SDL_Texture* imageTex = getTex(renderer, text);
	shared_ptr<object> obj = createObject(renderer, imageTex, h, w, x, y);

	shared_ptr<elementObj> element = make_shared<elementObj>(obj, parent, elementType);
	elements.insert(elements.end(), element);

	return element;
}

clickable::clickable(std::function<void()> passedCallback) {
	callback = passedCallback;
}

const bool elementObj::checkMouseOver(SDL_Rect destR, int* mX, int* mY) {
	if (mX == nullptr) {
		return false;
	}
	
	if (mY == nullptr) {
		return false;
	}

	int x = *mX;
	int y = *mY;

	if (destR.x > x) {
		return false;
	}

	if (destR.y > y) {
		return false;
	}

	if (destR.x + destR.w < x) return false;
	if (destR.y + destR.h < y) return false;
	
	return true;
}

void getScaledElement(SDL_Rect&destR, elementObj* element, bool*&isVisible, bool dontIncludeScroll) {
	int w = (*element->obj).destR.w;
	int h = (*element->obj).destR.h;

	int x = (*element->obj).destR.x;
	int y = (*element->obj).destR.y;

	shared_ptr<elementObj> parentElement = element->parent;

	bool appliedScroll = dontIncludeScroll;
	int addedScrollOffsetY = 0;

	while (parentElement != nullptr) {
		bool parentVisible = (*parentElement).visible;

		if (!parentVisible) {
			isVisible = &parentVisible;
		}
		
		SDL_Rect parentDestR = (*(*parentElement).obj).destR;

		if (!appliedScroll) {
			appliedScroll = true;

			y -= parentElement->scrollOffsetY;
			addedScrollOffsetY += parentElement->scrollOffsetY;
		}

		int addingX = parentDestR.x;
		int addingY = parentDestR.y;

		x += addingX;
		y += addingY;

		parentElement = (*parentElement).parent;
	}

	destR.w = getScale(w, 0);
	destR.h = getScale(h, 1);

	destR.x = getScale(x, 0);
	destR.y = getScale(y, 1);

	shared_ptr<elementObj> directParent = element->parent;
	
	if (!directParent) {
		return;
	}

	int elementBottomY = destR.y + destR.h + addedScrollOffsetY;

	if ((*directParent).contentMinY > destR.y + addedScrollOffsetY) {
		(*directParent).contentMinY = destR.y + addedScrollOffsetY;
	}

	if ((*directParent).contentMaxY < elementBottomY) {
		(*directParent).contentMaxY = elementBottomY;
	}
}


void elementObj::updateClipping(SDL_Rect &dR, SDL_Rect& sR, int w, int h, int x, int y) {
	shared_ptr<elementObj>* parentElement = &(*this).parent; // dont hold a reference (use pointer instead) because itll delete the parent

	if (!*parentElement) {
		return;
	}

	do {

		SDL_Rect parentDestR;
		bool* visible;

		getScaledElement(parentDestR, &**parentElement, visible);

		int parentEndX = (parentDestR.x + parentDestR.w);
		int elementEndX = (x + w);

		int xExtend = (elementEndX - parentEndX);
		int xBefore = parentDestR.x - x;

		if (xExtend > 0) {
			float clipXPerc = (float)xExtend / (float)dR.w;
			int xClip = clipXPerc * (float)sR.w;

			sR.x -= xClip;
			dR.w -= xExtend;
		}
		
		if (xBefore > 0) {
			float clipXPerc = (float)xBefore / (float)dR.w;
			int xClip = clipXPerc * (float)sR.w;

			sR.x += xClip;
			sR.w -= xClip;

			dR.w -= xBefore;
			dR.x += xBefore;
		}

		int parentEndY = (parentDestR.y + parentDestR.h);
		int elementEndY = (y + h);

		int yExtend = (elementEndY - parentEndY);
		int yBefore = parentDestR.y - y;

		if (yExtend > 0) {
			float clipYPerc = (float)yExtend / (float)dR.h;
			int yClip = clipYPerc * (float)sR.h;

			sR.y -= yClip;
			dR.h -= yExtend;
		}

		if (yBefore > 0) {
			float clipYPerc = (float)yBefore / (float)dR.h;
			int yClip = clipYPerc * (float)sR.h;

			sR.y += yClip;
			sR.h -= yClip;

			dR.h -= yBefore;
			dR.y += yBefore;
		}

		shared_ptr<elementObj>* parentsParent = &(**parentElement).parent;

		if (!*parentsParent) break;
		parentElement = parentsParent;
	} while (*parentElement);
}


void elementObj::getScaledValues(SDL_Rect & dR, SDL_Rect& sR, int*& w, int*& h, int*& x, int*& y, bool*& isVisible) {
	sR = (*obj).srcR;

	w = &(*obj).destR.w;
	h = &(*obj).destR.h;

	x = &(*obj).destR.x;
	y = &(*obj).destR.y;

	getScaledElement(dR, this, isVisible);

	x = &dR.x;
	y = &dR.y;

	w = &dR.w;
	h = &dR.h;
}

void elementObj::update(int*mX, int*mY, int mStateValue) {
	SDL_Rect dR, sR;
	int* w = nullptr, * h = nullptr, * x = nullptr, * y = nullptr;
	bool* isVisible = &this->visible;

	getScaledValues(dR, sR, w, h, x, y, isVisible);

	if (*isVisible) {
		if (elementType == elementTypes::button) {
			if (checkMouseOver(dR, mX, mY)) {
				if (mStateValue == mState::mDown) updateButton(this, true);
			}
			else if (elementType == elementTypes::button) {
				updateButton(this);
			}
		}
		else if (elementType == elementTypes::draggable) {
			updateDraggable(this, mStateValue, checkMouseOver(dR, mX, mY));
		}
		else if (elementType == elementTypes::scrollingFrame) {
			updateScrollingFrame(this, checkMouseOver(dR, &mouseX, &mouseY));
		}

		updateClipping(dR, sR, *w, *h, *x, *y);
		SDL_RenderCopy((*obj).renderer, (*obj).tex, &sR, &dR);
	}

	this->contentMinY = 0;
	this->contentMaxY = 0;
}

elementObj::elementObj(shared_ptr<object> objRef, shared_ptr<elementObj> passedParent, int elementTypeRef) {
	obj = objRef;
	elementType = elementTypeRef;

	objSize = (*objRef).destR;
	parent = passedParent;
}


elementObj::~elementObj() {
	cout << "deleted" << endl;
}