#pragma once
#include "PacManError.h"
class SDLError :
	public PacManError
{
public:
	SDLError(const string& _Message);
	~SDLError();
};

