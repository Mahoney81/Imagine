#include "globals.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *pParent) : QMainWindow(pParent)
{
	setupUi();
	
	const QString folder = QSettings().value("LastFolder", m_defaultFolder).toString();
	const QModelIndex index = m_pFoldersModel->setRootPath(folder);
	m_pFoldersView->setRootIndex(index);
	
	setWindowTitle(QApplication::applicationDisplayName());
}

MainWindow::~MainWindow(void)
{
	QSettings().setValue("LastFolder", m_pFoldersModel->rootPath());
	QSettings().setValue("MainWindow/Size", size());
	QSettings().setValue("MainWindow/SplitterState", m_pSplitter->saveState());
	QSettings().setValue("MainWindow/SplitterGeometry", m_pSplitter->saveGeometry());
}

void MainWindow::setupUi(void)
{
	setupActions();
	
	QVBoxLayout *pMainLayout = new QVBoxLayout();
	pMainLayout->setMargin(4);
	
	setupMenuBar();
	setupToolBar();
	
	// Splitter
	m_pSplitter = new QSplitter(Qt::Horizontal, this);
	setupFoldersView(m_pSplitter);
	setupImageWidget(m_pSplitter);
	pMainLayout->addWidget(m_pSplitter);
	const QByteArray splitterState = QSettings().value("MainWindow/SplitterState").toByteArray();
	m_pSplitter->restoreState(splitterState);
	
	const QByteArray splitterGeometry = QSettings().value("MainWindow/SplitterGeometry").toByteArray();
	m_pSplitter->restoreGeometry(splitterGeometry);
	if(splitterGeometry.isEmpty())
		m_pSplitter->setSizes({200, 400});
	
	setupStatusBar();
	
	setCentralWidget(new QWidget(this));
	centralWidget()->setLayout(pMainLayout);
	
	// Sizing matters
	QSize defaultSize(1440, 880);
	if(QDesktopWidget().width() < defaultSize.width())
		defaultSize.setWidth(QDesktopWidget().width());
	if(QDesktopWidget().height() < defaultSize.height())
		defaultSize.setHeight(QDesktopWidget().height() - 20);
	QSize windowSize = QSettings().value("MainWindow/Size", defaultSize).toSize();
	resize(windowSize);
	
	// Just for Mac
	setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::setupActions(void)
{
	m_pOpenFolderAction = new QAction(QIcon(":/icons/open-folder-icon"), tr("Open Folder..."), this);
	m_pOpenFolderAction->setStatusTip(tr("Choose root folder for browser"));
	connect(m_pOpenFolderAction, &QAction::triggered, this, &MainWindow::onOpenFolderActionTriggered);
	
	m_pExitAction = new QAction(tr("Exit"), this);
	connect(m_pExitAction, &QAction::triggered, qApp, &QApplication::quit);
	
	m_pPreviousAction = new QAction(QIcon(":/icons/previous-icon"), tr("Previous"), this);
	connect(m_pPreviousAction, &QAction::triggered, this, &MainWindow::onPreviousActionTriggered);
	
	m_pNextAction = new QAction(QIcon(":/icons/next-icon"), tr("Next"), this);
	connect(m_pNextAction, &QAction::triggered, this, &MainWindow::onNextActionTriggered);
	
	m_pAboutQtAction = new QAction(tr("About Qt..."), this);
	connect(m_pAboutQtAction, &QAction::triggered, this, &MainWindow::onAboutQtActionTriggered);
	
	m_pAboutAppAction = new QAction(tr("About NixImage..."), this);
	connect(m_pAboutAppAction, &QAction::triggered, this, &MainWindow::onAboutAppActionTriggered);
}

void MainWindow::setupMenuBar(void)
{
	m_pFileMenu = new QMenu(tr("&File"), this);
	m_pFileMenu->addAction(m_pOpenFolderAction);
	m_pFileMenu->addSeparator();
	m_pFileMenu->addAction(m_pExitAction);
	
	m_pHelpMenu = new QMenu(tr("&Help"), this);
	m_pHelpMenu->addAction(m_pAboutQtAction);
	m_pHelpMenu->addAction(m_pAboutAppAction);
	
	m_pMenuBar = new QMenuBar(this);
	m_pMenuBar->addMenu(m_pFileMenu);
	m_pMenuBar->addMenu(m_pHelpMenu);
	
	setMenuBar(m_pMenuBar);
}

void MainWindow::setupToolBar(void)
{
	m_pToolBar = new QToolBar(tr("Toolbar"), this);
	m_pToolBar->addAction(m_pOpenFolderAction);
	m_pToolBar->addSeparator();
	m_pToolBar->addAction(m_pPreviousAction);
	m_pToolBar->addAction(m_pNextAction);
	
	addToolBar(m_pToolBar);
}

void MainWindow::setupFoldersView(QSplitter *pSplitter)
{
	m_pFoldersModel = new QFileSystemModel(this);
	m_pFoldersModel->setNameFilters({ "*.jpg", "*.jpeg", "*.bmp", "*.png", "*.tif", "*.tiff", "*.gif" });
	m_pFoldersModel->setNameFilterDisables(false);
	
	m_pFoldersView = new QTreeView(this);
	m_pFoldersView->setModel(m_pFoldersModel);
	m_pFoldersView->hideColumn(1);
	m_pFoldersView->hideColumn(2);
	m_pFoldersView->hideColumn(3);
	m_pFoldersView->header()->hide();
	m_pFoldersView->setExpandsOnDoubleClick(false);
	m_pFoldersView->sortByColumn(0);
	connect(m_pFoldersView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::onFoldersViewCurrentRowChanged);
	
	pSplitter->addWidget(m_pFoldersView);
	
	auto onDirectoryLoaded = [this](const QString& path)
	{
		const QModelIndex rootIndex = m_pFoldersModel->index(path);
		const int count = m_pFoldersModel->rowCount(rootIndex);
		for(int i = 0; i < count; i++)
		{
			const QModelIndex index = m_pFoldersModel->index(i, 0, rootIndex);
			if(!index.isValid())
				break;
			const QFileInfo fi = m_pFoldersModel->fileInfo(index);
			if(fi.isFile())
			{
				m_pFoldersView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
				break;
			}
		}
	};
	
	connect(m_pFoldersModel, &QFileSystemModel::directoryLoaded, this, onDirectoryLoaded);
}

void MainWindow::setupImageWidget(QSplitter *pSplitter)
{
	m_pImageWidget = new ImageWidget(this);
	
	pSplitter->addWidget(m_pImageWidget);
}

void MainWindow::setupStatusBar(void)
{
	m_pStatusBar = new QStatusBar(this);
	
	setStatusBar(m_pStatusBar);
}

void MainWindow::onOpenFolderActionTriggered(void)
{
	const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Media Folder"), m_defaultFolder);
	if(!folderPath.isEmpty())
	{
		const QModelIndex index = m_pFoldersModel->setRootPath(folderPath);
		m_pFoldersView->setRootIndex(index);
	}
}

void MainWindow::onFoldersViewCurrentRowChanged(const QModelIndex& index)
{
	const QFileInfo fi = m_pFoldersModel->fileInfo(index);
	m_pImageWidget->setImagesFolder(fi.absoluteFilePath());
}

void MainWindow::onAboutQtActionTriggered(void)
{
	QMessageBox::aboutQt(this);
}

void MainWindow::onAboutAppActionTriggered(void)
{
	QMessageBox::about(this, tr("About ") + QApplication::applicationName(),
		tr("<b>%1</b><br>Version %2<br><br>Copyright © 2019 by %3. All rights reserved!")
		    .arg(QApplication::applicationName())
		    .arg(QApplication::applicationVersion())
		    .arg(QApplication::organizationName()));
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
	QWidget::closeEvent(pEvent);
}

void MainWindow::onNextActionTriggered(void)
{

}

void MainWindow::onPreviousActionTriggered(void)
{

}
