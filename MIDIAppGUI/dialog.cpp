#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getNombre() const
{
    return ui->lineEditNombre->text();
}

int Dialog::getCanalMIDI() const
{
    return ui->spinBoxCanalMIDI->value();
}

bool Dialog::es127() const
{
    return ui->radioButton127->isChecked();
}
