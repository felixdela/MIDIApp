#include "seleccioncanaldialog.h"
#include "ui_seleccioncanaldialog.h"
#include <QMessageBox>
SeleccionCanalDialog::SeleccionCanalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeleccionCanalDialog)
{
    ui->setupUi(this);
}

SeleccionCanalDialog::~SeleccionCanalDialog()
{
    delete ui;
}

void SeleccionCanalDialog::setInstrumentos(const QList<Instrumento *> &instrumentos)
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
        //Crear los elementos instrumento-canal en la ventana emergente según el número de instrumentos que haya
        for (Instrumento* instrumento : instrumentos) {
            QWidget* widget = new QWidget(this);
            QHBoxLayout* hLayout = new QHBoxLayout(widget);

            QLabel* label = new QLabel(instrumento->getId(), widget);
            QLabel* canalText = new QLabel("Canal:", widget);
            QComboBox* comboBox = new QComboBox(widget);
            for (int i = 1; i <= 16; ++i) {
                comboBox->addItem(QString::number(i));
            } // Añadiendo los 16 canales disponibles al QComboBox

            QHBoxLayout* canalLayout = new QHBoxLayout();
            canalLayout->addWidget(canalText);
            canalLayout->addWidget(comboBox);

            hLayout->addWidget(label);
            hLayout->addLayout(canalLayout);

            layout->addWidget(widget);
            comboBoxes.append(comboBox);
        }
}

QList<QString> SeleccionCanalDialog::getSelectedCanales() const
{
    QList<QString> selectedCanales;
        for (QComboBox* comboBox : comboBoxes) {
            selectedCanales.append(comboBox->currentText());
        }
        return selectedCanales;
}

void SeleccionCanalDialog::accept()
{
    if (validateUniqueCanales()) {
            QDialog::accept();
        } else {
            QMessageBox::warning(this, tr("Selección inválida"), tr("No se puede seleccionar el mismo canal para dos instrumentos diferentes. Por favor, seleccione canales únicos para cada instrumento."));
        }
}

bool SeleccionCanalDialog::validateUniqueCanales() const
{
    QSet<QString> uniqueCanales;
        for (QComboBox* comboBox : comboBoxes) {
            QString selectedCanal = comboBox->currentText();
            if (uniqueCanales.contains(selectedCanal)) {
                return false; // Canal duplicado encontrado
            }
            uniqueCanales.insert(selectedCanal);
        }
        return true; // Todos los canales son únicos
}
