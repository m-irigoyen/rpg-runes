#ifndef _ENGINE_WIDGETS_RUNEDICTIONNARY_H_
#define _ENGINE_WIDGETS_RUNEDICTIONNARY_H_

#include <vector>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Engine/Runes/RuneEngine.h"

namespace Runes
{
	class RuneDictionnary : public QWidget
	{
	public:
		RuneDictionnary(RuneEngine& engine, std::vector<QPixmap>& runeSprites);

		// Model management
		//! @brief inits model from the list of userRunes
		void init();

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
		QStandardItemModel model_;
		vector<QStandardItem*> items_;
	};
}

#endif