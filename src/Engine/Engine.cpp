#include "Engine.h"

using namespace Qt;

namespace Runes
{
	Engine::Engine() : runeEngine_(), graphicsEngine_(runeEngine_)
	{
		
	}

	void Engine::init()
	{
		// User authentication
		QString name;

		do
		{
			bool ok;
			name = QInputDialog::getText(this, tr("Authentication"),
				tr("Enter your username:"), QLineEdit::Normal,
				"username", &ok);

			if (!ok)
				this->close();
		} while (!runeEngine_.isUserValid(name));

		runeEngine_.init(name);
		network_.init();

		graphicsEngine_.init();
		graphicsView_ = graphicsEngine_.getView();

		graphicsView_->scale(0.5, 0.5);
		//graphicsView_->scale(0.25, 0.25);
		graphicsView_->show();

		this->setCentralWidget(graphicsView_);

		connect(&runeEngine_, SIGNAL(redrawNeeded()), &graphicsEngine_, SLOT(redrawSpell()));

		this->createMenus();
		this->createToolbars();
		this->createDockWidgets();
		this->createActions();

		this->showMaximized();
	}

	void Engine::run()
	{

	}

	void Engine::closeEvent(QCloseEvent *event)
	{
		runeEngine_.checkSave(); // saving everything if there were changes
		QMainWindow::closeEvent(event);
	}

	void Engine::createActions()
	{
		//newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
		openSpellAction_ = new QAction("Open spell", this);
		openSpellAction_->setShortcut(QKeySequence::Open);
		connect(openSpellAction_, SIGNAL(triggered()), &runeEngine_, SLOT(loadSpellFromFile()));

		saveSpellAction_ = new QAction("Save spell", this);
		saveSpellAction_->setShortcut(QKeySequence::Save);
		connect(saveSpellAction_, SIGNAL(triggered()), &runeEngine_, SLOT(saveCurrentSpell()));

		newSpellAction_ = new QAction("New spell", this);
		newSpellAction_->setShortcut(QKeySequence::New);
		connect(newSpellAction_, SIGNAL(triggered()), &runeEngine_, SLOT(newSpell()));

		discoverRuneAction_ = new QAction("Discover Rune", this);
		connect(discoverRuneAction_, SIGNAL(triggered()), &runeEngine_, SLOT(discoverRune()));

		toggleTextImageAction_ = new QAction("Toggle Text/Image", this);
		connect(toggleTextImageAction_, SIGNAL(triggered()), &graphicsEngine_, SLOT(toggleText()));

		menus_file_->addAction(newSpellAction_);
		menus_file_->addAction(openSpellAction_);
		menus_file_->addAction(saveSpellAction_);
		menus_file_->addAction(discoverRuneAction_);
		
		if (runeEngine_.isMasterMode())
		{
			showManagerAction_ = new QAction("Show Rune Manager", this);
			connect(showManagerAction_, SIGNAL(triggered()), runeManager_, SLOT(show()));
			menus_view_->addAction(showManagerAction_);
		}
		else
		{
			showDictionaryAction_ = new QAction("Show Rune Dictionary", this);
			connect(showDictionaryAction_, SIGNAL(triggered()), runeDictionary_, SLOT(show()));
			menus_view_->addAction(showDictionaryAction_);
		}

	
	}

	void Engine::createMenus()
	{
		menus_file_ = this->menuBar()->addMenu(tr("&File"));
		menus_view_ = this->menuBar()->addMenu(tr("&View"));
	}

	void Engine::createToolbars()
	{
		/*fileToolBar = addToolBar(tr("File"));
		fileToolBar->addAction(newAct);*/
	}

	void Engine::createDockWidgets()
	{
		// Dictionary / master widget
		if (runeEngine_.isMasterMode())
		{
			runeManager_ = new RuneManager(runeEngine_, graphicsEngine_.getSprites());

			dictionaryWidget_ = new QDockWidget("Rune manager", this);
			dictionaryWidget_->setAllowedAreas(Qt::DockWidgetAreas(Qt::DockWidgetArea::TopDockWidgetArea || Qt::DockWidgetArea::BottomDockWidgetArea));
			dictionaryWidget_->setWidget(runeManager_);
			addDockWidget(Qt::TopDockWidgetArea, dictionaryWidget_);

			QObject::connect(runeManager_->getReloadButton(), SIGNAL(clicked()),
				&graphicsEngine_, SLOT(reloadSprites()));
		}
		else
		{
			// No master mode, display the normal dictionary for the player
			runeDictionary_ = new RuneDictionary(runeEngine_, graphicsEngine_.getSprites());

			dictionaryWidget_ = new QDockWidget("Rune dictionary", this);
			dictionaryWidget_->setAllowedAreas(Qt::DockWidgetAreas(Qt::DockWidgetArea::TopDockWidgetArea || Qt::DockWidgetArea::BottomDockWidgetArea));
			dictionaryWidget_->setWidget(runeDictionary_);
			addDockWidget(Qt::TopDockWidgetArea, dictionaryWidget_);
		}
	}



}

