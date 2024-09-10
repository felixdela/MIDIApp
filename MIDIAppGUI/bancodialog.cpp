#include "bancodialog.h"
#include "ui_bancodialog.h"
#include "newbancodialog.h"
#include <QInputDialog>
#include <QMessageBox>
BancoDialog::BancoDialog(QWidget *parent) :
    QewExtensibleDialog(parent),
    ui(new Ui::BancoDialog)
{
    ui->setupUi(this);
    //conectar la señal itemChanged al slot handleItemChanged para que al cambiar un objeto de la tabla se modifique la lista de programas
    connect(ui->tableProgWidget, &QTableWidget::itemChanged, this, &BancoDialog::handleItemChanged);
    connect(ui->lineEditNombre, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    connect(ui->pushButton_Aniadir, &QPushButton::clicked, this, &BancoDialog::addProgram);
    connect(ui->pushButton_Borrar, &QPushButton::clicked, this, &BancoDialog::removeProgram);
    connect(ui->pushButton_Ascender, &QPushButton::clicked, this, &BancoDialog::moveProgramUp);
    connect(ui->pushButton_Descender, &QPushButton::clicked, this, &BancoDialog::moveProgramDown);
    ui->pushButton_Ascender->setIcon(QIcon(":/resources/img/flecha_arriba.png"));
    ui->pushButton_Descender->setIcon(QIcon(":/resources/img/flecha_abajo.png"));
    ui->pushButton_Aniadir->setIcon(QIcon(":/resources/img/mas.png"));
    ui->pushButton_Borrar->setIcon(QIcon(":/resources/img/menos.png"));
}
BancoDialog::~BancoDialog()
{
    delete ui;
}

bool BancoDialog::setUpDialog()
{
    bool ret = QewExtensibleDialog::setUpDialog();
    NewBancoDialog newDialog(this);
    return ret;
}

Banco *BancoDialog::getBanco() const
{
    return banco;
}

void BancoDialog::setBanco(Banco *newBanco)
{
    banco = newBanco;
    numBanco = banco->getNumBanco();
    LSB = banco->getLSB();
    MSB = banco->getMSB();
    programas = &banco->getProgramas();
    fillUi();

}

void BancoDialog::saveThis()
{
    //qDebug("Saving BancoDialog data");
    numBanco = ui->lineEditNombre->text().trimmed();
    banco->setNumBanco(numBanco);
    LSB = ui->spinBoxLSB->text().trimmed();
    banco->setLSB(LSB);
    MSB = ui->spinBoxMSB->text().trimmed();
    banco->setMSB(MSB);
    // Limpiar la lista de programas antes de llenarla con nuevos datos
        programas->clear();
    // Obtener los datos del QTableWidget y almacenarlos en la lista de programas
    QTableWidget *tableWidget = ui->tableProgWidget;
    int rowCount = tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *item = tableWidget->item(row, 0); // Asumiendo que los datos están en la columna 0
        if (item) {
            programas->append(qMakePair(row + 1, item->text().trimmed())); // Añadiendo el número del programa como clave
        }
    }

/*TODO:
 * hacer que se pueda quitar y poner un programa hacer que reconozca los programas insertados por parámetro,
 * arreglar que salga projectsaved igualmente*
 * arreglar fillui para que salga el arbol*/
}

void BancoDialog::fillUi()
{
    ui->lineEditNombre->setText(numBanco);
    ui->spinBoxLSB->setValue(LSB.toInt());
    ui->spinBoxMSB->setValue(MSB.toInt());
    llenarTablaProgramas(ui->tableProgWidget);
}
void BancoDialog::llenarTablaProgramas(QTableWidget *tableWidget) {
    tableWidget->verticalHeader()->setVisible(false);  // Oculta el encabezado de las filas
    if (programas == nullptr)
            return;
    // Establecer el número de filas
    tableWidget->setRowCount(programas->size());
    //qDebug() << "tamaño:" << programas->size();
    // Llenar la columna "programa"
    for (int i = 0; i < programas->size(); ++i) {
            // Crear un nuevo item con el texto del nombre del programa
            QTableWidgetItem *item = new QTableWidgetItem(programas->at(i).second);
           //qDebug() << i;
            // Insertar el item en la primera columna (columna 0) de la fila i
            tableWidget->setItem(i, 0, item);
    }
}

void BancoDialog::handleItemChanged(QTableWidgetItem *item)
{
    int row = item->row();
        QString newName = item->text().trimmed();

        if (row >= 0 && row < programas->size()) {
            // Actualizar el nombre del programa en la lista de pares clave-valor
            programas->replace(row, qMakePair(programas->at(row).first, newName));
            //qDebug() << "Programa actualizado: clave =" << programas->at(row).first << ", nuevo nombre =" << newName;
        }
}

void BancoDialog::editingFinished()
{
    if(prevId != ui->lineEditNombre->text()) {
            emit sendIdChanged(getItemTree(), ui->lineEditNombre->text().trimmed());
            prevId = ui->lineEditNombre->text().trimmed();
            setWindowTitle(prevId);
            setDataChangedThis(true);
    }
}

void BancoDialog::addProgram() {
    // Crear un nuevo programa y añadirlo al final de la tabla
    bool ok;
    QString text = QInputDialog::getText(this, tr("Añadir Programa"),
                                         tr("Nombre del programa:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        int newRow = ui->tableProgWidget->rowCount();
        ui->tableProgWidget->insertRow(newRow);
        QTableWidgetItem *item = new QTableWidgetItem(text);
        ui->tableProgWidget->setItem(newRow, 0, item);
        // Añadir el nuevo programa a la lista de programas
        programas->append(qMakePair(newRow + 1, text));
    }
}

void BancoDialog::removeProgram() {
    // Eliminar el programa seleccionado
    int currentRow = ui->tableProgWidget->currentRow();
    if (currentRow != -1) {
        ui->tableProgWidget->removeRow(currentRow);
        // Eliminar el programa de la lista de programas
        programas->removeAt(currentRow);
    } else {
        QMessageBox::warning(this, tr("Eliminar Programa"),
                             tr("Por favor, selecciona un programa para eliminar."));
    }
}

void BancoDialog::moveProgramUp() {
    int currentRow = ui->tableProgWidget->currentRow();
    if (currentRow > 0) {
        // Intercambiar las filas en la tabla
        ui->tableProgWidget->insertRow(currentRow - 1);
        for (int i = 0; i < ui->tableProgWidget->columnCount(); ++i) {
            ui->tableProgWidget->setItem(currentRow - 1, i, ui->tableProgWidget->takeItem(currentRow + 1, i));
        }
        ui->tableProgWidget->removeRow(currentRow + 1);
        // Intercambiar los elementos en la lista de programas
        programas->swapItemsAt(currentRow, currentRow - 1);
        ui->tableProgWidget->setCurrentCell(currentRow - 1, 0);
    }
}

void BancoDialog::moveProgramDown() {
    int currentRow = ui->tableProgWidget->currentRow();
    if (currentRow < ui->tableProgWidget->rowCount() - 1) {
        // Intercambiar las filas en la tabla
        ui->tableProgWidget->insertRow(currentRow + 2);
        for (int i = 0; i < ui->tableProgWidget->columnCount(); ++i) {
            ui->tableProgWidget->setItem(currentRow + 2, i, ui->tableProgWidget->takeItem(currentRow, i));
        }
        ui->tableProgWidget->removeRow(currentRow);
        // Intercambiar los elementos en la lista de programas
        programas->swapItemsAt(currentRow, currentRow + 1);
        ui->tableProgWidget->setCurrentCell(currentRow + 1, 0);
    }
}

