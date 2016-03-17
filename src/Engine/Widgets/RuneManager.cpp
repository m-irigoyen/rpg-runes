#include "RuneManager.h"

namespace Runes
{

	RuneManager::RuneManager(RuneEngine& engine, std::vector<QPixmap>& runeSprites) : runeEngine_(engine), globalRunes_(runeEngine_.getRunes()), runeSprites_(runeSprites), currentDescriptor(NULL)
	{
		init();
	}

	void RuneManager::init()
	{
		// Size hints

		// layouting the whole thing
		mainLayout_.addWidget(&view_);
		mainLayout_.addLayout(&rightLayout_, 10);

		rightLayout_.addWidget(&image_);
		rightLayout_.addLayout(&rightRightLayout_, 5);

		rightRightLayout_.addWidget(&ancientName_);
		rightRightLayout_.addWidget(&name_);
		rightRightLayout_.addWidget(&description_, 10);
		rightRightLayout_.addWidget(&newRuneButton_);

		newRuneButton_.setText("Add a new rune");

		// Filling the list
		initList();

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

	void RuneManager::clicked(QModelIndex index)
	{
		QString data = model_.data(index, 0).toString();
		fillData(data);
	}

	void RuneManager::editedName(QString& text)
	{
		currentDescriptor->setNaturalName(text);
		emit runesUpdated();
	}

	void RuneManager::editedAncientName(QString& text)
	{
		currentDescriptor->setName(text);
		emit runesUpdated();
	}

	void RuneManager::editedDescription(QString& text)
	{
		currentDescriptor->setDescription(text);
		emit runesUpdated();
	}

	void RuneManager::addNewRune()
	{
		runeEngine_.addNewRune();
	}

	void RuneManager::initList()
	{
		QStringList list;
		for (RunesContainer::iterator it = globalRunes_.begin(); it != globalRunes_.end(); ++it)
		{
			list << it->getDescriptor().getNaturalName();
		}
		model_.setStringList(list);

		view_.setModel(&model_);
		view_.setDragEnabled(false);
		view_.show();
	}

	void RuneManager::fillData(QString name)
	{
		int i = 0;
		currentDescriptor = &(runeEngine_.getRuneDescriptorByNaturalName(name));

		// Fill the display with corresponding values
		if (i <  runeSprites_.size())
			image_.setPixmap(runeSprites_.at(i));
		name_.setText(currentDescriptor->getNaturalName());
		ancientName_.setText(currentDescriptor->getName());
		description_.setText(currentDescriptor->getDescription());
	}

}