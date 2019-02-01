#include "application.h"
#include "utils.h"
#include "image.h"
#include <math.h>

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Application Variables
Image framebuffer(800, 800);
int modo = 0;

/*Editor - Mode 3*/
Image* img;

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;

	//here add your init stuff
	if (modo == 1)							//Ventana del paint - Modo 1
	{
		framebuffer.fill(Color::WHITE);

		for (int x = 0; x < 800; x++)
		{
			for (int y = 0; y < 30; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}
		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 800; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}
		for (int x = 750; x < 800; x++)
		{
			for (int y = 0; y < 800; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}
		for (int x = 0; x < 800; x++)
		{
			for (int y = 550; y < 800; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}

		/*Colors*/
		for (int x = 755; x < 770; x++)
		{
			for (int y = 530; y < 545; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLACK);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 510; y < 525; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLUE);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 490; y < 505; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::RED);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 470; y < 485; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::GREEN);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 450; y < 465; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::WHITE);
			}
		}

		/*Set Pixel Size*/
		for (int x = 730; x < 745; x++)
		{
			for (int y = 555; y < 570; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLACK);
				framebuffer.drawLine(730, 563, 744, 563);
				framebuffer.drawLine(730, 562, 744, 562);
				framebuffer.drawLine(730, 561, 744, 561);

				framebuffer.drawLine(737, 555, 737, 569);
				framebuffer.drawLine(738, 555, 738, 569);
				framebuffer.drawLine(736, 555, 736, 569);
			}
		}
		for (int x = 710; x < 725; x++)
		{
			for (int y = 555; y < 570; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLACK);
				framebuffer.drawLine(710, 563, 724, 563);
				framebuffer.drawLine(710, 562, 724, 562);
				framebuffer.drawLine(710, 561, 724, 561);
			}
		}
	}

	img = new Image();					//Load image
	if (img->loadTGA("imagen.tga"))
	{
		std::cout << "NOT FOUND" << std::endl;
	}
}

/*Geometric Figures Variables - Mode 1*/
int posFirstLineX, posFirstLineY, posSecondLineX, posSecondLineY;
int posFirstRectX, posFirstRectY, posSecondRectX, posSecondRectY;
int posFirstCircX, posFirstCircY, posSecondCircX, posSecondCircY;
boolean isFirstLine;
boolean isFirstRect;
boolean isFirstCirc;

/*Paint Variables - Mode 2*/
Color paintColor = Color::BLACK;
int paintSize = 5;
boolean paintAllowed;

/*Image Editor Variables - Mode 3*/
int posFirstTimeX, posFirstTimeY, posSecondTimeX, posSecondTimeY;
boolean isFirstTime;
boolean allowAnimationMode3_1;
boolean allowAnimationMode3_2;

/*Spiral Animation Variables - Mode 4*/
int t = 0;
boolean allowAnimation = false;
int startSpiralX;
int startSpiralY;

//render one frame
void Application::render(void)
{
	if (modo == 1)//Paint
	{
		if (paintAllowed)					//Para pintar mientras arrastras manteniendo pulsado el mouse izquierdo.
		{
			for (unsigned int i = 0; i < paintSize; i++)
			{
				for (unsigned int j = 0; j < paintSize; j++)
				{
					if (((mouse_position.x + paintSize) < 750 && (mouse_position.x - paintSize) > 10) && ((mouse_position.y + paintSize) < 550 && (mouse_position.y - paintSize) >= 30))
					{
						framebuffer.setPixel((mouse_position.x + i), (mouse_position.y + j), paintColor);
						framebuffer.setPixel((mouse_position.x + i), (mouse_position.y - j), paintColor);
						framebuffer.setPixel((mouse_position.x - i), (mouse_position.y + j), paintColor);
						framebuffer.setPixel((mouse_position.x - i), (mouse_position.y - j), paintColor);
					}
				}
			}
		}
	}
	else if (modo == 2)//Image Editor
	{
		if (allowAnimationMode3_1)									//Imagen con movimiento -> pulsar space.
		{
			for (int x = 0; x < img->width; ++x)
			{
				for (int y = 0; y < img->height; ++y)
				{
					Color c = img->getPixel(x, y);
					framebuffer.setPixelSafe((x + 50 * sin(time + y * 0.005)), (-(y - 500) + 50 * sin(time + x * 0.01)), c); //Move pixels
				}
			}
		}
		else if (allowAnimationMode3_2)								//Imagen con filtro -> pulsar click izquierdo.
		{
			for (int x = 0; x < 256; ++x)
			{
				for (int y = 0; y < 256; ++y)
				{
					int r = (int)framebuffer.getPixelSafe(x, y).r;
					int g = (int)framebuffer.getPixelSafe(x, y).g;
					int b = (int)framebuffer.getPixelSafe(x, y).b;

					int s = 0.2126 * r + 0.7152 * g + 0.0722 * b; //Transforma de RGB a grayscale
					Color c = Color(s, s, s);
					framebuffer.setPixelSafe(x, y, c);
				}
			}
		}
		else														//Imagen normal.
		{
			for (int x = 0; x < img->width; ++x)
			{
				for (int y = 0; y < img->height; ++y)
				{
					Color c = img->getPixel(x, y);
					framebuffer.setPixelSafe(x, -(y - 500), c);
				}
			}
		}
	}
	else if(modo == 3)//Animator - Mode 3 - Colored Spirals
	{
		if (allowAnimation){
			float x = 400;
			float y = 400;
			float angle = 0.0f;

			// Space between the spirals
			int a = 2, b = 2;
			angle = 0.1 * t;

			x = startSpiralX + (a + b * angle) * cos(angle);
			y = startSpiralY + (a + b * angle) * sin(angle);
			//Dibuja espiral pixel por pixel en función del tiempo, con colores random
			framebuffer.setPixelSafe(x, y, Color(rand() % 255, rand() % 255, rand() % 255));
			t++;
		}
	}

	//send image to screen
	showImage( &framebuffer );
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		if (modo == 2)		//Hacer que la imagen se mueva.
		{
			framebuffer.fill(Color(0, 0, 0));
			allowAnimationMode3_1 = true;
		}
	}
	/*RESTART MODES*/
	/*Each time a number key is pressed 1-4, mode is changed and screen restarted*/

	if (keystate[SDL_SCANCODE_1]) //if key 1 is pressed
	{
		modo = 0;
		framebuffer.fill(Color(0, 0, 0));
	}

	if (keystate[SDL_SCANCODE_2]) //if key 2 is pressed
	{									
		modo = 1;									//Ventana del paint.
		paintAllowed = false;
		framebuffer.fill(Color::WHITE);

		//Background colors
		for (int x = 0; x < 800; x++)
		{
			for (int y = 0; y < 30; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}
		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 800; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}
		for (int x = 750; x < 800; x++)
		{
			for (int y = 0; y < 800; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}
		for (int x = 0; x < 800; x++)
		{
			for (int y = 550; y < 800; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::CYAN);
			}
		}

		/*Selector de colores*/
		for (int x = 755; x < 770; x++)
		{
			for (int y = 530; y < 545; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLACK);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 510; y < 525; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLUE);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 490; y < 505; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::RED);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 470; y < 485; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::GREEN);
			}
		}
		for (int x = 755; x < 770; x++)
		{
			for (int y = 450; y < 465; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::WHITE);
			}
		}

		/*Set Pixel Size*/
		//Increase pixel size icon
		for (int x = 730; x < 745; x++)
		{
			for (int y = 555; y < 570; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLACK);
				framebuffer.drawLine(730, 563, 744, 563);
				framebuffer.drawLine(730, 562, 744, 562);
				framebuffer.drawLine(730, 561, 744, 561);

				framebuffer.drawLine(737, 555, 737, 569);
				framebuffer.drawLine(738, 555, 738, 569);
				framebuffer.drawLine(736, 555, 736, 569);
			}
		}
		//Decrease pixel size icon
		for (int x = 710; x < 725; x++)
		{
			for (int y = 555; y < 570; y++)
			{
				framebuffer.setPixelSafe(x, y, Color::BLACK);
				framebuffer.drawLine(710, 563, 724, 563);
				framebuffer.drawLine(710, 562, 724, 562);
				framebuffer.drawLine(710, 561, 724, 561);
			}
		}
	}

	if (keystate[SDL_SCANCODE_3]) //if key 3 is pressed
	{
		modo = 2;								//Se muestra la imagen original.
		framebuffer.fill(Color(0, 0, 0));

		allowAnimationMode3_1 = false;
		allowAnimationMode3_2 = false;
	}

	if (keystate[SDL_SCANCODE_4]) //if key 4 is pressed
	{
		modo = 3;
		framebuffer.fill(Color(0, 0, 0));

		allowAnimation = false;
	}
	//to read mouse position use mouse_position
}

//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch(event.keysym.scancode)
	{
		case SDL_SCANCODE_ESCAPE:
			exit(0); 
			break; //ESC key, kill the app

		case SDL_SCANCODE_SPACE:
			if (modo == 0)
			{
				framebuffer.fill(Color(0, 0, 0));
				framebuffer.drawLine(100,500,200, 500);
				framebuffer.drawLine(150,550, 150, 450);
				framebuffer.drawRectangle(300, 450, 50, 100);
				framebuffer.drawLine(450, 450, 500, 500);
				framebuffer.drawLine(450, 500, 500, 450);
				framebuffer.drawRectangle(600, 450, 100, 50);

				framebuffer.drawLine(100, 300, 150, 350);
				framebuffer.drawLine(150, 350, 200, 300);
				framebuffer.drawLine(200, 300, 250, 350);
				framebuffer.drawLine(250, 350, 300, 300);
				framebuffer.drawLine(300, 300, 350, 350);
				framebuffer.drawLine(350, 350, 400, 300);
				framebuffer.drawLine(400, 300, 450, 350);
				framebuffer.drawLine(450, 350, 500, 300);
				framebuffer.drawLine(500, 300, 550, 350);
				framebuffer.drawLine(550, 350, 600, 300);
				framebuffer.drawLine(600, 300, 650, 350);
				framebuffer.drawLine(650, 350, 700, 300);

				framebuffer.drawLine(200, 100, 200, 100);
				framebuffer.drawCircle(200, 100, 20);
				framebuffer.drawLine(400, 100, 400, 100);
				framebuffer.drawCircle(400, 100, 50);
				framebuffer.drawLine(600, 100, 600, 100);
				framebuffer.drawCircle(600, 100, 20);
			}
			break; //Demo Geometric Figures
	}

	if (modo == 1)				//Cambiar color del fondo - Paint Mode
	{
		if (keystate[SDL_SCANCODE_Q])
		{
			for (int x = 10; x < 750; x++)
			{
				for (int y = 30; y < 550; y++)
				{
					framebuffer.setPixelSafe(x, y, Color::BLACK);
				}
			}
		}
		if (keystate[SDL_SCANCODE_W])
		{
			for (int x = 10; x < 750; x++)
			{
				for (int y = 30; y < 550; y++)
				{
					framebuffer.setPixelSafe(x, y, Color::BLUE);
				}
			}
		}
		if (keystate[SDL_SCANCODE_E])
		{
			for (int x = 10; x < 750; x++)
			{
				for (int y = 30; y < 550; y++)
				{
					framebuffer.setPixelSafe(x, y, Color::RED);
				}
			}
		}
		if (keystate[SDL_SCANCODE_R])
		{
			for (int x = 10; x < 750; x++)
			{
				for (int y = 30; y < 550; y++)
				{
					framebuffer.setPixelSafe(x, y, Color::GREEN);
				}
			}
		}
		if (keystate[SDL_SCANCODE_TAB])
		{
			for (int x = 10; x < 750; x++)
			{
				for (int y = 30; y < 550; y++)
				{
					framebuffer.setPixelSafe(x, y, Color::WHITE);
				}
			}
		}
	}
}

//keyboard key up event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//...
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{	
	//Modo 0 - Geometric figures
	if(modo == 0){ 
		//Guardar dos puntos para pintar una línia entre ellos
		if(event.button == SDL_BUTTON_LEFT) //left mouse pressed
		{
			if (isFirstLine) {
				posFirstLineX = mouse_position.x;
				posFirstLineY = mouse_position.y;
				isFirstLine = false;
			}
			else
			{
				posSecondLineX = mouse_position.x;
				posSecondLineY = mouse_position.y;
				isFirstLine = true;
			}

			if (!isFirstLine)
			{
				if (posFirstLineX <= posSecondLineX)
				{
					framebuffer.drawLine(posFirstLineX, posFirstLineY, posSecondLineX, posSecondLineY);
				}
				else
				{
					framebuffer.drawLine(posSecondLineX, posSecondLineY, posFirstLineX, posFirstLineY);
				}
			}
		}

		//Seleccionar dos puntos para crear un rectangulo entre ambos (los dos puntos son diagonales)
		if (event.button == SDL_BUTTON_RIGHT) //left mouse pressed
		{
			if (isFirstRect) {
				posFirstRectX = mouse_position.x;
				posFirstRectY = mouse_position.y;
				isFirstRect = false;
			}
			else
			{
				posSecondRectX = mouse_position.x;
				posSecondRectY = mouse_position.y;
				isFirstRect = true;
			}

			if (!isFirstRect)
			{
				int width = posSecondRectX - posFirstRectX;
				int height = posSecondRectY - posFirstRectY;

				framebuffer.drawRectangle(posFirstRectX, posFirstRectY, width, height);
			}
		}

		//Seleccionar centro y radio del círculo para su dibujado
		if (event.button == SDL_BUTTON_MIDDLE) //left mouse pressed
		{
			int centerX;
			int centerY;
			int radius;

			if (isFirstCirc) {
				posFirstCircX = mouse_position.x;
				posFirstCircY = mouse_position.y;
				isFirstCirc = false;
			}
			else
			{
				posSecondCircX = mouse_position.x;
				posSecondCircY = mouse_position.y;
				isFirstCirc = true;
			}

			centerX = posSecondCircX;
			centerY = posSecondCircY;
			radius = sqrt((posFirstCircX - centerX)*(posFirstCircX - centerX) + (posFirstCircY - centerY) * (posFirstCircY - centerY));

			if (!isFirstCirc)
			{
				framebuffer.drawCircle(centerX, centerY, radius);
			}
		}
	}

	if (modo == 1) //Paint mode
	{
		if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
		{
			paintAllowed = true;
			/*Para que se pinte cuando se hace click izquierdo*/
			for (unsigned int i = 0; i < paintSize; i++)
			{
				for (unsigned int j = 0; j < paintSize; j++)
				{
					/*La zona en la que se puede pintar esta limitada*/
					if (((mouse_position.x + paintSize) < 750 && (mouse_position.x - paintSize) > 10) && ((mouse_position.y + paintSize) < 550 && (mouse_position.y - paintSize) >= 30))
					{
						framebuffer.setPixel((mouse_position.x + i), (mouse_position.y + j), paintColor);
						framebuffer.setPixel((mouse_position.x + i), (mouse_position.y - j), paintColor);
						framebuffer.setPixel((mouse_position.x - i), (mouse_position.y + j), paintColor);
						framebuffer.setPixel((mouse_position.x - i), (mouse_position.y - j), paintColor);
					}
				}
			}

			/*Para cambiar de color en el que se pinta*/
			if ((mouse_position.x < 770 && mouse_position.x >= 755) && (mouse_position.y < 545 && mouse_position.y >= 530))
			{
				paintColor = Color::BLACK;
			}
			if ((mouse_position.x < 770 && mouse_position.x >= 755) && (mouse_position.y < 525 && mouse_position.y >= 510))
			{
				paintColor = Color::BLUE;
			}
			if ((mouse_position.x < 770 && mouse_position.x >= 755) && (mouse_position.y < 505 && mouse_position.y >= 490))
			{
				paintColor = Color::RED;
			}
			if ((mouse_position.x < 770 && mouse_position.x >= 755) && (mouse_position.y < 485 && mouse_position.y >= 470))
			{
				paintColor = Color::GREEN;
			}
			if ((mouse_position.x < 770 && mouse_position.x >= 755) && (mouse_position.y < 465 && mouse_position.y >= 450))
			{
				paintColor = Color::WHITE;
			}

			/*Para cambiar el tamaño en el que se pinta*/
			if ((mouse_position.x < 745 && mouse_position.x >= 730) && (mouse_position.y < 570 && mouse_position.y >= 555))
			{
				if (paintSize <= 20)
				{
					paintSize += 1;
				}
			}
			if ((mouse_position.x < 725 && mouse_position.x >= 710) && (mouse_position.y < 570 && mouse_position.y >= 555))
			{
				if (paintSize > 1)
				{
					paintSize -= 1;
				}
			}
		}
	}

	if (modo == 2)	//Para poner un filtro en blanco y negro
	{
		if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
		{
			framebuffer.fill(Color(0, 0, 0));
			allowAnimationMode3_1 = false;
			allowAnimationMode3_2 = true;
			for (int x = 0; x < img->width; ++x)
			{
				for (int y = 0; y < img->height; ++y)
				{
					Color c = img->getPixel(x, y);
					framebuffer.setPixelSafe(x, -(y - 500), c);
				}
			}
		}
	}

	//Seleccionar punto de inicio de las espirales
	if (modo == 3) { 
		allowAnimation = true;
		t = 0;
		startSpiralX = mouse_position.x;
		startSpiralY = mouse_position.y;
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		if (modo == 1)						//Para pintar mientras arrastras manteniendo pulsado el mouse izquierdo.
		{
			paintAllowed = false;
		}
	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}