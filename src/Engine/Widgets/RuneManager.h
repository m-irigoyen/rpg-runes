#ifndef _ENGINE_WIDGETS_RUNEMANAGER_H_
#define _ENGINE_WIDGETS_RUNEMANAGER_H_

#include <vector>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QListView>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QStringListModel>
#include <QPushButton>

#include "Engine/Runes/RuneEngine.h"

namespace Runes
{
	class RuneManager : public QWidget
	{
		Q_OBJECT
	public:
		RuneManager(RuneEngine& engine, std::vector<QPixmap>& runeSprites);

		// Model management
		//! @brief inits model from the list of userRunes
		void init();

		public slots:

		void clicked(QModelIndex index);
		void currentChanged(QModelIndex current, QModelIndex previous);
		void editedName(const QString& text);
		void editedAncientName(const QString& text);
		void editedDescription(const QString& text);

		void initList();

		QPushButton* getReloadButton();

	signals:
		void runesUpdated();
		void runeImagesUpdated();

	public slots :
		void reload();

	protected:
		// References
		RuneEngine& runeEngine_;
		RunesContainer& globalRunes_;
		std::vector<QPixmap>& runeSprites_;

		// Layout
		QHBoxLayout mainLayout_;		//!< Main : two parts. Left is the list of runes, right is the display / edit
		QHBoxLayout rightLayout_;		//!< Right : two parts. Left is the image, right the name/desc
		QVBoxLayout rightRightLayout_;	//!< Right right : two parts. Top is the name, bottom the description
		QHBoxLayout rightRightBottomLayout_;	//!< The two buttons

		QHBoxLayout nameLayout_;
		QHBoxLayout ancientNameLayout_;
		QHBoxLayout descriptionLayout_;

		// Widgets
		QListView view_;
		QLabel image_;
		QLineEdit name_;
		QLineEdit ancientName_;
		QLineEdit description_;
		QLabel nameLabel_;
		QLabel ancientNameLabel_;
		QLabel descriptionLabel_;
		QPushButton newRuneButton_;
		QPushButton reloadButton_;

		// Rune list
		QStringListModel model_;

		// Modification
		RuneDescriptor* currentDescriptor_;

		// helper functions
		void fillData(QString name);
		QModelIndex currentlyClicked;
	};
}

#endif