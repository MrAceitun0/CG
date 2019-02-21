#include "application.h"
#include "utils.h"
#include "image.h"
#include "mesh.h"
#include "time.h"
#include <math.h>

using namespace std;

Mesh* mesh = NULL;
Camera* camera = NULL;

// normalize the depth in the range [near, far]
float normalizeDepth(float d)
{
	float   rmin = 0, // minimum of the range of depths -- empirically determined
		rmax = 2147483648, // maximum of the range of depths -- empirically determined
		tmin = camera->near_plane, // minimum of the range of target scale (near)
		tmax = camera->far_plane; // maximum of the range of target scale (far)

	return ((d - rmin) / (rmax - rmin)) * (tmax - tmin) + tmin; // just math!
}

// barycentric interpolation of triangles
// this is useful to get the depth of every pixel inside the triangle
float getDepth(Vector3 p1, Vector3 p2, Vector3 p3, unsigned int x, unsigned int y)
{
	float w1 = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) /
		((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

	if (w1 < 0) return INT_MAX;

	float w2 = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) /
		((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

	if (w1 < 0) return INT_MAX;

	float w3 = 1 - w1 - w2;

	return normalizeDepth((p1 * w1 + p2 * w2 + p3 * w3).z);
}

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

//render framebuffer
Image framebuffer(800, 800);
FloatImage zbuffer(800,800);

//render one frame
void Application::render(void)
{
	//float xRatio = fabs(1 - (-1)) / fabs(this->window_width - 0);
	//float yRatio = fabs(1 - (-1)) / fabs(this->window_height - 0);

	framebuffer.fill(Color(40, 45, 60 )); //pale blue

	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 800; j++)
		{
			zbuffer.setPixel(i, j, INT_MAX);
		}
	}

	//for every point of the mesh (to draw triangles take three points each time and connect the points between them (1,2,3,   4,5,6,   ... )
	for (int i = 0; i != mesh->vertices.size(); ++i)
	{
		Vector3 vertex = mesh->vertices[i]; //extract vertex from mesh
		
		//project every point in the mesh to normalized coordinates using the viewprojection_matrix inside camera
		Vector3 normalized_point = camera->projectVector( vertex );

		if (normalized_point.x >= -1 && normalized_point.x <= 1 && normalized_point.y >= -1 && normalized_point.y <= 1)
		{
			float finalX;
			float finalY;

			if (normalized_point.x > 0)
			{
				finalX = (normalized_point.x * (this->window_width / 2)) + (this->window_width / 2);
			}
			else if (normalized_point.x < 0)
			{
				finalX = (normalized_point.x * (this->window_width / 2)) + (this->window_width / 2);
			}
			else {
				finalX = this->window_width / 2;
			}

			if (normalized_point.y > 0)
			{
				finalY = (normalized_point.y * (this->window_height / 2)) + (this->window_height / 2);
			}
			else if (normalized_point.y < 0)
			{
				finalY = (normalized_point.y * (this->window_height / 2)) + (this->window_height / 2);
			}
			else {
				finalY = this->window_height / 2;
			}

			//paint point in framebuffer
			Vector2 finalV;
			finalV.x = finalX;
			finalV.y = finalY;
			points.push_back(finalV);
		}
	}

	Vector3 tri0;
	Vector3 tri1;
	Vector3 tri2;

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
	}
	
	points.clear();
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

	if (keystate[SDL_SCANCODE_UP])
	{
		camera->eye.y -= 5 * seconds_elapsed;
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		camera->eye.y += 5 * seconds_elapsed;
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		camera->eye.x += 5 * seconds_elapsed;
	}
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		camera->eye.x -= 5 * seconds_elapsed;
	}

	if (keystate[SDL_SCANCODE_A])
	{
		camera->center.x -= 5 * seconds_elapsed;
	}
	if (keystate[SDL_SCANCODE_D])
	{
		camera->center.x += 5 * seconds_elapsed;
	}
	if (keystate[SDL_SCANCODE_W])
	{
		camera->center.y += 5 * seconds_elapsed;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		camera->center.y -= 5 * seconds_elapsed;
	}

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
