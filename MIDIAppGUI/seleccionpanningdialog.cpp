#include "seleccionpanningdialog.h"
#include "ui_seleccionpanningdialog.h"
#include <QMessageBox>

SeleccionPanningDialog::SeleccionPanningDialog(Configuracion *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeleccionPanningDialog),
    config(config)
{
    ui->setupUi(this);

    // Llenar el QTreeWidget solo con instrumentos
    QList<Instrumento*> instrumentos = config->getInstrumentos();
    for (Instrumento* instrumento : instrumentos) {
        QTreeWidgetItem *instrumentItem = new QTreeWidgetItem(ui->instrumentosTreeWidget);
        instrumentItem->setText(0, instrumento->getId());
        instrumentItem->setData(0, Qt::UserRole, QVariant::fromValue(instrumento));
        //qDebug() << "Añadiendo instrumento:" << instrumento->getId();
    }

    connect(ui->okButton, &QPushButton::clicked, this, &SeleccionPanningDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SeleccionPanningDialog::reject);

    // Conectar la señal valueChanged del QSlider a una ranura para actualizar el QLabel
    connect(ui->panningSlider, &QSlider::valueChanged, this, &SeleccionPanningDialog::updatePanningLabel);

    // Inicializar el QLabel con el valor inicial del QSlider
    updatePanningLabel(ui->panningSlider->value());
}

SeleccionPanningDialog::~SeleccionPanningDialog()
{
    delete ui;
}

void SeleccionPanningDialog::accept()
{
    QTreeWidgetItem *selectedItem = ui->instrumentosTreeWidget->currentItem();
    if (selectedItem) {
        // El elemento seleccionado es un instrumento
        Instrumento *selectedInstrumento = selectedItem->data(0, Qt::UserRole).value<Instrumento*>();

        if (selectedInstrumento) {
            int panning = ui->panningSlider->value(); // Obtener el valor del panning
            // Emitir señal con los datos del instrumento y panning seleccionados
            emit instrumentoYPanningSeleccionado(selectedInstrumento->getId(), panning);
        } else {
            QMessageBox::warning(this, tr("Selección incompleta"), tr("Debe seleccionar un instrumento."));
            return; // No cerrar el diálogo si la selección está incompleta
        }
    }

    QDialog::accept();
}

void SeleccionPanningDialog::updatePanningLabel(int value)
{
    ui->label_panning->setText(QString::number(value));
}
