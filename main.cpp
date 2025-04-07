#include <iostream>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include<math.h>

#define COLOR_WHITE 0xffffffff
#define COLOR_RAY 0xc78718
#define COLOR_BLACK 0x00000000
#define RAYS_NUMBER 300
#define OBSTACLE_CIRCLE_COUNT 5

struct Circle {
	double x;
	double y;
	double radius;
};

void FillCircle(SDL_Surface* surface, struct Circle* circle, Uint32 color, int size) {
	for ( int i=0; i<size; i++) {
		for (double x= circle[i].x- circle[i].radius; x<= circle[i].x+circle[i].radius; x++) {
			for (double y= circle[i].y- circle[i].radius; y<= circle[i].y+circle[i].radius; y++) {
				if ((x-circle[i].x)*(x-circle[i].x) + (y-circle[i].y)*(y-circle[i].y) < circle[i].radius*circle[i].radius) {
					SDL_Rect temp= {x,y,1,1};
					SDL_FillRect(surface, &temp, color);
				}
			}
		}
	}
};

void draw_rays(SDL_Surface* surface, SDL_Rect rect, Uint32 color, struct Circle* circle) {
	double angle =0;
	double x= rect.x;
	double y= rect.y;
	int end_of_screen;
	int object_hit;
	int step;
	for ( int i=0; i<RAYS_NUMBER; i++) {
		end_of_screen = 0;
		object_hit= 0;
		step= 0;
		while (!end_of_screen && !object_hit) {
			rect.x = x+ step*cos(angle);
			rect.y = y+ step*sin(angle);
			if (rect.x > 800 || rect.y > 600 || rect.x< 0 || rect.y < 0) {
				end_of_screen = 1;
			}
			for ( int j=0; j<OBSTACLE_CIRCLE_COUNT; j++) {
				if (pow(rect.x-circle[j].x,2) + pow(rect.y-circle[j].y,2) <= circle[j].radius*circle[j].radius) {
					object_hit = 1;
				}
			}
			SDL_FillRect(surface, &rect, color);
			step++;
		}
		angle = angle + (2*M_PI/RAYS_NUMBER);
	}
};

int main( int argc, char *argv[]) {
	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "subsystems initialised"<< std::endl;	
	}
	SDL_Window *window = SDL_CreateWindow("hello world", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window) {
		std::cout << "Could not create window\n" << SDL_GetError << std::endl;
	}

	SDL_Surface* surface = SDL_GetWindowSurface(window);
	
	SDL_Rect erase_rect = { 0 , 0 , 800 , 600 };

	struct Circle circle[1];
	circle[0]= { 200, 200, 25 };
	struct Circle circle2[OBSTACLE_CIRCLE_COUNT];
	circle2[0]= {450, 450, 50};
	circle2[1]= {350, 250, 67};
	circle2[2]= {400, 120, 10};
	circle2[3]= {200, 350, 20};
	circle2[4]= {200, 150, 40};

	int obstacle_speed_y = 1;
	bool status= true;
	SDL_Event event;
	while (status) {
		
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			status = false;
		}
			if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
				circle[0].y = circle[0].y - 10;
				std::cout<< "moving up"<< std::endl;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
				circle[0].x = circle[0].x + 10;
				std::cout<< "moving right" << std::endl;
			}
			else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
				circle[0].x = circle[0].x - 10;
				std::cout<< "moving left" << std::endl;
			}
			else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
				circle[0].y = circle[0].y + 10;
				std::cout<< "moving down" << std::endl;
			}
		}
		if (event.type == SDL_MOUSEMOTION && event.motion.state) {
			circle[0].x = event.motion.x;
			circle[0].y = event.motion.y;
			std::cout<< event.motion.x << " "<< event.motion.y<< std::endl;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			std::cout<< "mouse button pressed"<< std::endl;
		}
		
		SDL_Rect lines= {circle[0].x, circle[0].y,1,1};
		
		SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
		draw_rays(surface, lines, COLOR_RAY, circle2);
		FillCircle(surface, circle2, COLOR_WHITE, OBSTACLE_CIRCLE_COUNT);
		FillCircle(surface, circle, COLOR_WHITE, 1);
		
		SDL_UpdateWindowSurface(window);

		// SDL_Delay(10);

	}
	
	std::cout<< "closing"<< std::endl;
	SDL_DestroyWindow(window);
	SDL_Quit();
	// SDL_Delay(3000);

	return EXIT_SUCCESS;
}