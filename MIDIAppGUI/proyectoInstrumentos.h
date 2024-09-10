#ifndef PROYECTOINSTRUMENTOS_H
#define PROYECTOINSTRUMENTOS_H

#include "instrumento.h"
#include <xmlwritable.h>
class ProyectoInstrumentos : public XMLWritable
{
public:
    explicit ProyectoInstrumentos();
    ~ProyectoInstrumentos();

    Instrumento* getInstrumento(QString id);
    QList<Instrumento*> *getInstrumentos();
    void setInstrumentos(const QList<Instrumento *> &newInstrumentos);
    QString getProyectoID() const;
    void setProyectoID(const QString &newProyectoID);
    QString getDescripcion() const;
    void setDescripcion(const QString &newDescripcion);
    void addInstrumento(Instrumento* instrumento);
    void delInstrumento(Instrumento* instrumento);


    void writeToXml(QDomDocument &doc, QDomElement &e);
    QString generateHtml(int prevIndent)const override{return "";} ;
    static QString XMLDocName();

protected:
    void clearLists();
    void write_constructor(QDomElement& e){};
    void readFromXML(QDomElement &e);
protected:

    QList<Instrumento*> instrumentos;
    QString proyectoID;
    QString descripcion;
};

#endif // PROYECTOINSTRUMENTOS_H
