#ifndef PLAYER_H
#define PLAYER_H

#include "framework.h"
#include "sprite.h"
#include "camera.h"

class Player 
{
public:
	Sprite splayer;
	float speed;

	enum { FACE_DOWN, FACE_RIGHT, FACE_LEFT, FACE_UP };
	int facing;

	bool isCarryingBomb;
	int bombCarrying;

	Player();
	Player(Image* _sprite_player, Camera* _camera, Vector2 _starting_pos);

	void init(Vector2 _starting_pos, int _facing, float _speed, bool _isCarryingBomb);

	void setPosition(Vector2 position);
	Vector2 getPosition();

	void setSpriteImage(Image* im);
	void setSpriteType(int type);
	void setFacing(int face);
	void setCamera(Camera* camera);
	void setSpeed(float sp);
	float getSpeed();
	void setIsCarryingBomb(bool type);
	bool getIsCarryingBomb();
	void setBombCarrying(int type);
	int getBombCarrying();

	void render(Image& framebuffer, float time);

	/*
	void pickItem(int x, int y);
	void dropItem(int x, int y);
	void carryBomb(int type);
	void dropBomb(int type);
	*/
};

#endif