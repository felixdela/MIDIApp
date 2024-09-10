#include "newinstrumentosdialog.h"
#include "ui_newinstrumentosdialog.h"

NewInstrumentosDialog::NewInstrumentosDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewInstrumentosDialog)
{
    ui->setupUi(this);
}

NewInstrumentosDialog::~NewInstrumentosDialog()
{
    delete ui;
}

QString NewInstrumentosDialog::getID()
{
    return ui->idNewInstLineEdit->text().trimmed();
}
