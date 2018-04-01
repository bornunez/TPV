#include "Colisiones.h"



Colisiones::Colisiones(Game* game) : GameState(game)
{
	cout << "Empezando carga de recursos....";
	right = new Personaje(new Rect(0, 50, 10, 10), game->getTexture(Wall));
	stage.push_back(right);
	left = new Personaje(new Rect(800,50,10,10), game->getTexture(PowerUp));
	stage.push_back(left);
	pared = new Personaje(new Rect(400, 0, 50, 800), game->getTexture(Wall));
	stage.push_back(pared);
}


Colisiones::~Colisiones()
{
}

void Colisiones::update()
{

	if(!Physics::SDL_BoudingCollide(right->getRect(), pared->getRect()))
		dynamic_cast<Personaje*> (right)->getRect()->x += 5;
	dynamic_cast<Personaje*> (left)->getRect()->x -= 2;

//	Physics::collide(left->getRect(), pared->getRect());
}
