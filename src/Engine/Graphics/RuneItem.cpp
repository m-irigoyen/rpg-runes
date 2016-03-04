#include "RuneItem.h"

namespace Runes
{
	RuneItem::RuneItem(Spell* s, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes) : spell_(s)
	{
		outerCircle_.setParentItem(this);
		innerCircle_.setParentItem(this);
		

		Q_ASSERT(s);
		if (s->isCenterSpell())
		{

		}
		else
		{
			rune_.setParentItem(this);

			// Setting the text
			UserRune& ur = userRunes.at(s->getRune());
			if (ur.second)
				text_.setText(ur.first.getNaturalName());
			// Setting the image
			rune_.setPixmap(runeImages[s->getRune()]);

			// Setting the transform point
			rune_.setTransformOriginPoint(_GRAPHICS_RUNEITEM_RUNEW / 2, _GRAPHICS_RUNEITEM_RUNEH / 2);

			// Inner circle : around the rune
			innerCircle_.setTransformOriginPoint(0, 0);
			innerCircle_.setRect(0, 0, _GRAPHICS_RUNEITEM_RUNEW, _GRAPHICS_RUNEITEM_RUNEH);

			if (s->getComponents().empty())
			{
				// no components : outer circle = inner circle
				//innerCircle_.
			}
			else
			{				
				innerCircle_.setTransformOriginPoint(0, 0);
				innerCircle_.setRect(0, 0, _GRAPHICS_RUNEITEM_RUNEW, _GRAPHICS_RUNEITEM_RUNEH);
			}
			

			// Outer circle 
			/* TODO : create all component items
				1- Create an item for each component (and childrens) of the spell
					1-1 Position the child in the continuity of the parent (modulate direction vector depending on the number of children)
				2- Compute the radius recusively from everyone else's radius, taking the maximum
			*/

			//outerCircle_.setRect()
		}
	}

	void RuneItem::updateChidlren()
	{

	}

	float RuneItem::getTotalRadius()
	{
		return 0.0f;
	}

}