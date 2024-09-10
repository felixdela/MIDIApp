#ifndef SELECCIONPANNINGDIALOG_H
#define SELECCIONPANNINGDIALOG_H

#include <QDialog>
#include "configuracion.h"
#include "instrumento.h"

namespace Ui {
class SeleccionPanningDialog;
}

class SeleccionPanningDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SeleccionPanningDialog(Configuracion *config, QWidget *parent = nullptr);
    ~SeleccionPanningDialog();

signals:
    void instrumentoYPanningSeleccionado(const QString &instrumentoId, int panning);

private slots:
    void accept() override;
    void updatePanningLabel(int value);

private:
    Ui::SeleccionPanningDialog *ui;
    Configuracion *config;
};

#endif // SELECCIONPANNINGDIALOG_H
