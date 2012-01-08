#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QTableView >
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QUrl>
#include <QIODevice>
#include <QTextStream>
#include <QTextCodec>
#include <QXmlStreamWriter>

#include "ReplacementItem.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
//	MainWindow(QWidget *parent = 0);
	MainWindow();
	~MainWindow();

private:
	QHBoxLayout *horizontalLayout, *buttonLayout, *button2Layout;
	QPushButton *bAdd, *bRemove, *bUp, *bDown;
	QVBoxLayout *verticalLayout, *leftLayout;
	QTextEdit *before, *after;
	QTableWidget *tableWidget;
	QMenuBar *menu;
	QMenu *schemaMenu, *textMenu;
	QAction *openSchema, *saveSchema, *openText, *updateText, *batch, *copy;
	QAction *aAdd, *aRemove, *aUp, *aDown;
	QList<ReplacementItem*> list;

	void DrawList();
	void UpdateAfter();
	void GetListFromTable();
	QString doReplacements(QString str);

private slots:
	void slUp();
	void slDown();
	void slOpenSchema();
	void slSaveSchema();
	void slOpenText();
	void slUpdateText();
	void slAddRow();
	void slRemoveRow();
	void CurrentCellChanged(int R, int C, int pR, int pC);
	void batchTransform();
	void copyResult();
};

#endif // MAINWINDOW_H
