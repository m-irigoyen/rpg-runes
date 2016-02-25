#ifndef _RUNES_SPELL_H_
#define _RUNES_SPELL_H_

#include "Engine/Utilities/Serializable.h"

#include <vector>

using namespace std;

namespace Runes
{
	class Spell : public Serializable
	{
	public:
		Spell(int rune);

		// Hérité via Serializable
		virtual bool serialize(xml_node * node) override;
		virtual bool unserialize(xml_node * node) override;

		void setRune(int newRune);
		int getRune();

		void setParent(Spell* parent);
		Spell* getParent();

		// Children management
		void addChild(Spell* child);
		void removeChild(Spell* child);
		vector<Spell*>& getChildren();

		// Components management
		void addComponent(Spell* component);
		void removeComponent(Spell* component);
		vector<Spell*>& getComponents();

		// Else
		virtual void clear();

	private:
		int rune_;
		Spell* parent_;
		vector<Spell*> children_; //!< Children are spells that apply on this spell as a whole
		vector<Spell*> components_; //!< Components are the runes that apply on the main rune of this spell
	};
}


#endif //_RUNES_SPELL_H_