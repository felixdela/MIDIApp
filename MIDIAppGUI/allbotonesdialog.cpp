#include "allbotonesdialog.h"
#include "ui_allbotonesdialog.h"
#include "qvariantptr.h"
AllBotonesDialog::AllBotonesDialog(Configuracion *config, QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::AllBotonesDialog),
    configuracion(config)
{
    ui->setupUi(this);
}

AllBotonesDialog::~AllBotonesDialog()
{
    delete ui;
}

void AllBotonesDialog::addExtensibleChild(QewExtensibleDialog *extensible_child, const QString &caption, QTreeWidgetItem *parentItem, int index)
{
    if(extensible_child->inherits("BotonDialog"))
        dialogs.append((BotonDialog*)extensible_child);
    QewTreeViewDialog::addExtensibleChild(extensible_child, caption, parentItem, index);
}

bool AllBotonesDialog::setUpDialog()
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

void AllBotonesDialog::newBoton(int index)
{
    Boton* boton = createNewBoton(index);
    addBoton(boton);
    QewTreeViewDialog* extensibleParent = 0;
    QTreeWidget* treeWidget = getSuperTreeView(&extensibleParent);
    treeWidget->setCurrentItem(getItemTree());
    QTreeWidgetItem* currentItem = treeWidget->currentItem();
    //El índice debe empezar en 0 para el treeWidget, a pesar de que los botones están enumerados del 1 al 8.
    QTreeWidgetItem* item = treeWidget->currentItem()->child(index-1);
    treeWidget->setCurrentItem(item);
    extensibleParent->setCurrentWidget(item);
    emit emitDialogSaved();
}

QList<BotonDialog *> AllBotonesDialog::getDialogs() const
{
    return dialogs;
}

void AllBotonesDialog::removeDialogs()
{
    for(int i = dialogs.size() - 1; i >= 0; i--)
        removeExtensibleChild(extensibleChildren().at(i));
    extensibleChildren().clear();
    dialogs.clear();
}

Boton *AllBotonesDialog::createNewBoton(int index)
{
    Boton* boton = new Boton;
    QString id = "Boton " + QString::number(index);
    boton->setNumBoton(id);
    botones->append(boton);
    return boton;
}


void AllBotonesDialog::addBoton(Boton *boton)
{
    BotonDialog* dialog = getDialog(boton);  // Pasa el objeto Boton aquí
    dialog->setUpDialog();
    dialog->setBoton(boton);
    connect(dialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)));
    addExtensibleChild(dialog, boton->getNumBoton(), getItemTree());
}

BotonDialog *AllBotonesDialog::getDialog(Boton *boton)
{
    BotonDialog* dialog = new BotonDialog(configuracion, boton, this);  // Pasa el objeto Boton al constructor
    dialog->setSaveButtonShowsUpdate(true);
    return dialog;
}

QList<Boton *> *AllBotonesDialog::getBotones() const
{
    return botones;
}

void AllBotonesDialog::setBotones(QList<Boton *> *newBotones)
{
    botones = newBotones;
    for (int i= 0; i <botones->size();i++)
        {
        addBoton(botones->at(i));

    }
}

void AllBotonesDialog::getExternVal(const QString &var_name, const QVariant &var_val)
{
    if(this != QVariantPtr<QewExtensibleDialog>::asPtr(var_val))
        return;

    //if(var_name == "addElement")
        //newBoton();
}
