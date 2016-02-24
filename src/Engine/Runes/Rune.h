#ifndef _RUNES_RUNE_H_
#define _RUNES_RUNE_H_

#include "Engine/Utilities/Serializable.h"

#include <string>

using namespace std;

namespace Runes
{
	class Rune : public Serializable
	{
	public:
		Rune(int index, string name);

		// Hérité via Serializable
		virtual bool serialize(xml_node * node) override;
		virtual bool unserialize(xml_node * node) override;

		const int getIndex();
		const string getName();

	private:
		int index_;
		string name_;
	};
}


#endif //_RUNES_RUNE_H_