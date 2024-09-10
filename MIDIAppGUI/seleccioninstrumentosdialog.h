#ifndef SELECCIONINSTRUMENTOSDIALOG_H
#define SELECCIONINSTRUMENTOSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include "configuracion.h"
namespace Ui {
class SeleccionInstrumentosDialog;
}

class SeleccionInstrumentosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SeleccionInstrumentosDialog(Configuracion *config, QWidget *parent = nullptr);
    ~SeleccionInstrumentosDialog();

private slots:
    void accept() override;
signals:
    void instrumentoSeleccionado(const QString &message);
private:
    Ui::SeleccionInstrumentosDialog *ui;
    Configuracion *configuracion;
};

#endif // SeleccionInstrumentosDialog_H
