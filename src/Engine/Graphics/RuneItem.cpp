#include "RuneItem.h"
#include <QGraphicsScene>

namespace Runes
{
	RuneItem::RuneItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes) : AbstractItem(s, parent, runeImages, runes, userRunes)
	{

	}

	RuneItem::~RuneItem()
	{
		clearItem();
	}

	float RuneItem::getTotalRadius()
	{
		throw std::logic_error("The method or operation is not implemented.");
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
	}

	void RuneItem::drawSpell()
	{
		if (parent_ == NULL)
		{
			cout << "ERROR : RuneItems must have a parent" << endl;
			abort();
		}
		
		this->setParentItem(parent_);
		this->setPen(this->getDefaultPen());
		this->setBrush(QBrush(colorBackground()));

		// Empty spell : only display the inner circle
		
		// If the spell isn't empty
		if (spell_->getRune() != -1)
		{
			// Setting the text
			text_.setParentItem(this);
			RuneDescriptor& rd = userRunes_.at(spell_->getRune());
			text_.setText(rd.getNaturalName());
			text_.setPos(-text_.boundingRect().width() / 2, -text_.boundingRect().height() / 2);
			text_.hide();

			// Displaying the rune
			image_.setParentItem(this);
			image_.setPixmap(runeImages_[spell_->getRune()]);
			image_.setPos(-_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS, -_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS);
		}

		this->setRect(-_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS, -_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS, _GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS * 2, _GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS * 2);
	}

	void RuneItem::colorCenterPart(bool isCenterSpell)
	{
		QColor c;
		if (isCenterSpell)
			c = colorCenter();
		else
			c = Qt::GlobalColor::transparent;

		this->setBrush((QBrush(c)));
	}

	void RuneItem::clearItem()
	{
		if (this->scene())
		{
			this->scene()->removeItem(&this->image_);
			this->scene()->removeItem(&this->text_);
			this->scene()->removeItem(this);
		}
	}

	void RuneItem::focusInEvent(QFocusEvent * event)
	{
		this->setBrush(QBrush(colorSelected()));
		this->parentItem()->grabKeyboard();
		this->scene()->update();
	}

	void RuneItem::focusOutEvent(QFocusEvent * event)
	{
		this->setBrush(QBrush(colorBackground()));
		this->scene()->update();
	}

	void RuneItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
	{
		this->setFocus(Qt::FocusReason::MouseFocusReason);
	}

	void RuneItem::setIsText(bool isText)
	{
		text_.setVisible(isText);
		image_.setVisible(!isText);
	}

}

