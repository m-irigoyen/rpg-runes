#ifndef _GRAPHICS_RUNEITEM_H_
#define _GRAPHICS_RUNEITEM_H_

#include "Engine/Runes/RuneEngine.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsPixMapItem>

#define _GRAPHICS_RUNEITEM_RUNEW 10
#define _GRAPHICS_RUNEITEM_RUNEH 10

namespace Runes
{
	class RuneItem : public QGraphicsEllipseItem
	{
	public:
		RuneItem(Spell* s, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes);

		//!@ brief updates circle radiuses given the spell
		void updateChidlren();

		//! @brief returns the total radius of this spell. Depending wether the spell is a top level or not, won't return the same
		float getTotalRadius();


	protected:
		// Members
		Spell* spell_;	//!< The spell this rune draws

		// The graphics
		QGraphicsSimpleTextItem text_;	//!< If the center par of the spell is a rune, this is its natural name
		QGraphicsPixmapItem rune_;	//!< If the center part of the spel is a rune, this is its image
		QGraphicsEllipseItem innerCircle_;	//!< The circle around the center part of the spell.
		QGraphicsEllipseItem outerCircle_;	//!< The circle around the components of the spell.
	};
}


#endif //_GRAPHICS_RUNEITEM_H_