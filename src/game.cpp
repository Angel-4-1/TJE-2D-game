#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "stage.h"
#include "gamemap.h"

#include <cmath>

Game* Game::instance = NULL;

Image font;
Image minifont;
Image sprite;
Color bgcolor(130, 80, 100);
Image tileset;

// Stages
Stage* current_stage = NULL;
Stage* stage = NULL;
IntroStage* intro_stage = NULL;
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

	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	sprite.loadTGA("data/spritesheet.tga"); //example to load an sprite
	tileset.loadTGA("data/elementos3.tga");

	enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/coin.wav",1,true);
	//synth.osc1.amplitude = 0.5;

	camera = new Camera(65,50);

	// Init gamemap
	map = new GameMap(128, 128, camera);
	map->tileset = &tileset;
	map->loadMap("data/elementos3.map");

	// Init stages
	stage = new Stage(&font, &minifont, &time, &synth);
	intro_stage = new IntroStage(&font, &minifont, &time, &synth);
	play_stage = new PlayStage(&font, &minifont, &sprite, &time, &synth, camera, map);
	final_stage = new FinalStage(&font, &minifont, &time, &synth);
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

	//some new useful functions
		//framebuffer.fill( bgcolor );								//fills the image with one color
		//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
		//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
		//framebuffer.drawImage( sprite, 0, 0, framebuffer.width, framebuffer.height );			//draws a scaled image
		//framebuffer.drawImage( sprite, 0, 0, Area(0,0,14,18) );	//draws only a part of an image
		//framebuffer.drawText( "Hello World", 0, 0, font );				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		//framebuffer.drawText( toString(time), 1, 10, minifont,4,6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)

	//send image to screen
	//showFramebuffer(&framebuffer);
}

void Game::update(double seconds_elapsed)
{
	//Add here your update method
	current_stage->update(seconds_elapsed);
	
	/*
	// musica
	//const int notes[] = { 69, 71,72,74,76 };
	//synth.osc1.setNote(notes[int(time) % 5]);
	
	
	float speed = 30;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		facing = FACE_UP;
		player_pos.y -= speed * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		facing = FACE_DOWN;
		player_pos.y += speed * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
	{
		facing = FACE_LEFT;
		player_pos.x -= speed * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		facing = FACE_RIGHT;
		player_pos.x += speed * seconds_elapsed;
	}

	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	{
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		bgcolor.set(0, 255, 0);
	}
	*/
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch (event.keysym.sym)
	{
		case SDLK_ESCAPE: 
			must_exit = true; 
			break; //ESC key, kill the app
		
		/***FOR DEBUGGING***/
		case SDLK_1:
			current_stage = stage;
			break;
		case SDLK_2:
			current_stage = intro_stage;
			break;
		case SDLK_3:
			current_stage = play_stage;
			break;
		case SDLK_4:
			current_stage = final_stage;
			break;
	}

	current_stage->onKeyDown(event);

	// Change stage
	if (current_stage->change) {
		current_stage->change = false;
		switch (current_stage->change_to)
		{
		case Stage::STAGE:
			current_stage = stage;
			break;
		case Stage::INTRO_STAGE:
			// If we go back to the Intro stage that means that we are going to restart the game
			current_stage = intro_stage;
			// Free what we have on play_stage
			free(play_stage);
			// Create a new play_stage
			play_stage = new PlayStage(&font, &minifont, &sprite, &time, &synth, camera, map);
			break;
		case Stage::PLAY_STAGE:
			current_stage = play_stage;
			break;
		case Stage::EXIT_STAGE:	// Exit the game
			must_exit = true;
			break;
		}
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
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
