#include "User.h"



namespace Runes
{
	Runes::User::User(string name)
	{
	}

	string Runes::User::getName()
	{
		return name_;
	}

	bool User::serialize(QXmlStreamWriter& stream)
	{
		// TODO : implement that
	}

	bool User::unserialize(QXmlStreamWriter& stream)
	{
		// TODO : implement that
	}
}