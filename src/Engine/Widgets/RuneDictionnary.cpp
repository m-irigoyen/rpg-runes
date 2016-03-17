#include "RuneDictionnary.h"

namespace Runes
{

	RuneDictionnary::RuneDictionnary(RuneEngine& engine, std::vector<QPixmap>& runeSprites) : runeEngine_(engine), globalRunes_(runeEngine_.getRunes()), userRunes_(runeEngine_.getUserRunes()), runeSprites_(runeSprites), currentDescriptor(NULL)
	{
		init();
	}

	void RuneDictionnary::init()
	{
		// Size hints

		// layouting the whole thing
		mainLayout_.addWidget(&view_, 0);
		mainLayout_.addLayout(&rightLayout_, 10);

		rightLayout_.addWidget(&image_);
		rightLayout_.addLayout(&rightRightLayout_, 5);

		rightRightLayout_.addWidget(&name_);
		rightRightLayout_.addWidget(&description_,10);

		// Filling the list
		QStringList list;
		for (UserRunesContainer::iterator it = userRunes_.begin(); it != userRunes_.end(); ++it)
		{
			list << it->second.getNaturalName();
		}
		model_.setStringList(list);

		view_.setModel(&model_);
		view_.setDragEnabled(false);
		view_.show();

		// Connecting slots
		connect(&view_, SIGNAL(clicked(QModelIndex)), 
			this, SLOT(clicked(QModelIndex)));
		connect(&name_, SIGNAL(textChanged(QString&)),
			this, SLOT(editedName(QString&)));
		connect(&description_, SIGNAL(textChanged(QString&)),
			this, SLOT(editedDescription(QString&)));

		connect(this, SIGNAL(userRunesUpdated()),
			&runeEngine_, SLOT(changedProfile()));

		this->setLayout(&mainLayout_);

		// Setting the first rune
		clicked(model_.index(0, 0));
	}

	void RuneDictionnary::clicked(QModelIndex index)
	{
		QString data = model_.data(index,0).toString();
		fillData(data);
	}

	void RuneDictionnary::editedName(QString& text)
	{
		currentDescriptor->setNaturalName(text);
		emit userRunesUpdated();
	}

	void RuneDictionnary::editedDescription(QString& text)
	{
		currentDescriptor->setDescription(text);
		emit userRunesUpdated();
	}

	void RuneDictionnary::fillData(QString name)
	{
		int i = 0;
		currentDescriptor = &(RuneEngine::getUserRuneByNaturalName(name, userRunes_, &i));

		// Fill the display with corresponding values
		image_.setPixmap(runeSprites_.at(i));
		name_.setText(currentDescriptor->getNaturalName());
		description_.setText(currentDescriptor->getDescription());
	}

}