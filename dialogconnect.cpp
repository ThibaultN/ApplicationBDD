#include "dialogconnect.h"
#include "ui_dialogconnect.h"

DialogConnect::DialogConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnect)
{
    ui->setupUi(this);
    ui->lineEditIPConnexion->setText("localhost");
}

DialogConnect::~DialogConnect()
{
    delete ui;
}

QString DialogConnect::getId()
{
    return ui->lineEditLoginConnexion->text();
}

QString DialogConnect::getMdp()
{
    return ui->lineEditMdpConnexion->text();
}

QString DialogConnect::getIP()
{
    return ui->lineEditIPConnexion->text();
}

void DialogConnect::setMessage()
{
    ui->labelConnectionErreur->setText("Erreurs dans vos identifiants");
}
