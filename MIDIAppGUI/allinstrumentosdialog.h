#ifndef ALLINSTRUMENTOSDIALOG_H
#define ALLINSTRUMENTOSDIALOG_H

#include "instrumento.h"
#include "instrumentoDialog.h"
#include <qewtreeviewdialog.h>
//#include "newinstrumentosdialog.h"

namespace Ui {
class AllInstrumentosDialog;
}

class AllInstrumentosDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit AllInstrumentosDialog(QWidget *parent = nullptr);
    ~AllInstrumentosDialog();

    void addExtensibleChild(QewExtensibleDialog* extensible_child, const QString& caption,
                            QTreeWidgetItem* parentItem = 0, int index = -1);
    bool setUpDialog();


    void newInstrumento();
    QList<InstrumentoDialog *> getDialogs() const;
    void removeDialogs();


    QList<Instrumento *> *getInstrumentos() const;
    void setInstrumentos(QList<Instrumento *> *newInstrumentos);
private:
    bool checkInstrumentoExists(QString id);

public slots:
    void getExternVal(const QString& var_name, const QVariant& var_val);

private:
    Ui::AllInstrumentosDialog *ui;

protected:

    virtual Instrumento* createNewInstrumento(const QString& id);
    void addInstrumento(Instrumento *instrumento, bool newInst = false);
    virtual InstrumentoDialog* getDialog();

signals:
    void sendIdChanged(QTreeWidgetItem*, QString);

protected:
    QList<Instrumento*>* instrumentos;
    QList<InstrumentoDialog*> dialogs;

};

#endif // ALLINSTRUMENTOSDIALOG_H
