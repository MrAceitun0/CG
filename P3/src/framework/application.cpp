#include "application.h"
#include "utils.h"
#include "image.h"
#include "mesh.h"
#include "time.h"
#include <math.h>

using namespace std;

Mesh* mesh = NULL;
Camera* camera = NULL;

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

vector<Vector2> points;

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,10,20),Vector3(0,10,0),Vector3(0,1,0)); //define eye,center,up
	camera->perspective(60, window_width / (float)window_height, 0.1, 10000); //define fov,aspect,near,far

	//load a mesh
	mesh = new Mesh();
	if( !mesh->loadOBJ("lee.obj") )
		std::cout << "FILE Lee.obj NOT FOUND" << std::endl;
}  

//render one frame
void Application::render(void)
{
	//render framebuffer
	Image framebuffer( window_width, window_height );

	float xRatio = fabs(1 - (-1)) / fabs(this->window_width - 0);
	float yRatio = fabs(1 - (-1)) / fabs(this->window_height - 0);

	framebuffer.fill(Color(40, 45, 60 )); //pale blue

	//for every point of the mesh (to draw triangles take three points each time and connect the points between them (1,2,3,   4,5,6,   ... )
	for (int i = 0; i != mesh->vertices.size(); ++i)
	{
		Vector3 vertex = mesh->vertices[i]; //extract vertex from mesh
		
		//project every point in the mesh to normalized coordinates using the viewprojection_matrix inside camera
		Vector3 normalized_point = camera->projectVector( vertex );

		if (normalized_point.x >= -1 && normalized_point.x <= 1 && normalized_point.y >= -1 && normalized_point.y <= 1)
		{
			//convert from normalized (-1 to +1) to framebuffer coordinates (0,W)
			float finalX = normalized_point.x * xRatio;
			float finalY = normalized_point.y * yRatio;

			//paint point in framebuffer
			Vector2 finalV;
			finalV.x = finalX;
			finalV.y = finalY;
			points.push_back(finalV);
		}
	}

	Vector2 tri0;
	Vector2 tri1;
	Vector2 tri2;

	for (std::vector<Vector2>::size_type i = 0; i < points.size(); ++i) 
	{
		if (i % 3 == 0)
		{
			tri0.x = points[i].x;
			tri0.y = points[i].y;
		}
		else if (i % 3 == 1)
		{
			tri1.x = points[i].x;
			tri1.y = points[i].y;
		}
		else if (i % 3 == 2)
		{
			tri2.x = points[i].x;
			tri2.y = points[i].y;
			framebuffer.drawTriangleBarycenter(tri0.x, tri0.y, tri1.x, tri1.y, tri2.x, tri2.y, Color::RED, Color::GREEN, Color::BLUE);
		}
		//framebuffer.drawTriangleBarycenter(points[i].x, points[i].y, points[i+1].x, points[i+1].y, points[i+2].y, points[i+2].y, Color::RED, Color::GREEN, Color::BLUE);
	}


	//render to screen
	showImage( &framebuffer );

}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE])
	{
		//...
	}

	//example to move eye
	if (keystate[SDL_SCANCODE_LEFT])
		camera->eye.x -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_RIGHT])
		camera->eye.x += 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_UP])
		camera->eye.z -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_DOWN])
		camera->eye.z += 5 * seconds_elapsed;

	if (keystate[SDL_SCANCODE_A])
		camera->center.x -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_D])
		camera->center.x += 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_W])
		camera->center.z -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_S])
		camera->center.z += 5 * seconds_elapsed;

	if (keystate[SDL_SCANCODE_F])
		camera->fov -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_G])
		camera->fov += 5 * seconds_elapsed;

	//if we modify the camera fields, then update matrices
	camera->updateViewMatrix();
	camera->updateProjectionMatrix();
}

//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
	}
}

//keyboard released event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{

	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
