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

void Stage::onKeyDown(SDL_KeyboardEvent event) { }

void Stage::onKeyUp(SDL_KeyboardEvent event) { }

/**********INTRO STAGE***********/
IntroStage::IntroStage(Image* _font, Image* _minifont, float* _time, Synth* _synth): Stage(_font, _minifont, _time, _synth) { }

void IntroStage::render(Image& framebuffer)
{ 
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
	player.pos = _map->getStart();
	player.type = Sprite::PLAYER;
	player.image = _sprite_player;
	facing = FACE_DOWN;
	camera = _camera;
	camera->position = player.pos - Vector2(65, 50);
	map = _map;
	speed = 30;
	isCarryingBomb = false;
	for (int i = 0; i < num_bombs; i++)
		bombs_saved[i] = 0;
}

void PlayStage::render(Image& framebuffer)
{
	framebuffer.drawText("PLAY STAGE", 40, 25, *font);
	player.render(framebuffer, Area((int(*time * 15) % 4) * 14, facing * 18, 14, 18));
}

// Check if the target position is valid
bool PlayStage::isValidPosition(Vector2 target, int factor)
{
	// when going down --> factor = 2 & when going up --> factor = 1, it is just for simulating the prespective
	int x = (int)(((float)(target.x + 7.0)) / 16.0);
	int y = (int)(((float)(target.y + 8.0 * factor)) / 16.0);

	if (target.y >= 0 && target.y <= map->height * 16) {	// If it is inside the map
		int type = (int)map->getCell(x, y).type;	
		if (type == eCellType::EMPTY || type == eCellType::START || type == eCellType::TIERRA || type == eCellType::CESPED ||
			type == eCellType::PUENTE || type == eCellType::INTERIOR || type == eCellType::PUERTA || type == eCellType::ROJO || 
			type == eCellType::AZUL || type == eCellType::AMARILLO) {
			return true;
		}
	}

	return false;
}

void PlayStage::update(double seconds_elapsed)
{
	// Move the character
	Vector2 target = player.pos;

	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		facing = FACE_UP;
		target.y -= speed * seconds_elapsed;

		if (isValidPosition(target, 1)) {
				player.pos = target;
		}
				
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		facing = FACE_DOWN;
		target.y += speed * seconds_elapsed;

		if (isValidPosition(target, 2)) {
			player.pos = target;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		facing = FACE_LEFT;
		target.x -= speed * seconds_elapsed;

		if (isValidPosition(target, 1)) {
			player.pos = target;
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		facing = FACE_RIGHT;
		target.x += speed * seconds_elapsed;

		if (isValidPosition(target, 1)) {
			player.pos = target;
		}
	}

	// Update camera position
	camera->position = player.pos - Vector2(65, 50);

	// Music
	game_synth->osc1.amplitude = 0.0;
}

// Restart the playstage
void PlayStage::reset()
{
	player.pos = map->getStart();
	facing = FACE_DOWN;
	camera->position = player.pos - Vector2(65, 50);
	speed = 30;
}

void PlayStage::onKeyDown(SDL_KeyboardEvent event) 
{ 
	switch (event.keysym.sym)
	{
	case SDLK_a:	// Increase the speed
		speed = 90;
		break;
	case SDLK_z:
		if (!isCarryingBomb)
			pickItem(player.pos.x, player.pos.y);
		else
			dropItem(player.pos.x, player.pos.y);
		break;
	}
}

void PlayStage::onKeyUp(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_a:	// Decrease the speed
		speed = 30;
		break;
	}
}

void PlayStage::pickItem(int x, int y) 
{
	x = (int)(float)(x / 16);
	y = (int)(float)(y / 16);
	int type = (int)map->getCell(x, y).type;

	switch (type)
	{
	case eCellType::BOMBAROJA:
		isCarryingBomb = true;
		bombCarrying = REDBOMB;
		map->getCell(x, y).type = eCellType::INTERIOR;
		break;
	case eCellType::BOMBAAZUL:
		isCarryingBomb = true;
		bombCarrying = BLUEBOMB;
		map->getCell(x, y).type = eCellType::INTERIOR;
		break;
	case eCellType::BOMBAAMARILLA:
		isCarryingBomb = true;
		bombCarrying = YELLOWBOMB;
		map->getCell(x, y).type = eCellType::INTERIOR;
		break;
	}
}

void PlayStage::dropItem(int x, int y)
{
	x = (int)(float)(x / 16);
	y = (int)(float)(y / 16);
	int type = (int)map->getCell(x, y).type;
	switch (type)
	{
	case eCellType::ROJO:
		if (bombCarrying == REDBOMB)
		{
			isCarryingBomb = false;
			map->getCell(x, y).type = eCellType::BOMBAROJA;
			bombs_saved[REDBOMB]++;
		}
		break;
	case eCellType::AZUL:
		if (bombCarrying == BLUEBOMB)
		{
			isCarryingBomb = false;
			map->getCell(x, y).type = eCellType::BOMBAAZUL;
			bombs_saved[BLUEBOMB]++;
		}
		break;
	case eCellType::AMARILLO:
		if (bombCarrying == YELLOWBOMB)
		{
			isCarryingBomb = false;
			map->getCell(x, y).type = eCellType::BOMBAAMARILLA;
			bombs_saved[YELLOWBOMB]++;
		}
		break;
	}
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