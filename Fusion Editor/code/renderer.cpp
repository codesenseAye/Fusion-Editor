#define SDL_MAIN_HANDLED

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <thread>
#include <chrono>
#include <iostream>

#include <string>
using namespace std;

#include "texture/texture.h"
#include "object/object.h"

#include "build/background/frames.h"

int main(int* argc, char* argv[]) {
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	SDL_Event event;

	const int FRAMES_PER_SEC = 60;
	const float frameTime = 1000.0f / (float)FRAMES_PER_SEC;

	windowSize.x = (int)(2706.0f * 0.5f);
	windowSize.y = (int)(1080.0f * 0.5f);

	SDL_Window* window = SDL_CreateWindow("Fusion Editor", 200, 200, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	loadFrames(renderer);
	vector<shared_ptr<SDL_Texture*>> lineNums = {}; // pass the texture's pointer as a reference (*&)

	for (int i = 0; i < 86; i++) {
		SDL_Texture* line = getTex(renderer, to_string(i));
		lineNums.insert(lineNums.end(), make_shared<SDL_Texture*>(line));
	}

	bool editorRunning = true;

	while (editorRunning) {
		SDL_RenderClear(renderer);

		int mX = -1;
		int mY = -1;

		bool mDown = false;
		bool mUp = false;

		scrolling = false;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				cout << "Key press detected" << endl;
				break;

			case SDL_KEYUP:
				cout << "Key release detected" << endl;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mX = event.button.x;
				mY = event.button.y;

				mDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mX = event.button.x;
				mY = event.button.y;

				mUp = true;
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.button.x;
				mouseY = event.button.y;
				break;
			case SDL_MOUSEWHEEL:
				wheelX = event.wheel.x;
				wheelY = event.wheel.y;

				scrolling = true;

				break;
			case SDL_QUIT:
				editorRunning = false;
				break;
			default:
				break;
			}
		}

		captureWindowSize(window);

		for (shared_ptr<elementObj> element: elements) {
			if (!element->visible) {
				continue;
			}

			if (!mDown && !mUp) {
				element->update();
			}
			else {
				element->update(&mX, &mY, mDown ? mState::mDown : mUp ? mState::mUp : mState::mNone);
			}
		}

		int numLine = 0;

		for (shared_ptr<SDL_Texture*> lineNum : lineNums) {
			SDL_Rect destR;

			destR.h = getScale(25, 1);
			destR.w = getScale(6, 0);

			destR.x = getScale(682, 0);
			destR.y = getScale((int)(2 + (float)numLine * 25.0f), 1);
			
			SDL_RenderCopy(renderer, *lineNum, NULL, &destR);
			numLine++;
		}

		int canvasObjectNum = 0;

		for (shared_ptr<canvasObject> canvasObj : canvasObjects) {

			(*(*canvasObj).listObj).parent = elementsList;
			canvasObjectNum++;
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(frameTime);
	}

	return 0;
}