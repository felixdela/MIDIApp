#ifndef NEWCONFIGURACIONESDIALOG_H
#define NEWCONFIGURACIONESDIALOG_H
#include <QDialog>
#include "instrumento.h"
namespace Ui {
class NewConfiguracionesDialog;
}

class NewConfiguracionesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewConfiguracionesDialog(QWidget *parent = nullptr);
    ~NewConfiguracionesDialog();
    QString getID();
    //void setProyectoInstrumentos(ProyectoInstrumentos *proyectoInst){};
private:

private:
    Ui::NewConfiguracionesDialog *ui;
    //ProyectoInstrumentos *proyectoInstrumentos;
};

#endif // NEWCONFIGURACIONESDIALOG_H
