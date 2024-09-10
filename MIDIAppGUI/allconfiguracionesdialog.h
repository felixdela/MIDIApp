#ifndef ALLCONFIGURACIONESDIALOG_H
#define ALLCONFIGURACIONESDIALOG_H

#include "qewtreeviewdialog.h"
#include "configuracion.h"
#include "configuraciondialog.h"
namespace Ui {
class AllConfiguracionesDialog;
}

class AllConfiguracionesDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit AllConfiguracionesDialog(QWidget *parent = nullptr);
    ~AllConfiguracionesDialog();

    void addExtensibleChild(QewExtensibleDialog* extensible_child, const QString& caption,
                            QTreeWidgetItem* parentItem = 0, int index = -1);
    bool setUpDialog();

    void newConfiguracion();
    QList<ConfiguracionDialog *> getDialogs() const;
    void removeDialogs();

    QList<Configuracion *> *getConfiguraciones() const;
    void setConfiguraciones(QList<Configuracion *> *newConfiguraciones);

    QList<Instrumento *> *getInstrumentos() const;
    void setInstrumentos(QList<Instrumento *> *newInstrumentos);
public slots:
    void getExternVal(const QString& var_name, const QVariant& var_val);
private:
    Ui::AllConfiguracionesDialog *ui;
protected:
    virtual Configuracion* createNewConfiguracion(const QString& id);
    void addConfiguracion(Configuracion *configuracion, bool newConf = false);
    virtual ConfiguracionDialog* getDialog();
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);
protected:
    QList<Configuracion*>* configuraciones;
    QList<Instrumento*>* instrumentos;
    QList<ConfiguracionDialog*> dialogs;
};

#endif // ALLCONFIGURACIONESDIALOG_H
