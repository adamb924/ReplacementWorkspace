#include "mainwindow.h"
#include "ReplacementItem.h"
#include "readsettings.h"

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QClipboard>

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    tableWidget = new QTableWidget;

    horizontalLayout = new QHBoxLayout;
    buttonLayout = new QHBoxLayout;
    button2Layout = new QHBoxLayout;
    verticalLayout = new QVBoxLayout;
    leftLayout = new QVBoxLayout;

    bAdd = new QPushButton("Add",this);
    bRemove = new QPushButton("Remove",this);
    buttonLayout->addWidget(bAdd);
    buttonLayout->addWidget(bRemove);

    bUp = new QPushButton("Move Up",this);
    bDown = new QPushButton("Move Down",this);
    button2Layout->addWidget(bUp);
    button2Layout->addWidget(bDown);

    leftLayout->addWidget(tableWidget);
    leftLayout->addLayout(buttonLayout);
    leftLayout->addLayout(button2Layout);

    connect(bAdd, SIGNAL(clicked()), this, SLOT(slAddRow()));
    connect(bRemove, SIGNAL(clicked()), this, SLOT(slRemoveRow()));
    connect(bUp, SIGNAL(clicked()), this, SLOT(slUp()));
    connect(bDown, SIGNAL(clicked()), this, SLOT(slDown()));

    bUp->setEnabled(false);
    bDown->setEnabled(false);

    horizontalLayout->addLayout(leftLayout,30);
    horizontalLayout->addLayout(verticalLayout,70);


    openSchema = new QAction(tr("&Open Schema"), this);
    openSchema->setStatusTip(tr("Open a schema file"));
    openSchema->setShortcut(QKeySequence("Ctrl+O"));

    saveSchema = new QAction(tr("&Save Schema"), this);
    saveSchema->setStatusTip(tr("Save this schema file"));
    saveSchema->setShortcut(QKeySequence("Ctrl+S"));

    openText = new QAction(tr("&Load a text file"), this);
    openText->setStatusTip(tr("Load a text file"));
    openText->setShortcut(QKeySequence("Ctrl+L"));

    updateText = new QAction(tr("Update result"),this);
    updateText->setStatusTip(tr("Update the text replacement"));
    updateText->setShortcut(QKeySequence("Ctrl+U"));

    schemaMenu = menuBar()->addMenu(tr("File"));
    textMenu = menuBar()->addMenu(tr("Edit"));

    menuBar()->addAction(updateText);

    bAdd->setEnabled(true);
    bRemove->setEnabled(false);
    aAdd = new QAction(tr("Add Row"), this);
    aAdd->setShortcut(QKeySequence("Ctrl++"));
    aRemove = new QAction(tr("Remove Row"), this);
    aRemove->setShortcut(QKeySequence("Ctrl+-"));
    aUp = new QAction(tr("Move Up"), this);
    aUp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    aDown = new QAction(tr("Move Down"), this);
    aDown->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    copy = new QAction(tr("Copy result to clipboard"),this);
    menuBar()->addAction(copy);
    batch = new QAction(tr("Batch Transform"),this);
    menuBar()->addAction(batch);

    schemaMenu->addAction(openSchema);
    schemaMenu->addAction(saveSchema);
    schemaMenu->addAction(openText);

    textMenu->addAction(aAdd);
    textMenu->addAction(aRemove);
    textMenu->addAction(aUp);
    textMenu->addAction(aDown);

    aUp->setEnabled(false);
    aDown->setEnabled(false);
    aAdd->setEnabled(true);
    aRemove->setEnabled(false);

    connect(tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(CurrentCellChanged(int,int,int,int)));

    connect(openSchema, SIGNAL(triggered()), this, SLOT(slOpenSchema()));
    connect(saveSchema, SIGNAL(triggered()), this, SLOT(slSaveSchema()));
    connect(openText, SIGNAL(triggered()), this, SLOT(slOpenText()));
    connect(updateText, SIGNAL(triggered()), this, SLOT(slUpdateText()));
    connect(batch, SIGNAL(triggered()), this, SLOT(batchTransform()));
    connect(copy, SIGNAL(triggered()), this, SLOT(copyResult()));

    before = new QTextEdit(this);
    after = new QTextEdit(this);
    verticalLayout->addWidget(before);
    verticalLayout->addWidget(after);

    after->setReadOnly(true);

    tableWidget->insertColumn(0);
    tableWidget->insertColumn(1);
    tableWidget->insertColumn(2);
    tableWidget->insertColumn(3);
    tableWidget->insertColumn(4);

    QStringList headers;
    headers << "RE";
    headers << "R";
    headers << "CS";
    headers << "Replace";
    headers << "With";
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setColumnWidth(0,22);
    tableWidget->setColumnWidth(1,22);
    tableWidget->setColumnWidth(2,22);
    tableWidget->setColumnWidth(3,140);
    tableWidget->setColumnWidth(4,140);
    tableWidget->setMaximumWidth(365);
    tableWidget->setMinimumWidth(365);
    tableWidget->setShowGrid(0);

    widget->setLayout(horizontalLayout);

    connect(aAdd, SIGNAL(triggered()), this, SLOT(slAddRow()));
    connect(aRemove, SIGNAL(triggered()), this, SLOT(slRemoveRow()));
    connect(aUp, SIGNAL(triggered()), this, SLOT(slUp()));
    connect(aDown, SIGNAL(triggered()), this, SLOT(slDown()));

    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setWindowTitle(tr("Replacement Workspace (written by Adam Baker; released under GPL)"));

    DrawList();
}

MainWindow::~MainWindow()
{
}

void MainWindow::CurrentCellChanged(int R, int C, int pR, int pC)
{
    if(tableWidget->currentRow()!=-1)
    {
	bUp->setEnabled(true);
	bDown->setEnabled(true);
	bRemove->setEnabled(true);
	aUp->setEnabled(true);
	aDown->setEnabled(true);
	aRemove->setEnabled(true);
    }
    else
    {
	bUp->setEnabled(false);
	bDown->setEnabled(false);
	bRemove->setEnabled(false);
	aUp->setEnabled(false);
	aDown->setEnabled(false);
	aRemove->setEnabled(false);
    }

    if(tableWidget->currentRow()==tableWidget->rowCount()-1)
    {
	bDown->setEnabled(false);
	aDown->setEnabled(false);
    }
    if(tableWidget->currentRow()==0)
    {
	bUp->setEnabled(false);
	aUp->setEnabled(false);
    }
}

void MainWindow::slSaveSchema()
{
    GetListFromTable();

    QString theXML;
    QXmlStreamWriter xmlstream(&theXML);
    xmlstream.setCodec("UTF-8");
    xmlstream.setAutoFormatting(true);
    xmlstream.writeStartDocument();
    xmlstream.writeStartElement("replacements");
    for(int i=0; i<list.length(); i++)
    {
	xmlstream.writeStartElement("item");
	xmlstream.writeAttribute("casesensitive", QString::number(list[i]->cs));
	xmlstream.writeAttribute("recursive", QString::number(list[i]->recursive));
	xmlstream.writeAttribute("regex", QString::number(list[i]->regex));
	xmlstream.writeTextElement("replace", list[i]->replace);
	xmlstream.writeTextElement("with", list[i]->with);
	xmlstream.writeEndElement(); // item
    }
    xmlstream.writeEndDocument(); // replacements


    QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"), "", tr("XML Files (*.xml)"));
    if (!fileName.isEmpty())
    {
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
	    QMessageBox::warning(this, tr("Codecs"),tr("Cannot write file %1:\n%2").arg(fileName).arg(file.errorString()));
	    return;
	}

	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << theXML;
    }
}

void MainWindow::slOpenSchema()
{
    QString fnSettings = QFileDialog::getOpenFileName(this,tr("Open Schema File"), "data", tr("XML Files (*.xml)"));
    if(fnSettings==NULL || !(fnSettings.length()>0))
    {
	return;
    }

    QFile file(fnSettings);
    QXmlInputSource inputSource(&file);

    QXmlSimpleReader reader;
    ReadSettings handler;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    reader.parse(inputSource);

    while(list.size())
    {
	list.removeAt(0);
    }
    for(int i=0; i<handler.list.size(); i++)
    {
	list << new ReplacementItem(handler.list[i]->replace,handler.list[i]->with,handler.list[i]->regex,handler.list[i]->recursive,handler.list[i]->cs);
    }
    DrawList();
}

void MainWindow::slOpenText()
{
    QString fn;
    fn = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("All Files (*.*)"));

    FILE *in = fopen(fn.toAscii(),"r");
    QTextStream *stream = new QTextStream(in,QIODevice::ReadOnly);
    QTextCodec *cod;
    cod = QTextCodec::codecForName("UTF-8");
    stream->setCodec(cod);
    stream->setAutoDetectUnicode(true);
    before->setPlainText(stream->readAll());
    slUpdateText();
    fclose(in);
}

void MainWindow::slUpdateText()
{
    GetListFromTable();
    UpdateAfter();
}

void MainWindow::slAddRow()
{
    int row = tableWidget->currentRow();
    bool re=false, r=false, cs=false;
    if(row!=-1)
    {
	re = ((QCheckBox*)tableWidget->cellWidget(row,0))->isChecked();
	r = ((QCheckBox*)tableWidget->cellWidget(row,1))->isChecked();
	cs = ((QCheckBox*)tableWidget->cellWidget(row,2))->isChecked();
    }

    row++;
    tableWidget->insertRow(row);

    tableWidget->setCellWidget (row,0,new QCheckBox);
    tableWidget->setCellWidget (row,1,new QCheckBox);
    tableWidget->setCellWidget (row,2,new QCheckBox);
    tableWidget->setCellWidget (row,3,new QLineEdit);
    tableWidget->setCellWidget (row,4,new QLineEdit);

    ((QCheckBox*)tableWidget->cellWidget(row,0))->setChecked(re);
    ((QCheckBox*)tableWidget->cellWidget(row,1))->setChecked(r);
    ((QCheckBox*)tableWidget->cellWidget(row,2))->setChecked(cs);

    //   tableWidget->cellWidget();

    tableWidget->setCurrentCell(row,0);
}

void MainWindow::slRemoveRow()
{
    int row = tableWidget->currentRow();
    if(row!=-1)
    {
	GetListFromTable();
	list.removeAt(row);
	DrawList();
	if(tableWidget->rowCount()>=row)
	{
	    tableWidget->setCurrentCell(row,0);
	}
    }
    qDebug() << tableWidget->rowCount();
}

void MainWindow::GetListFromTable()
{
    int i;

    while(list.size())
    {
	list.removeAt(0);
    }
    if(!list.isEmpty())
    {
	QMessageBox::information(this,"Information","An inexplicable error has occurred. Things will be strange from here on out.");
    }

    for(i=0; i<tableWidget->rowCount(); i++)
    {
	list << new ReplacementItem( ((QLineEdit*)tableWidget->cellWidget(i,3))->text(),((QLineEdit*)tableWidget->cellWidget(i,4))->text(),((QCheckBox*)tableWidget->cellWidget(i,0))->isChecked(),((QCheckBox*)tableWidget->cellWidget(i,1))->isChecked(),((QCheckBox*)tableWidget->cellWidget(i,2))->isChecked() );
    }
}

void MainWindow::DrawList()
{
    while(tableWidget->rowCount())
    {
	tableWidget->removeRow(0);
    }
    int i;
    for(i=0; i<list.size(); i++)
    {
	tableWidget->insertRow(i);
	tableWidget->setCellWidget (i,0,new QCheckBox);
	((QCheckBox*)tableWidget->cellWidget(i,0))->setChecked(list[i]->regex);
	tableWidget->setCellWidget (i,1,new QCheckBox);
	((QCheckBox*)tableWidget->cellWidget(i,1))->setChecked(list[i]->recursive);
	tableWidget->setCellWidget (i,2,new QCheckBox);
	((QCheckBox*)tableWidget->cellWidget(i,2))->setChecked(list[i]->cs);
	tableWidget->setCellWidget (i,3,new QLineEdit);
	((QLineEdit*)tableWidget->cellWidget(i,3))->setText(list[i]->replace);
	tableWidget->setCellWidget (i,4,new QLineEdit);
	((QLineEdit*)tableWidget->cellWidget(i,4))->setText(list[i]->with);

    }
}

void MainWindow::UpdateAfter()
{
    after->setText(doReplacements(before->toPlainText()));
}

QString MainWindow::doReplacements(QString str)
{
    QString old;
    for(int i=0; i<list.length(); i++)
    {
	if(list[i]->replace.length()==0)
	    continue;
	QString tmp = list[i]->with;
	if(list[i]->recursive)
	{
	    do
	    {
		old = str;
		if(list[i]->regex)
		{
		    tmp.replace("\\n","\n");
		    tmp.replace("\\r","\r");
		    tmp.replace("\\t","\t");
		    if(list[i]->cs)
		    {
			str.replace(QRegExp(list[i]->replace,Qt::CaseSensitive,QRegExp::RegExp2),tmp);
		    }
		    else
		    {
			str.replace(QRegExp(list[i]->replace,Qt::CaseInsensitive,QRegExp::RegExp2),tmp);
		    }
		}
		else
		{
		    tmp.replace("\n","\\n");
		    tmp.replace("\r","\\r");
		    tmp.replace("\t","\\t");
		    if(list[i]->cs)
		    {
			str.replace(list[i]->replace,list[i]->with,Qt::CaseSensitive);
		    }
		    else
		    {
			str.replace(list[i]->replace,list[i]->with,Qt::CaseInsensitive);
		    }
		}
	    }
	    while(old!=str);
	}
	else
	{
	    if(list[i]->regex)
	    {
		tmp.replace("\\n","\n");
		tmp.replace("\\r","\r");
		tmp.replace("\\t","\t");
		if(list[i]->cs)
		{
		    str.replace(QRegExp(list[i]->replace,Qt::CaseSensitive,QRegExp::RegExp2),tmp);
		}
		else
		{
		    str.replace(QRegExp(list[i]->replace,Qt::CaseInsensitive,QRegExp::RegExp2),tmp);
		}
	    }
	    else
	    {
		tmp.replace("\n","\\n");
		tmp.replace("\r","\\r");
		tmp.replace("\t","\\t");
		if(list[i]->cs)
		{
		    str.replace(list[i]->replace,list[i]->with,Qt::CaseSensitive);
		}
		else
		{
		    str.replace(list[i]->replace,list[i]->with,Qt::CaseInsensitive);
		}
	    }
	}
    }

    return str;
}

void MainWindow::slUp()
{
    int row = tableWidget->currentRow();
    if(row>0 && row < tableWidget->rowCount())
    {
	GetListFromTable();
	list.swap(row,row-1);
	DrawList();
	tableWidget->setCurrentCell(row-1,0);
    }
}

void MainWindow::slDown()
{
    int row = tableWidget->currentRow();
    if(row>-1)
    {
	if(row<tableWidget->rowCount()-1)
	{
	    GetListFromTable();
	    list.swap(row,row+1);
	    DrawList();
	    tableWidget->setCurrentCell(row+1,0);
	}
    }
}


void MainWindow::batchTransform()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList fileNames;
    if (dialog.exec())
	fileNames = dialog.selectedFiles();
    else
	return;

    QString prefix = QInputDialog::getText(this, tr("Replacement Workspace"),
					   tr("Enter a prefix for the new files:"));
    QString suffix = QInputDialog::getText(this, tr("Replacement Workspace"),
					   tr("Enter a suffix for the new files:"));
    QString extension;

    GetListFromTable();

    QString content;
    for(int i=0; i<fileNames.length(); i++)
    {
	QFile *file = new QFile(fileNames.at(i));
	if (file->open(QFile::ReadOnly))
	{
	    QTextStream *stream = new QTextStream(file);
	    content = stream->readAll();

	    content = doReplacements(content);

	    QFileInfo *info = new QFileInfo(*file);

	    if( info->suffix().length() == 0 ) { extension = ""; } else { extension = "." +  info->suffix(); }
	    QFile *outfile = new QFile(info->absolutePath() + "/" + prefix + info->completeBaseName() + suffix + extension);
	    if (outfile->open(QFile::WriteOnly))
	    {
		QTextStream *outstream = new QTextStream(outfile);
		outstream->setCodec("UTF-8");
		*(outstream) << content;
	    }
	    delete outfile;
	    delete stream;
	    delete info;
	}
	delete file;
    }
}

void MainWindow::copyResult()
{
//    qDebug() << "here";
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(after->toPlainText());
}
