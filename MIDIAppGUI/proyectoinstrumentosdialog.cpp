#include "proyectoinstrumentosdialog.h"
#include "ui_proyectoinstrumentosdialog.h"
#include <QMessageBox>
ProyectoInstrumentosDialog::ProyectoInstrumentosDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::ProyectoInstrumentosDialog)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 5);
    //inicializamos todos los punteros a 0, para que posteriormente se les pueda reconocer con una llamada al !puntero
    proyectoInstrumentos = 0;
    instrumentosDialog = 0;
    instItem = 0;
    dataItem = 0;
    prevCurrentItem = 0;
    connect(ui->idInstLineEdit, SIGNAL(editingFinished()), this, SLOT(setProyectoID()));
    connect(ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(onDescriptionModified()));


}
bool ProyectoInstrumentosDialog::setUpDialog()
{
    bool ret = QewTreeViewDialog::setUpDialog();
    if(ret){
        instItem = getTreeView()->topLevelItem(0);
    }
    if(!ret)
        return ret;
    connectDialog(this);
    QString title;
    if(ret)
    {
        prevCurrentItem = instItem;
        dataItem = instItem->child(0);
        if(!instrumentosDialog) {
            instrumentosDialog = new AllInstrumentosDialog(this);
            if(instrumentosDialog->setUpDialog()){}
            title = tr("Instrumentos");
            instrumentosDialog->setWindowTitle(title);
            addExtensibleChild(instrumentosDialog, title);
            instrumentosItem = instrumentosDialog->getItemTree();
            connect(instrumentosDialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SLOT(changeCurrentItemId(QTreeWidgetItem*, QString)));

        }

    }
    return ret;
}
ProyectoInstrumentosDialog::~ProyectoInstrumentosDialog()
{
    delete ui;
}

Ui::ProyectoInstrumentosDialog *ProyectoInstrumentosDialog::getUi() const
{
    return ui;
}

void ProyectoInstrumentosDialog::setUi(Ui::ProyectoInstrumentosDialog *newUi)
{
    ui = newUi;
}

ProyectoInstrumentos *ProyectoInstrumentosDialog::getProyectoInstrumentos() const
{
    return proyectoInstrumentos;
}

void ProyectoInstrumentosDialog::setProyectoInstrumentos(ProyectoInstrumentos *newProyectoInstrumentos)
{

    proyectoInstrumentos = newProyectoInstrumentos;
    cleanProject();
    proyectoID = proyectoInstrumentos->getProyectoID();
    proyectoDescripcion = proyectoInstrumentos->getDescripcion();
    fillUi();
    setCurrentWidget(dataItem);
    instrumentosDialog->setInstrumentos(proyectoInstrumentos->getInstrumentos());

}

void ProyectoInstrumentosDialog::setProyectoID()
{
    proyectoID = ui->idInstLineEdit->text().trimmed();
    instItem->setText(0, proyectoID);
    setDataChangedThis(true);
}

void ProyectoInstrumentosDialog::onDescriptionModified()
{
    setDataChangedThis(true);
}

void ProyectoInstrumentosDialog::onCleanProject()
{
    if(!canCloseProject())
        return;
    if(proyectoInstrumentos) {
        delete proyectoInstrumentos;
        proyectoInstrumentos = 0;
    }

    cleanProject();
    instItem->setText(0, tr( "Project"));
    prevCurrentItem = instItem;
}

void ProyectoInstrumentosDialog::saveThis()
{
    //qDebug("Saving ProyectoInstrumentosDialog data");
    proyectoID = ui->idInstLineEdit->text().trimmed();
    proyectoInstrumentos->setProyectoID(proyectoID);
    proyectoDescripcion = ui->descriptionTextEdit->toPlainText();
    proyectoInstrumentos->setDescripcion(proyectoDescripcion);
    QList<Instrumento*>* lista = instrumentosDialog->getInstrumentos();
    proyectoInstrumentos->setInstrumentos(*lista);
}

void ProyectoInstrumentosDialog::fillUi()
{
    ui->idInstLineEdit->setText(proyectoID);
    ui->descriptionTextEdit->setText(proyectoDescripcion);
}

void ProyectoInstrumentosDialog::cleanProject()
{
    instrumentosDialog->removeDialogs();
    int ninst = instrumentosItem->childCount();
   //qDebug() << "Número de hijos:" << instrumentosItem->childCount();
    for(int i = 0; i < ninst; i++){
       //qDebug() << "Número de hijos:" << instrumentosItem->childCount();
        bool alsoWidgets = true;
        //instrumentoDialog->cleanProject();
        removeItemFromTree(instrumentosItem->child(0), alsoWidgets);
    }
}

bool ProyectoInstrumentosDialog::canCloseProject()
{
    if(!projectNotSaved)
        return true;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("Closing current project"));
    msgBox.setText(tr("Current project has not been saved.\nDo you want to continue?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
        return false;
    else {
        projectNotSaved = false;
        return true;
    }
}


QTreeWidgetItem *ProyectoInstrumentosDialog::getInstItem() const
{
    return instItem;
}

void ProyectoInstrumentosDialog::setInstItem(QTreeWidgetItem *newInstItem)
{
    instItem = newInstItem;
}

void ProyectoInstrumentosDialog::addInstrumentoDialog()
{
    instrumentosDialog->newInstrumento();
}

void ProyectoInstrumentosDialog::saveXML()
{
    if(proyectoID == tr("Nuevo instrumento") || proyectoID.trimmed().isEmpty()) {
        QMessageBox::critical(this, tr("Saving project"), tr("Project ID not valid"), QMessageBox::Ok);
        return;
    }
    //
    save();
    QDomDocument doc(ProyectoInstrumentos::XMLDocName());
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    QDomElement root = doc.createElement("proyectoInstrumentos");
    doc.appendChild(root);
    proyectoInstrumentos->writeToXml(doc, root);

    QString projectFilename = "XMLInstrumentosTest.xml";
    QString projectsDirectory = "/home/felix/Documentos/";
    QFileInfo fileInfo = QFileInfo(QDir(projectsDirectory), projectFilename);
    QString filename = fileInfo.absoluteFilePath();
    QString content = doc.toString(4);
    QString sender = tr("Save project");
    QString type_file = "";
    QString extensions = "xml";
    XMLOperator* xml_operator = XMLOperator::instance();
    bool change_name = true;

    bool saveSuccess = xml_operator->saveDocument(filename, content, type_file, extensions, sender, change_name);

        if (saveSuccess) {
            QMessageBox::information(this, tr("Project saved"), tr("Project saved successfully"), QMessageBox::Ok);
            projectNotSaved = false;
        } else {
            QMessageBox::critical(this, tr("Saving project"), tr("Failed to save project"), QMessageBox::Ok);
            projectNotSaved = true;
        }
}

void ProyectoInstrumentosDialog::loadXML()
{
    QTreeWidgetItem *selectedItem = ui->qewListViewInst->currentItem();

        if (selectedItem) {
            // Deseleccionar el elemento
            ui->qewListViewInst->setCurrentItem(nullptr);
            selectedItem->setSelected(false);
        }
    XMLOperator* xml_operator = XMLOperator::instance();
    QString filename = "/home/felix/Documentos/XMLInstrumentosTest.xml";
    if(filename.isEmpty())
        filename = QDir::homePath();
    QString docName = ProyectoInstrumentos::XMLDocName();
    QString sender = tr("Open project");
    QString type_file = "";
    QString extensions = "";
    bool ret = xml_operator->getFileDialog(filename, sender, QFileDialog::AcceptOpen, QFileDialog::ExistingFile, type_file, extensions);
    if(!ret)
        return;
    QDomDocument doc = xml_operator->loadXML(filename, docName, sender);

    //     QDomNode n = doc.firstChild();
    //     if(n.isNull())
    //         return;
    //     QDomElement e_root = n.toElement();
        QDomElement e_root = doc.documentElement();
        if(e_root.isNull())
            return;
        if(e_root.tagName() != "proyectoInstrumentos")
            return;
        proyectoFileName = filename;
        if(proyectoInstrumentos)
            delete proyectoInstrumentos;
        proyectoInstrumentos = new ProyectoInstrumentos;
        proyectoInstrumentos->setXMLElement(e_root);

        setProyectoInstrumentos(proyectoInstrumentos);
}

void ProyectoInstrumentosDialog::newProyectoInstrumentos()
{
    //if(!canCloseProject())
        //return;
//    saveIfNeeded();

    ui->idInstLineEdit->setEnabled(true);
    ui->descriptionTextEdit->setEnabled(true);
    proyectoInstrumentos = new ProyectoInstrumentos();
    setProyectoInstrumentos(proyectoInstrumentos);
}
void ProyectoInstrumentosDialog::changeCurrentItemId(QTreeWidgetItem *item, QString newId)
{
    changeItemText(item, newId);
}
void ProyectoInstrumentosDialog::onTreeItemSelected()
{

}
