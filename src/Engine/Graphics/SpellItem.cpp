#include "SpellItem.h"

#include <Engine/Utilities/MathStuff.h>
#include <QGraphicsScene>
#include <QVector2D>
#include <QKeyEvent>

namespace Runes
{

	SpellItem::SpellItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene) : AbstractItem(s, parent, runeImages, runes, userRunes, scene), innerSpell_(NULL)
	{

	}

	SpellItem::SpellItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes) : AbstractItem(s, parent, runeImages, runes, userRunes), innerSpell_(NULL)
	{

	}

SpellItem::~SpellItem()
{
	this->clearItem();
}

void SpellItem::drawSpell()
{
	Q_ASSERT(spell_ != NULL);

	// Setting brush
	this->setPen(this->getDefaultPen());
	this->setBrush(QBrush(colorBackground()));

	float innerRadius = 0.0f;
	float outerRadius = 0.0f;
			
	// Center part
	if (spell_->isCenterSpell())
	{
		innerSpell_ = new SpellItem(spell_->getCenterSpell(), this, runeImages_, runes_, userRunes_);
		innerSpell_->drawSpell();

		innerRadius = innerSpell_->getTotalRadius();

		// Positioning outer circle
		this->setRect(-outerRadius, -outerRadius, outerRadius * 2, outerRadius * 2);
	}
	else
	{
		innerRune_ = new RuneItem(spell_, this, runeImages_, runes_, userRunes_);
		innerRune_->drawSpell();

		innerRadius = this->getInnerRadius();
	}
	
	// Creating and positioning components and their paths
	if (!spell_->getComponents().empty())
	{
		vector<Spell*> spellComponents = spell_->getComponents();
		for (Spell* comp : spellComponents)
		{
			// Components
			SpellItem* ri = new SpellItem(comp, this, runeImages_, runes_, userRunes_);
			ri->drawSpell();
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
			SpellItem* ri = new SpellItem(child, this, runeImages_, runes_, userRunes_);
			ri->drawSpell();
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
		return this->innerSpell_->getTotalRadius();
	else
		return _GRAPHICS_SPELLITEM_RUNE_RADIUS;
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

		// Don't remove yourself from the scene
	}

	if (innerRune_ != NULL)
	{
		innerRune_->clearItem();
		delete(innerRune_);
		innerRune_ = NULL;
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
	QPointF pos = ri->pos();	// child position
	float childRadius = ri->getTotalRadius();
	QPointF thisStraight, childStraight;
	{
		// Computing point on the parent circle (this)
		QVector2D centerToPos(pos);
		centerToPos.normalize();
		centerToPos *= thisRadius;
		thisStraight.setX(centerToPos.x());
		thisStraight.setY(centerToPos.y());
	}
	{
		// Computing point on the child circle (this)
		QVector2D posToC(-pos);
		posToC.normalize();
		posToC *= childRadius;
		childStraight.setX(posToC.x() + pos.x());
		childStraight.setY(posToC.y() + pos.y());
	}

	QPointF thisRotated, childRotated;
	{
		// Start point of the curve
		QVector2D centerToPos(pos);
		centerToPos.normalize();
		centerToPos = Mathstuff::rotate(centerToPos, _GRAPHICS_SPELLITEM_CURVEDISPLACEMENT); // rotating the vector (for the curve start)
		centerToPos.normalize();
		centerToPos *= thisRadius;
		thisRotated.setX(centerToPos.x());	// Point rotated
		thisRotated.setY(centerToPos.y());	// Point rotated
	}
	{
		// Start point of the curve
		QVector2D posToCenter(-pos);
		posToCenter.normalize();
		posToCenter = Mathstuff::rotate(posToCenter, _GRAPHICS_SPELLITEM_CURVEDISPLACEMENT); // rotating the vector (for the curve start)
		posToCenter.normalize();
		posToCenter *= childRadius;
		childRotated.setX(posToCenter.x() + pos.x());	// Point rotated
		childRotated.setY(posToCenter.y() + pos.y());	// Point rotated
	}

	QPointF centerPath;
	{
		QVector2D v(childStraight - thisStraight);
		v *= 0.5f;

		centerPath.setX(thisStraight.x() + v.x());
		centerPath.setY(thisStraight.y() + v.y());
	}

	// This position is 0,0

	//FIXME : this doesn't work u_u
	// First curve : from parent circle to halfway
	{
		// Control point1 
		QVector2D centerToPos(pos);
		centerToPos.normalize();
		centerToPos *= _GRAPHICS_SPELLITEM_DISTANCE / 4.0f;
		QPointF pC1;
		pC1.setX(centerToPos.x() + thisStraight.x());
		pC1.setY(centerToPos.y() + thisStraight.y());
		
		// Creating the path
		QPainterPath p(thisRotated);
		p.quadTo(pC1, centerPath);
		//p.lineTo(centerPath);
		// Setting the constructed path
		QGraphicsPathItem* path = new QGraphicsPathItem(p, this);
		path->setPen(this->getDefaultPen());

		paths_.push_back(path);
	}
	


	// Secdond curve : from child circle to halfway
	{
		// Control point1 
		QVector2D posToCenter(-pos);
		posToCenter.normalize();
		posToCenter *= _GRAPHICS_SPELLITEM_DISTANCE / 4.0f;
		QPointF pC1;
		pC1.setX(posToCenter.x() + childStraight.x());
		pC1.setY(posToCenter.y() + childStraight.y());

		// Creating the path
		QPainterPath p(childRotated);
		p.quadTo(pC1, centerPath);
		//p.lineTo(centerPath);
		// Setting the constructed path
		QGraphicsPathItem* path = new QGraphicsPathItem(p, this);
		path->setPen(this->getDefaultPen());

		paths_.push_back(path);
	}
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

void SpellItem::checkColorCenter()
{
	if (parentItem() == NULL)
		colorCenterPart(true);
}

void SpellItem::redraw()
{
	if (parent_ != NULL)
	{
		((SpellItem*)parent_)->redraw();
		return;
	}

	// Clear everything
	clearItem();

	// redraw everything
	drawSpell();
	scene()->update();
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
	this->setBrush(QBrush(colorSelected()));
	this->scene()->update();
}

void SpellItem::focusOutEvent(QFocusEvent * event)
{
	if (this->parentItem() == NULL)
		colorCenterPart(true);
		
	this->setBrush(QBrush(colorBackground()));
	checkColorCenter();
	this->scene()->update();
}

void SpellItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	this->setFocus(Qt::FocusReason::MouseFocusReason);
}

void SpellItem::keyPressEvent(QKeyEvent * event)
{
	Spell * spellParent = NULL;
	switch (event->key())
	{
		// Delete : remove that spell
	case Qt::Key::Key_Delete :
		spellParent = spell_->getParent();
		if (spellParent == NULL)
		{
			// Can't remove the top level spell, so just clear it
			cout << "Clearing top level spell" << endl;
			spell_->clear();
		}
		else
		{
			cout << "Removing spell" << endl;
			spellParent->remove(spell_);
		}
		redraw();
		break;
	case Qt::Key::Key_Plus:
	case Qt::Key::Key_A:
		// alternative add : add a child
		if (event->modifiers() & Qt::ShiftModifier)
			spell_->addEmptyChild();
		else
			spell_->addEmptyComponent();
		redraw();
		break;
	}
}

}