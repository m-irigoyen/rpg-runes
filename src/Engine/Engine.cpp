#include "Engine.h"

using namespace Qt;

namespace Runes
{
	Engine::Engine()
	{
	}

	void Engine::init()
	{
		this->createMenus();
		this->createToolbars();
		this->createDockWidgets();

		this->runes.init();
		this->network.init();
		this->audio.init();
		this->graphics.init();
	}

	void Engine::run()
	{

	}

	void Engine::createActions()
	{
		//newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	}

	void Engine::createMenus()
	{
		menus_file_ = this->menuBar()->addMenu(tr("&File"));
		/*menus_file_->addAction(newAct);
		menus_file_->addAction(openAct);
		menus_file_->addAction(saveAct);*/
	}

	void Engine::createToolbars()
	{
		/*fileToolBar = addToolBar(tr("File"));
		fileToolBar->addAction(newAct);*/
	}

	void Engine::createDockWidgets()
	{
		/*QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
		dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
			Qt::RightDockWidgetArea);
		dockWidget->setWidget(dockWidgetContents);
		addDockWidget(Qt::LeftDockWidgetArea, dockWidget);*/
	}
}

