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

		graphicsEngine_.init();
		graphicsView_ = graphicsEngine_.getView();

		graphicsView_->scale(0.25, 0.25);
		graphicsView_->show();

		this->setCentralWidget(graphicsView_);

		this->createMenus();
		this->createToolbars();
		this->createDockWidgets();

		// this->showMaximized();
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
		// Dictionnary widget
		runeDictionnary_ = new RuneDictionnary(runeEngine_, graphicsEngine_.getSprites());

		dictionnaryWidget_ = new QDockWidget("Rune dictionnary", this);
		dictionnaryWidget_->setAllowedAreas(Qt::DockWidgetAreas(Qt::DockWidgetArea::TopDockWidgetArea || Qt::DockWidgetArea::BottomDockWidgetArea));
		dictionnaryWidget_->setWidget(runeDictionnary_);
		addDockWidget(Qt::TopDockWidgetArea, dictionnaryWidget_);
	}
}

