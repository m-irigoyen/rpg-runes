#include "RuneDictionnary.h"

namespace Runes
{

	RuneDictionnary::RuneDictionnary(RuneEngine& engine, std::vector<QPixmap>& runeSprites) : runeEngine_(engine), globalRunes_(runeEngine_.getRunes()), userRunes_(runeEngine_.getUserRunes()), runeSprites_(runeSprites)
	{
		init();
	}

	void RuneDictionnary::init()
	{
		QStandardItem *parentItem = model_.invisibleRootItem();
		for (int i = 0; i < userRunes_.size(); ++i) 
		{
			QStandardItem *item = new QStandardItem();
			parentItem->appendRow(item);
			parentItem = item;
		}
	}

}