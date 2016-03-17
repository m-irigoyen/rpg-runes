#include "Engine.h"

using namespace Qt;

namespace Runes
{
	Engine::Engine() : runeEngine_(), graphicsEngine_(runeEngine_), masterMode_(false)
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
		} while (!isUserValid(name));

		//runeEngine_.init(name);
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

	void Engine::closeEvent(QCloseEvent *event)
	{
		runeEngine_.clear(); // saving everything if there were changes
		QMainWindow::closeEvent(event);
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
		// Dictionnary / master widget
		if (masterMode_)
		{
			runeManager_ = new RuneManager(runeEngine_, graphicsEngine_.getSprites());

			dictionnaryWidget_ = new QDockWidget("Rune manager", this);
			dictionnaryWidget_->setAllowedAreas(Qt::DockWidgetAreas(Qt::DockWidgetArea::TopDockWidgetArea || Qt::DockWidgetArea::BottomDockWidgetArea));
			dictionnaryWidget_->setWidget(runeManager_);
			addDockWidget(Qt::TopDockWidgetArea, dictionnaryWidget_);
		}
		else
		{
			// No master mode, display the normal dictionnary for the player
			runeDictionnary_ = new RuneDictionnary(runeEngine_, graphicsEngine_.getSprites());

			dictionnaryWidget_ = new QDockWidget("Rune dictionnary", this);
			dictionnaryWidget_->setAllowedAreas(Qt::DockWidgetAreas(Qt::DockWidgetArea::TopDockWidgetArea || Qt::DockWidgetArea::BottomDockWidgetArea));
			dictionnaryWidget_->setWidget(runeDictionnary_);
			addDockWidget(Qt::TopDockWidgetArea, dictionnaryWidget_);
		}
	}

	bool Engine::isUserValid(QString name)
	{
		if (name.compare(MASTER_NAME))
		{
			masterMode_ = true;
			return true;
		}

		bool result = false;
		QXmlStreamReader stream;
		QFile file;

		if (Serializable::openFile("runes", Paths::RUNES, file))
		{
			Serializable::initReader(&file, stream);
			stream.readNextStartElement();
			if (stream.isStartElement() && stream.name() == "runes")
			{
				result = true;
				file.close();
			}
		}
		return result;
	}

}

