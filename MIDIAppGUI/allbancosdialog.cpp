#include "allbancosdialog.h"
#include "ui_allbancosdialog.h"
#include "qvariantptr.h"
#include "newbancodialog.h"
AllBancosDialog::AllBancosDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::AllBancosDialog)
{
    ui->setupUi(this);
    bancos = 0;
}

AllBancosDialog::~AllBancosDialog()
{
    delete ui;
}

void AllBancosDialog::addExtensibleChild(QewExtensibleDialog *extensible_child, const QString &caption, QTreeWidgetItem *parentItem, int index)
{
    if(extensible_child->inherits("BancoDialog"))
        dialogs.append((BancoDialog*)extensible_child);
    QewTreeViewDialog::addExtensibleChild(extensible_child, caption, parentItem, index);
}

bool AllBancosDialog::setUpDialog()
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

void AllBancosDialog::newBanco(QString nomenclatura)
{
    NewBancoDialog newDialog(this);
    int result = newDialog.exec();
    QString num = newDialog.getNumBanco();
    int progs = newDialog.getNumProgramas();
    if(result == QDialog::Accepted && !num.isEmpty()){
        int index = childExists(num);
        if (index == -1)
        {
            Banco* banco = createNewBanco(num,progs,nomenclatura);
            addBanco(banco);
            QewTreeViewDialog* extensibleParent = 0;
            QTreeWidget* treeWidget = getSuperTreeView(&extensibleParent);
            treeWidget->setCurrentItem(getItemTree());
            index = childExists(num, treeWidget->currentItem());
            QTreeWidgetItem* item = treeWidget->currentItem()->child(index);
            QString text = item->text(0);
            treeWidget->setCurrentItem(item);
            extensibleParent->setCurrentWidget(item);
            emit emitDialogSaved();
        }
        else
            currentItemTextNotChanged(tr("banco existente ") + num);{
        }
    }
}

QList<Banco *> *AllBancosDialog::getBancos() const
{
    return bancos;
}

void AllBancosDialog::setBancos(QList<Banco *> *newBancos)
{
    bancos = newBancos;
    for (int i= 0; i <bancos->size();i++)
        {
        addBanco(bancos->at(i));

    }
}

void AllBancosDialog::getExternVal(const QString &var_name, const QVariant &var_val)
{
    if(this != QVariantPtr<QewExtensibleDialog>::asPtr(var_val))
        return;

    if(var_name == "addElement")
        newBanco("no se usa esta función, por si acaso ponemos esto");
}

Banco *AllBancosDialog::createNewBanco(const QString &id, int &numProgramas, QString nomenclatura)
{
    Banco* banco = new Banco;
    banco->setNumBanco(id);
    banco->generarProgramas(numProgramas, nomenclatura);
    bancos->append(banco);
    return banco;
}

void AllBancosDialog::addBanco(Banco *banco)
{
    BancoDialog* dialog = getDialog();
    dialog->setUpDialog();
    connect(dialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)));
    //connect(dialog, SIGNAL(sendRemoveDialog(InstrumentoDialog* )),
                //this, SLOT(removeInstrumentoDialog(InstrumentoDialog* )));


    addExtensibleChild(dialog, banco->getNumBanco(), getItemTree());
    dialog->setBanco(banco);
    //bancos->append(banco);
}

BancoDialog *AllBancosDialog::getDialog()
{
    BancoDialog* dialog = new BancoDialog(this);
    dialog->setSaveButtonShowsUpdate(true);
    return dialog;
}

QList<BancoDialog *> AllBancosDialog::getDialogs() const
{
    return dialogs;
}

void AllBancosDialog::removeDialogs()
{
    for(int i = dialogs.size() - 1; i >= 0; i--)
        removeExtensibleChild(extensibleChildren().at(i));
    extensibleChildren().clear();
    dialogs.clear();
}
