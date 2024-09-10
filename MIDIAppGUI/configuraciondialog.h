#ifndef CONFIGURACIONDIALOG_H
#define CONFIGURACIONDIALOG_H

#include "qewtreeviewdialog.h"
#include "allbotonesdialog.h"
#include "configuracion.h"
#include "mainwindow.h"
namespace Ui {
class ConfiguracionDialog;
}

class ConfiguracionDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit ConfiguracionDialog(QWidget *parent = nullptr);
    ~ConfiguracionDialog();
    bool setUpDialog();

    Configuracion *getConfiguracion() const;
    void setConfiguracion(Configuracion *configuracion);

    void setBotones();
    void setNewConf(bool newNewConf);
    QString getArduinoCLIPath() const;
    void setArduinoCLIPath(const QString &newArduinoCLIPath);

protected:
    void saveThis();
    void fillUi();
    void cleanProject();
private:
    void obtenerInstrumentosMarcados(Configuracion* configuracion);
    void generarArchivoArduino();
    void compilarProyectoArduino();
    void cargarProyectoArduino();
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);
private slots:
    void editingFinished();
    void addBotonesDialog();
    void changeCurrentItemId(QTreeWidgetItem *item, QString newId);
    QList<Instrumento*> cargarNombresInstrumento();
    void initializeInstrumentList();
    //el tipo de señal QPushButton::clicked(bool) no puede ser conectado directamente a un slot que recibe un parámetro adicional como Configuracion*.
    //La solución es crear un nuevo slot que no tenga parámetros adicionales y que luego llame a obtenerInstrumentosMarcados con el parámetro adecuado.
    void obtenerInstrumentosMarcados();
    void addNewBotones();
protected slots:
    void chooseInstFile();

protected:
    QString configuracionID;
    QString prevId;
    QString descripcion;
    QString instFile;

private:
    MainWindow *mainwindow;
    Ui::ConfiguracionDialog *ui;
    Configuracion *configuracion;
    QTreeWidgetItem* botonItem;
    QTreeWidgetItem* botonesItem;
    QTreeWidgetItem* configuracionItem;
    QTreeWidgetItem* dataItem;
    QTreeWidgetItem* prevCurrentItem;
    QString titleBotones;
    AllBotonesDialog* botonesDialog;
    bool newConf;
    QList<Instrumento*> instrumentosCD;
    QString arduinoFile;
    QString arduinoCLIPath;
    QString proyectoPath;

};

#endif // CONFIGURACIONDIALOG_H
