#include "Texture.h"



Texture::Texture(SDL_Renderer* r,const string path)
{
	renderer = r;
	PATH = path;
}


Texture::~Texture()
{
}
bool Texture:: load(string filename, uint numRows, uint numCols) {
	//Carga de textura en la estructura auxiliar
	string name = PATH + filename;//Hay que hacer la concatenacion aqui
	SDL_Surface* surface = IMG_Load(name.c_str()); 
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == nullptr) {
		SDL_FreeSurface(surface); 
		cout << "Fallo cargando textura " << filename << endl;
		return true; //SI NO SE CARGA SALIMOS

	}
	else {
		//Cuando tengamos seguro que hay textura, actualizamos nuestros datos
		w = surface->w;
		h = surface->h;
		fw = w / numCols;
		fh = h / numRows;
	}
	SDL_FreeSurface(surface); // Se borra la estructura auxiliar
	return false;
}
void Texture::render(const SDL_Rect&	rect, SDL_RendererFlip	flip) {
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
void Texture:: renderFrame(const SDL_Rect&	destRect, int	row, int	col, SDL_RendererFlip	flip) {
	//CREAMOS EL RECT DEL FRAME
	SDL_Rect srcRect;
	srcRect.h = fh;
	srcRect.w = fw;
	srcRect.x = col * fw;
	srcRect.y = row * fh;
	//Y LO AÑADIMOS A LA COLA
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
