#include "newbancodialog.h"
#include "ui_newbancodialog.h"

NewBancoDialog::NewBancoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewBancoDialog)
{
    ui->setupUi(this);
}

NewBancoDialog::~NewBancoDialog()
{
    delete ui;
}

QString NewBancoDialog::getNumBanco()
{
    return ui->lineEdit_banco->text().trimmed();
}

int NewBancoDialog::getNumProgramas()
{
    return ui->spinBox_programas->value();
}
