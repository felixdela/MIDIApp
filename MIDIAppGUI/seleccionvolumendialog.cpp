#include "seleccionvolumendialog.h"
#include "ui_seleccionvolumendialog.h"
#include <QMessageBox>

SeleccionVolumenDialog::SeleccionVolumenDialog(Configuracion *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeleccionVolumenDialog),
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

    connect(ui->okButton, &QPushButton::clicked, this, &SeleccionVolumenDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SeleccionVolumenDialog::reject);

    // Conectar la señal valueChanged del QSlider a una ranura para actualizar el QLabel
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &SeleccionVolumenDialog::updateVolumeLabel);

    // Inicializar el QLabel con el valor inicial del QSlider
    updateVolumeLabel(ui->volumeSlider->value());
}

SeleccionVolumenDialog::~SeleccionVolumenDialog()
{
    delete ui;
}

void SeleccionVolumenDialog::accept()
{
    QTreeWidgetItem *selectedItem = ui->instrumentosTreeWidget->currentItem();
    if (selectedItem) {
        // El elemento seleccionado es un instrumento
        Instrumento *selectedInstrumento = selectedItem->data(0, Qt::UserRole).value<Instrumento*>();

        if (selectedInstrumento) {
            int volumen = ui->volumeSlider->value(); // Obtener el valor del volumen
            // Emitir señal con los datos del instrumento y volumen seleccionados
            emit instrumentoYVolumenSeleccionado(selectedInstrumento->getId(), volumen);
        } else {
            QMessageBox::warning(this, tr("Selección incompleta"), tr("Debe seleccionar un instrumento."));
            return; // No cerrar el diálogo si la selección está incompleta
        }
    }

    QDialog::accept();
}

void SeleccionVolumenDialog::updateVolumeLabel(int value)
{
    ui->label_volumen->setText(QString::number(value));
}
