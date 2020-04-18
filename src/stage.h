#ifndef STAGE_H
#define STAGE_H

#include <string>
#include "includes.h"
#include "game.h"
#include "input.h"
#include "synth.h"
#include "sprite.h"
#include "gamemap.h"

class Stage
{
public:
	Image* font;
	Image* minifont;
	float* time;
	Synth* game_synth;

	enum {STAGE, INTRO_STAGE, PLAY_STAGE, FINAL_STAGE, EXIT_STAGE};
	bool change;	// if true --> we want to change to another stage
	int change_to;	// stage to which we want to change

	Stage(Image* _font, Image* _minifont, float* _time, Synth* _synth);

	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);

	//events
	virtual void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown(SDL_MouseButtonEvent event);
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseMove(SDL_MouseMotionEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
};


class IntroStage : public Stage
{
public:
	IntroStage(Image* _font, Image* _minifont, float* _time, Synth* _synth);

	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void onKeyDown(SDL_KeyboardEvent event);
};

class PlayStage : public Stage
{
public:
	Sprite player;
	Camera* camera;
	GameMap* map;

	enum { FACE_DOWN, FACE_RIGHT, FACE_LEFT, FACE_UP };
	int facing;

	PlayStage(Image* _font, Image* _minifont, Image* _sprite_player, float* _time, Synth* _synth, Camera* _camera, GameMap* _map);
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void reset();	
};

class FinalStage : public Stage
{
public:
	void render(Image& framebuffer);
	void onKeyDown(SDL_KeyboardEvent event);

	FinalStage(Image* _font, Image* _minifont, float* time, Synth* _synth);
};
#endif