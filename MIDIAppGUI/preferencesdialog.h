#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include <QLineEdit>
namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();
    const QString &getArduinoCLIPath() const;
    void setArduinoCLIPath(const QString &newArduinoCLIPath);
protected:
    void changeEvent(QEvent *e) override;
    void elegirPath(QString& dirname, QString sender, QLineEdit* lineEdit);
protected slots:
    void elegirArduinoCLIPath();
    void accept() override;
private:
    Ui::PreferencesDialog *ui;
protected:
    QString arduinoCLIPath;
};

#endif // PREFERENCESDIALOG_H
