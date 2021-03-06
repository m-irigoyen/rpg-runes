#ifndef _RUNES_SPELL_H_
#define _RUNES_SPELL_H_

#include "Engine/Utilities/Serializable.h"

#include <vector>

#define NB_MAX_MODIFIERS 10

using namespace std;

/*
	A spell is made of 2 things : the center part (required), and the components. The center part can be a simple rune, or another spell entirely. A spell stores a pointer to each of its components. Each of these components can be a simple rune, or another spell (with its own components and children). A spell also has a reference to its parent. The spell who has no parent is the top level spell. There is only one top level spell per hierarchy.
	
	
	Let's study two spells A and B. There are 4 possible cases for the value of A's parent : 
	- If A is the top level spell, it has no parent. Then A is the "root" spell, the highest in the hierarchy.
	- If A is a component of B, then A's parent_ points to B, and A can be found in B's components_ vector.
	- If A is a children of B, then A's parent_ points to B, and A can be found in B's chlidren_ vector.
	- If A is the center part of B, then A's parent_ points to B, and B's centerSpell_ is a pointer to A.
	
	- A spell whose center part is a rune, and has no children and no components, is the equivalent of a simple rune.

	The top level spell cannot have children. It just can't
*/

namespace Runes
{
	class Spell : public Serializable
	{
	public:
		Spell(int rune = -1);
		~Spell();

		// Serialization
		virtual bool serialize(QXmlStreamWriter& stream) override;
		virtual bool unserialize(QXmlStreamReader& stream) override;

		bool isCenterSpell();
		Spell* getCenterSpell();
		void setCenterSpell(Spell*);
		void setRune(int newRune);
		int getRune();

		// Parenting spell
		void setParent(Spell* parent);
		Spell* getParent();
		bool isTopLevel();

		// Children management
		void addEmptyChild();
		void addChild(Spell* child);
		void removeChild(Spell* child);
		void removeChildWithoutClear(Spell* child);
		void removeLastChild();
		vector<Spell*>& getChildren();

		// Component management
		void addComponent(Spell* component);
		void addEmptyComponent();
		void removeComponent(Spell* component);
		void removeComponentWithoutClear(Spell* component);
		void removeLastComponent();
		vector<Spell*>& getComponents();

		// Modifier management
		bool isModifier();	// Returns true if this is a modifier of its parent spell
		bool isModifier(Spell* s); // Returns true if s is a modifier of this
		void addModifier(Spell* modifier);
		void addEmptyModifier();
		void removeModifier(Spell* modifier);
		void removeModifierWithoutClear(Spell* modifier);
		void removeLastModifier();
		vector<Spell*>& getModifiers();

		// helper 
		//! @brief Search for the spell in both components and children, and remove it if found
		void remove(Spell* s, bool destroy = false);

		//! @brief same as above, but without clearing/deleting the item
		void removeWithoutClear(Spell* s);

		// Else
		virtual void clear();
		virtual void destroy();

	private:
		// The center component is either a spell, or a rune.
		int centerRune_;
		Spell* centerSpell_;
		bool centerIsSpell_;

		Spell* parent_;	// If this spell is a component, parent_ contains a pointer to the parent spell. If this spell is the center of another spell, then parent_ contains a poitner to that other spell
		vector<Spell*> children_; //!< Children are spells that apply on this spell as a whole. They are not part of this spell.
		vector<Spell*> components_;	//!< Components are spells that apply on the center part of this spell. Components are part of this spell.
		vector<Spell*> modifiers_;	//!< Modifiers are runes applied on a spell. Only size or shape specifiers apply : the rest are used to define an ID for sender/receiver spells
	};
}

#endif //_RUNES_SPELL_H_