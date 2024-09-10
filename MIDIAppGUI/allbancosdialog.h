#ifndef ALLBANCOSDIALOG_H
#define ALLBANCOSDIALOG_H
#include "banco.h"
#include "bancodialog.h"
#include <qewtreeviewdialog.h>
#include "instrumento.h"
//#include "newbancodialog.h"
namespace Ui {
class AllBancosDialog;
}

class AllBancosDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit AllBancosDialog(QWidget *parent = nullptr);
    ~AllBancosDialog();

    void addExtensibleChild(QewExtensibleDialog* extensible_child, const QString& caption,
                            QTreeWidgetItem* parentItem = 0, int index = -1);
    bool setUpDialog();

    void newBanco(QString nomenclatura);

    QList<BancoDialog *> getDialogs() const;
    void removeDialogs();

    QList<Banco *> *getBancos() const;
    void setBancos(QList<Banco *> *newBancos);

public slots:
    void getExternVal(const QString& var_name, const QVariant& var_val);
protected:

    virtual Banco* createNewBanco(const QString& id, int& numProgramas, QString nomenclatura);
    void addBanco(Banco *banco);
    virtual BancoDialog* getDialog();

private:
    Ui::AllBancosDialog *ui;
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);

protected:
    QList<Banco*>* bancos;
    QList<BancoDialog*> dialogs;

};

#endif // ALLBANCOSDIALOG_H
