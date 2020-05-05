#include "gamemap.h"
#include "game.h"

GameMap::GameMap(int _width, int _height, Camera* _camera)
{
	width = _width;
	height = _height;
	data = new sCell[_width * _height];
	camera = _camera;
}

sCell& GameMap::getCell(int x, int y)
{
	return data[x + y * width];
}

// Set if the position is valid for the player to step into it
bool setIsValid(eCellType type) 
{
	// Check if the cell is valid
	if (type == EMPTY || type == START || type == TIERRA || type == CESPED ||
		type == PUENTE || type == PUENTE2 || type == INTERIOR || type == PUERTA ||
		type == ROJO || type == AZUL || type == AMARILLO) {
		return true;
	}

	return false;
}

bool GameMap::loadMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return false;

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);

	assert(header.bytes >= 1);	//always control bad cases

	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w*header.h*header.bytes];
	fread(cells, header.bytes, header.w*header.h, file);

	width = header.w;
	height = header.h;
	
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			getCell(x, y).type = (eCellType)cells[(x + y * width) * header.bytes];
			getCell(x, y).isValid = setIsValid(getCell(x, y).type);
			if (header.bytes == 2) {
				getCell(x, y).item = (eItemType)cells[((x + y * width) * header.bytes) + 1];
			}
		}
	}

	delete[] cells; //always free any memory allocated!

	return true;
}

// Draw the map taking into aaccount the position of the camera
void GameMap::drawMap(Image* framebuffer)
{
	int cs = tileset->width / 16; //size in pixels of a cell
	float time = Game::instance->getTime() * 0.8;	// for animating cells
	//for every cell
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			//get cell info
			sCell& cell = getCell(x, y);

			/******DRAW THE TYPE*****/
			int type = (int)cell.type;
			if (type == 0) //skip empty
				continue;
			// Animate this cell
			if (type == AGUA) {
				type = AGUA + (int(time * 6)) % 5;
			}
			//compute tile pos in tileset image
			int tilex = (type % 16)*cs;
			int tiley = floor(type / 16)*cs;
			Area area(tilex, tiley, cs, cs);
			//avoid rendering out of screen stuff
			if (x < -cs || (x*cs - (int)camera->position.x) + cs > framebuffer->width + cs ||
				y < -cs || (y*cs - (int)camera->position.y) + cs > framebuffer->height + cs)
				continue;
			//draw area of tileset inside framebuffer
			framebuffer->drawImage(*tileset, (x*cs - (int)camera->position.x) , (y*cs - (int)camera->position.y) , area);

			/******DRAW THE ITEM*****/
			int itemtype = (int)cell.item;
			if (itemtype == 0)
				continue;
			switch (itemtype) {
				case BOMBAROJA:
					itemtype = BOMBAROJA + (int(time * 16)) % 4;
					break;
				case BOMBAAZUL:
					itemtype = BOMBAAZUL + (int(time * 16)) % 4;
					break;
				case BOMBAAMARILLA:
					itemtype = BOMBAAMARILLA + (int(time * 16)) % 4;
					break;
			}
			int itemtilex = (itemtype % 16)*cs;
			int itemtiley = floor(itemtype / 16)*cs;
			Area itemarea(itemtilex, itemtiley, cs, cs);
			framebuffer->drawImage(*tileset, (x*cs - (int)camera->position.x), (y*cs - (int)camera->position.y), itemarea);
		}
}

Vector2 GameMap::getStart() {
	int cs = tileset->width / 16; //size in pixels of a cell

	//for every cell
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y)
		{
			sCell& cell = getCell(x, y);
			if (cell.type == START) //get starting position 
				return Vector2(x*cs, y*cs);
		}
	}

	//otherwise return a random point
	return Vector2(60, 50);
}