#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "includes.h"
#include "framework.h"
#include "image.h"
#include "game.h"
#include "camera.h"

class Sprite
{
public:
	Vector2 pos;
	Image* image;
	Camera* camera;

	enum { PLAYER, OTHER};
	int type;

	Sprite();
	Sprite(int pos_x, int pos_y, Image* _image, Camera* _camera, int _type);

	void render(Image& fbuff, Area rect);
};

#endif