#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Audio/AudioEngine.h"
#include "Graphics/GraphicsEngine.h"
#include "Network/NetworkEngine.h"
#include "Runes/RuneEngine.h"
#include "Widgets/RuneDictionary.h"
#include "Widgets/RuneManager.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QInputDialog>

using namespace Qt;

namespace Runes
{
	class Engine : public QMainWindow
	{
		Q_OBJECT

	public:
		Engine();

		// Init all modules, and make everything run
		void init();

		void run();

		void closeEvent(QCloseEvent *event) override;

	private:
		AudioEngine audio_;
		NetworkEngine network_;
		RuneEngine runeEngine_;
		GraphicsEngine graphicsEngine_;
		QGraphicsView* graphicsView_;

		//------------------
		// GUI members
		QMenu* menus_file_;
		QMenu* menus_view_;
		RuneDictionary* runeDictionary_;
		RuneManager* runeManager_;
		QDockWidget* dictionaryWidget_;

		// Actions
		QAction* openSpellAction_;
		QAction* saveSpellAction_;
		QAction* newSpellAction_;
		QAction* discoverRuneAction_;
		QAction* toggleTextImageAction_;
		QAction* showDictionaryAction_;
		QAction* showManagerAction_;

		//------------------
		// Private functions

		// Init GUI
		//! @brief Create the actions
		void createActions();

		//! @brief Create the main menus
		void createMenus();

		//! @brief Create toolbars
		void createToolbars();

		//! @brief Creates dock widgets
		void createDockWidgets();

		
	};
}


#endif //_ENGINE_H_