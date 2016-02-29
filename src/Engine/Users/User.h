#ifndef _USERS_USER_H_
#define _USERS_USER_H_

#include "Engine/Utilities/Serializable.h"
#include "Engine/Runes/Rune.h"

#include <vector>

using namespace std;

namespace Runes
{
	class User : public Serializable
	{
	public:
		User(string name);

		// Inherited via Serializable
		//! @brief Serializes the content of the object into the given node. Returns true if succesful, false otherwise
		virtual bool serialize(QXmlStreamWriter& stream) override;
		//! @brief Unserializes the content of the given node into the object. Returns true if succesful, false otherwise
		virtual bool unserialize(QXmlStreamWriter& stream) override;

		string getName();

	private:
		string name_;	//!< The name of this user
		vector<Rune> runes_;	//!< The runes this user has discovered
	};
}


#endif //_USERS_USER_H_