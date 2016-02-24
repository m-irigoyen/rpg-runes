#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Audio/AudioEngine.h"
#include "Graphics/GraphicsEngine.h"
#include "Network/NetworkEngine.h"
#include "Runes/RuneEngine.h"

namespace Runes
{
	class Engine
	{
	public:
		Engine();

		// Init all modules, and make everything run
		void init();

		// Runs the game
		void run();

	private:
		AudioEngine audio;
		GraphicsEngine graphics;
		NetworkEngine network;
		RuneEngine runes;


	};
}


#endif //_ENGINE_H_