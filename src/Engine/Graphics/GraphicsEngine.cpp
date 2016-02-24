#include "GraphicsEngine.h"

Runes::GraphicsEngine::GraphicsEngine()
{
}

Runes::GraphicsEngine::~GraphicsEngine()
{
	if (window)
	{
		window->close();
		delete(window);
	}
	
}

void Runes::GraphicsEngine::init()
{
}
