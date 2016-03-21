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

		void setIndex(int index);

		// Serialization
		virtual bool serialize(QXmlStreamWriter& stream) override;
		virtual bool unserialize(QXmlStreamReader& stream) override;

		const int getIndex();

		RuneDescriptor descriptor_;

	protected:
		int index_;
		
	};
}


#endif //_RUNES_RUNE_H_