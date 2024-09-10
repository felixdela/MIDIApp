#include "allinstrumentosdialog.h"
#include "ui_allinstrumentosdialog.h"
#include "qvariantptr.h"
#include "newinstrumentosdialog.h"
AllInstrumentosDialog::AllInstrumentosDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::AllInstrumentosDialog)
{
    ui->setupUi(this);
    instrumentos = 0;
}

AllInstrumentosDialog::~AllInstrumentosDialog()
{
    delete ui;
}

void AllInstrumentosDialog::addExtensibleChild(QewExtensibleDialog *extensible_child, const QString &caption, QTreeWidgetItem *parentItem, int index)
{
    if(extensible_child->inherits("InstrumentoDialog"))
        dialogs.append((InstrumentoDialog*)extensible_child);
    QewTreeViewDialog::addExtensibleChild(extensible_child, caption, parentItem, index);
}

bool AllInstrumentosDialog::setUpDialog()
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

QList<InstrumentoDialog *> AllInstrumentosDialog::getDialogs() const
{
    return dialogs;
}

void AllInstrumentosDialog::removeDialogs()// el usuario quiere borrar un instrumento
{
    for(int i = dialogs.size() - 1; i >= 0; i--)
        removeExtensibleChild(extensibleChildren().at(i));
    extensibleChildren().clear();
    dialogs.clear();
}

void AllInstrumentosDialog::getExternVal(const QString &var_name, const QVariant &var_val)
{
    if(this != QVariantPtr<QewExtensibleDialog>::asPtr(var_val))
        return;

    if(var_name == "addElement")
        newInstrumento();
}

Instrumento *AllInstrumentosDialog::createNewInstrumento(const QString &id)
{
    Instrumento* instrumento = new Instrumento;
    instrumento ->setId(id);
    instrumentos->append(instrumento);
    return instrumento;
}

void AllInstrumentosDialog::addInstrumento(Instrumento *instrumento, bool newInst)
{
    InstrumentoDialog* dialog = getDialog();
    dialog->setNewInst(newInst);
    dialog->setUpDialog();
    connect(dialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)));
    //connect(dialog, SIGNAL(sendRemoveDialog(InstrumentoDialog* )),
                //this, SLOT(removeInstrumentoDialog(InstrumentoDialog* )));


    addExtensibleChild(dialog, instrumento->getId(), getItemTree());
    dialog->setInstrumento(instrumento);
    //instrumentos->append(instrumento);
}
InstrumentoDialog *AllInstrumentosDialog::getDialog()
{

    InstrumentoDialog* dialog = new InstrumentoDialog(this);
    dialog->setSaveButtonShowsUpdate(true);
    return dialog;
}

QList<Instrumento *> *AllInstrumentosDialog::getInstrumentos() const
{
    return instrumentos;
}

void AllInstrumentosDialog::setInstrumentos(QList<Instrumento *> *newInstrumentos)
{
    instrumentos = newInstrumentos;
    for(int i = 0; i < this->instrumentos->size(); i++)
        addInstrumento(this->instrumentos->at(i));
}
bool AllInstrumentosDialog::checkInstrumentoExists(QString id)//Función que chequea si existe un instrumento con dicho ID
{
    bool resul = false;
    QList<Instrumento*>* instrumentos = this->getInstrumentos();
    for(Instrumento* instrumento : *instrumentos)
    {
        if(instrumento->getId()==id)
        {
            resul = true;
        }
    }
    return resul;
}

void AllInstrumentosDialog::newInstrumento()
{
//     bool ok;
//     QString id = QInputDialog::getText(this, tr("New reconstruction"),
//                                          tr("Reconstruction id:"), QLineEdit::Normal, "", &ok);
    NewInstrumentosDialog newDialog(this);
    int result = newDialog.exec();
    QString id = newDialog.getID();
    bool exists = checkInstrumentoExists(id);
//     if (ok && !id.isEmpty()) {
    if (result == QDialog::Accepted && !id.isEmpty()) {
        int index = childExists(id);
        if(index == -1 && !exists) {
            Instrumento* instrumento = createNewInstrumento(id);// hacerlo segun mi clase
            bool newInst = true;
            addInstrumento(instrumento, newInst);

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
            currentItemTextNotChanged(tr("instrumento existente ") + id);
    }
}
//enlazar acciones de menú con métodos de esta clase
