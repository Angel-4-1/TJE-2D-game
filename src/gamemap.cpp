#include "gamemap.h"

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

bool GameMap::loadMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return false;

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);

	assert(header.bytes == 1); //always control bad cases!!

	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w*header.h];
	fread(cells, header.bytes, header.w*header.h, file);

	width = header.w;
	height = header.h;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			getCell(x, y).type = (eCellType)cells[x + y * width];
			std::cout << getCell(x, y).type << ' ';

		}
		std::cout << '\n';
	}


	delete[] cells; //always free any memory allocated!

	return true;
}

// Draw the map taking into aaccount the position of the camera
void GameMap::drawMap(Image* framebuffer)
{
	int cs = tileset->width / 16; //size in pixels of a cell
	
	//for every cell
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			//get cell info
			sCell& cell = getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			//compute tile pos in tileset image
			int tilex = (cell.type % 16)*cs;
			int tiley = floor(cell.type / 16)*cs;
			Area area(tilex, tiley, cs, cs);
			//avoid rendering out of screen stuff
			
			if (x < -cs || (x*cs - (int)camera->position.x) + cs > framebuffer->width + cs ||
				y < -cs || (y*cs - (int)camera->position.y) + cs > framebuffer->height + cs)
				continue;
			//draw area of tileset inside framebuffer
			framebuffer->drawImage(*tileset, (x*cs - (int)camera->position.x) , (y*cs - (int)camera->position.y) , area);
			/*
			if (x < -cs || x*cs > framebuffer->width  ||
				y < -cs || y*cs > framebuffer->height )
				continue;
			//draw area of tileset inside framebuffer
			framebuffer->drawImage(*tileset, x*cs , y*cs , area);*/
		}
}

Vector2 GameMap::getStart() {
	int cs = tileset->width / 16; //size in pixels of a cell

	//for every cell
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			sCell& cell = getCell(x, y);
			if (cell.type == START) //skip empty
				return Vector2(x*cs, y*cs);
		}

	return Vector2(60, 50);
}