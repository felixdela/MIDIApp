#ifndef NEWINSTRUMENTOSDIALOG_H
#define NEWINSTRUMENTOSDIALOG_H

#include <QDialog>

namespace Ui {
class NewInstrumentosDialog;
}

class NewInstrumentosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewInstrumentosDialog(QWidget *parent = nullptr);
    ~NewInstrumentosDialog();
    QString getID();

private:
    Ui::NewInstrumentosDialog *ui;
};

#endif // NEWINSTRUMENTOSDIALOG_H
