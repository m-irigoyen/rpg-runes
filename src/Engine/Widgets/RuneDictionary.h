#ifndef _ENGINE_WIDGETS_RUNEDICTIONNARY_H_
#define _ENGINE_WIDGETS_RUNEDICTIONNARY_H_

#include <vector>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QListView>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QStringListModel>

#include "Engine/Runes/RuneEngine.h"

namespace Runes
{
	class RuneDictionary : public QWidget
	{
		Q_OBJECT
	public:
		RuneDictionary(RuneEngine& engine, std::vector<QPixmap>& runeSprites);

		// Model management
		//! @brief inits model from the list of userRunes
		void init();

	public slots:

		void clicked(QModelIndex index);
		void editedName(const QString& text);
		void editedDescription(const QString& text);

	signals:
		void userRunesUpdated();

	protected:
		// References
		RuneEngine& runeEngine_;
		RunesContainer& globalRunes_;
		UserRunesContainer& userRunes_;
		std::vector<QPixmap>& runeSprites_;

		// Layouts
		QHBoxLayout mainLayout_;		//!< Main : two parts. Left is the list of runes, right is the display / edit
		QHBoxLayout rightLayout_;		//!< Right : two parts. Left is the image, right the name/desc
		QVBoxLayout rightRightLayout_;	//!< Right right : two parts. Top is the name, bottom the description

		QHBoxLayout nameLayout_;
		QHBoxLayout descriptionLayout_;

		// Widgets
		QLabel nameLabel_;
		QLabel descriptionLabel_;
		QListView view_;
		QLabel image_;
		QLineEdit name_;
		QLineEdit description_;

		// Rune list
		QStringListModel model_;

		// Modification
		RuneDescriptor* currentDescriptor;

		// helper functions
		void fillData(QString name);
	};
}

#endif