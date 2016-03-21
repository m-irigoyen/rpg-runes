#ifndef _GRAPHICS_RUNEITEM_H_
#define _GRAPHICS_RUNEITEM_H_

#define _GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS 64
#define _GRAPHICS_SPELLITEM_RUNE_RADIUS 64

#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>

#include "AbstractItem.h"


namespace Runes
{
	class RuneItem : public AbstractItem
	{
		Q_OBJECT
	public:
		RuneItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RuneEngine& runeEngine);
		~RuneItem();

		virtual float getTotalRadius() override;

		virtual void toggleText() override;

		virtual void setIsText(bool isText) override;

		virtual void drawSpell() override;

		virtual void RuneItem::colorCenterPart(bool isCenterSpell = false) override;

		virtual void clearItem() override;

	signals:
		void requestRedraw();

	protected:
		QGraphicsSimpleTextItem text_;		//!< If the center part of the spell is a rune, this is its natural name
		QGraphicsPixmapItem image_;			//!< If the center part of the spell is a rune, this is its image

		// EVENTS
		void focusInEvent(QFocusEvent * event);
		void focusOutEvent(QFocusEvent * event);
		void mousePressEvent(QGraphicsSceneMouseEvent * event);
		void keyPressEvent(QKeyEvent * event);
	};
}

#endif