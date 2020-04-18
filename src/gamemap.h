#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "framework.h"
#include "image.h"
#include "camera.h"
#include "game.h"

enum eCellType : uint8 { EMPTY, START, TIERRA, CESPED, AGUA, AGUA2, AGUA3, AGUA4, AGUA5, AGUACESPED, ARBOL, PUENTE,
                         INTERIOR = 16, PARED, PUERTA, PARED2, PARED3, PARED4, PARED5, PARED6,
						 BOMBAROJA = 32, BOMBAAZUL, BOMBAAMARILLA,
						 ROJO = 48, AZUL, AMARILLO};
enum eItemType : uint8 { NOTHING };

struct sCell {
	eCellType type;
	eItemType item;
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

	Vector2 getStart();
};

#endif