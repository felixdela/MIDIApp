#ifndef SELECCIONVOLUMENDIALOG_H
#define SELECCIONVOLUMENDIALOG_H

#include <QDialog>
#include "configuracion.h"

namespace Ui {
class SeleccionVolumenDialog;
}

class SeleccionVolumenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SeleccionVolumenDialog(Configuracion *config, QWidget *parent = nullptr);
    ~SeleccionVolumenDialog();

signals:
    void instrumentoYVolumenSeleccionado(const QString &instrumentoId, int volumen);

private slots:
    void accept() override;
    void updateVolumeLabel(int value);

private:
    Ui::SeleccionVolumenDialog *ui;
    Configuracion *config;
};

#endif // SELECCIONVOLUMENDIALOG_H
