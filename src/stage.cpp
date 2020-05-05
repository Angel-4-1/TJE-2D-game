#include "stage.h"

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
IntroStage::IntroStage(Image* _font, Image* _dark_font, Image* _minifont, float* _time, Synth* _synth, Image* _intro_image): Stage(_font, _minifont, _time, _synth) {
	dark_font = _dark_font;
	intro_image = _intro_image;
}

void IntroStage::render(Image& framebuffer)
{ 
	framebuffer.fill(Color::RED);
	framebuffer.drawImage(*intro_image, 0, 0, framebuffer.width, framebuffer.height);
	framebuffer.drawText("SAVE THE", 5, 25, *dark_font);
	framebuffer.drawText("BOMBS", 5, 45, *dark_font);
	framebuffer.drawText("PULSA 'A' PARA EMPEZAR", 35, 100, *minifont, 4, 6);
}

void IntroStage::update(double seconds_elapsed)
{
}

void IntroStage::onKeyDown(SDL_KeyboardEvent event) 
{
	switch (event.keysym.sym)
	{
		case SDLK_a:	// Change to the tutorial
			change = true;
			change_to = TUTORIAL_STAGE;
			break;
	}
}

/**********TUTORIAL STAGE********/
TutorialStage::TutorialStage(Image * _font, Image * _minifont, float * _time, Synth * _synth, Image* _tutorial_image, Image* _player1, Image* _player2, Image* _arrow): Stage(_font, _minifont, _time, _synth) 
{ 
	player1 = _player1;
	player2 = _player2;
	tutorial_image = _tutorial_image;
	arrow = _arrow;
	arrow_pos = Vector2(57, 40);
	next = 0;
	selected = PLAYER1;
}

void TutorialStage::render(Image & framebuffer)
{	
	framebuffer.drawImage(*tutorial_image, 0, 0, framebuffer.width, framebuffer.height);
	switch (next) {
		case 0:
			framebuffer.drawText("CONTROLES:", 5, 10, *font);
			framebuffer.drawText("CURSORES: MOVER EL PERSONAJE", 10, 35, *minifont, 4, 6);
			framebuffer.drawText("TECLA A: CORRER", 10, 55, *minifont, 4, 6);
			framebuffer.drawText("TECLA Z: COGER / DEJAR BOMBAS", 10, 75, *minifont, 4, 6);
			break;
		case 1:
			framebuffer.drawText("ALGUIEN HA COLOCADO 3 PARES DE BOMBAS", 5, 5, *minifont, 4, 6);
			framebuffer.drawText("EN MEDIO DEL PUEBLO. TU MISION, LLEVAR", 5, 25, *minifont, 4, 6);
			framebuffer.drawText("CADA PAR (ROJAS, AZULES, AMARILLAS) A", 5, 45, *minifont, 4, 6);
			framebuffer.drawText("SUS RESPECTIVAS CASAS PARA QUE PUEDAN", 5, 65, *minifont, 4, 6);
			framebuffer.drawText("SER DESACTIVADAS.", 5, 85, *minifont, 4, 6);
			break;
		case 2:
			framebuffer.drawText("PERO CUIDADO, CADA BOMBA HA DE", 5, 5, *minifont, 4, 6);
			framebuffer.drawText("PERMANECER JUNTO A SU PAREJA EN TODO", 5, 20, *minifont, 4, 6);
			framebuffer.drawText("MOMENTO DE LO CONTRARIO EXPLOTARAN.", 5, 35, *minifont, 4, 6);
			framebuffer.drawText("SOLO SE PERMITEN ESTAR SEPARADAS 30 ", 5, 50, *minifont, 4, 6);
			framebuffer.drawText("SEGUNDOS, TIEMPO SUFICIENTE PARA", 5, 65, *minifont, 4, 6);
			framebuffer.drawText("LLEVAR LA A SU CASA DONDE SE", 5, 80, *minifont, 4, 6);
			framebuffer.drawText("REJUNTARAN.", 5, 95, *minifont, 4, 6); 
			break;
		case 3:
			framebuffer.drawText("SELECCIONA UN PERSONAJE", 30, 10, *minifont, 4, 6);
			framebuffer.drawImage(*player1, 55, 50, Area(0, 0, 14, 18));
			framebuffer.drawImage(*player2, 95, 49, Area(0, 0, 14, 18));
			framebuffer.drawImage(*arrow, arrow_pos.x, arrow_pos.y, Area(0, 0, 8, 6));
			break;
	}
	framebuffer.drawText("PULSA 'A' PARA CONTINUAR", 60, 109, *minifont, 4, 6);
}

void TutorialStage::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_a:	// Change to the play stage to start the game
		if (next < 3) {
			next++;
		}
		else {
			next = 0;
			change = true;
			change_to = PLAY_STAGE;
		}
		break;
	case SDLK_LEFT:
		selected = PLAYER1;
		arrow_pos = Vector2(57, 40);
		break;
	case SDLK_RIGHT:
		selected = PLAYER2;
		arrow_pos = Vector2(97, 40);
		break;
	}
}

Image* TutorialStage::getPlayerSelected() 
{ 
	if (selected == 0) {
		return player1;
	}
	else {
		return player2;
	}
}

/***********PLAY STAGE***********/
PlayStage::PlayStage(Image* _font, Image* _minifont, Image* _sprite_player, float* _time, Synth* _synth, Camera* _camera, GameMap* _map) : Stage(_font, _minifont, _time, _synth)
{
	// Init the player
	player.init(_map->getStart(), Player::FACE_DOWN, 30, false);
	player.setCamera(_camera);
	player.setSpriteImage(_sprite_player);

	camera = _camera;
	camera->position = player.getPosition() - Vector2(65, 50);
	map = _map;

	// Initially no bombs saved
	for (int i = 0; i < num_bombs; i++)
		bombs_saved[i] = 0;
	time_to_explote = 0;
	remaining = 10;

	// Bomb previously saved
	previous_saved = 0;
}

void PlayStage::setPlayerSprite(Image* sprite)
{
	player.setSpriteImage(sprite);
}

void PlayStage::render(Image& framebuffer)
{
	// Player
	player.render(framebuffer, *time);
	
	// Time counter
	if (time_to_explote != 0) {
		framebuffer.drawText(toString(remaining), 1, 10, *minifont, 4, 6);
	}
}

// Check if the target position is valid
bool PlayStage::isValidPosition(Vector2 target, int factor)
{
	// when going down --> factor = 2 & when going up --> factor = 1, it is just for simulating the prespective
	int x = (int)(((float)(target.x + 7.0)) / 16.0);
	int y = (int)(((float)(target.y + 8.0 * factor)) / 16.0);

	if (target.y >= 0 && target.y <= map->height * 16) {	// If it is inside the map
		if (map->getCell(x, y).isValid) {	// Check the var isValid of the cell
			return true;
		}
	}

	return false;
}

void PlayStage::update(double seconds_elapsed)
{
	// Move the character
	Vector2 target = player.getPosition();

	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		player.setFacing(Player::FACE_UP);
		target.y -= player.getSpeed() * seconds_elapsed;

		if (isValidPosition(target, 1)) {
				player.setPosition(target);
		}
				
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		player.setFacing(Player::FACE_DOWN);
		target.y += player.getSpeed() * seconds_elapsed;

		if (isValidPosition(target, 2)) {
			player.setPosition(target);
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		player.setFacing(Player::FACE_LEFT);
		target.x -= player.getSpeed() * seconds_elapsed;

		if (isValidPosition(target, 1)) {
			player.setPosition(target);
		}
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		player.setFacing(Player::FACE_RIGHT);
		target.x += player.getSpeed() * seconds_elapsed;

		if (isValidPosition(target, 1)) {
			player.setPosition(target);
		}
	}

	// Update camera position
	camera->position = player.getPosition() - Vector2(65, 50);

	// Update time remaining
	updateTimeRemaining();
}

void PlayStage::updateTimeRemaining()
{
	// If in 30 segons the bombs are not saved, the game will be finished
	if (time_to_explote != 0) {
		remaining = 30 - (*time - time_to_explote);
		if (remaining < 10 && remaining > 0) {
			game_synth->playSample("data/alarm.wav", 0.7, false);
		}
		if (remaining <= 0) {	// The player has lost the game
			change = true;
			change_to = GAMEOVER_STAGE;
		}
	}
}

// Restart the playstage
void PlayStage::reset()
{
	// Reset all the values
	player.init(map->getStart(), Player::FACE_DOWN, 30, false);
	camera->position = player.getPosition() - Vector2(65, 50);
	time_to_explote = 0;
	remaining = 10;

	// Initially no bombs saved
	for (int i = 0; i < num_bombs; i++)
		bombs_saved[i] = 0;

	previous_saved = 0;
}

void PlayStage::onKeyDown(SDL_KeyboardEvent event) 
{ 
	switch (event.keysym.sym)
	{
	case SDLK_a:	// Increase the speed
		player.setSpeed(90);
		break;
	case SDLK_z:	// Pick / Drop a bomb
		if (!player.getIsCarryingBomb())
			pickItem(player.getPosition().x, player.getPosition().y);
		else
		{
			dropItem(player.getPosition().x, player.getPosition().y);
			if (isCompleted())
			{
				change = true;
				change_to = WIN_STAGE;
			}
		}
		break;
	}
}

void PlayStage::onKeyUp(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_a:	// Decrease the speed
		player.setSpeed(30);
		break;
	}
}

void PlayStage::carryBomb(int type)
{
	player.setIsCarryingBomb(true);
	player.setBombCarrying(type);
	
	game_synth->playSample("data/coin.wav", 1, false);

	if (bombs_saved[type] == 0) {
		// Check if the previous picked bomb is saved with its partner
		if (bombs_saved[previous_saved] != 1) {	
			previous_saved = type;
			time_to_explote = *time;
		}
	}
}

void PlayStage::pickItem(int x, int y) 
{
	x = (int)(float)(x / 16.0);
	y = (int)(float)(y / 16.0);
	int type = (int)map->getCell(x, y).item;

	switch (type)
	{
	case eItemType::BOMBAROJA:
		map->getCell(x, y).item = eItemType::NOTHING;
		carryBomb(REDBOMB);
		break;
	case eItemType::BOMBAAZUL:
		map->getCell(x, y).item = eItemType::NOTHING;
		carryBomb(BLUEBOMB);
		break;
	case eItemType::BOMBAAMARILLA:
		map->getCell(x, y).item = eItemType::NOTHING;
		carryBomb(YELLOWBOMB);
		break;
	}
}

void PlayStage::dropBomb(int type)
{
	player.setIsCarryingBomb(false);
	game_synth->playSample("data/drop.wav", 1, false);
	bombs_saved[type]++;
	if (bombs_saved[type] == 2)
		time_to_explote = 0;
}

void PlayStage::dropItem(int x, int y)
{
	x = (int)(float)(x / 16);
	y = (int)(float)(y / 16);
	int type = (int)map->getCell(x, y).type;
	int item_type = (int)map->getCell(x, y).item;

	if (item_type == eItemType::NOTHING) {
		switch (type)
		{
		case eCellType::ROJO:
			if (player.getBombCarrying() == REDBOMB)
			{
				map->getCell(x, y).item = eItemType::BOMBAROJA;
				dropBomb(REDBOMB);
			}
			break;
		case eCellType::AZUL:
			if (player.getBombCarrying() == BLUEBOMB)
			{
				map->getCell(x, y).item = eItemType::BOMBAAZUL;
				dropBomb(BLUEBOMB);
			}
			break;
		case eCellType::AMARILLO:
			if (player.getBombCarrying() == YELLOWBOMB)
			{
				map->getCell(x, y).item = eItemType::BOMBAAMARILLA;
				dropBomb(YELLOWBOMB);
			}
			break;
		}
	}
}

// To check if all bombs have been saved
bool PlayStage::isCompleted() 
{
	int contador = 0;
	for (int i = 0; i < (num_bombs/2); i++) {
		contador += bombs_saved[i];
	}

	if (contador == num_bombs)
	{
		return true;
	}

	return false;
}


/**********FINAL STAGE***********/
FinalStage::FinalStage(Image* _font, Image* _minifont, Image* _lose_image, Image* _win, float* _time, Synth* _synth) : Stage(_font, _minifont, _time, _synth) {
	isWin = true;	// By default consider that the player is going to win
	lose_image = _lose_image;
	win_image = _win;
}

void FinalStage::render(Image& framebuffer)
{
	if (isWin) {
		renderWin(framebuffer);
	}
	else {
		renderGameOver(framebuffer);
	}

	framebuffer.drawText("PULSA:", 10, 75, *minifont, 4, 6);
	framebuffer.drawText("'A': VOLVER A EMPEZAR", 20, 85, *minifont, 4, 6);
	framebuffer.drawText("'Z': SALIR", 20, 95, *minifont, 4, 6);
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

void FinalStage::renderWin(Image & framebuffer)
{
	framebuffer.drawImage(*win_image, 0, 0, framebuffer.width, framebuffer.height);
	framebuffer.drawText("BOMBAS NEUTRALIZADAS", 10, 25, *font);
	framebuffer.drawText(" ! ! ! ", 55, 35, *font);
}

void FinalStage::renderGameOver(Image & framebuffer)
{
	framebuffer.drawImage(*lose_image, 0, 0, framebuffer.width, framebuffer.height);
	framebuffer.drawText("GAME OVER", 45, 65, *font);
}

void FinalStage::changeWin(bool state)
{
	isWin = state;
}


