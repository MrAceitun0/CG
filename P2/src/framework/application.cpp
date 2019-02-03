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
Image framebuffer(1800, 900);

int figure = 1;

boolean fill = false;
Color fillColor = Color(255, 255, 255);

/*DDA*/
int posFirstLineDDAX, posFirstLineDDAY, posSecondLineDDAX, posSecondLineDDAY;
boolean isFirstLineDDA;

/*Bresenham*/
int x_B0, y_B0, x_B1, y_B1;
boolean isFirstLineB;

/*Círculo*/
int posFirstCircX, posFirstCircY, posSecondCircX, posSecondCircY;
boolean isFirstCirc;

/*Triángulo*/
int x_tri0, y_tri0, x_tri1, y_tri1, x_tri2, y_tri2;
int click_time = 0;
Color vertex0 = Color::WHITE;
Color vertex1 = Color::WHITE;
Color vertex2 = Color::WHITE;

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
}

//render one frame
void Application::render(void)
{
	//send image to screen
	showImage( &framebuffer );
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	/*Demo*/
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed, 
	{
		figure = 0;

		click_time = 0;		//Para reiniciar el número de clicks del triangulo
	}
	/*Line DDA*/
	if (keystate[SDL_SCANCODE_1]) //if key 1 is pressed, 
	{
		figure = 0;

		click_time = 0;		//Para reiniciar el número de clicks del triangulo
	}
	/*Line Bresenham*/
	if (keystate[SDL_SCANCODE_2]) //if key 2 is pressed, 
	{
		figure = 1;

		click_time = 0;		//Para reiniciar el número de clicks del triangulo
	}
	/*Circle*/
	if (keystate[SDL_SCANCODE_3]) //if key 3 is pressed, 
	{
		figure = 2;

		click_time = 0;		//Para reiniciar el número de clicks del triangulo
	}
	/*Triangle*/
	if (keystate[SDL_SCANCODE_4]) //if key 4 is pressed, 
	{
		figure = 3;
	}
	/*Fill*/
	if (keystate[SDL_SCANCODE_Q]) //if key TAB is pressed, 
	{
		fill = true;
	}
	if (keystate[SDL_SCANCODE_W]) //if key TAB is pressed, 
	{
		fill = false;
	}
	/*RESTART*/
	if (keystate[SDL_SCANCODE_5]) //if key 5 is pressed, restart app showing a black screen
	{
		framebuffer.fill(Color(0, 0, 0));
	}
	/*Fill with white color*/
	if (keystate[SDL_SCANCODE_A]) //if key A is pressed, 
	{
		fillColor = Color::WHITE;
	}
	/*Fill with red*/
	if (keystate[SDL_SCANCODE_S]) //if key S is pressed, 
	{
		fillColor = Color::RED;
	}
	/*Fill with green*/
	if (keystate[SDL_SCANCODE_D]) //if key D is pressed, 
	{
		fillColor = Color::GREEN;
	}
	/*Fill with blue*/
	if (keystate[SDL_SCANCODE_F]) //if key F is pressed, 
	{
		fillColor = Color::BLUE;
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
			break; //Demo Geometric Figures
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
	if(event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		/*Línea DDA*/
		if (figure == 0)
		{
			if (isFirstLineDDA) {
				posFirstLineDDAX = mouse_position.x;
				posFirstLineDDAY = mouse_position.y;
				isFirstLineDDA = false;
			}
			else
			{
				posSecondLineDDAX = mouse_position.x;
				posSecondLineDDAY = mouse_position.y;
				isFirstLineDDA = true;
			}

			if (!isFirstLineDDA)
			{
				if (posFirstLineDDAX <= posSecondLineDDAX)
				{
					framebuffer.drawLineDDA(posFirstLineDDAX, posFirstLineDDAY, posSecondLineDDAX, posSecondLineDDAY, fillColor);
				}
				else
				{
					framebuffer.drawLineDDA(posSecondLineDDAX, posSecondLineDDAY, posFirstLineDDAX, posFirstLineDDAY, fillColor);
				}
			}
		}
		/*Línea Bresenham*/
		if (figure == 1)
		{
			if (isFirstLineB) 
			{
				x_B0 = mouse_position.x;
				y_B0 = mouse_position.y;
				isFirstLineB = false;
			}
			else
			{
				x_B1 = mouse_position.x;
				y_B1 = mouse_position.y;
				isFirstLineB = true;
			}

			if (!isFirstLineB)
			{
				framebuffer.drawLineBresenham(x_B0, y_B0, x_B1, y_B1, fillColor);
			}
		}
		/*Círculo*/
		if (figure == 2)
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
				framebuffer.drawCircleBresenham(centerX, centerY, radius, fillColor, fill);
			}
		}
		/*Triángulo*/
		if (figure == 3)
		{
			if (click_time == 0)
			{
				x_tri0 = mouse_position.x;
				y_tri0 = mouse_position.y;

				click_time++;
			}
			else if (click_time == 1)
			{
				x_tri1 = mouse_position.x;
				y_tri1 = mouse_position.y;

				click_time++;
			}
			else if (click_time == 2)
			{
				x_tri2 = mouse_position.x;
				y_tri2 = mouse_position.y;

				click_time = 0;

				framebuffer.drawTriangle(x_tri0, y_tri0, x_tri1, y_tri1, x_tri2, y_tri2, fillColor, fill);
			}
		}	
	}
}


void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}