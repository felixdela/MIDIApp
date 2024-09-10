#ifndef BOTONDIALOG_H
#define BOTONDIALOG_H

#include <qewextensibledialog.h>
#include "boton.h"
#include "xmlwritable.h"
#include "configuracion.h"
namespace Ui {
class BotonDialog;
}

class BotonDialog : public QewExtensibleDialog
{
    Q_OBJECT

public:
    explicit BotonDialog(Configuracion *config, Boton *boton, QWidget *parent = nullptr);;
    ~BotonDialog();

    bool setUpDialog();
    Boton *getBoton() const;
    void setBoton(Boton *newBoton);
    QString generateHtml(int /*prevIndent*/) const;
    Configuracion *getConfiguracion() const;
    void setConfiguracion(Configuracion *newConfiguracion);
    QStringList getFuncionesFromListWidget() const;
private slots:
    void showSeleccionInstrumentosDialog();

    void showSeleccionVolumenDialog();
    void showSeleccionPanningDialog();
    void agregarInstrumentoEnLista(const QString &message);
    void agregarVolumenEnLista(const QString &instrumentoId,int volumen);
    void agregarPanningEnLista(const QString &instrumentoId, int panning);


    void onRemoveFunction();
    void onMoveFunctionUp();
    void onMoveFunctionDown();
protected:
    void saveThis();
    void fillUi();
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);
protected:
    QString numBoton;
    QList<QString> funciones;
private:
    Ui::BotonDialog *ui;
    Boton *boton;
    Configuracion *configuracion;
};

#endif // BOTONDIALOG_H
