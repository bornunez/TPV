#pragma once
#include "PacManError.h"
class FileNotFoundError :
	public PacManError
{
public:
	FileNotFoundError(const string& _Message, string filename);
	~FileNotFoundError();
};

