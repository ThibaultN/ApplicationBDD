#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QTextCodec>
#include "mainwindowprincipal.h"
#include "dialogconnect.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
            a.installTranslator(&qtTranslator);
            QTranslator myappTranslator;
            QString nomFichier="applicationBDD_" + QLocale::system().name()+".qm";
            qDebug()<<nomFichier;
            QString baseName;

    #ifdef Q_OS_LINUX
            baseName="/usr/share/applications/ApplicationBDD/translations/";
    #endif
    #ifdef Q_OS_MACOS
            baseName=QApplication::applicationDirPath()+QLatin1String("/../Resources/translations/"); // path defaults to app dir.
    #endif
    #ifdef Q_OS_DARWIN
           baseName=QApplication::applicationDirPath()+QLatin1String("/translations/"); // path defaults to app dir.
    #endif
    #ifdef Q_OS_WIN
            baseName=QApplication::applicationDirPath();
    #endif
            myappTranslator.load(baseName+nomFichier);
            myappTranslator.load("/usr/share/applications/ApplicationBDD/translations/application_BDD_" + QLocale::system().name()+".qm");
            a.installTranslator(&myappTranslator);
            qDebug()<<QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    DialogConnect connect;
    while(connect.exec()==QDialog::Accepted)
    {
        QString login = connect.getId();
        QString mdp = connect.getMdp();
        QString IP = connect.getIP();
        QSqlDatabase maBase = QSqlDatabase::addDatabase("QMYSQL");
        maBase.setHostName(IP);
        maBase.setUserName(login);
        maBase.setPassword(mdp);
        qDebug() << login;
        qDebug() << mdp;
        if(maBase.open())
        {
            MainWindowPrincipal dialogPrincipale;
            dialogPrincipale.show();
            return a.exec();
        }
        else
        {
            connect.setMessage();
        }
    }
}
