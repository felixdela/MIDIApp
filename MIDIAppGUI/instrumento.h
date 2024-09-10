#ifndef INSTRUMENTO_H
#define INSTRUMENTO_H

#include "banco.h"
#include "qtreewidget.h"
#include <QString>
#include <QList>
#include <xmlwritable.h>

class Instrumento : public XMLWritable{

public:
    Instrumento();
    virtual ~Instrumento();

    void writeToXml(QDomDocument& doc, QDomElement& e);
    QString generateHtml(int prevIndent) const{};

    Banco* getBanco(QString id);

    QString getId() const;
    void setId(const QString &newId);
    QString getCanal() const;
    void setCanal(const QString &newCanal);
    QString getNomenclatura() const;
    void setNomenclatura(const QString &newNomenclatura);
    QList<Banco *>* getBancos();
    void setBancos(const QList<Banco *> &newBancos);
    //public para poder accederse desde configuracionDialog
    void readFromXML(QDomElement& e);
protected:
    void clearLists();

    void write_constructor(QDomElement& ){}

protected:
    QString id;
    QString canal;
    QString nomenclatura;
    QList <Banco*> bancos;
};

#endif // INSTRUMENTO_H
