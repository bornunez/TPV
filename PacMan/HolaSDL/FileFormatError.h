#pragma once
#include "PacManError.h"
class FileFormatError :
	public PacManError
{
public:
	FileFormatError(const string& _Message);
	~FileFormatError();
};

