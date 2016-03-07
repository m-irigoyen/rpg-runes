#include "GraphicsEngine.h"

namespace Runes
{
	GraphicsEngine::GraphicsEngine(RuneEngine& runeEngineRef) : runeEngine_(runeEngineRef), globalRunes_(runeEngine_.getRunes()), userRunes_(runeEngine_.getUserRunes())
	{
		//init();
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
			// Load all rune images
			for (Rune r : globalRunes_)
			{
				QString name = r.getNaturalName();

				QPixmap runeImage;
				if (runeImage.load(runeFilePath + name.toLower() + Paths::IMAGEEXTENSION))
				{
					runeSprites_.push_back(runeImage);
				}
				else
				{
					cout << "ERROR : couldn't load " << (runeFilePath + name.toLower() + Paths::IMAGEEXTENSION).toStdString() << endl;
					abort();
				}
			}
		}
		
		// Setting up the scene
		Spell* s = runeEngine_.getCurrentSpell();
		Q_ASSERT(s);
		
		// Loading the current spell
		drawSpell(s);

		cout << "The scene contains : " << scene_.items().size() << endl;
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
		runeItem_.drawSpell(s, runeSprites_, globalRunes_, userRunes_, scene_, NULL);
	}

}

