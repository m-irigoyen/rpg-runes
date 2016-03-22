#include "GraphicsEngine.h"

namespace Runes
{
	GraphicsEngine::GraphicsEngine(RuneEngine& runeEngineRef) : runeEngine_(runeEngineRef), globalRunes_(runeEngine_.getRunes()), userRunes_(runeEngine_.getUserRunes()), currentSpellItem_(NULL), isText_(false)
	{
		//init();
	}

	GraphicsEngine::~GraphicsEngine()
	{

	}

	void GraphicsEngine::init()
	{
		scene_.setBackgroundBrush(RuneItem::colorBackground());
		view_.setScene(&scene_);

		// Loading rune images
		reloadSprites();
		
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

	vector<QPixmap>& GraphicsEngine::getSprites()
	{
		return runeSprites_;
	}

	void GraphicsEngine::toggleText()
	{
		isText_ = !isText_;
		currentSpellItem_->setIsText(isText_);
	}

	void GraphicsEngine::reloadSprites()
	{
		runeSprites_.clear();
		QString runeFilePath = Paths::RUNES;
		// Load all rune images
		for (Rune r : globalRunes_)
		{
			QString name = r.descriptor_.naturalName_;

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

	void GraphicsEngine::drawSpell(Spell* s)
	{
		if (currentSpellItem_ != NULL)
			currentSpellItem_->clearItem();
		scene_.clear();
		delete(currentSpellItem_);

		currentSpellItem_ = new SpellItem(s, NULL, runeSprites_, runeEngine_, scene_);
		currentSpellItem_->drawSpell();
	}

}

