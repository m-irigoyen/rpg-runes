#include "Engine.h"

using namespace Qt;

namespace Runes
{
	Engine::Engine() : runeEngine_(), graphicsEngine_(runeEngine_)
	{
		
	}

	void Engine::init()
	{
		//runeEngine_.init("Edrevan");
		runeEngine_.testInit();
		network_.init();

		this->createMenus();
		this->createToolbars();
		this->createDockWidgets();

		graphicsEngine_.init();
		graphicsView_ = graphicsEngine_.getView();

		this->setCentralWidget(graphicsView_);

		//this->showMaximized();

		graphicsView_->scale(0.35, 0.35);
		graphicsView_->show();
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

