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

int figure = 0;

boolean fill = false;
Color fillColor = Color(255, 255, 255);

int posFirstLineDDAX, posFirstLineDDAY, posSecondLineDDAX, posSecondLineDDAY;
boolean isFirstLineDDA;
int posFirstLineBX, posFirstLineBY, posSecondLineBX, posSecondLineBY;
boolean isFirstLineB;
int posFirstCircX, posFirstCircY, posSecondCircX, posSecondCircY;
boolean isFirstCirc;

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
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		figure = 0;
	}
	/*Line DDA*/
	if (keystate[SDL_SCANCODE_1]) //if key 1 is pressed, restart app showing a black screen
	{
		figure = 0;
	}
	/*Line Bresenham*/
	if (keystate[SDL_SCANCODE_2]) //if key 1 is pressed, restart app showing a black screen
	{
		figure = 1;
	}
	/*Circle*/
	if (keystate[SDL_SCANCODE_3]) //if key 1 is pressed, restart app showing a black screen
	{
		figure = 2;
	}
	/*Triangle*/
	if (keystate[SDL_SCANCODE_4]) //if key 1 is pressed, restart app showing a black screen
	{
		figure = 3;
	}
	/*Fill*/
	if (keystate[SDL_SCANCODE_TAB]) //if key 1 is pressed, restart app showing a black screen
	{
		fill = !fill;
	}
	/*RESTART*/
	if (keystate[SDL_SCANCODE_5]) //if key 1 is pressed, restart app showing a black screen
	{
		framebuffer.fill(Color(0, 0, 0));
	}
	/*Fill with white color*/
	if (keystate[SDL_SCANCODE_A]) //if key 1 is pressed, restart app showing a black screen
	{
		fillColor = Color(255, 255, 255);
	}
	/*Fill with red*/
	if (keystate[SDL_SCANCODE_S]) //if key 1 is pressed, restart app showing a black screen
	{
		fillColor = Color(255, 0, 0);
	}
	/*Fill with green*/
	if (keystate[SDL_SCANCODE_D]) //if key 1 is pressed, restart app showing a black screen
	{
		fillColor = Color(0, 255, 0);
	}
	/*Fill with blue*/
	if (keystate[SDL_SCANCODE_F]) //if key 1 is pressed, restart app showing a black screen
	{
		fillColor = Color(0, 0, 255);
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
	//Guardar dos puntos para pintar una línia entre ellos
	if(event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
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

		if (figure == 1)
		{
			if (isFirstLineB) {
				posFirstLineBX = mouse_position.x;
				posFirstLineBY = mouse_position.y;
				isFirstLineB = false;
			}
			else
			{
				posSecondLineBX = mouse_position.x;
				posSecondLineBY = mouse_position.y;
				isFirstLineB = true;
			}

			if (!isFirstLineB)
			{
				if (posFirstLineBX <= posSecondLineBX)
				{
					framebuffer.drawLineB(posFirstLineBX, posFirstLineBY, posSecondLineBX, posSecondLineBY, fillColor);
				}
				else
				{
					framebuffer.drawLineB(posSecondLineBX, posSecondLineBY, posFirstLineBX, posFirstLineBY, fillColor);
				}
			}
		}

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

		if (figure == 3)
		{

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