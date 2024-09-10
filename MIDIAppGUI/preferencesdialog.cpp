#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <xmloperator.h>
PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    connect(ui->toolButtonArduinoCLIPath, SIGNAL(clicked(bool)), this, SLOT(elegirArduinoCLIPath()));

}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

const QString &PreferencesDialog::getArduinoCLIPath() const
{
    return arduinoCLIPath;
}

void PreferencesDialog::setArduinoCLIPath(const QString &newArduinoCLIPath)
{
    arduinoCLIPath = newArduinoCLIPath;
    ui->lineEditArduinoCLIPAth->setText(arduinoCLIPath);
}

void PreferencesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PreferencesDialog::elegirPath(QString &filename, QString sender, QLineEdit *lineEdit)
{
    XMLOperator* xml_operator = XMLOperator::instance();
    QString type_file = "";
    QString extensions = "";
    QString file = filename;
    bool ret = xml_operator->getFileDialog(file, sender, QFileDialog::AcceptOpen,
                                               QFileDialog::AnyFile, type_file, extensions);
    if(ret) {
        if(filename != file.trimmed()) {
            filename = file.trimmed();
            lineEdit->setText(filename);
        }
    }
}

void PreferencesDialog::elegirArduinoCLIPath()
{
    elegirPath(arduinoCLIPath, tr("arduino CLI directory path"), ui->lineEditArduinoCLIPAth);

}

void PreferencesDialog::accept()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    settings.setValue("arduinoCLIPath", arduinoCLIPath);
    QDialog::accept();
}
