#include "SpellItem.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QGraphicsScene>
#include <QVector2D>

namespace Runes
{

	SpellItem::SpellItem(Spell* s, SpellItem* parent) : AbstractItem(s,parent), innerSpell_(NULL)
{
		
}

SpellItem::~SpellItem()
{
	this->clearItem();
}

void SpellItem::drawSpell(vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene)
{
	Q_ASSERT(spell_ != NULL);
	if (parent_ == NULL)
		scene.addItem(this);
	else
		this->setParentItem(parent_);

	// Preparing event reception
	this->setAcceptHoverEvents(true);
	this->setAcceptTouchEvents(true);

	// Setting brush
	this->setPen(this->getDefaultPen());

	float innerRadius = 0.0f;
	float outerRadius = 0.0f;
			
	// Center part
	if (spell_->isCenterSpell())
	{
		innerSpell_ = new SpellItem(spell_->getCenterSpell(), this);
		innerSpell_->drawSpell(runeImages, runes, userRunes, scene);

		innerRadius = innerSpell_->getTotalRadius();
	}
	else
	{
		if (spell_->getComponents().empty())
		{
			this->setRect(-_GRAPHICS_SPELLITEM_RUNE_RADIUS, -_GRAPHICS_SPELLITEM_RUNE_RADIUS, _GRAPHICS_SPELLITEM_RUNE_RADIUS * 2, _GRAPHICS_SPELLITEM_RUNE_RADIUS * 2);
		}

		innerRune_ = new RuneItem(spell_, this);
		innerRune_->setZValue(0);
		innerRune_->drawSpell(runeImages, runes, userRunes, scene);

		innerRadius = this->getInnerRadius();
	}
	
	// Creating and positioning components and their paths
	if (!spell_->getComponents().empty())
	{
		vector<Spell*> spellComponents = spell_->getComponents();
		for (Spell* comp : spellComponents)
		{
			// Components
			SpellItem* ri = new SpellItem(comp, this);
			ri->drawSpell(runeImages, runes, userRunes, scene);
			this->components_.push_back(ri);
		}

		// Positioning components
		float componentRadius = this->getBiggestComponentRadius();
		componentRadius += innerRadius + _GRAPHICS_SPELLITEM_DISTANCE;
		int nbComponents = this->components_.size();
		int i = 0;
		for (SpellItem* ri : components_)
		{
			QPointF pos = getPositionOnSpell(i, nbComponents, componentRadius, this->pos());
			ri->setPos(pos);
			this->positionPath(ri, innerRadius);
			++i;
		}

		outerRadius = this->getTotalRadius();

		// Positioning outer circle
		this->setZValue(0);
		this->setRect(-outerRadius, -outerRadius, outerRadius * 2, outerRadius * 2);
	}
	else
		outerRadius = innerRadius;

	// Creating and positioning children and their paths
	if (!spell_->getChildren().empty())
	{
		vector<Spell*> spellChildren = spell_->getChildren();
		for (Spell* child : spellChildren)
		{
			// Components
			SpellItem* ri = new SpellItem(child, this);
			ri->drawSpell(runeImages, runes, userRunes, scene);
			this->children_.push_back(ri);
		}

		float biggestChildRadius = this->getBiggestChildrenRadius();
		biggestChildRadius += outerRadius + _GRAPHICS_SPELLITEM_DISTANCE;
		float nbComponents = this->components_.size();
		int i = 0;
		for (SpellItem* ri : children_)
		{
			QPointF pos = getPositionOnSpell(i, components_.size(), biggestChildRadius, this->pos());
			ri->setPos(pos);
			this->positionPath(ri, outerRadius);
			++i;
		}
	}

	if (this->parentItem() == NULL)
		colorCenterPart(true);
}

float SpellItem::getTotalRadius()
{
	float innerRadius = this->getInnerRadius();
		
	if (components_.empty())
	{
		return innerRadius;
	}
	else
	{
		float maxRadius = 0.0f;
		for (SpellItem* ri : components_)
		{
			float r = ri->getTotalRadiusWithChildren();
			if (r > maxRadius)
				maxRadius = r;
		}
		return innerRadius + maxRadius * 2 + 2 * _GRAPHICS_SPELLITEM_DISTANCE;
	}
}

float SpellItem::getInnerRadius()
{
		
	if (spell_->isCenterSpell() && innerSpell_ != NULL)
	{
		return this->innerSpell_->getTotalRadius();
	}
	else
	{
		return _GRAPHICS_SPELLITEM_RUNE_RADIUS;
	}
}

float SpellItem::getTotalRadiusWithChildren()
{
	if (children_.empty())
	{
		return this->getTotalRadius();
	}
	else
	{
		float biggest = 0.0f;

		for (SpellItem* ri : children_)
		{
			float radius = ri->getTotalRadiusWithChildren();
			if (radius > biggest)
				biggest = radius;
		}

		return this->getTotalRadius() + biggest*2 + 2 * _GRAPHICS_SPELLITEM_DISTANCE;
	}
}

void SpellItem::clearItem()
{
	if (this->scene())
	{
		for (QGraphicsPathItem* path : paths_)
		{
			this->scene()->removeItem(path);
		}

		this->scene()->removeItem(this);
	}

	for (SpellItem* r : children_)
	{
		r->clearItem();
		delete(r);
	}
	for (SpellItem* r : components_)
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

void SpellItem::positionPath(SpellItem* ri, float thisRadius)
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

void SpellItem::toggleText()
{
	

	for (SpellItem* ri : children_)
	{
		ri->toggleText();
	}

	for (SpellItem* ri : components_)
	{
		ri->toggleText();
	}
}

float SpellItem::getBiggestComponentRadius()
{
	float biggest = 0.0f;

	for (SpellItem* ri : components_)
	{
		float radius = ri->getTotalRadiusWithChildren();
		if (radius > biggest)
			biggest = radius;
	}
	return biggest;
}

float SpellItem::getBiggestChildrenRadius()
{
	float biggest = 0.0f;

	for (SpellItem* ri : children_)
	{
		float radius = ri->getTotalRadius();
		if (radius > biggest)
			biggest = radius;
	}

	return biggest;
}

QPointF SpellItem::getPositionOnSpell(int nb, int nbTotal, float radius, QPointF parentPosition)
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
	//QPointF result(cos((2 * nb * (M_PI * (_GRAPHICS_SPELLITEM_CHILDSPACINGCOEFF*2))) / nbTotal + cosOffset), sin((2 * nb * (M_PI * (_GRAPHICS_SPELLITEM_CHILDSPACINGCOEFF*2))) / nbTotal + sinOffset));
	QPointF result(cos((2 * nb * M_PI) / nbTotal + cosOffset), sin((2 * nb * M_PI ) / nbTotal + sinOffset));
	result *= radius;
	return result;
}



void SpellItem::colorCenterPart(bool isCenterSpell)
{
	/*if (this->innerSpell_ != NULL)
		this->innerSpell_->colorCenterPart(false);
	else
		innerRune_->colorCenterPart(isCenterSpell);*/
	if (this->innerSpell_ == NULL)
		innerRune_->colorCenterPart(true);
	else
		this->innerSpell_->colorCenterPart(true);
}



void SpellItem::focusInEvent(QFocusEvent * event)
{
	//this->setPen(QPen(QBrush(Qt::GlobalColor::green), 3));
	this->setBrush(QBrush(colorSelected()));
	this->scene()->update();
}

void SpellItem::focusOutEvent(QFocusEvent * event)
{
	//this->setPen(this->getDefaultPen());
	if (this->parentItem() == NULL)
		colorCenterPart(true);
		
	this->setBrush(QBrush(Qt::GlobalColor::transparent));
	this->scene()->update();
}



}