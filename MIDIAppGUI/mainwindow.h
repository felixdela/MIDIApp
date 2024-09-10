#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qewtreeviewdialog.h>
#include <QMainWindow>
#include <QTreeWidgetItem> // Incluimos QTreeWidgetItem
#include "preferencesdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Configuracion;
class ProyectoConfiguracionesDialog;
class ConfiguracionDialog;
class ProyectoInstrumentosDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_actionNuevoProyectoInst_triggered();
    void on_actionNuevoProyectoConf_triggered();
    void on_actionNuevoInst_triggered();
    void on_actionNuevaConf_triggered();
    void on_actionMostrarConf_triggered();
    void on_actionMostrarInst_triggered();
    void on_actionCargarInst_triggered();
    void on_actionCargarConf_triggered();
    void on_actionGuardarInst_triggered();
    void on_actionGuardarConf_triggered();
    void on_actionArduino_CLI_Path_triggered();

private:
    void hideAllActions();
    void readSettings();

private:
    Ui::MainWindow *ui;
    Configuracion* configuracion;
    ConfiguracionDialog* configuracionDialog;
    ProyectoConfiguracionesDialog* proyectoConfiguracionesDialog;
    ProyectoInstrumentosDialog* proyectoInstrumentosDialog;
    PreferencesDialog* preferencesDialog;

public:
    QString arduinoCLIPath;



    QString getArduinoCLIPath() const;
    void setArduinoCLIPath(const QString &newArduinoCLIPath);
};

#endif // MAINWINDOW_H
