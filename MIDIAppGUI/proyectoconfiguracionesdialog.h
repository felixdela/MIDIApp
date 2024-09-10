#ifndef PROYECTOCONFIGURACIONESDIALOG_H
#define PROYECTOCONFIGURACIONESDIALOG_H

#include "proyectoconfiguraciones.h"
#include "proyectoInstrumentos.h"
#include "qewtreeviewdialog.h"
#include "allconfiguracionesdialog.h"

namespace Ui {
class ProyectoConfiguracionesDialog;
}
class Configuracion;

class ProyectoConfiguracionesDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit ProyectoConfiguracionesDialog(QWidget *parent = nullptr);
    ~ProyectoConfiguracionesDialog();
    bool setUpDialog();

    Ui::ProyectoConfiguracionesDialog *getUi() const;
    void setUi(Ui::ProyectoConfiguracionesDialog *newUi);

    ProyectoConfiguraciones *getProyectoConfiguraciones() const;
    void setProyectoConfiguraciones(ProyectoConfiguraciones *newProyectoConfiguraciones);


    QTreeWidgetItem *getConfItem() const;
    void setConfItem(QTreeWidgetItem *newConfItem);
    void addConfiguracionDialog();
    //QString openDirectoryDialog();
    void saveXML();
    void loadXML();
public slots:
    void newProyectoConfiguraciones();

private slots:
    //void on_pushButton_Dir_clicked();
    void onTreeItemSelected();
    void onCleanProject();
    void setProyectoID();
    void changeCurrentItemId(QTreeWidgetItem *item, QString newId);
protected:
    //void setDataChangedThis(bool dataChanged);
    void saveThis();
    void fillUi();
    void cleanProject();
    bool canCloseProject();
private:
    Ui::ProyectoConfiguracionesDialog *ui;
    ProyectoConfiguraciones* proyectoConfiguraciones;
    ProyectoInstrumentos* proyectoInstrumentos;

    QString proyectoConfiguracionesID;
    QString proyectoConfDescripcion;
    QString proyectoFileName;

    AllConfiguracionesDialog* configuracionesDialog;
    ConfiguracionDialog* configuracionDialog; //para cuando se cree allconfiguracionesdialog
    QTreeWidgetItem* confItem;
    QTreeWidgetItem* dataItem;
    QTreeWidgetItem* prevCurrentItem;
    QTreeWidgetItem* configuracionesItem;
protected:
    bool projectNotSaved;
};

#endif // PROYECTOCONFIGURACIONESDIALOG_H
