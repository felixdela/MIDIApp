#include "proyectoconfiguracionesdialog.h"
#include "ui_proyectoconfiguracionesdialog.h"
#include <QMessageBox>
ProyectoConfiguracionesDialog::ProyectoConfiguracionesDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::ProyectoConfiguracionesDialog)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 5);
    configuracionesDialog = 0;
    proyectoConfiguraciones = 0;
    confItem = 0;
    dataItem = 0;
    prevCurrentItem = 0;
    connect(ui->idConfLineEdit,SIGNAL(editingFinished()),this, SLOT(setProyectoID()));

}

ProyectoConfiguracionesDialog::~ProyectoConfiguracionesDialog()
{
    delete ui;
}

bool ProyectoConfiguracionesDialog::setUpDialog()
{
    bool ret = QewTreeViewDialog::setUpDialog();
    if(ret){
        confItem = getTreeView()->topLevelItem(0);
    }
    if(!ret)
        return ret;
    connectDialog(this);
    QString title;
    if(ret)
    {
        prevCurrentItem = confItem;
        dataItem = confItem->child(0);
        if(!configuracionesDialog) {
            configuracionesDialog = new AllConfiguracionesDialog(this);
            if(configuracionesDialog->setUpDialog()){}
            title = tr("Configuraciones");
            configuracionesDialog->setWindowTitle(title);
            addExtensibleChild(configuracionesDialog, title);
            configuracionesItem = configuracionesDialog->getItemTree();
            connect(configuracionesDialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SLOT(changeCurrentItemId(QTreeWidgetItem*, QString)));

        }

    }
    return ret;
}

ProyectoConfiguraciones *ProyectoConfiguracionesDialog::getProyectoConfiguraciones() const
{
    return proyectoConfiguraciones;
}

void ProyectoConfiguracionesDialog::setProyectoConfiguraciones(ProyectoConfiguraciones *newProyectoConfiguraciones)
{
    proyectoConfiguraciones = newProyectoConfiguraciones;
    cleanProject();
    proyectoConfiguracionesID = proyectoConfiguraciones->getProyectoID();
    proyectoConfDescripcion = proyectoConfiguraciones->getDescripcion();
    fillUi();
    setCurrentWidget(dataItem);
    configuracionesDialog->setConfiguraciones(proyectoConfiguraciones->getConfiguraciones());
    configuracionesDialog->setInstrumentos(proyectoInstrumentos->getInstrumentos());
}

void ProyectoConfiguracionesDialog::onTreeItemSelected()
{

}

void ProyectoConfiguracionesDialog::cleanProject()
{
    configuracionesDialog->removeDialogs();
    int nconf = configuracionesItem->childCount();
    for(int i = 0; i < nconf; i++){
        bool alsoWidgets = true;
        //instrumentoDialog->cleanProject();
        removeItemFromTree(configuracionesItem->child(0), alsoWidgets);
    }
}
void ProyectoConfiguracionesDialog::onCleanProject()
{
    if(!canCloseProject())
        return;
    if(proyectoInstrumentos) {
        delete proyectoInstrumentos;
        proyectoInstrumentos = 0;
    }

    cleanProject();
    confItem->setText(0, tr( "Project"));
    prevCurrentItem = confItem;
}

void ProyectoConfiguracionesDialog::setProyectoID()
{
    proyectoConfiguracionesID = ui->idConfLineEdit->text().trimmed();
    confItem->setText(0, proyectoConfiguracionesID);
    setDataChangedThis(true);
}

void ProyectoConfiguracionesDialog::changeCurrentItemId(QTreeWidgetItem *item, QString newId)
{
    changeItemText(item, newId);
}

void ProyectoConfiguracionesDialog::saveThis()
{
    //qDebug("Saving ProyectoConfiguracionesDialog data");
    proyectoConfiguracionesID = ui->idConfLineEdit->text().trimmed();
    proyectoConfiguraciones->setProyectoID(proyectoConfiguracionesID);
    proyectoConfDescripcion = ui->descripcionTextEdit->toPlainText();
    proyectoConfiguraciones->setDescripcion(proyectoConfDescripcion);
    QList<Configuracion*>* lista = configuracionesDialog->getConfiguraciones();
    proyectoConfiguraciones->setConfiguraciones(*lista);

}

void ProyectoConfiguracionesDialog::fillUi()
{
    ui->idConfLineEdit->setText(proyectoConfiguracionesID);
    ui->descripcionTextEdit->setText(proyectoConfDescripcion);
}


bool ProyectoConfiguracionesDialog::canCloseProject()
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

QTreeWidgetItem *ProyectoConfiguracionesDialog::getConfItem() const
{
    return confItem;
}

void ProyectoConfiguracionesDialog::setConfItem(QTreeWidgetItem *newConfItem)
{
    confItem = newConfItem;
}

void ProyectoConfiguracionesDialog::addConfiguracionDialog()
{
    configuracionesDialog->newConfiguracion();
}

void ProyectoConfiguracionesDialog::saveXML()
{
    if(proyectoConfiguracionesID == tr("Nueva Configuracion") || proyectoConfiguracionesID.trimmed().isEmpty())
    {
        QMessageBox::critical(this, tr("Saving project"), tr("Project ID not valid"), QMessageBox::Ok);
        return;
    }
    save();
    QDomDocument doc(ProyectoConfiguraciones::XMLDocName());
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    QDomElement root = doc.createElement("proyectoConfiguraciones");
    doc.appendChild(root);
    proyectoConfiguraciones->writeToXml(doc, root);

    QString projectFilename = "configuracionesMIDI.xml";
    QString projectsDirectory = "/home/felix/Documentos/";
    QFileInfo fileInfo = QFileInfo(QDir(projectsDirectory), projectFilename);
    QString filename = fileInfo.absoluteFilePath();
    QString content = doc.toString(4);
    QString sender = tr("Save project");
    QString type_file = "";
    QString extensions = "xml";
    XMLOperator* xml_operator = XMLOperator::instance();
    bool change_name = true;
    xml_operator->saveDocument(filename, content, type_file, extensions, sender, change_name);

    QMessageBox::information(this, "Project saved", "Project saved", QMessageBox::Ok);
}

void ProyectoConfiguracionesDialog::loadXML()
{
    QTreeWidgetItem *selectedItem = ui->qewListViewConf->currentItem();

        if (selectedItem) {
            // Deseleccionar el elemento
            ui->qewListViewConf->setCurrentItem(nullptr);
            selectedItem->setSelected(false);
        }
    XMLOperator* xml_operator = XMLOperator::instance();
    QString filename = "/home/felix/Documentos/configuracionesMIDI.xml";
    if(filename.isEmpty())
        filename = QDir::homePath();
    QString docName = ProyectoConfiguraciones::XMLDocName();
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
    if(e_root.tagName() != "proyectoConfiguraciones")
        return;
    proyectoFileName = filename;
    if(proyectoConfiguraciones)
        delete proyectoConfiguraciones;
    proyectoConfiguraciones = new ProyectoConfiguraciones;
    proyectoConfiguraciones->setXMLElement(e_root);

    setProyectoConfiguraciones(proyectoConfiguraciones);
}

void ProyectoConfiguracionesDialog::newProyectoConfiguraciones()
{
    ui->idConfLineEdit->setEnabled(true);
    ui->descripcionTextEdit->setEnabled(true);
    proyectoConfiguraciones = new ProyectoConfiguraciones();
    setProyectoConfiguraciones(proyectoConfiguraciones);
}

Ui::ProyectoConfiguracionesDialog *ProyectoConfiguracionesDialog::getUi() const
{
    return ui;
}

void ProyectoConfiguracionesDialog::setUi(Ui::ProyectoConfiguracionesDialog *newUi)
{
    ui = newUi;
}
/*void ProyectoConfiguracionesDialog::on_pushButton_Dir_clicked()
{
    proyectoConfiguraciones->setInstrumentosFile(openDirectoryDialog());
    if (!proyectoConfiguraciones->getInstrumentosFile().isEmpty()) {
        //qDebug() << "Selected Directory:" << proyectoConfiguraciones->getInstrumentosFile();
        // Aquí puedes utilizar la ruta como necesites
    }
}

QString ProyectoConfiguracionesDialog::openDirectoryDialog()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    return directory;
}*/
