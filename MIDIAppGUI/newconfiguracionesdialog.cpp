#include "newconfiguracionesdialog.h"

#include "ui_newconfiguracionesdialog.h"

NewConfiguracionesDialog::NewConfiguracionesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewConfiguracionesDialog)

{
    ui->setupUi(this);;
}

NewConfiguracionesDialog::~NewConfiguracionesDialog()
{
    delete ui;
}

QString NewConfiguracionesDialog::getID()
{
    return ui->lineEditId->text().trimmed();
}

