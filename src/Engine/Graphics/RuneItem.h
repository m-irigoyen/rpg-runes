#ifndef _GRAPHICS_RUNEITEM_H_
#define _GRAPHICS_RUNEITEM_H_

#include "Engine/Runes/RuneEngine.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsPixMapItem>

#define _GRAPHICS_RUNEITEM_RUNE_RADIUS 64
#define _GRAPHICS_RUNEITEM_DISTANCE 30

namespace Runes
{
	class RuneItem : public QGraphicsEllipseItem
	{
	public:
		RuneItem();
		~RuneItem();

		// draw spell
		void drawSpell(Spell* s, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene, RuneItem* parent);

		//! @brief returns the total radius of this spell. Depending wether the spell is a top level or not, won't return the same
		float getTotalRadius();
		//! @brief returns the radius of the inner part of the spell.
		float getInnerRadius();

		//! @brief returns the total radius of this spell, children included.
		//! @param isComponent : is this spell a component or a child
		float getTotalRadiusWithChildren();

		void clearItem();

		//! @brief positions the children and components path to correctly point at this
		void positionCompAndChildrenPaths(bool isComponent);


		//! @brief toggles between rune image or rune text
		void toggleText();

	protected:
		// Members
		Spell* spell_;	//!< The spell this rune draws

		vector<RuneItem*> children_;
		vector<RuneItem*> components_;
		RuneItem* innerSpell_;	// If the center part of this spell is another spell, that's a pointer to it

		// The graphics
		QGraphicsSimpleTextItem text_;		//!< If the center par of the spell is a rune, this is its natural name
		QGraphicsPixmapItem image_;			//!< If the center part of the spel is a rune, this is its image
		QGraphicsEllipseItem innerCircle_;	//!< The circle around the center part of the spell.
		QGraphicsPathItem path_;			//!< the curve linking this spell to its parent
		// The outer circle is *this

		//---------------
		// HELPER FUNCTIONS
		float getBiggestComponentRadius();
		float getBiggestChildrenRadius();
		QPointF getPositionOnSpell(int nb, int nbTotal, float radius);
	};
}


#endif //_GRAPHICS_RUNEITEM_H_