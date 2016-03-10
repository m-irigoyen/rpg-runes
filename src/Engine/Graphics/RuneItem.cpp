#include "RuneItem.h"
#include <QGraphicsScene>

namespace Runes
{
	RuneItem::RuneItem(Spell* s, AbstractItem* parent) : AbstractItem(s, parent)
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

	void RuneItem::drawSpell(vector<QPixmap>& runeImages, RunesContainer& runes, UserRunesContainer& userRunes, QGraphicsScene& scene)
	{
		// Setting the text
		text_.setParentItem(this);
		UserRune& ur = userRunes.at(spell_->getRune());
		if (ur.second)
			text_.setText(ur.first.getNaturalName());
		text_.hide();

		// Displaying the rune
		image_.setParentItem(this);
		image_.setPixmap(runeImages[spell_->getRune()]);
		image_.setPos(-_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS, -_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS);
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
		this->scene()->removeItem(&this->image_);
		this->scene()->removeItem(&this->text_);
		this->scene()->removeItem(this);
	}

	void RuneItem::focusInEvent(QFocusEvent * event)
	{
		// TODO : focus event
	}

	void RuneItem::focusOutEvent(QFocusEvent * event)
	{
		// TODO : focus event
	}

}

