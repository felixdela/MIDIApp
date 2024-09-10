#ifndef PROYECTOINSTRUMENTOSDIALOG_H
#define PROYECTOINSTRUMENTOSDIALOG_H

#include <qewtreeviewdialog.h>
#include "proyectoInstrumentos.h"
#include "allinstrumentosdialog.h"

namespace Ui {
class ProyectoInstrumentosDialog;
}
class Instrumento;

class ProyectoInstrumentosDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit ProyectoInstrumentosDialog(QWidget *parent = nullptr);
    ~ProyectoInstrumentosDialog();
    bool setUpDialog();

    Ui::ProyectoInstrumentosDialog *getUi() const;
    void setUi(Ui::ProyectoInstrumentosDialog *newUi);

    ProyectoInstrumentos *getProyectoInstrumentos() const;
    void setProyectoInstrumentos(ProyectoInstrumentos *newProyectoInstrumentos);


    QTreeWidgetItem *getInstItem() const;
    void setInstItem(QTreeWidgetItem *newInstItem);

    void addInstrumentoDialog();

    void saveXML();
    void loadXML();
public slots:

    void newProyectoInstrumentos();
private slots:
    void onTreeItemSelected();
    void setProyectoID();
    void changeCurrentItemId(QTreeWidgetItem *item, QString newId);
protected slots:
    void onDescriptionModified();
    void onCleanProject();

protected:
    //void setDataChangedThis(bool dataChanged);
    void saveThis();
    void fillUi();
    void cleanProject();
    bool canCloseProject();


private:
    Ui::ProyectoInstrumentosDialog *ui;
    ProyectoInstrumentos* proyectoInstrumentos;
    QString proyectoID;
    QString proyectoDescripcion;
    QString proyectoFileName;

    AllInstrumentosDialog* instrumentosDialog;
    InstrumentoDialog* instrumentoDialog;
    QTreeWidgetItem* instItem;
    QTreeWidgetItem* dataItem;
    QTreeWidgetItem* prevCurrentItem;
    QTreeWidgetItem* instrumentosItem;
protected:
    bool projectNotSaved;

};

#endif // PROYECTOINSTRUMENTOSDIALOG_H
