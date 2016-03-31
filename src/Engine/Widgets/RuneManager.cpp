#include "RuneManager.h"

namespace Runes
{

	RuneManager::RuneManager(RuneEngine& engine, std::vector<QPixmap>& runeSprites) : runeEngine_(engine), globalRunes_(runeEngine_.getRunes()), runeSprites_(runeSprites), currentDescriptor_(NULL)
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

		ancientName_.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum));
		name_.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Maximum));
		description_.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
		ancientNameLayout_.addWidget(&ancientNameLabel_);
		ancientNameLayout_.addWidget(&ancientName_);
		nameLayout_.addWidget(&nameLabel_);
		nameLayout_.addWidget(&name_);
		descriptionLayout_.addWidget(&descriptionLabel_);
		descriptionLayout_.addWidget(&description_);
		ancientNameLabel_.setText("Ancient Name : ");
		nameLabel_.setText("Natural Name : ");
		descriptionLabel_.setText("Description : ");
		
		rightRightLayout_.addLayout(&ancientNameLayout_, 0);
		rightRightLayout_.addLayout(&nameLayout_, 0);
		rightRightLayout_.addLayout(&descriptionLayout_, 10);
		rightRightLayout_.addLayout(&rightRightBottomLayout_,0);

		rightRightBottomLayout_.addWidget(&newRuneButton_);
		rightRightBottomLayout_.addWidget(&reloadButton_);

		newRuneButton_.setText("Add a new rune");
		reloadButton_.setText("Reload dictionnary");

		// Filling the list
		initList();

		// Connecting button slots

		// Connecting view slots
		connect(&view_, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));
		connect(&view_, SIGNAL(activated(QModelIndex)),	this, SLOT(clicked(QModelIndex)));
		connect(&view_, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(currentChanged(QModelIndex, QModelIndex)));
		connect(&name_, SIGNAL(textEdited(const QString&)), this, SLOT(editedName(const QString&)));
		connect(&ancientName_, SIGNAL(textEdited(const QString&)), this, SLOT(editedAncientName(const QString&)));
		connect(&description_, SIGNAL(textEdited(const QString&)), this, SLOT(editedDescription(const QString&)));

		connect(this, SIGNAL(runesUpdated()),&runeEngine_, SLOT(changedRunes()));

		connect(&newRuneButton_, SIGNAL(clicked()),	&runeEngine_, SLOT(addNewRune()));
		connect(&newRuneButton_, SIGNAL(clicked()),	this, SLOT(reload()));


		this->setLayout(&mainLayout_);

		// Setting the first rune
		clicked(model_.index(0, 0));
	}

	void RuneManager::clicked(QModelIndex index)
	{
		currentlyClicked = index;
		QString data = model_.data(index, 0).toString();
		fillData(data);
	}

	void RuneManager::currentChanged(QModelIndex current, QModelIndex previous)
	{
		this->clicked(current);
	}

	void RuneManager::editedName(const QString& text)
	{
		currentDescriptor_->naturalName_ = text;
		emit runesUpdated();
		model_.setData(currentlyClicked, text, 0);
	}

	void RuneManager::editedAncientName(const QString& text)
	{
		currentDescriptor_->name_ = text;
		emit runesUpdated();
	}

	void RuneManager::editedDescription(const QString& text)
	{
		currentDescriptor_->description_ = text;
		emit runesUpdated();
	}

	void RuneManager::initList()
	{
		model_.setStringList(runeEngine_.getRuneList());

		view_.setModel(&model_);
		view_.setDragEnabled(false);
		view_.show();
	}

	QPushButton* RuneManager::getReloadButton()
	{
		return &reloadButton_;
	}

	void RuneManager::fillData(QString name)
	{
		int i = 0;
		currentDescriptor_ = &runeEngine_.getRuneByNaturalName(name)->descriptor_;
		i = runeEngine_.getRuneByNaturalName(name)->getIndex();
		// Fill the display with corresponding values

		currentDescriptor_ = &runeEngine_.getRuneByNaturalName(name)->descriptor_;
		QString nn(currentDescriptor_->naturalName_);
		currentDescriptor_ = &runeEngine_.getRuneByNaturalName(name)->descriptor_;
		QString n(currentDescriptor_->name_);
		currentDescriptor_ = &runeEngine_.getRuneByNaturalName(name)->descriptor_;
		QString d(currentDescriptor_->description_);

		name_.setText(nn);
		ancientName_.setText(n);
		description_.setText(d);

		if (i < runeSprites_.size())
			image_.setPixmap(runeSprites_.at(i));
		else
			image_.setPixmap(QPixmap());
	}

	void RuneManager::reload()
	{
		initList();
	}

}