#include "mainwindowprincipal.h"
#include "ui_mainwindowprincipal.h"
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>

MainWindowPrincipal::MainWindowPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowPrincipal)
{
    ui->setupUi(this);

    ui->textEditErreurRequete->hide();

    chargerLesDatabases();
}

MainWindowPrincipal::~MainWindowPrincipal()
{
    delete ui;
}

/**
 * @brief MainWindowPrincipal::on_pushButton_4_clicked
 */
void MainWindowPrincipal::on_pushButton_4_clicked()
{
    this->close();
}

// ------------------- Database -------------------
/**
 * @brief MainWindowPrincipal::chargerLesDatabases
 * On charge les databases via requete SQL
 */
void MainWindowPrincipal::chargerLesDatabases()
{
    //Création de la requete pour récupérer les données
    QString requeteSQLDatabase = "show databases";
    if(requeteSQL.exec(requeteSQLDatabase))
    {
        while(requeteSQL.next())
        {
            QStringList uneDatabase;
            uneDatabase.push_back(requeteSQL.value(0).toString());
            ui->comboBoxDatabase->addItems(uneDatabase);
        }
   }
}

/**
 * @brief MainWindowPrincipal::on_comboBoxDatabase_currentIndexChanged
 * @param arg1
 * Affichage les tables lorsqu'on sélectionne une database depuis la combobox
 * Et réiniatilise l'affichage à chaque changement au niveau des tables et champs
 */
void MainWindowPrincipal::on_comboBoxDatabase_currentIndexChanged(const QString &arg1)
{
    ui->listWidgetTable->clear();
    ui->listWidgetChamps->clear();
    QSqlQuery req;
    QString reqText="use "+arg1+"";
    if(req.exec(reqText))
    {
       chargerLesTables();
    }
}


// ------------------- Tables -------------------
/**
 * @brief MainWindowPrincipal::on_listWidgetTable_itemClicked
 * @param item
 * Lorsqu'on sélectionne une table affiche les champs contenus
 */
void MainWindowPrincipal::on_listWidgetTable_itemClicked(QListWidgetItem *item)
{
    ui->listWidgetChamps->clear();
    nomTable=item->text();
    chargerLesChamps();
}

/**
 * @brief MainWindowPrincipal::chargerLesTables
 * Charge les tables via requete SQL
 */
void MainWindowPrincipal::chargerLesTables()
{
    //Création de la requete pour recuperer les tables
    QString requeteSQLTables = "show tables";
    QSqlQuery requeteSQL;
    if(requeteSQL.exec(requeteSQLTables))
    {
        while(requeteSQL.next())
        {
            ui->listWidgetTable->addItem(requeteSQL.value(0).toString());
        }
    }
}

// ------------------- Champs -------------------
/**
 * @brief MainWindowPrincipal::chargerLesChamps
 * Charge les champs via requete SQL
 */
void MainWindowPrincipal::chargerLesChamps()
{
    //Création de la requete pour recuperer les champs
    QString requeteSQLChamps = "show fields from "+nomTable+"";
    if(requeteSQL.exec(requeteSQLChamps))
    {
        while(requeteSQL.next())
        {
            ui->listWidgetChamps->addItem(requeteSQL.value(0).toString());
        }
    }
}

// ------------------- Requete -------------------
/**
 * @brief MainWindowPrincipal::on_textEditRequete_textChanged
 * Lorsqu'on ecris notre requete on la stock dans une QString qui est execute par la methode
 * resultatRequete
 */
void MainWindowPrincipal::on_textEditRequete_textChanged()
{
    QString texteRequete=ui->textEditRequete->toPlainText();
    resultatRequeteFinal=""+texteRequete+"";

    resultatRequete();

    //Si la zone d'ecriture de la requete est vide on nettoie la table widget
    if(texteRequete.isEmpty())
    {
        ui->tableWidgetResultatRequete->clear();
        ui->tableWidgetResultatRequete->setRowCount(0);
        ui->tableWidgetResultatRequete->setColumnCount(0);
    }
}

/**
 * @brief MainWindowPrincipal::on_pushButtonVerificationRequete_clicked
 * Indique les erreurs lors du clique sur le bouton "?" si la requete est invalide
 */
void MainWindowPrincipal::on_pushButtonVerificationRequete_clicked()
{
    //On recupere l'erreur
    if(requeteSQL.lastError().isValid())
    {
        //Et l'affiche dans le textEditerreurRequete
        ui->textEditErreurRequete->show();
        ui->textEditErreurRequete->setText(requeteSQL.lastError().text());
    }
}

/**
 * @brief MainWindowPrincipal::on_pushButtonQuitter_clicked
 */
void MainWindowPrincipal::on_pushButtonQuitter_clicked()
{
    this->close();
}

/**
 * @brief MainWindowPrincipal::resultatRequete
 * Verifie la requete est l'execute si elle est exacte
 * Sinon le bouton "?" s'affiche en rouge indiquant une erreur dans la requete
 */
void MainWindowPrincipal::resultatRequete()
{

    int noLigne=0;
    int noColonne=0;

    //Si la requete est exacte le bouton appaitra en vert
    if(requeteSQL.exec(resultatRequeteFinal))
    {
        noLigne=requeteSQL.size();
        QSqlRecord req=requeteSQL.record();
        noColonne=req.count();

        //Le bouton d'erreur de requete est desactiver
        ui->pushButtonVerificationRequete->setDisabled(true);
        //Et le bouton apparait en vert
        ui->pushButtonVerificationRequete->setStyleSheet("background-color:green");
        //Si la requete est bonne et que le textEditErreurRequete est afficher on le cache
        ui->textEditErreurRequete->hide();

        ui->tableWidgetResultatRequete->setRowCount(noLigne);
        ui->tableWidgetResultatRequete->setColumnCount(noColonne);
        requeteSQL.first();

        for(int cpt=0; cpt<noLigne; cpt++)
        {
            for(int cpt2=0; cpt2<noColonne; cpt2++)
            {
               ui->tableWidgetResultatRequete->setItem(cpt,cpt2,new QTableWidgetItem(requeteSQL.value(cpt2).toString()));
            }
            requeteSQL.next();
        }
    }
    //Si la requete est fausse le bouton apparaitra en rouge
    else
    {
        ui->pushButtonVerificationRequete->setDisabled(false);
        ui->pushButtonVerificationRequete->setStyleSheet("background-color:red");

        //On fais en sorte que l'application efface le/les resultats si la requete change et qu'elle est fausse
        ui->tableWidgetResultatRequete->clear();
        //Et on efface les lignes/colonnes du tableau restant
        ui->tableWidgetResultatRequete->setRowCount(0);
        ui->tableWidgetResultatRequete->setColumnCount(0);
    }
}

// ------------------- Historisation -------------------
/**
 * @brief MainWindowPrincipal::on_pushButtonEnregistrerRequete_clicked
 * Enregistrement d'une requete dans l'historique
 */
void MainWindowPrincipal::on_pushButtonEnregistrerRequete_clicked()
{
    QString texteRequete = ui->textEditRequete->toPlainText();
    ui->listWidgetRequeteHistorique->addItem(texteRequete);
    vectHistorique.push_back(texteRequete);
}

/**
 * @brief MainWindowPrincipal::on_pushButtonEnregistrerHistorique_clicked
 * Sauvegarde de l'historique
 */
void MainWindowPrincipal::on_pushButtonEnregistrerHistorique_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Open Files"), "/home/tnour1/.sql", tr("SQL Files (*.sql)"));
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    int nbOccurence=vectHistorique.size();
    for (int i=0; i<nbOccurence; i++)
    {
        out << vectHistorique[i] << "\n";
    }
}

/**
 * @brief MainWindowPrincipal::on_pushButtonChargerHistorique_clicked
 * Ouverture d'un historique
 */
void MainWindowPrincipal::on_pushButtonChargerHistorique_clicked()
{
    ui->listWidgetRequeteHistorique->clear();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Files"), "/home/", tr("SQL Files (*.sql)"));

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream monHistorique(&file);
    while(!monHistorique.atEnd())
    {
        QString ligne = monHistorique.readLine();
        ui->listWidgetRequeteHistorique->addItem(ligne);
    }

}

/**
 * @brief MainWindowPrincipal::on_listWidgetRequeteHistorique_itemClicked
 * @param item
 * Lorsqu'on clique sur un des items qui est une requete on l'execute
 */
void MainWindowPrincipal::on_listWidgetRequeteHistorique_itemClicked(QListWidgetItem *item)
{
    resultatRequeteFinal=item->text();
    qDebug() << resultatRequeteFinal;
    resultatRequete();
}

// ------------------- Exportation en CSV -------------------
/**
 * @brief MainWindowPrincipal::on_pushButtonExporterCSV_clicked
 * Exporte les résultats afficher au moment
 */
void MainWindowPrincipal::on_pushButtonExporterCSV_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "MainWindowPrincipalTitle", "filename.csv", "CSV files (.csv);;Zip files (.zip, *.7z)", 0, 0);
    QFile f(filename);
    if(f.open(QFile::WriteOnly |QFile::Truncate))
    {
        QTextStream data( &f);
        QStringList list;
        for(int i=0; i<ui->tableWidgetResultatRequete->rowCount();i++)
        {
            list.clear();
            for(int a=0; a<ui->tableWidgetResultatRequete->columnCount();a++)
            {
                list << "\" "+ui->tableWidgetResultatRequete->item(i,a)->text()+"\" ";
            }
            data<<list.join(";")+"\n";
        }
        f.close();
    }
}

// --------------------------------------
void MainWindowPrincipal::on_action_Quit_triggered()
{
    this->close();
}
