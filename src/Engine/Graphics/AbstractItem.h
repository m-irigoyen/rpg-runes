#ifndef _GRAPHICS_ABSTRACTITEM_H_
#define _GRAPHICS_ABSTRACTITEM_H_

#include <QGraphicsEllipseItem>
#include <QPen>
#include "Engine/Runes/RuneEngine.h"

namespace Runes
{
	class AbstractItem : public QGraphicsEllipseItem
	{
	public:
		// First constructor : only used for first item
		AbstractItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene);
		// All other items take the parent's scene
		AbstractItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes);

		//! @brief returns the total radius of this spell. Depending whether the spell is a top level or not, won't return the same
		virtual float getTotalRadius() = 0;

		//! @brief toggles between displaying text and image (only for RuneItems, but is propagated through all abstract items recursively)
		virtual void toggleText() = 0;

		virtual void setIsText(bool isText) = 0;

		//! @brief Draws the spell
		virtual void drawSpell() = 0;

		//! @brief used to maintain hovering logic
		void childHovering(bool requests);

		virtual void colorCenterPart(bool isCenterSpell = false) = 0;

		virtual void clearItem() = 0;

		//-----------------
		// STATIC FUNCTIONS
		static QColor colorCenter();
		static QColor colorCenterSpell();
		static QColor colorPen();
		static QColor colorSelected();
		static QColor colorBackground();
		static QColor colorHover();
		static QPen getDefaultPen();
		static int getPenWidth();

		//-------
		// EVENTS
		void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
		void mousePressEvent(QGraphicsSceneMouseEvent * event);

	protected:
		Spell* spell_;	//!< The spell this rune draws
		AbstractItem* parent_;	//!< The parent of this spell (can be NULL)

		// Used to propagate hover events
		bool isHovered_;

		// references
		vector<QPixmap>& runeImages_;
		RunesContainer& runes_;
		UserRunesContainer& userRunes_;
	};
}

#endif