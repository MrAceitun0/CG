#include "image.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height*sizeof(Color)];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = getPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

//change image size and scale the content
void Image::scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = getPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++x)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.setPixel( x, y, getPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::flipX()
{
	for(unsigned int x = 0; x < width * 0.5; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel( width - x - 1, y, getPixel(x,y));
			setPixel( x, y, temp );
		}
}

void Image::flipY()
{
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel( x, height - y - 1, getPixel(x,y) );
			setPixel( x, y, temp );
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE * file = fopen(filename, "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << filename << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "TGA file seems to have errors or it is compressed, only uncompressed TGAs supported" << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	//save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	//convert to float all pixels
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x , height - y - 1, Color( tgainfo->data[pos+2], tgainfo->data[pos+1], tgainfo->data[pos]) );
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FILE *file = fopen(filename, "wb");
	if ( file == NULL )
	{
		fclose(file);
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	//tgainfo->width = header[1] * 256 + header[0];
	//tgainfo->height = header[3] * 256 + header[2];

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	//convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height-y-1)*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);
	return true;
}

float Image::abs(float n)
{
	return ((n > 0) ? n : (n * (-1)));
}

void Image::drawLineDDA(int posFirstX, int posFirstY, int posSecondX, int posSecondY, Color c) 
{
	float dy = posSecondY - posFirstY;
	float dx = posSecondX - posFirstX;

	//Computar distancia de pintado necesaria entre puntos
	float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	//Computar incremento en X,Y por cada step
	float incX = dx / steps;
	float incY = dy / steps;

	float x = posFirstX;
	float y = posFirstY;

	for (int i = 0; i <= steps; i++)
	{
		setPixelSafe(x, y, c);
		x += incX;
		y += incY;
	}
}

struct sCelda {
	int minx;
	int maxx;
};

void Image::drawLineDDAwithTable(int posFirstX, int posFirstY, int posSecondX, int posSecondY, Color c, std::vector<sCelda> &table)
{
	float dy = posSecondY - posFirstY;
	float dx = posSecondX - posFirstX;

	//Computar distancia de pintado necesaria entre puntos
	float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	//Computar incremento en X,Y por cada step
	float incX = dx / steps;
	float incY = dy / steps;

	float x = posFirstX;
	float y = posFirstY;

	for (int i = 0; i <= steps; i++)
	{
		setPixelSafe(x, y, c);

		if (x < table[y].minx)
		{
			table[y].minx = x;
		}
		
		if (x > table[y].maxx)
		{
			table[y].maxx = x;
		}

		x += incX;
		y += incY;
	}
}

void Image::drawLineBresenham(int x0, int y0, int x1, int y1, Color c)
{
	int x, y, dx, dy, d, inc_E, inc_NE;

	/*int xi = x0;
	int xj = x1;
	int yi = y0;
	int yj = y1;

	if (x0 > x1)
	{
		x0 = xj;
		x1 = xi;
	}
	if (y0 > y1)
	{
		y0 = yj;
		y1 = yi;
	}*/

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);

	x = x0;
	y = y0;

	setPixelSafe(x, y, c);

	if (dx > dy)
	{
		if (y0 < y1)
		{
			if (x0 < x1)	//5
			{
				d = 2 * dy - dx;
				inc_E = 2 * dy;
				inc_NE = 2 * (dy - dx);

				while (x < x1)
				{
					if (d <= 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						y++;
					}

					x++;
					setPixelSafe(x, y, c);
				}
			}
			if(x0 > x1)		//8
			{
				d = 2 * dy - dx;
				inc_E = 2 * dy;
				inc_NE = 2 * (dy - dx);

				while (x > x1)
				{
					if (d <= 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						y++;
					}

					x--;
					setPixelSafe(x, y, c);
				}
			}
		}
		else
		{
			if (x0 < x1)	//4
			{
				d = 2 * dy - dx;
				inc_E = 2 * dy;
				inc_NE = 2 * (dy - dx);

				while (x < x1)
				{
					if (d <= 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						y--;
					}

					x++;
					setPixelSafe(x, y, c);
				}
			}
			if (x0 > x1)	//1
			{
				d = 2 * dy - dx;
				inc_E = 2 * dy;
				inc_NE = 2 * (dy - dx);

				while (x > x1)
				{
					if (d <= 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						y--;
					}

					x--;
					setPixelSafe(x, y, c);
				}
			}
		}
	}
	else	//dx < dy
	{
		if (x0 < x1)
		{
			if (y0 < y1)	//6
			{
				d = 2 * dx - dy;
				inc_E = 2 * dx;
				inc_NE = 2 * (dx - dy);

				while (y < y1)
				{
					if (d < 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						x++;
					}

					y++;
					setPixelSafe(x, y, c);
				}
			}
			if (y0 > y1)	//3
			{
				d = 2 * dx - dy;
				inc_E = 2 * dx;
				inc_NE = 2 * (dx - dy);

				while (y > y1)
				{
					if (d < 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						x++;
					}

					y--;
					setPixelSafe(x, y, c);
				}
			}
		}
		else
		{
			if (y0 < y1)	//7
			{
				d = 2 * dx - dy;
				inc_E = 2 * dx;
				inc_NE = 2 * (dx - dy);

				while (y < y1)
				{
					if (d < 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						x--;
					}

					y++;
					setPixelSafe(x, y, c);
				}
			}
			if (y0 > y1)	//2
			{
				d = 2 * dx - dy;
				inc_E = 2 * dx;
				inc_NE = 2 * (dx - dy);

				while (y > y1)
				{
					if (d < 0)
					{
						d += inc_E;
					}
					else
					{
						d += inc_NE;
						x--;
					}

					y--;
					setPixelSafe(x, y, c);
				}
			}
		}
	}	
}

void Image::drawCircle(int centerX, int centerY, int x, int y, Color c)
{
	setPixelSafe(x + centerX, y + centerY, c);
	setPixelSafe(-x + centerX, y + centerY, c);
	setPixelSafe(x + centerX, -y + centerY, c);
	setPixelSafe(-x + centerX, -y + centerY, c);

	setPixelSafe(y + centerX, x + centerY, c);
	setPixelSafe(-y + centerX, x + centerY, c);
	setPixelSafe(y + centerX, -x + centerY, c);
	setPixelSafe(-y + centerX, -x + centerY, c);
}

void Image::DrawFilledCircle(int x0, int y0, int radius, Color c)
{
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = x0 - x; i <= x0 + x; i++)
		{
			setPixelSafe(i, y0 + y, c);
			setPixelSafe(i, y0 - y, c);
		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			setPixelSafe(i, y0 + x, c);
			setPixelSafe(i, y0 - x, c);
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void Image::drawCircleBresenham(int centerX, int centerY, int radius, Color c, bool fill)
{
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;
	drawCircle(centerX, centerY, x, y, c);

	while (y >= x)
	{
		// for each pixel we will 
		// draw all eight pixels 

		x++;

		// check for decision parameter 
		// and correspondingly  
		// update d, x, y 
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		drawCircle(centerX, centerY, x, y, c);
	}

	if (fill)
	{
		DrawFilledCircle(centerX, centerY, radius, c);
	}
}

void Image::drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color, bool fill)
{
	if (fill)
	{
		std::vector<sCelda> table;
		table.resize(this->height);

		//inicializar tabla
		for (int i = 0; i < table.size(); i++)
		{
			table[i].minx = 100000; //very big number
			table[i].maxx = -100000; //very small number
		}

		//Raster lines algorithm
		drawLineDDAwithTable(x0, y0, x1, y1, color, table);
		drawLineDDAwithTable(x1, y1, x2, y2, color, table);
		drawLineDDAwithTable(x2, y2, x0, y0, color, table);

		//Filling triangle algorithm
		for (int i = 0; i < table.size(); i++)
		{
			if (table[i].minx < table[i].maxx)
			{
				drawLineDDA(table[i].minx, i, table[i].maxx, i, color);
			}
		}

	}
	else
	{ 
		drawLineDDA(x0, y0, x1, y1, color);
		drawLineDDA(x1, y1, x2, y2, color);
		drawLineDDA(x2, y2, x0, y0, color);
	}
}


#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif