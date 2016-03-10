#include "AbstractItem.h"
#include <QGraphicsScene>

namespace Runes
{
	AbstractItem::AbstractItem(Spell* s, AbstractItem* parent) : spell_(s), parent_(parent), isHovered_(false)
	{

	}

	int AbstractItem::getPenWidth()
	{
		return 2;
	}

	QPen AbstractItem::getDefaultPen()
	{
		return QPen(QBrush(colorPen()), getPenWidth());
	}

	void AbstractItem::childHovering(bool requests)
	{
		// A child requests hovering focus
		if (requests == true)
		{
			if (isHovered_ == true)
			{
				// Drop hovering focus
				this->setPen(this->getDefaultPen());
			}
		}
		else
		{
			// The child has released hovering focus. We need to grab it back
			if (isHovered_ == true)
			{
				// Reactivate hover
				this->setPen(QPen(QBrush(colorHover()), getPenWidth()));
			}
		}
	}

	QColor AbstractItem::colorCenter()
	{
		return QColor(94, 0, 0);
	}

	QColor AbstractItem::colorCenterSpell()
	{
		return QColor(128, 0, 0);
	}

	QColor AbstractItem::colorPen()
	{
		return QColor(60, 98, 130);
	}

	QColor AbstractItem::colorSelected()
	{
		return QColor(0, 155, 178);
	}

	QColor AbstractItem::colorBackground()
	{
		return QColor(37, 37, 37);
	}

	QColor AbstractItem::colorHover()
	{
		return QColor(255, 255, 255);
	}

	void AbstractItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
	{
		isHovered_ = true;

		// If this has a parent, then ask the parent to stop hovering
		if (this->parentItem() != NULL)
			((AbstractItem*)parentItem())->childHovering(true);

		this->setPen(QPen(QBrush(colorHover()), getPenWidth()));
		this->scene()->update();
	}

	void AbstractItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
	{
		// If this has a parent, then tell the parent to grab the hovering back
		if (this->parentItem() != NULL)
			((AbstractItem*)parentItem())->childHovering(false);

		isHovered_ = false;
		this->setPen(this->getDefaultPen());
		this->scene()->update();
	}

}



