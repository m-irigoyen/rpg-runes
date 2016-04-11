#ifndef _GRAPHICS_SPELLITEM_H_
#define _GRAPHICS_SPELLITEM_H_

//#define _GRAPHICS_SPELLITEM_DISTANCE 64.0f
#define _GRAPHICS_SPELLITEM_DISTANCE 16.0f
#define _GRAPHICS_SPELLITEM_CHILDSPACINGCOEFF 5.0f/6.0f
#define _GRAPHICS_SPELLITEM_CURVEDISPLACEMENT 25.0f

#include "AbstractItem.h"
#include "RuneItem.h"

namespace Runes
{
	class SpellItem : public AbstractItem
	{
		Q_OBJECT
	public:
		// First constructor : only used for first item
		SpellItem(Spell* s,
			AbstractItem* parent,
			vector<QPixmap>& runeImages,
			RuneEngine& runeEngine,
			QGraphicsScene& scene);
		// All other items take the parent's scene
		SpellItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RuneEngine& runeEngine);
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

		virtual void setIsText(bool isText) override;

		virtual void colorCenterPart(bool isCenterSpell = false) override;

		static QPointF getPositionOnSpell(int nb, int nbTotal, float radius, QPointF parentPosition);
		static QPointF getModifierPositionOnSpell(int nb, int nbTotal, float radius, QPointF parentPosition);

		void checkColorCenter();

		virtual void childHovering(bool requests) override;

		// Focus reset
		bool resetFocus();

	public slots:
		//! @brief forces a redraw of the entire spell
		void redraw();

	protected:
		// Members
		vector<SpellItem*> children_;
		vector<SpellItem*> components_;
		vector<SpellItem*> modifiers_;
		SpellItem* innerSpell_;	// If the center part of this spell is another spell, that's a pointer to it
		RuneItem* innerRune_;	// If the center part of this spell is a rune, that's a pointer to it

		// The graphics
		
		vector<QGraphicsPathItem*> paths_;	//!< the curves linking this spell to its children and components
		// The outer circle is *this

		//---------------
		// HELPER FUNCTIONS
		float getBiggestComponentRadius();
		float getBiggestChildrenRadius();
		void updatePathsColor(QBrush brush);
		
		// EVENTS
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
		void focusInEvent(QFocusEvent * event);
		void focusOutEvent(QFocusEvent * event);
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void keyPressEvent(QKeyEvent * event);
	};
}


#endif //_GRAPHICS_SPELLITEM_H_