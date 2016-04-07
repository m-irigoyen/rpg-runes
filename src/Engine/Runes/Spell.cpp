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
		
		if (!components_.empty())
		{
			stream.writeStartElement("components");
			for (Spell* component : components_)
				component->serialize(stream);
			stream.writeEndElement();
		}
		
		if (!children_.empty())
		{
			stream.writeStartElement("children");
			for (Spell* child : children_)
				child->serialize(stream);
			stream.writeEndElement();
		}

		if (!modifiers_.empty())
		{
			stream.writeStartElement("modifiers");
			for (Spell* mod : modifiers_)
				mod->serialize(stream);
			stream.writeEndElement();
		}

		stream.writeEndElement();
		return true;
	}

	bool Spell::unserialize(QXmlStreamReader& stream)
	{
		if (stream.name() == "spell")
		{
			for (const QXmlStreamAttribute &attr : stream.attributes())
			{
				if (attr.name().toString() == QLatin1String("rune"))
				{
					this->centerRune_ = attr.value().toInt();
				}
			}
			stream.readNextStartElement();

			if (stream.name() == "components")
			{
				while (stream.readNextStartElement())
				{
					Spell* s = new Spell();
					s->unserialize(stream);
					components_.push_back(s);
				}
				stream.readNextStartElement();
			}

			if (stream.name() == "children")
			{
				while (stream.readNextStartElement())
				{
					Spell* s = new Spell();
					s->unserialize(stream);
					children_.push_back(s);
				}
				stream.readNextStartElement();
			}

			if (stream.name() == "modifiers")
			{
				while (stream.readNextStartElement())
				{
					Spell* s = new Spell();
					s->unserialize(stream);
					modifiers_.push_back(s);
				}
				stream.readNextStartElement();
			}
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

		if (parent_ != NULL)
			parent_->removeChildWithoutClear(this);
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

	void Spell::addEmptyChild()
	{
		Spell* s = new Spell();
		s->setParent(this);
		this->children_.push_back(new Spell());
	}

	void Spell::addChild(Spell * child)
	{
		if (child == NULL)
			return;

		// Clearing previous parent
		if (child->getParent() != NULL)
			child->getParent()->removeWithoutClear(child);

		child->setParent(this);
		if (find(children_.begin(), children_.end(), child) == children_.end())
			children_.push_back(child);
		else
			abort();
	}

	void Spell::removeChild(Spell * child)
	{
		if (child == NULL)
			return;
		vector<Spell*>::iterator it = find(children_.begin(), children_.end(), child);
		if (it != children_.end())
		{
			children_.erase(it);
			child->clear();
			delete(child);
		}
	}

	void Spell::removeChildWithoutClear(Spell* child)
	{
		if (child == NULL)
			return;
		vector<Spell*>::iterator it = find(children_.begin(), children_.end(), child);
		if (it != children_.end())
			children_.erase(it);
	}

	void Spell::removeLastChild()
	{
		if (!children_.empty())
			children_.back()->destroy();
	}

	vector<Spell*>& Spell::getChildren()
	{
		return children_;
	}

	// Component management
	void Spell::addComponent(Spell* component)
	{
		if (component == NULL)
			return;
		// Clearing previous parent
		if (component->getParent() != NULL)
			component->getParent()->removeWithoutClear(component);

		component->setParent(this);
		if (find(components_.begin(), components_.end(), component) == components_.end())
			components_.push_back(component);
		else
			abort();
	}

	void Spell::addEmptyComponent()
	{
		Spell* s = new Spell();
		s->setParent(this);
		this->components_.push_back(s);
	}

	void Spell::removeComponent(Spell* component)
	{
		if (component == NULL)
			return;
		vector<Spell*>::iterator it = find(components_.begin(), components_.end(), component);
		if (it != components_.end())
		{
			components_.erase(it);
			component->clear();
			delete(component);
		}
	}

	void Spell::removeComponentWithoutClear(Spell* component)
	{
		if (component == NULL)
			return;
		vector<Spell*>::iterator it = find(components_.begin(), components_.end(), component);
		if (it != components_.end())
			components_.erase(it);
	}

	void Spell::removeLastComponent()
	{
		if (!components_.empty())
			components_.back()->destroy();
	}

	vector<Spell*>& Spell::getComponents()
	{
		return components_;
	}

	bool Spell::isModifier()
	{
		if (parent_ != NULL && parent_->isModifier(this))
			return true;
		return false;
	}

	bool Spell::isModifier(Spell* s)
	{
		if (find(modifiers_.begin(), modifiers_.end(), s) != modifiers_.end())
			return true;
		return false;
	}

	void Spell::addModifier(Spell* modifier)
	{
		if (modifier == NULL)
			return;
		else if (modifier->isCenterSpell() || modifiers_.size() > NB_MAX_MODIFIERS)
		{
			delete (modifier);
			return;
		}

		// Clearing previous parent
		if (modifier->getParent() != NULL)
			modifier->getParent()->removeWithoutClear(modifier);

		modifier->setParent(this);
		if (find(modifiers_.begin(), modifiers_.end(), modifier) == modifiers_.end())
			modifiers_.push_back(modifier);
		else
			abort();
	}

	void Spell::addEmptyModifier()
	{
		Spell* s = new Spell();
		s->setParent(this);
		this->modifiers_.push_back(s);
	}

	void Spell::removeModifier(Spell* modifier)
	{
		if (modifier == NULL)
			return;
		vector<Spell*>::iterator it = find(modifiers_.begin(), modifiers_.end(), modifier);
		if (it != modifiers_.end())
		{
			modifiers_.erase(it);
			modifier->clear();
			delete(modifier);
		}
	}

	void Spell::removeModifierWithoutClear(Spell* modifier)
	{
		if (modifier == NULL)
			return;
		vector<Spell*>::iterator it = find(modifiers_.begin(), modifiers_.end(), modifier);
		if (it != modifiers_.end())
			modifiers_.erase(it);
	}

	void Spell::removeLastModifier()
	{
		if (!modifiers_.empty())
			modifiers_.back()->destroy();
	}

	vector<Spell*>& Spell::getModifiers()
	{
		return modifiers_;
	}

	void Spell::remove(Spell* s, bool destroy)
	{
		if (find(children_.begin(), children_.end(), s) != children_.end())
			removeChild(s);
		else if (find(components_.begin(), components_.end(), s) != components_.end())
			removeComponent(s);
		else
			return;
		
		if (destroy)
			delete(s);
	}

	void Spell::removeWithoutClear(Spell* s)
	{
		if (find(children_.begin(), children_.end(), s) != children_.end())
			removeChildWithoutClear(s);
		else if (find(components_.begin(), components_.end(), s) != components_.end())
			removeComponentWithoutClear(s);
		else
			abort();
	}

	void Spell::clear()
	{
		centerRune_ = -1;

		if (centerSpell_ != NULL)
		{
			centerSpell_->clear();
			delete(centerSpell_);
			centerSpell_ = NULL;
		}
		centerIsSpell_ = false;

		for (Spell* child : children_)
		{
			child->clear();
			delete(child);
		}
		children_.clear();
		for (Spell* comp : components_)
		{
			comp->clear();
			delete(comp);
		}
		components_.clear();
		for (Spell* mod : modifiers_)
		{
			mod->clear();
			delete(mod);
		}
		modifiers_.clear();
	}

	void Spell::destroy()
	{
		clear();

		if (parent_ != NULL)
		{
			parent_->remove(this, true);
		}
	}

}

