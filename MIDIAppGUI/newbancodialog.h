#ifndef NEWBANCODIALOG_H
#define NEWBANCODIALOG_H

#include <QDialog>

namespace Ui {
class NewBancoDialog;
}

class NewBancoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewBancoDialog(QWidget *parent = nullptr);
    ~NewBancoDialog();
    QString getNumBanco();
    int getNumProgramas();

private:
    Ui::NewBancoDialog *ui;
};

#endif // NEWBANCODIALOG_H
