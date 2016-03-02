#ifndef _GRAPHICS_RUNEITEM_H_
#define _GRAPHICS_RUNEITEM_H_

#include "Engine/Runes/Spell.h"

#include <QGraphicsEllipseItem>

namespace Runes
{
	class RuneItem : public QGraphicsItem
	{
	public:
		RuneItem();

		void setCircleRadius(int circle, float radius);
		
		//!@ brief updates circle radiuses given the spell
		void updateChidlren();

		void addCircle();
		void removeCircle();

	protected:
		// Members
		Spell* spell;	//!< The spell this rune draws

		vector<QGraphicsEllipseItem> circles;	//!< The circles. 0 is the center rune. Each additionnal circles corresponds to the depth of children.

		//----------------
		// Helper functions
		//! @brief gets the item correpsonding to the given spell
		RuneItem* getItemFromSpell(Spell*);

	};
}


#endif //_GRAPHICS_RUNEITEM_H_