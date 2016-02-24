#include "Engine.h"

Runes::Engine::Engine()
{
}

void Runes::Engine::init()
{
	this->runes.init();
	this->network.init();
	this->audio.init();
	this->graphics.init();
}

void Runes::Engine::run()
{



}
