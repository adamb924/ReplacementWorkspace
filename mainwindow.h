#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "ReplacementItem.h"

class ReplacementEngine;
class QTextEdit;
class QTableWidget;
class QPushButton;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private:
	QTextEdit *before, *after;
	QTableWidget *tableWidget;
	QPushButton *bAdd, *bRemove, *bUp, *bDown;
	QMenuBar *menu;
	QMenu *schemaMenu, *textMenu;
	QAction *openSchema, *saveSchema, *openText, *updateText, *batch, *copy;
	QAction *aAdd, *aRemove, *aUp, *aDown;

	ReplacementEngine *mRe;

	void setupUi();
	void DrawList();
	void UpdateAfter();
	void GetListFromTable();

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
