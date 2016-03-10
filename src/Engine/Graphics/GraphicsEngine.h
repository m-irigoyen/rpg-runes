#ifndef _GRAPHICSENGINE_H_
#define _GRAPHICSENGINE_H_

#include "Engine/Runes/RuneEngine.h"
#include "SpellItem.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBitmap>

namespace Runes
{
	class GraphicsEngine
	{
	public:
		GraphicsEngine(RuneEngine& runeEngineRef);
		~GraphicsEngine();

		// Init the graphics engine
		void init();

		QGraphicsView* getView();		

		//! @brief Updates the scene to correspond to draw the current spell
		void updateScene();

	private:
		// Members
		QGraphicsScene scene_;
		QGraphicsView view_;

		vector<QPixmap> runeSprites_; //!< The sprites of the runes, ordered by index
		SpellItem* currentSpellItem_;
		
		// Rune Engine references
		RuneEngine& runeEngine_;
		RunesContainer& globalRunes_;
		UserRunesContainer& userRunes_;

		//----------
		//Functions
		//! @brief draws the given spell onto the scene
		void drawSpell(Spell* s);
	};
}


#endif //_GRAPHICSENGINE_H_