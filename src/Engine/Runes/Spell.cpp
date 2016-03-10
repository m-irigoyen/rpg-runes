#include "Spell.h"

namespace Runes
{
	Spell::Spell(int rune) : centerRune_(rune), parent_(NULL), centerSpell_(NULL), centerIsSpell_(false)
	{
	}

	Spell::~Spell()
	{
		this->clear();
	}

	bool Spell::serialize(QXmlStreamWriter& stream)
	{
		stream.writeStartElement("spell");
		stream.writeAttribute(QXmlStreamAttribute("rune", QString::number(this->centerRune_)));
		
		for (Spell* child : children_)
		{
			stream.writeStartElement("children");
			child->serialize(stream);
			stream.writeEndElement();
		}

		stream.writeEndElement();
		return true;
	}

	bool Spell::unserialize(QXmlStreamReader& stream)
	{
		Q_ASSERT(stream.isStartElement() && stream.name() == "spell");
		stream.readNextStartElement();
		for (const QXmlStreamAttribute &attr : stream.attributes())
		{
			if (attr.name().toString() == QLatin1String("rune"))
			{
				this->centerRune_ = attr.value().toInt();
			}
		}

		// children
		Q_ASSERT(stream.isStartElement() && stream.name() == "children");
		stream.readNextStartElement();
		while (stream.name() == "spell")
		{
			Spell* s = new Spell();
			children_.push_back(s);
			s->unserialize(stream);

			stream.readNextStartElement();
		}

		return true;
	}

	bool Spell::isCenterSpell()
	{
		return centerIsSpell_;
	}

	Spell* Spell::getCenterSpell()
	{
		if (centerIsSpell_)
			return centerSpell_;
		else
			return NULL;
	}

	void Spell::setCenterSpell(Spell* s)
	{
		centerRune_ = -1;

		if (centerIsSpell_)
		{
			s->clear();
			s = NULL;
			centerIsSpell_ = false;
		}

		if (s)
		{
			centerSpell_ = s;
			centerIsSpell_ = true;
		}
	}

	void Spell::setRune(int newRune)
	{
		if (newRune >= -1)
			centerRune_ = newRune;
	}

	int Spell::getRune()
	{
		return centerRune_;
	}

	void Spell::setParent(Spell * parent)
	{
		if (parent == NULL)
		{
			// TODO error code
			return;
		}

		parent_->removeChild(this);
		parent_ = parent;
	}

	Spell* Spell::getParent()
	{
		return parent_;
	}
	
	bool Spell::isTopLevel()
	{
		return (parent_ == NULL);
	}

	void Spell::addChild(Spell * child)
	{
		// TODO : add safety mechanism in the engine to prevent adding children on the top level spell
		/*if (this->parent_ == NULL)
		{
			cout << "ERROR ! The top level cannot have children. Convert this spell to the be the inner spell of a new spell." << endl;
			if (child)
			{
				delete(child);
				return;
			}
		}*/

		if (find(children_.begin(), children_.end(), child) == children_.end())
		{
			children_.push_back(child);
		}
	}

	void Spell::removeChild(Spell * child)
	{
		vector<Spell*>::iterator it = find(children_.begin(), children_.end(), child);
		if (it != children_.end())
		{
			children_.erase(it);
		}
	}

	vector<Spell*>& Spell::getChildren()
	{
		return children_;
	}

	// Component management
	void Spell::addComponent(Spell* component)
	{
		vector<Spell*>::iterator it = find(components_.begin(), components_.end(), component);
		if (it == components_.end())
		{
			components_.push_back(component);
		}
		else
		{
			cout << "ERROR!" << endl;
			abort();
		}
	}

	void Spell::removeComponent(Spell* component)
	{
		vector<Spell*>::iterator it = find(components_.begin(), components_.end(), component);
		if (it != components_.end())
		{
			components_.erase(it);
		}
	}

	vector<Spell*>& Spell::getComponents()
	{
		return components_;
	}

	void Spell::clear()
	{
		centerRune_ = -1;

		if (centerSpell_)
		{
			centerSpell_->clear();
			delete(centerSpell_);
			centerSpell_ = NULL;
		}

		if (parent_ != NULL)
		{
			parent_->removeChild(this);
			parent_ = NULL;
		}

		for (Spell* child : children_)
		{
			child->clear();
			delete(child);
		}
		children_.clear();
	}
}

