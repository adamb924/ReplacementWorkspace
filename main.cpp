#include <QtCore/QCoreApplication>
#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ReplacementEngine.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    if( argc == 2 && strcmp(argv[1], "--help") == 0 )
    {
        QCoreApplication a(argc, argv);
        // this doesn't work
        printf("Command line usage:\n\nReplacementWorkspace my-replacements.xml input.txt output.txt\n");
        return a.exec();
    }
    else if( argc == 4 ) {
        // this does nothing and it hangs
        QCoreApplication a(argc, argv);

        QString transformationFile( argv[1] );
    	QString sourceFile( argv[2] );
    	QString destFile( argv[3] );

    	ReplacementEngine re;
    	re.readFromFile(transformationFile);
    	re.processFile(sourceFile, destFile);

        return a.exec();
    }
    else
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
