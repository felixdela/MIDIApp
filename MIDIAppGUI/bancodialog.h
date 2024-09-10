#ifndef BANCODIALOG_H
#define BANCODIALOG_H

#include <qewextensibledialog.h>
#include "banco.h"
#include "qtablewidget.h"



namespace Ui {
class BancoDialog;
}

class BancoDialog : public QewExtensibleDialog
{
    Q_OBJECT

public:
    explicit BancoDialog(QWidget *parent = nullptr);
    ~BancoDialog();

    bool setUpDialog();
    Banco *getBanco() const;
    void setBanco(Banco *newBanco);
    QString generateHtml(int /*prevIndent*/) const;
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);
protected:
    void saveThis();
    void fillUi();
    void llenarTablaProgramas(QTableWidget *tableWidget);

protected:
    QString numBanco;
    QString MSB;
    QString LSB;
    QString prevId;
    QList<QPair<int, QString>> *programas; // Puntero a la lista de pares clave-valor
private slots:
    void handleItemChanged(QTableWidgetItem *item);
    void editingFinished();
    void addProgram();
    void removeProgram();
    void moveProgramUp();
    void moveProgramDown();
private:
    Ui::BancoDialog *ui;
    Banco *banco;

};

#endif // BANCODIALOG_H
