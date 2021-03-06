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

	try {
		if (texture == nullptr)
			//El mensaje Fallo cargando textura se pasa por constructor o lo pongo automatico?
			throw  FileNotFoundError("Fallo cargando textura: ", filename);
		
		//Cuando tengamos seguro que hay textura, actualizamos nuestros datos
		w = surface->w;
		h = surface->h;
		fw = w / numCols;
		fh = h / numRows;
	
		SDL_FreeSurface(surface); // Se borra la estructura auxiliar
		//return false;
		return true;
			
	}
	catch (exception &e)
	{
		cerr << "Caught: " << e.what() << endl;
		cerr << "Type: " << typeid(e).name() << endl;
		SDL_FreeSurface(surface);
		return false;
	};
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
	//Y LO A�ADIMOS A LA COLA
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

//CARGA LA TEXTURA A PARTIR DE UNA FUENTE DE LETRA
bool Texture::loadFromText(string	text, const	Font&	font, SDL_Color	color) {
	SDL_Surface*	textSurface = font.generateSurface(text, color);
	if (textSurface == nullptr)
		cout << "Unable	to	render	text	surface!	SDL_ttf	Error:	"
		<< TTF_GetError() << "\n";
	else {
		//free();
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == nullptr) {
			cout << "Unable	to	create	texture	from	text!	SDL	Error:	"
				<< SDL_GetError() << "\n";
			w = h = 0;
		}
		else {
			w = textSurface->w;
			h = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return	texture != nullptr;
}
