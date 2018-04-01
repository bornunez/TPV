#pragma once
#include "Rect.h"
#include "GameObject.h"
#include "assert.h"
/*returns maximum or minimum of number*/
#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)
class Physics
{
public:
	/*SDL_Collide.h by Amir 'Genjix' Taaki released under GNU GPL
				genjix@gmail.com
	so far only been used on Linux :D*/

/*To use this function simply call, does a pixel perfect check on 2 surfaces
	
	SDL_Collide( SurfaceA , Ax , Ay , SurfaceB , Bx , By );
	
where SurfaceA and SurfaceB are 2 surfaces you want to test for collision
and x and y are both their x and y values

	SDL_BoundingCollide( SurfaceA , Ax , Ay , SurfaceB , Bx , By );
	
uses exactly the same syntax, except that it's
a million times faster and should be used when prefferable
(such as on small objects), as it only checks the surfaces bounding box

	SDL_BoundingCollide(RectA , RectB);

does the same except variables are conveniently loaded into SDL_Rect's
and passed to function*/

/*if this header is not supported on your system comment out
the assert function call in SDL_TransparentPixel*/


/*
	returns 1 if offsets are a transparent pixel
*/
	static bool SDL_TransparentPixel(SDL_Surface *surface, int u, int v);

/*
	SDL pixel perfect collision test
*/
	static int SDL_Collide(SDL_Surface *as, int ax, int ay, SDL_Surface *bs, int bx, int by);

/*
	SDL bounding box collision test
*/
	static int SDL_BoundingCollide(SDL_Surface *sa, int ax, int ay, SDL_Surface *sb, int bx, int by);

/*
	SDL bounding box collision tests (works on SDL_Rect's)
*/
	static int SDL_BoudingCollide(Rect* a, Rect* b);


};

