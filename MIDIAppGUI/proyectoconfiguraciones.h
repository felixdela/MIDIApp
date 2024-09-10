#ifndef PROYECTOCONFIGURACIONES_H
#define PROYECTOCONFIGURACIONES_H

#include <xmlwritable.h>
#include "configuracion.h"
class ProyectoConfiguraciones : public XMLWritable
{
public:
    explicit ProyectoConfiguraciones();
    ~ProyectoConfiguraciones();

    QList<Configuracion *> *getConfiguraciones();
    void setConfiguraciones(const QList<Configuracion *> &newConfiguraciones);

    QString getProyectoID() const;
    void setProyectoID(const QString &newProyectoID);

    QString getDescripcion() const;
    void setDescripcion(const QString &newDescripcion);

    void writeToXml(QDomDocument &doc, QDomElement &e);
    QString generateHtml(int prevIndent)const override{return "";} ;
    static QString XMLDocName();

    static void setAa(const QString &newAa);

protected:
    void clearLists();
    void write_constructor(QDomElement& e){};
    void readFromXML(QDomElement &e);
signals:

protected:
    QList<Configuracion*> configuraciones;
    QString proyectoID;
    QString descripcion;
};

#endif // PROYECTOCONFIGURACIONES_H
