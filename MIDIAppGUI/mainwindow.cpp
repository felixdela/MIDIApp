#include "mainwindow.h"
#include "proyectoinstrumentosdialog.h"
#include "ui_mainwindow.h"
#include "proyectoconfiguracionesdialog.h"
#include <QMessageBox>
#include "configuraciondialog.h"
#include "preferencesdialog.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MidiAPP");
    QPixmap pixmap(":/resources/img/midiapp.jpeg"); // Ruta de la imagen
    QLabel *label = new QLabel(this); // Crear un QLabel
    label->setPixmap(pixmap); // Asignar el pixmap al QLabel
    label->setScaledContents(true); // Ajustar la imagen al tamaño del QLabel

    // Si deseas poner la imagen en el StackedWidget en el índice 0:
    ui->stackedWidget->insertWidget(0, label);
    ui->stackedWidget->setCurrentIndex(0);
    proyectoConfiguracionesDialog = new ProyectoConfiguracionesDialog(this);
    if(proyectoConfiguracionesDialog -> setUpDialog()){
            //
    }

    proyectoInstrumentosDialog = new ProyectoInstrumentosDialog(this);
    if(proyectoInstrumentosDialog -> setUpDialog())
    {
        //
    }
    ui->stackedWidget->addWidget(proyectoConfiguracionesDialog);
    ui->stackedWidget->addWidget(proyectoInstrumentosDialog);
    preferencesDialog = nullptr;
    readSettings();

}

void MainWindow::on_actionNuevaConf_triggered()
{
    proyectoConfiguracionesDialog->addConfiguracionDialog();
}

void MainWindow::on_actionMostrarConf_triggered()
{

    //configuracionDialog->exec();
    hideAllActions();
    ui->stackedWidget->setCurrentIndex(1);
    //ui->actionShow_projects->setText("Hide projects"); por si pantalla entrada

    //ui->actionDelete_project->setEnabled(!projectDialog->getSettingProject());

    ui->actionNuevoProyectoConf->setEnabled(true);

    //ui->actionSave_project->setEnabled(!projectDialog->getSettingProject());
    //ui->actionSave_project_as->setEnabled(!projectDialog->getSettingProject());


//    if(projectDialog->getSettingProject())
//        on_actionLoad_project_triggered();<
}
void MainWindow::on_actionMostrarInst_triggered()
{

    //configuracionDialog->exec();
    hideAllActions();
    ui->stackedWidget->setCurrentIndex(2);
    //ui->actionShow_projects->setText("Hide projects"); por si pantalla entrada

    //ui->actionDelete_project->setEnabled(!projectDialog->getSettingProject());
    ui->actionNuevoProyectoInst->setEnabled(true);

    //ui->actionSave_project->setEnabled(!projectDialog->getSettingProject());
    //ui->actionSave_project_as->setEnabled(!projectDialog->getSettingProject());


//    if(projectDialog->getSettingProject())
    //on_actionCargarInst_triggered();
}
void MainWindow::on_actionCargarInst_triggered()
{
    proyectoInstrumentosDialog->loadXML();
}

void MainWindow::on_actionCargarConf_triggered()
{
    proyectoConfiguracionesDialog->loadXML();
}

void MainWindow::on_actionGuardarInst_triggered()
{
    proyectoInstrumentosDialog->saveXML();
}

void MainWindow::on_actionGuardarConf_triggered()
{
    proyectoConfiguracionesDialog->saveXML();
}

void MainWindow::on_actionArduino_CLI_Path_triggered()
{
    preferencesDialog = new PreferencesDialog();
    preferencesDialog->setArduinoCLIPath(arduinoCLIPath);
    int result = preferencesDialog->exec();
    if(result == QDialog::Accepted)
        readSettings();
    delete preferencesDialog;
    preferencesDialog = nullptr;
}
void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    const QByteArray arduinoCLI = settings.value("arduinoCLIPath", QByteArray()).toByteArray();
    arduinoCLIPath = QString::fromStdString(arduinoCLI.toStdString());
    //Escribir código por si está vacío
    Configuracion::setArduinoCLIPath(arduinoCLIPath);
}
void MainWindow::on_actionNuevoInst_triggered()
{
    proyectoInstrumentosDialog->addInstrumentoDialog();
}
void MainWindow::hideAllActions()
{
//    ui->actionShow_projects->setText("Show projects");
//    ui->actionShow_sketches->setText("Show sketches");
//    ui->actionShow_subjects->setText("Show subjects");

    ui->actionNuevaConf->setEnabled(false);
    ui->actionCargarConf->setEnabled(false);
    ui->actionGuardarConf->setEnabled(false);

    ui->actionNuevoInst->setEnabled(false);
    ui->actionCargarInst->setEnabled(false);
    ui->actionGuardarInst->setEnabled(false);

    ui->actionAbrirConfigs->setEnabled(false);


}

QString MainWindow::getArduinoCLIPath() const
{
    return arduinoCLIPath;
}

void MainWindow::setArduinoCLIPath(const QString &newArduinoCLIPath)
{
    arduinoCLIPath = newArduinoCLIPath;
}
MainWindow::~MainWindow() {
    // Cualquier limpieza necesaria antes de que el objeto sea destruido
    delete ui; // Asumiendo que ui es un puntero a la interfaz de usuario
    // Si 'configuracion' y 'configuracionDialog' son objetos dinámicos, también deberías borrarlos aquí:
    //delete configuracionDialog;
    delete proyectoInstrumentosDialog;
}

void MainWindow::on_actionNuevoProyectoInst_triggered()
{
    proyectoInstrumentosDialog->newProyectoInstrumentos();
    /*ui->actionCargarInst->setEnabled(true);
    ui->actionGuardarInst->setEnabled(true);*/
    ui->actionNuevoInst->setEnabled(true);
    ui->actionCargarInst->setEnabled(true);
    ui->actionGuardarInst->setEnabled(true);
    ui->actionNuevoInst->setEnabled(true);
}

void MainWindow::on_actionNuevoProyectoConf_triggered()
{
    proyectoConfiguracionesDialog->newProyectoConfiguraciones();
    ui->actionCargarConf->setEnabled(true);
    ui->actionGuardarConf->setEnabled(true);
    ui->actionNuevaConf->setEnabled(true);
}


