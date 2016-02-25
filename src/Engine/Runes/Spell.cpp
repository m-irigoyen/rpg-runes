#include "Spell.h"

namespace Runes
{
	Runes::Spell::Spell(int rune) : rune_(rune), parent_(NULL)
	{
	}

	bool Runes::Spell::serialize(xml_node * node)
	{
		if (!node)
		{
			// TODO error code
			return false;
		}

		node->set_name("Spell");
		node->append_attribute("type").set_value("Spell");
		node->append_attribute("rune").set_value(rune_);

		for (Spell* child : children_)
		{
			if (!child->serialize(&node->append_child("child")))
			{
				// TODO error code
				return false;
			}
		}

		return true;
	}

	bool Runes::Spell::unserialize(xml_node * node)
	{
		if (!node)
		{
			// TODO error code
			return false;
		}

		if (string((node->attribute("type").as_string())).compare("Spell"))
		{
			rune_ = node->attribute("rune").as_int();
			// Nothing more to do : the children are handled by the RuneEngine
			return true;
		}
		else
		{
			// TODO error code
			return false;
		}
	}

	void Runes::Spell::setRune(int newRune)
	{
		if (newRune >= -1)
			rune_ = newRune;
	}

	int Runes::Spell::getRune()
	{
		return rune_;
	}

	void Runes::Spell::setParent(Spell * parent)
	{
		if (parent == NULL)
		{
			// TODO error code
			return;
		}

		parent_->removeChild(this);
		parent_ = parent;
	}

	Spell* Runes::Spell::getParent()
	{
		return parent_;
	}

	void Runes::Spell::addChild(Spell * child)
	{
		if (find(children_.begin(), children_.end(), child) != children_.end())
		{
			children_.push_back(child);
		}
	}

	void Runes::Spell::removeChild(Spell * child)
	{
		vector<Spell*>::iterator it = find(children_.begin(), children_.end(), child);
		if (it != children_.end())
		{
			children_.erase(it);
		}
	}

	vector<Spell*>& Runes::Spell::getChildren()
	{
		return children_;
	}

	void Runes::Spell::clear()
	{
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

