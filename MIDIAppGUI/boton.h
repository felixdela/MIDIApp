#ifndef BOTON_H
#define BOTON_H


#include <xmlwritable.h>
class Boton : public XMLWritable
{
public:
    Boton();
    virtual ~Boton();
    void writeToXml(QDomDocument& doc, QDomElement& e);
    QString generateHtml(int prevIndent) const{};
    QString getNumBoton() const;
    void setNumBoton(const QString &newNumBoton);

    QList<QString> getFunciones() const;
    void setFunciones(const QList<QString> &newFunciones);
    void addFuncion(const QString &newFuncion);
protected:
    void readFromXML(QDomElement& e);
    void write_constructor(QDomElement& /*e*/) {}
protected:
    QString numBoton;
    QList<QString> funciones;
};

#endif // BOTON_H
