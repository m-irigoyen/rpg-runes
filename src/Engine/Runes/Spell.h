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
		Spell(int rune = -1);
		~Spell();

		// Serialization
		virtual bool serialize(QXmlStreamWriter& stream) override;
		virtual bool unserialize(QXmlStreamReader& stream) override;

		void setRune(int newRune);
		int getRune();

		// Parenting spell
		void setParent(Spell* parent);
		Spell* getParent();
		bool isTopLevel();

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
		// The center component is either a spell, or a rune.
		int rune_;
		Spell* spell_;
		bool centerIsSpell_;

		Spell* parent_;
		vector<Spell*> children_; //!< Children are spells that apply on this spell as a whole
		vector<Spell*> components_; //!< Components are the runes that apply on the main rune of this spell
	};
}


#endif //_RUNES_SPELL_H_