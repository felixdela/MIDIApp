#include "seleccioninstrumentosdialog.h"
#include "ui_seleccioninstrumentosdialog.h"
#include <QMessageBox>
SeleccionInstrumentosDialog::SeleccionInstrumentosDialog(Configuracion *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeleccionInstrumentosDialog)
{
    ui->setupUi(this);

        // Llenar el QTreeWidget con instrumentos, bancos y programas
        QList<Instrumento*> instrumentos = config->getInstrumentos();
        for (Instrumento* instrumento : instrumentos) {
            QTreeWidgetItem *instrumentItem = new QTreeWidgetItem(ui->instrumentosTreeWidget);
            instrumentItem->setText(0, instrumento->getId());
            instrumentItem->setData(0, Qt::UserRole, QVariant::fromValue(instrumento));
            //qDebug() << "Añadiendo instrumento:" << instrumento->getId();

            QList<Banco*> *bancos = instrumento->getBancos();
            for (Banco* banco : *bancos) {
                QTreeWidgetItem *bancoItem = new QTreeWidgetItem(instrumentItem);
                bancoItem->setText(0, banco->getNumBanco());
                bancoItem->setData(0, Qt::UserRole, QVariant::fromValue(banco));
                //qDebug() << "  Añadiendo banco:" << banco->getNumBanco();

                QList<QPair<int, QString>> programas = banco->getProgramas();
                for (const auto &programa : programas) {
                    //qDebug() << "    Programa clave:" << programa.first << ", nombre:" << programa.second;
                    if (!programa.second.isEmpty()) { // Verifica que el nombre del programa no esté vacío
                        QTreeWidgetItem *programaItem = new QTreeWidgetItem(bancoItem);
                        programaItem->setText(0, programa.second); // Nombre del programa
                        programaItem->setData(0, Qt::UserRole, programa.first); // Clave del programa
                        //qDebug() << "      Añadiendo programa:" << programa.second;
                    }
                }
            }
        }

        connect(ui->okButton, &QPushButton::clicked, this, &SeleccionInstrumentosDialog::accept);
        connect(ui->cancelButton, &QPushButton::clicked, this, &SeleccionInstrumentosDialog::reject);


}

SeleccionInstrumentosDialog::~SeleccionInstrumentosDialog()
{
    delete ui;
}

void SeleccionInstrumentosDialog::accept()
{
    QTreeWidgetItem *selectedItem = ui->instrumentosTreeWidget->currentItem();
    if (selectedItem) {
        QString selectedPrograma;
        int programaClave = -1;
        Banco *selectedBanco = nullptr;
        Instrumento *selectedInstrument = nullptr;

        // Verificar si el elemento seleccionado es un programa, banco o instrumento
        if (selectedItem->parent() != nullptr) {
            QTreeWidgetItem *parentItem = selectedItem->parent();
            if (parentItem->parent() != nullptr) {
                // El elemento seleccionado es un programa
                selectedPrograma = selectedItem->text(0);
                programaClave = selectedItem->data(0, Qt::UserRole).toInt();  // Obtener el número del programa
                selectedBanco = parentItem->data(0, Qt::UserRole).value<Banco*>();
                selectedInstrument = parentItem->parent()->data(0, Qt::UserRole).value<Instrumento*>();
            } else {
                // El elemento seleccionado es un banco
                selectedBanco = selectedItem->data(0, Qt::UserRole).value<Banco*>();
                selectedInstrument = parentItem->data(0, Qt::UserRole).value<Instrumento*>();
            }
        } else {
            // El elemento seleccionado es un instrumento
            selectedInstrument = selectedItem->data(0, Qt::UserRole).value<Instrumento*>();
        }

        if (selectedInstrument && selectedBanco && !selectedPrograma.isEmpty() && programaClave != -1) {
            // Formatear el mensaje con número del programa y nombre del programa
            QString message = QString("%1: Cambio al programa %2: %3 del banco %4")
                                .arg(selectedInstrument->getId())
                                .arg(programaClave)
                                .arg(selectedPrograma)
                                .arg(selectedBanco->getNumBanco());

            // Emitir señal con el mensaje formateado
            emit instrumentoSeleccionado(message);
        } else {
            QMessageBox::warning(this, tr("Selección incompleta"), tr("Debe seleccionar un programa."));
            return; // No cerrar el diálogo si la selección está incompleta
        }
    }

    QDialog::accept();
}
