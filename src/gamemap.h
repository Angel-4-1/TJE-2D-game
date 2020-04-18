#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "framework.h"
#include "image.h"
#include "camera.h"

enum eCellType : uint8 { EMPTY, TIERRA, CESPED, AGUA, ARBOL, START };
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