#ifndef ALLBOTONESDIALOG_H
#define ALLBOTONESDIALOG_H
#include "boton.h"
#include "botondialog.h"
#include "qewtreeviewdialog.h"
#include "configuracion.h"
namespace Ui {
class AllBotonesDialog;
}

class AllBotonesDialog : public QewTreeViewDialog
{
    Q_OBJECT

public:
    explicit AllBotonesDialog(Configuracion *config, QWidget *parent = nullptr);
    ~AllBotonesDialog();

    void addExtensibleChild(QewExtensibleDialog* extensible_child, const QString& caption,
                            QTreeWidgetItem* parentItem = 0, int index = -1);

    bool setUpDialog();

    void newBoton(int index);

    QList<BotonDialog *> getDialogs() const;
    void removeDialogs();

    QList<Boton *> *getBotones() const;
    void setBotones(QList<Boton *> *newBotones);

public slots:

    void getExternVal(const QString& var_name, const QVariant& var_val);
protected:
    virtual Boton* createNewBoton(int index);
    void addBoton(Boton *boton);
    virtual BotonDialog* getDialog(Boton *boton);
private:
    Ui::AllBotonesDialog *ui;
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);
protected:
    QList<Boton*>* botones;
    QList<BotonDialog*> dialogs;
    Configuracion *configuracion;
};

#endif // ALLBOTONESDIALOG_H
