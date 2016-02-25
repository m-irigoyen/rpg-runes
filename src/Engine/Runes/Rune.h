#ifndef _RUNES_RUNE_H_
#define _RUNES_RUNE_H_

#include "Engine/Utilities/Serializable.h"
#include "RuneDescriptor.h"

#include <string>

using namespace std;

namespace Runes
{
	class Rune : public Serializable
	{
	public:
		Rune();
		Rune(int index, RuneDescriptor descriptor);

		// Hérité via Serializable
		virtual bool serialize(xml_node * node) override;
		virtual bool unserialize(xml_node * node) override;

		void setIndex(int index);

		const int getIndex();
		const string getName();
		const string getNaturalName();
		const string getDescription();
		const RuneDescriptor getDescriptor();

	protected:
		int index_;
		string name_;
		RuneDescriptor descriptor_;
	};
}


#endif //_RUNES_RUNE_H_