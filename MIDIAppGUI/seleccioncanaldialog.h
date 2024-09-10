#ifndef SELECCIONCANALDIALOG_H
#define SELECCIONCANALDIALOG_H

#include "qcombobox.h"
#include <QDialog>
#include "instrumento.h"
namespace Ui {
class SeleccionCanalDialog;
}

class SeleccionCanalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SeleccionCanalDialog(QWidget *parent = nullptr);
    ~SeleccionCanalDialog();
    void setInstrumentos(const QList<Instrumento*>& instrumentos);
    QList<QString> getSelectedCanales() const;
private slots:
    void accept() override;
private:
        bool validateUniqueCanales() const; // Método para validar canales únicos
private:
    Ui::SeleccionCanalDialog *ui;
    QList<QComboBox*> comboBoxes;
};

#endif // SELECCIONCANALDIALOG_H
