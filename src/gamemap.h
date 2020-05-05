#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "framework.h"
#include "image.h"
#include "camera.h"


enum eCellType : uint8 { EMPTY, START, TIERRA, CESPED, AGUA, AGUA2, AGUA3, AGUA4, AGUA5, AGUACESPED, ARBOL, PUENTE, PUENTE2, CACTUS,
                         INTERIOR = 16, PARED, PUERTA, PARED2, PARED3, PARED4, PARED5, PARED6,
						 ROJO = 48, AZUL, AMARILLO};

enum eItemType : uint8 { NOTHING, BOMBAROJA = 32, BOMBAAZUL = 36, BOMBAAMARILLA = 40};

struct sCell {
	eCellType type;
	eItemType item;
	bool isValid;
};

struct sMapHeader {
	int w;
	int h;
	unsigned char bytes;
	unsigned char extra[7];
};

class GameMap
{
public:
	int width;
	int height;
	sCell* data;
	Image* tileset;
	Camera* camera;

	GameMap(int _width, int _height, Camera* _camera);

	sCell& getCell(int x, int y);
	bool loadMap(const char* filename);
	void drawMap(Image* framebuffer);

	// Get the starting position 
	Vector2 getStart();
};

#endif