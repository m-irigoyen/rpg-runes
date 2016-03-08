#include "RuneItem.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QGraphicsScene>
#include <QVector2D>

namespace Runes
{

RuneItem::RuneItem() : spell_(NULL)
{
		
}

RuneItem::~RuneItem()
{
	this->clearItem();
}

void RuneItem::drawSpell(Spell* s, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene, RuneItem* parent)
{
	Q_ASSERT(s != NULL);
	spell_ = s;
	if (parent == NULL)
		scene.addItem(this);
	else
		this->setParentItem(parent);

	// Preparing event reception
	this->setAcceptHoverEvents(true);
	this->setAcceptTouchEvents(true);

	// Setting brush
	this->setPen(this->getDefaultPen());

	float innerRadius = 0.0f;
	float outerRadius = 0.0f;
			
	// Center part
	if (s->isCenterSpell())
	{
		innerSpell_ = new RuneItem();
		innerSpell_->drawSpell(s->getCenterSpell(), runeImages, runes, userRunes, scene, this);
		innerRadius = innerSpell_->getTotalRadius();
		// no need to set inner circle : the inner spell should already have an outer circle
	}
	else
	{
		// Displaying the rune
		image_.setParentItem(this);
		image_.setPixmap(runeImages[s->getRune()]);
		image_.setPos(-_GRAPHICS_RUNEITEM_RUNE_IMAGERADIUS, -_GRAPHICS_RUNEITEM_RUNE_IMAGERADIUS);

		// Setting the text
		text_.setParentItem(this);
		UserRune& ur = userRunes.at(s->getRune());
		if (ur.second)
			text_.setText(ur.first.getNaturalName());
		//text_.setTransformOriginPoint(0, 0);
		text_.hide();

		if (s->getComponents().empty())
		{
			// No inner circle, the outer circle is enough
			//this->setTransformOriginPoint(_GRAPHICS_RUNEITEM_RUNE_RADIUS, _GRAPHICS_RUNEITEM_RUNE_RADIUS);
			this->setRect(-_GRAPHICS_RUNEITEM_RUNE_RADIUS, -_GRAPHICS_RUNEITEM_RUNE_RADIUS, _GRAPHICS_RUNEITEM_RUNE_RADIUS * 2, _GRAPHICS_RUNEITEM_RUNE_RADIUS * 2);
		}
		else
		{
			// Positionning inner circle
			innerCircle_.setParentItem(this);
			innerCircle_.setRect(
				-_GRAPHICS_RUNEITEM_RUNE_RADIUS,
				-_GRAPHICS_RUNEITEM_RUNE_RADIUS,
				_GRAPHICS_RUNEITEM_RUNE_RADIUS * 2,
				_GRAPHICS_RUNEITEM_RUNE_RADIUS * 2);
			innerCircle_.setPen(this->getDefaultPen());
		}

		innerRadius = this->getInnerRadius();
	}
	
	// Creating and positionning components and their paths
	if (!s->getComponents().empty())
	{
		vector<Spell*> spellComponents = s->getComponents();
		for (Spell* comp : spellComponents)
		{
			// Components
			RuneItem* ri = new RuneItem();
			ri->drawSpell(comp, runeImages, runes, userRunes, scene, this);
			this->components_.push_back(ri);
		}

		// Positionning components
		float componentRadius = this->getBiggestComponentRadius();
		componentRadius += innerRadius + _GRAPHICS_RUNEITEM_DISTANCE;
		int nbComponents = this->components_.size();
		int i = 0;
		for (RuneItem* ri : components_)
		{
			QPointF pos = getPositionOnSpell(i, nbComponents, componentRadius, this->pos());
			ri->setPos(pos);
			this->positionPath(ri, innerRadius);
			++i;
		}

		outerRadius = this->getTotalRadius();

		// Positionning outer circle
		this->setZValue(0);
		this->setRect(-outerRadius, -outerRadius, outerRadius * 2, outerRadius * 2);
	}
	else
		outerRadius = innerRadius;
	

	// Creating and positionning children and their paths
	if (!s->getChildren().empty())
	{
		vector<Spell*> spellChildren = s->getChildren();
		for (Spell* comp : spellChildren)
		{
			// Components
			RuneItem* ri = new RuneItem();
			ri->drawSpell(comp, runeImages, runes, userRunes, scene, this);
			this->children_.push_back(ri);
		}

		float biggestChildRadius = this->getBiggestChildrenRadius();
		biggestChildRadius += outerRadius + _GRAPHICS_RUNEITEM_DISTANCE;
		float nbComponents = this->components_.size();
		int i = 0;
		for (RuneItem* ri : children_)
		{
			QPointF pos = getPositionOnSpell(i, components_.size(), biggestChildRadius, this->pos());
			ri->setPos(pos);
			this->positionPath(ri, outerRadius);
			++i;
		}
	}

	
}

float RuneItem::getTotalRadius()
{
	float innerRadius = this->getInnerRadius();
		
	if (components_.empty())
	{
		return innerRadius;
	}
	else
	{
		float maxRadius = 0.0f;
		for (RuneItem* ri : components_)
		{
			float r = ri->getTotalRadiusWithChildren();
			if (r > maxRadius)
				maxRadius = r;
		}
		return innerRadius + maxRadius * 2 + 2 * _GRAPHICS_RUNEITEM_DISTANCE;
	}
}

float RuneItem::getInnerRadius()
{
		
	if (spell_->isCenterSpell() && innerSpell_ != NULL)
	{
		return this->innerSpell_->getTotalRadius();
	}
	else
	{
		return _GRAPHICS_RUNEITEM_RUNE_RADIUS;
	}
}

float RuneItem::getTotalRadiusWithChildren()
{
	if (children_.empty())
	{
		return this->getTotalRadius();
	}
	else
	{
		float biggest = 0.0f;

		for (RuneItem* ri : children_)
		{
			float radius = ri->getTotalRadiusWithChildren();
			if (radius > biggest)
				biggest = radius;
		}

		return this->getTotalRadius() + biggest*2 + 2 * _GRAPHICS_RUNEITEM_DISTANCE;
	}
}

void RuneItem::clearItem()
{
	if (this->scene())
	{
		this->scene()->removeItem(&this->innerCircle_);
		this->scene()->removeItem(&this->image_);
		this->scene()->removeItem(&this->text_);

		for (QGraphicsPathItem* path : paths_)
		{
			this->scene()->removeItem(path);
		}

		this->scene()->removeItem(this);
	}

	for (RuneItem* r : children_)
	{
		r->clearItem();
		delete(r);
	}
	for (RuneItem* r : components_)
	{
		r->clearItem();
		delete(r);
	}
	for (QGraphicsPathItem* path : paths_)
	{
		delete(path);
	}
	this->children_.clear();
	this->components_.clear();
	this->paths_.clear();
}

void RuneItem::positionPath(RuneItem* ri, float thisRadius)
{
	// Computing point on the parent circle (this)
	QPointF pos = ri->pos();
	QVector2D centerToPos(pos);
	centerToPos.normalize();
	centerToPos *= thisRadius;

	QPointF pParentCircle(centerToPos.x(), centerToPos.y());

	// Computing point on the child circle
	QVector2D posToCenter(-pos);
	posToCenter.normalize();
	posToCenter *= ri->getTotalRadius();

	QPointF pThisCircle = pos + QPointF(posToCenter.x(), posToCenter.y());

	// Drawing path from one to the other
	QPainterPath p(pThisCircle);
	p.lineTo(pParentCircle);
	//p.addEllipse(pParentCircle, 10, 10);

	// Setting the constructed path
	QGraphicsPathItem* path = new QGraphicsPathItem(p, this);
	path->setPen(this->getDefaultPen());

	paths_.push_back(path);
}

void RuneItem::toggleText()
{
	if (image_.isVisible())
	{
		image_.hide();
		text_.setVisible(true);
	}
	else
	{
		image_.setVisible(true);
		text_.hide();
	}

	for (RuneItem* ri : children_)
	{
		ri->toggleText();
	}

	for (RuneItem* ri : components_)
	{
		ri->toggleText();
	}
}

float RuneItem::getBiggestComponentRadius()
{
	float biggest = 0.0f;

	for (RuneItem* ri : components_)
	{
		float radius = ri->getTotalRadiusWithChildren();
		if (radius > biggest)
			biggest = radius;
	}
	return biggest;
}

float RuneItem::getBiggestChildrenRadius()
{
	float biggest = 0.0f;

	for (RuneItem* ri : children_)
	{
		float radius = ri->getTotalRadius();
		if (radius > biggest)
			biggest = radius;
	}

	return biggest;
}

QPointF RuneItem::getPositionOnSpell(int nb, int nbTotal, float radius, QPointF parentPosition)
{
	++nb;
	float cosOffset = 0.0f, sinOffset = 0.0f;
	/*if (parentPosition != QPointF(0, 0))
	{
		QVector2D vecToPathStart(parentPosition * -1);
		vecToPathStart.normalize();
		cosOffset = acos(vecToPathStart.x());
		sinOffset = asin(vecToPathStart.y());
	}*/
	//QPointF result(cos((2 * nb * (M_PI * (_GRAPHICS_RUNEITEM_CHILDSPACINGCOEFF*2))) / nbTotal + cosOffset), sin((2 * nb * (M_PI * (_GRAPHICS_RUNEITEM_CHILDSPACINGCOEFF*2))) / nbTotal + sinOffset));
	QPointF result(cos((2 * nb * M_PI) / nbTotal + cosOffset), sin((2 * nb * M_PI ) / nbTotal + sinOffset));
	result *= radius;
	return result;
}

QPen RuneItem::getDefaultPen()
{
	return QPen(QBrush(Qt::GlobalColor::black), 2);
}

void RuneItem::focusInEvent(QFocusEvent * event)
{
	this->setPen(QPen(QBrush(Qt::GlobalColor::green), 3));
	this->setBrush(QBrush(Qt::GlobalColor::darkGreen));
	this->scene()->update();
}

void RuneItem::focusOutEvent(QFocusEvent * event)
{
	this->setPen(this->getDefaultPen());
	this->setBrush(QBrush(Qt::GlobalColor::transparent));
	this->scene()->update();
}

void RuneItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	// If this has a parent, then propagate that event to it
	if (this->parentItem() != NULL)
	{
		((RuneItem*)this->parentItem())->hoverLeaveEvent(NULL);
	}

	if (!this->hasFocus())
	{
		this->setPen(QPen(QBrush(Qt::GlobalColor::blue), 2));
		this->scene()->update();
	}
}

void RuneItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	if (!this->hasFocus())
	{
		this->setPen(this->getDefaultPen());
		this->scene()->update();
	}
}

}