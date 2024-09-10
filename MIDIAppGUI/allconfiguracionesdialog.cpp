#include "allconfiguracionesdialog.h"
#include "ui_allconfiguracionesdialog.h"
#include "newconfiguracionesdialog.h"
#include "qvariantptr.h"
AllConfiguracionesDialog::AllConfiguracionesDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::AllConfiguracionesDialog)
{
    ui->setupUi(this);
}

AllConfiguracionesDialog::~AllConfiguracionesDialog()
{
    delete ui;
}

void AllConfiguracionesDialog::addExtensibleChild(QewExtensibleDialog *extensible_child, const QString &caption, QTreeWidgetItem *parentItem, int index)
{
    if(extensible_child->inherits("ConfiguracionDialog"))
        dialogs.append((ConfiguracionDialog*)extensible_child);
    QewTreeViewDialog::addExtensibleChild(extensible_child, caption, parentItem, index);

}

bool AllConfiguracionesDialog::setUpDialog()
{
    bool ret = QewTreeViewDialog::setUpDialog();
    //if(ret){}
//    if(ret){
//        instItem = getTreeView()->topLevelItem(0);
//    }
    if(!ret)
        return ret;
    connectDialog(this);

    return ret;
}

void AllConfiguracionesDialog::newConfiguracion()
{
    NewConfiguracionesDialog newDialog(this);
    int result = newDialog.exec();
    QString id = newDialog.getID();
    if (result == QDialog::Accepted && !id.isEmpty()) {
        int index = childExists(id);
        if(index == -1) {
            Configuracion* configuracion = createNewConfiguracion(id);// hacerlo segun mi clase
            bool newConf = true;
            addConfiguracion(configuracion, newConf);
            QewTreeViewDialog* extensibleParent = 0;
            QTreeWidget* treeWidget = getSuperTreeView(&extensibleParent);
            treeWidget->setCurrentItem(getItemTree());
            index = childExists(id, treeWidget->currentItem());
            QTreeWidgetItem* item = treeWidget->currentItem()->child(index);
            treeWidget->setCurrentItem(item);
            extensibleParent->setCurrentWidget(item);

            emit emitDialogSaved();
        }
        else
            currentItemTextNotChanged(tr("configuracion existente ") + id);
    }
}

QList<ConfiguracionDialog *> AllConfiguracionesDialog::getDialogs() const
{
    return dialogs;
}

void AllConfiguracionesDialog::removeDialogs()
{
    for(int i = dialogs.size() - 1; i >= 0; i--)
        removeExtensibleChild(extensibleChildren().at(i));
    extensibleChildren().clear();
    dialogs.clear();
}

void AllConfiguracionesDialog::getExternVal(const QString &var_name, const QVariant &var_val)
{
    if(this != QVariantPtr<QewExtensibleDialog>::asPtr(var_val))
        return;

    if(var_name == "addElement")
        newConfiguracion();
}

Configuracion *AllConfiguracionesDialog::createNewConfiguracion(const QString &id)
{
    Configuracion* configuracion = new Configuracion;
    configuracion->setId(id);
    configuraciones->append(configuracion);
    return configuracion;

}

void AllConfiguracionesDialog::addConfiguracion(Configuracion *configuracion, bool newConf)
{
    ConfiguracionDialog* dialog = getDialog();
    dialog->setNewConf(newConf);
    dialog->setConfiguracion(configuracion);
    dialog->setUpDialog();
    connect(dialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)));
    addExtensibleChild(dialog, configuracion->getId(), getItemTree());
    dialog->setBotones();


}

ConfiguracionDialog *AllConfiguracionesDialog::getDialog()
{
    ConfiguracionDialog* dialog = new ConfiguracionDialog(this);
    dialog->setArduinoCLIPath(Configuracion::getArduinoCLIPath());
    dialog->setSaveButtonShowsUpdate(true);
    return dialog;
}

QList<Instrumento *> *AllConfiguracionesDialog::getInstrumentos() const
{
    return instrumentos;
}

void AllConfiguracionesDialog::setInstrumentos(QList<Instrumento *> *newInstrumentos)
{
    instrumentos = newInstrumentos;
}



QList<Configuracion *> *AllConfiguracionesDialog::getConfiguraciones() const
{
    return configuraciones;
}

void AllConfiguracionesDialog::setConfiguraciones(QList<Configuracion *> *newConfiguraciones)
{
    configuraciones = newConfiguraciones;
    for(int i = 0; i < this->configuraciones->size(); i++)
        addConfiguracion(this->configuraciones->at(i));
}
