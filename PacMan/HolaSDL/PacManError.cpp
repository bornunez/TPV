#include "PacManError.h"



PacManError::PacManError(const string& _Message) : logic_error(_Message)
{
}


PacManError::~PacManError()
{
}
