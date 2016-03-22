#include "RuneManager.h"

namespace Runes
{

	RuneManager::RuneManager(RuneEngine& engine, std::vector<QPixmap>& runeSprites) : runeEngine_(engine), globalRunes_(runeEngine_.getRunes()), runeSprites_(runeSprites), currentRune_(NULL)
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
		rightRightLayout_.addLayout(&rightRightBottomLayout_);

		rightRightBottomLayout_.addWidget(&newRuneButton_);
		rightRightBottomLayout_.addWidget(&reloadImagesButton_);

		newRuneButton_.setText("Add a new rune");

		// Filling the list
		initList();

		// Connecting button slots

		// Connecting view slots
		connect(&view_, SIGNAL(clicked(QModelIndex)),
			this, SLOT(clicked(QModelIndex)));
		connect(&name_, SIGNAL(textEdited(const QString&)),
			this, SLOT(editedName(const QString&)));
		connect(&ancientName_, SIGNAL(textEdited(const QString&)),
			this, SLOT(editedAncientName(const QString&)));
		connect(&description_, SIGNAL(textEdited(const QString&)),
			this, SLOT(editedDescription(const QString&)));

		connect(this, SIGNAL(runesUpdated()),
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

	void RuneManager::editedName(const QString& text)
	{
		currentRune_->descriptor_.naturalName_ = text;
		emit runesUpdated();
	}

	void RuneManager::editedAncientName(const QString& text)
	{
		currentRune_->descriptor_.name_ = text;
		emit runesUpdated();
	}

	void RuneManager::editedDescription(const QString& text)
	{
		currentRune_->descriptor_.description_ = text;
		emit runesUpdated();
	}

	void RuneManager::addNewRune()
	{
		runeEngine_.addNewRune();
	}

	void RuneManager::initList()
	{
		model_.setStringList(runeEngine_.getRuneList());

		view_.setModel(&model_);
		view_.setDragEnabled(false);
		view_.show();
	}

	QPushButton* RuneManager::getReloadImagesButton()
	{
		return &reloadImagesButton_;
	}

	void RuneManager::fillData(QString name)
	{
		int i = 0;
		currentRune_ = runeEngine_.getRuneByNaturalName(name);
		i = currentRune_->getIndex();
		QString test = currentRune_->descriptor_.name_;

		// Fill the display with corresponding values

		name_.setText(currentRune_->descriptor_.naturalName_);
		currentRune_ = runeEngine_.getRuneByNaturalName(name);
		ancientName_.setText(currentRune_->descriptor_.name_);
		currentRune_ = runeEngine_.getRuneByNaturalName(name);
		description_.setText(currentRune_->descriptor_.description_);
		currentRune_ = runeEngine_.getRuneByNaturalName(name);

		if (i < runeSprites_.size())
			image_.setPixmap(runeSprites_.at(i));
	}

}