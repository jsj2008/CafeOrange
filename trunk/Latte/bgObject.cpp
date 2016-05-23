#include "stdafx.h"
#include "bgObject.h"


bgObject::bgObject()
{
	modified = false;
}

bgObject::~bgObject()
{
}

bool bgObject::initialize()
{

	return true;
}

bool bgObject::update()
{
	return true;
}

bool bgObject::build(cRenderer& Renderer)
{
	return true;
}


