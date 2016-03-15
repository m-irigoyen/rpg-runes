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
	class RuneDictionnary : public QWidget
	{
		Q_OBJECT
	public:
		RuneDictionnary(RuneEngine& engine, std::vector<QPixmap>& runeSprites);

		// Model management
		//! @brief inits model from the list of userRunes
		void init();

	public slots:

		void clicked(QModelIndex index);
		void editedName(QString& text);
		void editedDescription(QString& text);

	signals:
		void userRunesUpdated();

	protected:
		// References
		RuneEngine& runeEngine_;
		RunesContainer& globalRunes_;
		UserRunesContainer& userRunes_;
		std::vector<QPixmap>& runeSprites_;

		// Layout
		QHBoxLayout mainLayout_;		//!< Main : two parts. Left is the list of runes, right is the display / edit
		QHBoxLayout rightLayout_;		//!< Right : two parts. Left is the image, right the name/desc
		QVBoxLayout rightRightLayout_;	//!< Right right : two parts. Top is the name, bottom the description

		// Widgets
		QListView view_;
		QLabel image_;
		QLineEdit name_;
		QTextEdit description_;

		// Rune list
		QStringListModel model_;

		// Modification
		RuneDescriptor* currentDescriptor;

		// helper functions
		void fillData(QString name);
	};
}

#endif