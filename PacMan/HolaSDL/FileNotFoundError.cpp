#include "FileNotFoundError.h"



FileNotFoundError::FileNotFoundError(const string& _Message, string filename) : PacManError(_Message + filename)
{
}


FileNotFoundError::~FileNotFoundError()
{
}
