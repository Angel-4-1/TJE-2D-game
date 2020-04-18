#include "stage.h"
#include "game.h"
#include "framework.h"
#include "image.h"
#include "includes.h"

/*********STAGE********/
Stage::Stage(Image* _font, Image* _minifont, float* _time, Synth* _synth) 
{
	font = _font;
	minifont = _minifont;
	change = false;
	time = _time;
	game_synth = _synth;
}

void Stage::render(Image& framebuffer)
{
	framebuffer.fill(Color::BLUE);
	framebuffer.drawText("STAGE", 45, 25, *font);
}

void Stage::update(double seconds_elapsed) 
{
	game_synth->osc1.amplitude = 0.0;
}

//Keyboard event handler (sync input)
void Stage::onKeyDown(SDL_KeyboardEvent event) { }

void Stage::onKeyUp(SDL_KeyboardEvent event) { }

void Stage::onGamepadButtonDown(SDL_JoyButtonEvent event) { }

void Stage::onGamepadButtonUp(SDL_JoyButtonEvent event) { }

void Stage::onMouseMove(SDL_MouseMotionEvent event) { }

void Stage::onMouseButtonDown(SDL_MouseButtonEvent event) { }

void Stage::onMouseButtonUp(SDL_MouseButtonEvent event) { }

void Stage::onMouseWheel(SDL_MouseWheelEvent event) { }

/**********INTRO STAGE***********/
IntroStage::IntroStage(Image* _font, Image* _minifont, float* _time, Synth* _synth): Stage(_font, _minifont, _time, _synth) { }

void IntroStage::render(Image& framebuffer)
{ 
	//GameMap::drawMap(&framebuffer);

	framebuffer.fill(Color::RED);
	framebuffer.drawText("INTRO STAGE", 40, 25, *font);
	framebuffer.drawText("PRESS 'A' TO START", 44, 75, *minifont, 4, 6);
}

void IntroStage::update(double seconds_elapsed)
{
	// Music
	const int notes[] = { 69,70,71,72,73,74,75,76,77 };
	game_synth->osc1.amplitude = 0.5;
	game_synth->osc1.setNote(notes[int(*time) % 5]);
}

void IntroStage::onKeyDown(SDL_KeyboardEvent event) 
{
	switch (event.keysym.sym)
	{
		case SDLK_a:	// Change to the play stage to start the game
			change = true;
			change_to = PLAY_STAGE;
			break;
	}
}


/**********PLAY STAGE***********/
PlayStage::PlayStage(Image* _font, Image* _minifont, Image* _sprite_player, float* _time, Synth* _synth, Camera* _camera, GameMap* _map) : Stage(_font, _minifont, _time, _synth)
{
	//player.pos = Vector2(65,50);
	player.pos = _map->getStart();
	player.type = Sprite::PLAYER;
	player.image = _sprite_player;
	facing = FACE_DOWN;
	camera = _camera;
	camera->position = player.pos - Vector2(65, 50);
	//camera->position = Vector2(0,0);
	map = _map;
}

void PlayStage::render(Image& framebuffer)
{
	framebuffer.drawText("PLAY STAGE", 40, 25, *font);
	player.render(framebuffer, Area((int(*time * 15) % 4) * 14, facing * 18, 14, 18));
}

void PlayStage::update(double seconds_elapsed)
{
	// Move the character
	float speed = 30;

	Vector2 target = player.pos;

	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		std::cout <<'\t'<<target.x << ' ' << target.y << '\n';
		facing = FACE_UP;
		target.y -= speed * seconds_elapsed;
		int x = (int) ( ((float)(target.x + 14.0)) / 16.0 );
		int y = (int) ( ((float)(target.y + 16.0)) / 16.0 );
		//camera->position.y -= speed * seconds_elapsed;
		std::cout << target.x << ' ' << target.y << '\t' << x << ' ' << y << '\t' << map->getCell(x, y).type << '\n';
		if (target.y >= 0 && target.y <= map->height * 16) {
			if (map->getCell(x, y).type != eCellType::AGUA && map->getCell(x, y).type != eCellType::ARBOL) {
				player.pos = target;
			}
		}
				
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		facing = FACE_DOWN;
		target.y += speed * seconds_elapsed;
		int x = (int)(((float)(target.x + 14.0)) / 16.0);
		int y = (int)(((float)(target.y + 16.0)) / 16.0);
		std::cout << target.x << ' ' << target.y << '\t' << x << ' ' << y << '\t' << map->getCell(x, y).type << '\n';
		if (target.y >= 0 && target.y <= map->height * 16) {
			if (map->getCell(x, y).type != eCellType::AGUA && map->getCell(x, y).type != eCellType::ARBOL) {
				player.pos = target;
			}
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		facing = FACE_LEFT;
		target.x -= speed * seconds_elapsed;
		int x = (int)(((float)(target.x )) / 16.0);
		int y = (int)(((float)(target.y + 16.0)) / 16.0);
		std::cout << target.x << ' ' << target.y << '\t' << x << ' ' << y << '\t' << map->getCell(x, y).type << '\n';
		if (target.x >= 0 && target.x <= map->width * 16) {
			if (map->getCell(x, y).type != eCellType::AGUA && map->getCell(x, y).type != eCellType::ARBOL) {
				player.pos = target;
			}
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		facing = FACE_RIGHT;
		target.x += speed * seconds_elapsed;
		int x = (int)(((float)(target.x + 14.0)) / 16.0);
		int y = (int)(((float)(target.y + 16.0)) / 16.0);
		std::cout << target.x << ' ' << target.y << '\t' << x << ' ' << y << '\t' << map->getCell(x, y).type << '\n';
		if (target.x >= 0 && target.x <= map->width * 16) {
			if (map->getCell(x, y).type != eCellType::AGUA && map->getCell(x, y).type != eCellType::ARBOL) {
				player.pos = target;
			}
		}
	}

	camera->position = player.pos - Vector2(65, 50);

	// Music
	game_synth->osc1.amplitude = 0.0;
}

void PlayStage::reset()
{
	camera->position = player.pos;
}

/**********FINAL STAGE***********/
FinalStage::FinalStage(Image* _font, Image* _minifont, float* _time, Synth* _synth) : Stage(_font, _minifont, _time, _synth) { }

void FinalStage::render(Image& framebuffer)
{
	framebuffer.fill(Color::GREEN);
	framebuffer.drawText("FINAL STAGE", 25, 25, *font);
	framebuffer.drawText("PRESS :", 10, 75, *minifont, 4, 6);
	framebuffer.drawText("'A': RESTART THE GAME", 20, 90, *minifont, 4, 6);
	framebuffer.drawText("'Z': EXIT THE GAME", 20, 100, *minifont, 4, 6);
}

void FinalStage::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_a:	// Go back to the intro stage
			change = true;
			change_to = INTRO_STAGE;
			break;
		case SDLK_z:	// Finish the game
			change = true;
			change_to = EXIT_STAGE;
			break;
	}
}