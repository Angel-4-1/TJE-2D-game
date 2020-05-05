#include "player.h"

Player::Player() {}

Player::Player(Image * _sprite_player, Camera * _camera, Vector2 _starting_pos)
{
	splayer.image = _sprite_player;
	splayer.camera = _camera;
	splayer.pos = _starting_pos;
	splayer.type = Sprite::PLAYER;
	facing = FACE_DOWN;
}

void Player::init(Vector2 _starting_pos, int _facing, float _speed, bool _isCarryingBomb)
{
	splayer.type = Sprite::PLAYER;
	splayer.pos = _starting_pos;
	facing = _facing;
	speed = _speed;
	isCarryingBomb = _isCarryingBomb;
}

void Player::setPosition(Vector2 position)
{
	splayer.pos = position;
}

Vector2 Player::getPosition()
{
	return Vector2(splayer.pos);
}

void Player::setSpriteImage(Image * im)
{
	splayer.image = im;
}

void Player::setSpriteType(int type)
{
	splayer.type = type;
}

void Player::setFacing(int face)
{
	facing = face;
}

void Player::setCamera(Camera * camera)
{
	splayer.camera = camera;
}

void Player::setSpeed(float sp)
{
	speed = sp;
}

float Player::getSpeed()
{
	return speed;
}

void Player::setIsCarryingBomb(bool type)
{
	isCarryingBomb = type;
}

bool Player::getIsCarryingBomb()
{
	return isCarryingBomb;
}

void Player::setBombCarrying(int type)
{
	bombCarrying = type;
}

int Player::getBombCarrying()
{
	return bombCarrying;
}

void Player::render(Image& framebuffer, float time)
{
	splayer.render(framebuffer, Area((int(time * 15) % 4) * 14, facing * 18, 14, 18));
}
