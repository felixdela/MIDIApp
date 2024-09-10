#include "botondialog.h"
#include "ui_botondialog.h"
#include "seleccioninstrumentosdialog.h"
#include "seleccionvolumendialog.h"
#include "seleccionpanningdialog.h"
#include <QMessageBox>
BotonDialog::BotonDialog(Configuracion *config, Boton *boton, QWidget *parent) :    QewExtensibleDialog(parent),
    ui(new Ui::BotonDialog),
    boton(boton),
    configuracion(config)
{
    ui->setupUi(this);
    connect(ui->pushButton_BYP, &QPushButton::clicked, this, &BotonDialog::showSeleccionInstrumentosDialog);
    connect(ui->pushButton_Volumen, &QPushButton::clicked, this, &BotonDialog::showSeleccionVolumenDialog);
    connect(ui->pushButton_Panning,  &QPushButton::clicked, this, &BotonDialog::showSeleccionPanningDialog);
    connect(ui->pushButton_Eliminar, &QPushButton::clicked, this, &BotonDialog::onRemoveFunction);
    connect(ui->pushButton_Ascender, &QPushButton::clicked, this, &BotonDialog::onMoveFunctionUp);
    connect(ui->pushButton_Descender, &QPushButton::clicked, this, &BotonDialog::onMoveFunctionDown);
    ui->pushButton_Ascender->setIcon(QIcon(":/resources/img/flecha_arriba.png"));
    ui->pushButton_Descender->setIcon(QIcon(":/resources/img/flecha_abajo.png"));
    ui->pushButton_Eliminar->setIcon(QIcon(":/resources/img/menos.png"));

}

BotonDialog::~BotonDialog()
{
    delete ui;
}

bool BotonDialog::setUpDialog()
{
    bool ret = QewExtensibleDialog::setUpDialog();
    return ret;
}

Boton *BotonDialog::getBoton() const
{
    return boton;
}

void BotonDialog::setBoton(Boton *newBoton)
{
    boton = newBoton;
    numBoton = boton->getNumBoton();
    funciones = boton->getFunciones();

    fillUi();
}
void BotonDialog::saveThis()
{
    boton->setFunciones(getFuncionesFromListWidget());
}

void BotonDialog::fillUi()
{
    ui->listWidgetFunciones->clear();
    for(const QString& item : funciones)
    {
        ui->listWidgetFunciones->addItem(item);
    }
}
Configuracion *BotonDialog::getConfiguracion() const
{
    return configuracion;
}

void BotonDialog::setConfiguracion(Configuracion *newConfiguracion)
{
    configuracion = newConfiguracion;
}


QStringList BotonDialog::getFuncionesFromListWidget() const
{
    QStringList funciones;
    for (int i = 0; i < ui->listWidgetFunciones->count(); ++i) {
        QListWidgetItem *item = ui->listWidgetFunciones->item(i);
        if (item) {
            funciones << item->text();
        }
    }
    return funciones;
}

void BotonDialog::agregarInstrumentoEnLista(const QString &message)
{
    // Formatear la línea de código usando los argumentos proporcionados
    QString lineaCodigo = QString("BYP|%1").arg(message);

    // Crear un nuevo QListWidgetItem con la línea de código formateada
    QListWidgetItem *item = new QListWidgetItem(lineaCodigo);

    // Añadir la línea de código a la lista de funciones del botón
    //boton->addFuncion(lineaCodigo);

    // Añadir el nuevo item al QListWidget de la interfaz de usuario
    ui->listWidgetFunciones->addItem(item);
}
void BotonDialog::agregarVolumenEnLista(const QString &instrumentoId, int volumen)
{
    // Aquí puedes manejar la selección del instrumento y el volumen
    //qDebug() << "Instrumento:" << instrumentoId << ", Volumen:" << volumen;

    // Formatear la línea de texto para la lista de funciones
    QString itemText = QString("VOL|%1: Volumen = %2")
                        .arg(instrumentoId)
                        .arg(volumen);
    ui->listWidgetFunciones->addItem(itemText);
}

void BotonDialog::agregarPanningEnLista(const QString &instrumentoId, int panning)
{
    // Aquí puedes manejar la selección del instrumento y el panning
        //qDebug() << "Instrumento:" << instrumentoId << ", Panning:" << panning;

        // Formatear la línea de texto para la lista de funciones
        QString itemText = QString("PAN|%1: Panning = %2")
                            .arg(instrumentoId)
                            .arg(panning);
        ui->listWidgetFunciones->addItem(itemText);
}
void BotonDialog::showSeleccionInstrumentosDialog()
{
    setConfiguracion(this->getConfiguracion());
        if (configuracion) {
            SeleccionInstrumentosDialog dialog(configuracion, this);
            connect(&dialog, &SeleccionInstrumentosDialog::instrumentoSeleccionado, this, &BotonDialog::agregarInstrumentoEnLista);
            dialog.exec();
        }
}


void BotonDialog::showSeleccionVolumenDialog()
{
    setConfiguracion(this->getConfiguracion());
    if (configuracion) {
        SeleccionVolumenDialog dialog(configuracion, this);
        connect(&dialog, &SeleccionVolumenDialog::instrumentoYVolumenSeleccionado, this, &BotonDialog::agregarVolumenEnLista);
        dialog.exec();
    }
}

void BotonDialog::showSeleccionPanningDialog()
{
    setConfiguracion(this->getConfiguracion());
        if (configuracion) {
            SeleccionPanningDialog dialog(configuracion, this);
            connect(&dialog, &SeleccionPanningDialog::instrumentoYPanningSeleccionado, this, &BotonDialog::agregarPanningEnLista);
            dialog.exec();
        }
}
void BotonDialog::onRemoveFunction()
{
    QListWidgetItem *item = ui->listWidgetFunciones->currentItem();
    if (item) {
        delete ui->listWidgetFunciones->takeItem(ui->listWidgetFunciones->row(item));
    } else {
        QMessageBox::warning(this, tr("Advertencia"), tr("Por favor, seleccione una función para eliminar."));
    }
}

void BotonDialog::onMoveFunctionUp()
{
    int currentIndex = ui->listWidgetFunciones->currentRow();
    if (currentIndex > 0) {
        QListWidgetItem *currentItem = ui->listWidgetFunciones->takeItem(currentIndex);
        ui->listWidgetFunciones->insertItem(currentIndex - 1, currentItem);
        ui->listWidgetFunciones->setCurrentRow(currentIndex - 1);
    }
}

void BotonDialog::onMoveFunctionDown()
{
    int currentIndex = ui->listWidgetFunciones->currentRow();
    if (currentIndex < ui->listWidgetFunciones->count() - 1) {
        QListWidgetItem *currentItem = ui->listWidgetFunciones->takeItem(currentIndex);
        ui->listWidgetFunciones->insertItem(currentIndex + 1, currentItem);
        ui->listWidgetFunciones->setCurrentRow(currentIndex + 1);
    }
}


