#ifndef STAGE_H
#define STAGE_H

#include <string>
#include "game.h"
#include "input.h"
#include "synth.h"
#include "sprite.h"
#include "gamemap.h"
#include "framework.h"
#include "image.h"
#include "camera.h"
#include "player.h"

class Stage
{
public:
	Image* font;
	Image* minifont;
	float* time;
	Synth* game_synth;

	enum {STAGE, INTRO_STAGE, TUTORIAL_STAGE, PLAY_STAGE, WIN_STAGE, GAMEOVER_STAGE, EXIT_STAGE};
	bool change;	// if true --> we want to change to another stage
	int change_to;	// stage to which we want to change

	Stage(Image* _font, Image* _minifont, float* _time, Synth* _synth);

	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);

	//events
	virtual void onKeyDown(SDL_KeyboardEvent event);
	virtual void onKeyUp(SDL_KeyboardEvent event);
};


class IntroStage : public Stage
{
public:
	Image* intro_image;
	Image* dark_font;
	IntroStage(Image* _font, Image* _dark_font, Image* _minifont, float* _time, Synth* _synth, Image* _intro_image);

	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void onKeyDown(SDL_KeyboardEvent event);
};

class TutorialStage : public Stage
{
public:
	Image* tutorial_image;
	Image* player1;
	Image* player2;
	Image* arrow;
	Vector2 arrow_pos;
	int next;

	enum { PLAYER1, PLAYER2 };
	int selected;
	TutorialStage(Image* _font, Image* _minifont, float* _time, Synth* _synth, Image* _tutorial_image, Image* _player1, Image* _player2, Image* _arrow);

	void render(Image& framebuffer);
	void onKeyDown(SDL_KeyboardEvent event);
	Image* getPlayerSelected();
};

class PlayStage : public Stage
{
public:
	Camera* camera;
	GameMap* map;
	Player player;

	Synth::Sample* pick;

	enum { REDBOMB, BLUEBOMB, YELLOWBOMB };
	static const int num_bombs = 6;
	int bombs_saved[num_bombs];
	int previous_saved;

	float time_to_explote;
	float remaining;

	PlayStage(Image* _font, Image* _minifont, Image* _sprite_player, float* _time, Synth* _synth, Camera* _camera, GameMap* _map);
	void render(Image& framebuffer);
	void update(double seconds_elapsed);
	void reset();
	bool isValidPosition(Vector2 target, int factor);
	void onKeyDown(SDL_KeyboardEvent event);
	void onKeyUp(SDL_KeyboardEvent event);
	void setPlayerSprite(Image* sprite);

	void pickItem(int x, int y);
	void dropItem(int x, int y);
	bool isCompleted();
	void carryBomb(int type);
	void dropBomb(int type);
	void updateTimeRemaining();
};

class FinalStage : public Stage
{
public:
	bool isWin;
	Image* lose_image;
	Image* win_image;

	void onKeyDown(SDL_KeyboardEvent event);

	// Depending on the value of isWin, render will call win or gameover
	void render(Image& framebuffer);
	void renderWin(Image& framebuffer);
	void renderGameOver(Image& framebuffer);

	// change value of isWin
	void changeWin(bool state);

	FinalStage(Image* _font, Image* _minifont, Image* _lose_image, Image* _win, float* time, Synth* _synth);
};

#endif