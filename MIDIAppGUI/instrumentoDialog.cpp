#include "instrumentoDialog.h"
#include "ui_instrumentoDialog.h"

InstrumentoDialog::InstrumentoDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::InstrumentoDialog)
{
    ui->setupUi(this);

    instrumentoItem=0;
    dataItem=0;
    bancosDialog = 0;
    titleBancos = tr("Bancos");
    newInst = false;
    //connect(ui->lineEdit_Id, SIGNAL(editingFinished()), this, SLOT(setInstrumentoID()));

    connect(ui->lineEdit_Id, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
}

InstrumentoDialog::~InstrumentoDialog()
{
    delete ui;
}

bool InstrumentoDialog::setUpDialog()
{
    bool ret = QewTreeViewDialog::setUpDialog();
    //    if(ret){
    //        instrumentoItem = getItemTree();
    //    }
    if(!ret)
        return ret;
    connectDialog(this);
    //QString title;
    if(ret)
    {
//        prevCurrentItem = instrumentoItem;
//        dataItem = instrumentoItem->child(0);
        if(!bancosDialog) {
            bancosDialog = new AllBancosDialog(this);
            if(bancosDialog->setUpDialog()){}
            //title = tr("Bancos");
            bancosDialog->setWindowTitle(titleBancos);
            // Si se añade sin singleShot aparece Bancos en dos posiciones del árbol
//            addExtensibleChild(bancosDialog, title);
//            bancosItem = bancosDialog->getItemTree();
//            connect(bancosDialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SLOT(changeCurrentItemId(QTreeWidgetItem*, QString)));
            if(newInst)
                QTimer::singleShot(100, this, SLOT(addBancosDialog()));
            else
                addBancosDialog();
        }

    }
    return ret;
}
Instrumento *InstrumentoDialog::getInstrumento() const
{
    return instrumento;
}

void InstrumentoDialog::setInstrumento(Instrumento *instrumento)
{
    this->instrumento = instrumento;
    instrumentoID = instrumento->getId();
    canal = instrumento->getCanal();
    nomenclatura = instrumento->getNomenclatura();
    ui->lineEdit_Id->setText(instrumento->getId());
    //instrumentoItem = getTreeView()->topLevelItem(0);
    fillUi();
    bancosDialog->setBancos(instrumento->getBancos());
}

void InstrumentoDialog::setBancosDialog(AllBancosDialog *newBancosDialog)
{
    bancosDialog = newBancosDialog;
}

void InstrumentoDialog::setNewInst(bool newNewInst)
{
    newInst = newNewInst;
}

QString InstrumentoDialog::getNomenclatura() const
{
    return nomenclatura;
}

void InstrumentoDialog::setNomenclatura(const QString &newNomenclatura)
{
    nomenclatura = newNomenclatura;
}

void InstrumentoDialog::saveThis()
{
    //qDebug("Saving InstrumentoDialog data");
    instrumentoID = ui->lineEdit_Id->text().trimmed();
    instrumento->setId(instrumentoID);
    //canal = ui->lineEdit_canal->text().trimmed();
    instrumento->setCanal(canal);
    if (ui->radioButton_0127->isChecked()) {
            nomenclatura = "0-127";
            instrumento->setNomenclatura(nomenclatura);
        } else if (ui->radioButton_1128->isChecked()) {
            nomenclatura = "1-128";
            instrumento->setNomenclatura(nomenclatura);
    }
}

void InstrumentoDialog::fillUi()
{
    ui->lineEdit_Id->setText(instrumentoID);
    //ui->lineEdit_canal->setText(canal);
    if(nomenclatura == "0-127")
    {
        ui->radioButton_0127->setChecked(true);
    }
    else if (nomenclatura == "1-128")
    {
        ui->radioButton_1128->setChecked(true);
    }
}

void InstrumentoDialog::cleanProject()
{
    bancosDialog->removeDialogs();
    for(int i = 0; i<bancosItem->childCount(); i++)
    {
        bool alsoWidgets=true;
        removeItemFromTree(bancosItem->child(0), alsoWidgets);
    }
}

void InstrumentoDialog::setInstrumentoID()
{
    instrumentoID = ui->lineEdit_Id->text().trimmed();
    instrumentoItem->setText(0, instrumentoID);
    setDataChangedThis(true);
}
void InstrumentoDialog::editingFinished()
{
    if(prevId != ui->lineEdit_Id->text()) {
            emit sendIdChanged(getItemTree(), ui->lineEdit_Id->text().trimmed());
            prevId = ui->lineEdit_Id->text().trimmed();
            setWindowTitle(prevId);
            setDataChangedThis(true);
    }
}

void InstrumentoDialog::on_pushButton_Banco_clicked()
{
    saveThis();
    bancosDialog->newBanco(nomenclatura);
}

void InstrumentoDialog::changeCurrentItemId(QTreeWidgetItem *item, QString newId)
{
    changeItemText(item, newId);
}

void InstrumentoDialog::addBancosDialog()
{
    addExtensibleChild(bancosDialog, titleBancos);
    bancosItem = bancosDialog->getItemTree();
    connect(bancosDialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SLOT(changeCurrentItemId(QTreeWidgetItem*, QString)));
}

