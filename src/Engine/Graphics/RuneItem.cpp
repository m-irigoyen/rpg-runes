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
		//innerCircle_.setParentItem(this);
		image_.setParentItem(this);
		text_.setParentItem(this);
		

		Q_ASSERT(s != NULL);
		if (parent == NULL)
			scene.addItem(this);
		else
			this->setParentItem(parent);
			


		if (s->isCenterSpell())
		{
			// TODO : 
		}
		else
		{
			if (!s->getComponents().empty())
			{
				// Creating components, and computing outer circle size
				{
					vector<Spell*> spellComponents = s->getComponents();
					for (Spell* comp : spellComponents)
					{
						// Components
						RuneItem* ri = new RuneItem();
						ri->drawSpell(comp, runeImages, runes, userRunes, scene, this);
						this->components_.push_back(ri);
					}

				}

				// Creating children
				{
					vector<Spell*> spellChildren = s->getChildren();
					for (Spell* comp : spellChildren)
					{
						// Components
						RuneItem* ri = new RuneItem();
						ri->drawSpell(comp, runeImages, runes, userRunes, scene, this);
						this->children_.push_back(ri);
					}
				}

					//-------------
					// POSITIONNING

				// Positionning outer circle
				float totalRadius = this->getTotalRadius();
				//this->setTransformOriginPoint(totalRadius, totalRadius);
				this->setRect(-totalRadius, -totalRadius, totalRadius * 2, totalRadius * 2);

				// Positionning inner circle
				innerCircle_.setParentItem(this);
				//innerCircle_.setTransformOriginPoint(_GRAPHICS_RUNEITEM_RUNE_RADIUS, _GRAPHICS_RUNEITEM_RUNE_RADIUS);
				innerCircle_.setRect(
					-_GRAPHICS_RUNEITEM_RUNE_RADIUS,
					-_GRAPHICS_RUNEITEM_RUNE_RADIUS,
					_GRAPHICS_RUNEITEM_RUNE_RADIUS*2, 
					_GRAPHICS_RUNEITEM_RUNE_RADIUS*2);

				// Positionning components
				{
					float innerRadius = this->getInnerRadius();
					float componentRadius = this->getBiggestComponentRadius();
					componentRadius += innerRadius + _GRAPHICS_RUNEITEM_DISTANCE;
					int nbComponents = this->components_.size();
					int i = 0;
					for (RuneItem* ri : components_)
					{
						QPointF pos = getPositionOnSpell(i + 1, nbComponents, componentRadius);
						ri->setPos(pos);
						this->positionPath(ri, innerRadius);
						++i;
					}
				}

				// Positionning children
				{
					float totalRadius = this->getTotalRadius();
					float componentRadius = this->getBiggestChildrenRadius();
					componentRadius += totalRadius + _GRAPHICS_RUNEITEM_DISTANCE;
					float nbComponents = this->components_.size();
					int i = 0;
					for (RuneItem* ri : children_)
					{
						QPointF pos = getPositionOnSpell(i + 1, components_.size(), componentRadius);
						ri->setPos(pos);
						this->positionPath(ri, totalRadius);
						++i;
					}
				}
			}
			else
			{
				// No inner circle, the outer circle is enough
				//this->setTransformOriginPoint(_GRAPHICS_RUNEITEM_RUNE_RADIUS, _GRAPHICS_RUNEITEM_RUNE_RADIUS);
				this->setRect(-_GRAPHICS_RUNEITEM_RUNE_RADIUS, -_GRAPHICS_RUNEITEM_RUNE_RADIUS, _GRAPHICS_RUNEITEM_RUNE_RADIUS * 2, _GRAPHICS_RUNEITEM_RUNE_RADIUS * 2);
			}

			// Setting the text
			UserRune& ur = userRunes.at(s->getRune());
			if (ur.second)
				text_.setText(ur.first.getNaturalName());
			//text_.setTransformOriginPoint(0, 0);
			text_.hide();

			// Setting the image
			image_.setPixmap(runeImages[s->getRune()]);
			/*image_.setTransformOriginPoint(
				_GRAPHICS_RUNEITEM_RUNE_RADIUS,
				_GRAPHICS_RUNEITEM_RUNE_RADIUS);*/
		}

		float totalRadius = this->getTotalRadius();
		/*innerCircle_.setPos(QPointF(0, 0) + QPointF(totalRadius, totalRadius));
		text_.setPos(QPointF(0, 0) + QPointF(totalRadius, totalRadius));
		image_.setPos(QPointF(0, 0) + QPointF(totalRadius, totalRadius));*/
		innerCircle_.setPos(0, 0);
		text_.setPos(0, 0);
		image_.setPos(-_GRAPHICS_RUNEITEM_RUNE_IMAGERADIUS, -_GRAPHICS_RUNEITEM_RUNE_IMAGERADIUS);
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
		
		if (this->spell_ && this->spell_->isCenterSpell())
		{
			//TODO : get the radius of the inner spell
			return 250.0f;
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
			float radius = ri->getTotalRadius();
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

	QPointF RuneItem::getPositionOnSpell(int nb, int nbTotal, float radius)
	{
		QPointF result(cos(((2 * nb * M_PI) / nbTotal)), sin(((2 * nb * M_PI) / nbTotal)));
		result *= radius;
		return result;
	}

}