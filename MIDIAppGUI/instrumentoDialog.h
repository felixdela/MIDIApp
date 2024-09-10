#ifndef INSTRUMENTODIALOG_H
#define INSTRUMENTODIALOG_H

#include <qewtreeviewdialog.h>
#include "allbancosdialog.h"
#include "instrumento.h"
//#include "xmlwritable.h"
#include <QButtonGroup>

namespace Ui {
class InstrumentoDialog;
}

class InstrumentoDialog : public QewTreeViewDialog//, public XMLWritable
{
    Q_OBJECT

public:
    explicit InstrumentoDialog(QWidget *parent = nullptr);
    ~InstrumentoDialog();

    bool setUpDialog();
    Instrumento *getInstrumento() const;
    void setInstrumento(Instrumento *instrumento);
    void setBancosDialog(AllBancosDialog *newBancosDialog);
    void setNewInst(bool newNewInst);
    //QString generateHtml(int /*prevIndent*/) const {return "";}

    QString getNomenclatura() const;
    void setNomenclatura(const QString &newNomenclatura);

protected:
    QString instrumentoID;
    QString prevId;
    QString canal;
    QString nomenclatura;
protected:
    void write_constructor(QDomElement& ){};
    void saveThis();
    void fillUi();

    void cleanProject();
signals:
    void sendIdChanged(QTreeWidgetItem*, QString);
private slots:
    void setInstrumentoID();
    void editingFinished();
    void on_pushButton_Banco_clicked();
    void changeCurrentItemId(QTreeWidgetItem *item, QString newId);
    void addBancosDialog();
private:
    Ui::InstrumentoDialog *ui;
    Instrumento *instrumento;
    QTreeWidgetItem* bancItem;
    QTreeWidgetItem* bancosItem;
    QTreeWidgetItem* instrumentoItem;
    QTreeWidgetItem* dataItem;
    QTreeWidgetItem* prevCurrentItem;
    BancoDialog* bancoDialog;
    QString titleBancos;
    AllBancosDialog* bancosDialog;
    bool newInst;

};

#endif // INSTRUMENTODIALOG_H
