#include "sprite.h"

Sprite::Sprite() {};

Sprite::Sprite(int pos_x, int pos_y, Image* _image, Camera* _camera, int _type)
{
	pos = Vector2(pos_x, pos_y);
	image = _image;
	camera = _camera;
	type = _type;
}

void Sprite::render(Image& fbuff, Area rect)
{
	switch (type)
	{
		case PLAYER:
			fbuff.drawImage(*image, 65, 50, rect);
			break;
		default:
			fbuff.drawImage(*image, pos.x - (int)camera->position.x, pos.y - (int)camera->position.y, rect);
			break;
	}
	
}