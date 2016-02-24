#ifndef _GRAPHICSENGINE_H_
#define _GRAPHICSENGINE_H_

#include "SFML/Graphics/RenderWindow.hpp"

using namespace sf;

namespace Runes
{
	class GraphicsEngine
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();

		// Init the graphics engine
		void init();


	private:
		RenderWindow* window;
	};
}


#endif //_GRAPHICSENGINE_H_