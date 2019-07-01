
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "ImageWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *pParent = nullptr);
	virtual ~MainWindow(void) override;

protected:
	virtual void closeEvent(QCloseEvent *pEvent) override;

private:
	void setupUi(void);
	void setupActions(void);
	void setupMenuBar(void);
	void setupToolBar(void);
	void setupFoldersView(QSplitter *pSplitter);
	void setupImageWidget(QSplitter *pSplitter);
	void setupStatusBar(void);

private slots:
	void onOpenFolderActionTriggered(void);
	void onFoldersViewCurrentRowChanged(const QModelIndex& index);
	void onPreviousActionTriggered(void);
	void onNextActionTriggered(void);
	void onAboutQtActionTriggered(void);
	void onAboutAppActionTriggered(void);
	
private:
	QMenuBar *m_pMenuBar;
	QToolBar *m_pToolBar;
	QTreeView *m_pFoldersView;
	ImageWidget *m_pImageWidget;
	QStatusBar *m_pStatusBar;
	QSplitter *m_pSplitter;
	
	QFileSystemModel *m_pFoldersModel;
	
	// Actions
	QAction *m_pOpenFolderAction;
	QAction *m_pExitAction;
	QAction *m_pPreviousAction;
	QAction *m_pNextAction;
	QAction *m_pAboutQtAction;
	QAction *m_pAboutAppAction;
	
	// Menus
	QMenu *m_pFileMenu;
	QMenu *m_pHelpMenu;
	
	const QString m_defaultFolder = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
};


#endif // MAINWINDOW_H
