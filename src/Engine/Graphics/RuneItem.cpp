#include "RuneItem.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QInputDialog>
#include <QApplication>

namespace Runes
{
	RuneItem::RuneItem(Spell* s, AbstractItem* parent, vector<QPixmap>& runeImages, RuneEngine& runeEngine)
		: AbstractItem(s, parent, runeImages, runeEngine)
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
			RuneDescriptor* rd = NULL;
			if (runeEngine_.isMasterMode())
				rd = &runes_->at(spell_->getRune())->descriptor_;
			else
				rd = &userRunes_->at(spell_->getRune());
			text_.setText(rd->naturalName_);
			text_.setPos(-text_.boundingRect().width() / 2, -text_.boundingRect().height() / 2);
			text_.hide();

			// Displaying the rune
			image_.setParentItem(this);
			resetImage();
			image_.setPos(-_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS, -_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS);
		}

		this->setRect(-_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS, -_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS,
			_GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS * 2, _GRAPHICS_SPELLITEM_RUNE_IMAGERADIUS * 2);
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

	void RuneItem::chooseRune()
	{
		bool ok;
		QString item = QInputDialog::getItem(NULL, tr("Choose a rune"),	tr("Rune:"),
			runeEngine_.getUserRuneList(), 0, false, &ok);
		if (ok && !item.isEmpty())
		{
			int ind = runeEngine_.getUserRuneIndexByNaturalName(item);
			if (ind != -1)
			{
				spell_->setRune(ind);
				resetImage();
			}
			else
				image_.setPixmap(QPixmap());
				
			emit(changedSpellButDontRedraw());
			update();
		}
	}

	void RuneItem::resetImage()
	{
		// Setting image too
		image_.setPixmap(runeImages_[spell_->getRune()]);
	}

	void RuneItem::focusInEvent(QFocusEvent * event)
	{
		this->setBrush(QBrush(colorSelected()));
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

	void RuneItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
	{
		this->setFocus(Qt::FocusReason::MouseFocusReason);
		this->chooseRune();
	}

	void RuneItem::keyPressEvent(QKeyEvent * event)
	{
		Spell * spellParent = NULL;
		switch (event->key())
		{
			// Delete : remove that spell
		case Qt::Key::Key_Delete:
			spellParent = spell_->getParent();
			if (spellParent == NULL)
			{
				// Can't remove the top level spell, so just clear it
				cout << "Clearing top level spell" << endl;
				spell_->clear();
				emit(changedSpell());
			}
			else
			{
				cout << "Removing spell" << endl;
				spellParent->remove(spell_);
				emit(changedSpell());
			}
			break;
		case Qt::Key::Key_Minus:
			// remove : remove last
			if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				spell_->removeLastChild();
			else if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
				spell_->removeLastModifier();
			else
				spell_->removeLastComponent();
			emit(changedSpell());
			break;
		case Qt::Key::Key_Plus:
		case Qt::Key::Key_Insert:
			// alternative add : add a child
			if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
				spell_->addEmptyChild();
			else if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
				spell_->addEmptyModifier();
			else
				spell_->addEmptyComponent();
			emit(changedSpell());
			break;
		case Qt::Key::Key_Back:
			spell_->setRune(-1);
			image_.setPixmap(QPixmap());
			emit(changedSpell());
			break;
		case Qt::Key::Key_Enter:
		case Qt::Key::Key_Return:
			chooseRune();
			setFocus(Qt::FocusReason::OtherFocusReason);
			break;
		case Qt::Key::Key_Escape:
			clearFocus();
			break;
		}
	}

	void RuneItem::setIsText(bool isText)
	{
		text_.setVisible(isText);
		image_.setVisible(!isText);
	}
}

