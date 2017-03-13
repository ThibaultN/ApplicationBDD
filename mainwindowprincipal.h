#ifndef MAINWINDOWPRINCIPAL_H
#define MAINWINDOWPRINCIPAL_H

#include <QMainWindow>
#include <QListWidget>
#include <QSqlQuery>
#include <QVector>

namespace Ui {
class MainWindowPrincipal;
}

class MainWindowPrincipal : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindowPrincipal(QWidget *parent = 0);
    ~MainWindowPrincipal();

    void chargerLesDatabases();
    void chargerLesTables();
    void chargerLesChamps();
    void resultatRequete();
    QVector <QString> vectHistorique;

    QString nomBase;
    QString nomTable;
    QString requete;
    QString resultatRequeteFinal;
    QSqlQuery requeteSQL;
    void on_pushButton_4_clicked();
private slots:


    void on_listWidgetTable_itemClicked(QListWidgetItem *item);

    void on_comboBoxDatabase_currentIndexChanged(const QString &arg1);

    void on_textEditRequete_textChanged();

    void on_pushButtonQuitter_clicked();

    void on_pushButtonEnregistrerRequete_clicked();

    void on_pushButtonEnregistrerHistorique_clicked();

    void on_pushButtonVerificationRequete_clicked();

    void on_pushButtonExporterCSV_clicked();

    void on_pushButtonChargerHistorique_clicked();

    void on_listWidgetRequeteHistorique_itemClicked(QListWidgetItem *item);
    
    void on_action_Quit_triggered();

private:
    Ui::MainWindowPrincipal *ui;
};

#endif // MAINWINDOWPRINCIPAL_H
