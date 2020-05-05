#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "stage.h"
#include "gamemap.h"

#include <cmath>

Game* Game::instance = NULL;

// Font images
Image font;
Image dark_font;
Image minifont;
Image dark_minifont;

// Sprites
Image sprite_sherif;
Image sprite_boy;

Color bgcolor(130, 80, 100);
Image tileset;
Image intro;
Image lose;
Image win;
Image tutorial;
Image arrow;

// Stages
Stage* current_stage = NULL;
IntroStage* intro_stage = NULL;
TutorialStage* tutorial_stage = NULL;
PlayStage* play_stage = NULL;
FinalStage* final_stage = NULL;

GameMap* map = NULL;
Camera* camera = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	// Load the fonts
	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	dark_font.loadTGA("data/bitmap-font-black.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	dark_minifont.loadTGA("data/mini-font-black-4x6.tga"); //load bitmap-font image
	
	// Load the sprite of the player
	sprite_sherif.loadTGA("data/sherifsprite.tga");
	sprite_boy.loadTGA("data/spritesheet.tga");

	// Load the tileset and some images
	tileset.loadTGA("data/fuego.tga");
	intro.loadTGA("data/intro.tga");
	lose.loadTGA("data/explosion.tga");
	win.loadTGA("data/win.tga");
	tutorial.loadTGA("data/tuto.tga");
	arrow.loadTGA("data/arrow.tga");

	enableAudio(); //enable this line if you plan to add audio to your application

	// Set the camera to a random to position, it will be changed later
	camera = new Camera(65,50);

	// Init gamemap
	map = new GameMap(128, 128, camera);
	map->tileset = &tileset;
	map->loadMap("data/mapabombas.map");

	// Init stages
	intro_stage = new IntroStage(&font, &dark_font, &minifont, &time, &synth, &intro);
	tutorial_stage = new TutorialStage(&dark_font, &dark_minifont, &time, &synth, &tutorial, &sprite_sherif, &sprite_boy, &arrow);
	play_stage = new PlayStage(&font, &minifont, &sprite_sherif, &time, &synth, camera, map);
	final_stage = new FinalStage(&font, &minifont, &lose, &win, &time, &synth);
	current_stage = intro_stage;
}

//what to do when the image has to be draw
void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	map->drawMap(&framebuffer);
	current_stage->render(framebuffer);
	showFramebuffer(&framebuffer);
}

void Game::update(double seconds_elapsed)
{
	// Call stage update
	current_stage->update(seconds_elapsed);

	// Change stage
	if (current_stage->change) {
		changeState();
	}
}

// Select to which state go
void Game::changeState()
{
	current_stage->change = false;
	switch (current_stage->change_to)
	{
	case Stage::INTRO_STAGE:
		// If we go back to the Intro stage that means that we are going to restart the game
		current_stage = intro_stage;
		map->loadMap("data/mapabombas.map");
		play_stage->reset();
		break;
	case Stage::TUTORIAL_STAGE:
		current_stage = tutorial_stage;
		break;
	case Stage::PLAY_STAGE:
		play_stage->setPlayerSprite(tutorial_stage->getPlayerSelected());
		synth.playSample("data/road.wav", 5, true);	// environmental song to play during play stage
		current_stage = play_stage;
		break;
	case Stage::WIN_STAGE:
		synth.stopAllSamples();
		final_stage->changeWin(true);
		current_stage = final_stage;
		break;
	case Stage::GAMEOVER_STAGE:
		synth.stopAllSamples();
		synth.playSample("data/explosion.wav", 10, false);
		final_stage->changeWin(false);
		current_stage = final_stage;
		break;
	case Stage::EXIT_STAGE:	// Exit the game
		must_exit = true;
		break;
	}
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch (event.keysym.sym)
	{
		case SDLK_ESCAPE: 
			must_exit = true; 
			break; //ESC key, kill the app
	}

	current_stage->onKeyDown(event);

	// Change stage
	if (current_stage->change) {
		changeState();
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
	current_stage->onKeyUp(event);
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
