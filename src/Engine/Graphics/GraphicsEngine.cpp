#include "GraphicsEngine.h"

namespace Runes
{
	GraphicsEngine::GraphicsEngine(RuneEngine& runeEngineRef) : runeEngine_(runeEngineRef), globalRunes_(runeEngine_.getRunes()), userRunes_(runeEngine_.getUserRunes())
	{
		init();
	}

	GraphicsEngine::~GraphicsEngine()
	{

	}

	void GraphicsEngine::init()
	{
		view_.setScene(&scene_);

		{
			QString runeFilePath = Paths::RUNES;
			QString extension = ".bmp";
			// Load all rune images
			for (Rune r : globalRunes_)
			{
				QString name = r.getName();

				QPixmap runeImage;
				if (runeImage.load(runeFilePath + name.toLower() + extension))
				{
					runeSprites_.push_back(runeImage);
				}
				else
				{
					//TODO : error code
				}
			}
		}
		
	}

	QGraphicsView* GraphicsEngine::getView()
	{
		return &view_;
	}

	void GraphicsEngine::updateScene()
	{
		scene_.clear();

		// Recusively add elements
	}

}

