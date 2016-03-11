#ifndef _GRAPHICS_SPELLITEM_H_
#define _GRAPHICS_SPELLITEM_H_

#define _GRAPHICS_SPELLITEM_DISTANCE 16
#define _GRAPHICS_SPELLITEM_CHILDSPACINGCOEFF 5/6

#include "AbstractItem.h"
#include "RuneItem.h"

namespace Runes
{
	class SpellItem : public AbstractItem
	{
	public:
		// First constructor : only used for first item
		SpellItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene);
		// All other items take the parent's scene
		SpellItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes);
		~SpellItem();

		// draw spell
		void drawSpell();

		float getTotalRadius() override;

		//! @brief returns the radius of the inner part of the spell.
		float getInnerRadius();

		//! @brief returns the total radius of this spell, children included.
		//! @param isComponent : is this spell a component or a child
		float getTotalRadiusWithChildren();

		virtual void clearItem() override;

		//! @brief positions the children and components path to correctly point at this
		void positionPath(SpellItem* ri, float thisRadius);

		//! @brief toggles between rune image or rune text
		void toggleText();

		virtual void colorCenterPart(bool isCenterSpell = false) override;

		static QPointF getPositionOnSpell(int nb, int nbTotal, float radius, QPointF parentPosition);

		void checkColorCenter();
		
		//! @brief forces a redraw of the entire spell
		void redraw();

	signals:


	protected:
		// Members
		vector<SpellItem*> children_;
		vector<SpellItem*> components_;
		SpellItem* innerSpell_;	// If the center part of this spell is another spell, that's a pointer to it
		RuneItem* innerRune_;	// If the center part of this spell is a rune, that's a pointer to it

		// The graphics
		
		vector<QGraphicsPathItem*> paths_;	//!< the curves linking this spell to its children and components
		// The outer circle is *this

		//---------------
		// HELPER FUNCTIONS
		float getBiggestComponentRadius();
		float getBiggestChildrenRadius();
		
		// EVENTS
		void focusInEvent(QFocusEvent * event);
		void focusOutEvent(QFocusEvent * event);
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void keyPressEvent(QKeyEvent * event);

	};
}


#endif //_GRAPHICS_SPELLITEM_H_