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

		name_.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum));
		description_.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
		nameLayout_.addWidget(&nameLabel_);
		nameLayout_.addWidget(&name_);
		descriptionLayout_.addWidget(&descriptionLabel_);
		descriptionLayout_.addWidget(&description_);

		nameLabel_.setText("Natural Name : ");
		descriptionLabel_.setText("Description : ");
		rightRightLayout_.addLayout(&nameLayout_, 0);
		rightRightLayout_.addLayout(&descriptionLayout_, 10);

		// Filling the list
		model_.setStringList(runeEngine_.getUserRuneList());

		view_.setModel(&model_);
		view_.setDragEnabled(false);
		view_.show();
		view_.setAcceptDrops(false);
		view_.setDragEnabled(false);

		// Connecting slots
		connect(&view_, SIGNAL(clicked(QModelIndex)), 
			this, SLOT(clicked(QModelIndex)));
		connect(&name_, SIGNAL(textEdited(const QString&)),
			this, SLOT(editedName(const QString&)));
		connect(&description_, SIGNAL(textEdited(const QString&)),
			this, SLOT(editedDescription(const QString&)));

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

	void RuneDictionnary::editedName(const QString& text)
	{
		currentDescriptor->naturalName_ = text;
		emit userRunesUpdated();
	}

	void RuneDictionnary::editedDescription(const QString& text)
	{
		currentDescriptor->description_ = text;
		emit userRunesUpdated();
	}

	void RuneDictionnary::fillData(QString name)
	{
		int i = 0;
		currentDescriptor = runeEngine_.getUserRuneByNaturalName(name);
		i = runeEngine_.getUserRuneIndexByNaturalName(name);

		// Fill the display with corresponding values
		if (i < runeSprites_.size())
			image_.setPixmap(runeSprites_.at(i));
		else
			image_.setPixmap(QPixmap());
		name_.setText(currentDescriptor->naturalName_);
		description_.setText(currentDescriptor->description_);
	}
}