#include "texture/texture.h"
#include "object.h"

float preservationScale = 1000.0f;

object::object(SDL_Renderer* rendererRef, SDL_Texture* texture, float h, float w, float x, float y)
{
	tex = texture;
	renderer = rendererRef;
	
	srcR.x = 0;
	srcR.y = 0;

	destR.h = (int)(h * preservationScale);
	destR.w = (int)(w * preservationScale);

	destR.x = (int)(x * preservationScale);
	destR.y = (int)(y * preservationScale);

	SDL_Point size;
	SDL_QueryTexture(tex, NULL, NULL, &size.x, &size.y);

	srcR.h = size.y;
	srcR.w = size.x;
};
 
object::object() {}
object::~object() {}

 shared_ptr<object> createObject(SDL_Renderer* renderer, SDL_Texture*tex, float h, float w, float x, float y) {
	 shared_ptr<object> shared_obj = make_shared<object>(renderer, tex, h, w, x, y);
	 return shared_obj;
 }

