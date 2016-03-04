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

		// Loading rune images
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
		
		// Setting up the scene
		Spell* s = runeEngine_.getCurrentSpell();
		Q_ASSERT(s);
		
		// Loading the current spell
	}

	QGraphicsView* GraphicsEngine::getView()
	{
		return &view_;
	}

	void GraphicsEngine::updateScene()
	{
		scene_.clear();

	}

	void GraphicsEngine::drawSpell(Spell* s)
	{
		
	}

	void GraphicsEngine::placeChild(RuneItem& child, RuneItem& parent)
	{

	}

	RuneItem* GraphicsEngine::createItem(Spell* s)
	{
		/*RuneItem item(s, runeSprites_);
		runeItems_.push_back(item);*/
		return NULL;
	}
}

