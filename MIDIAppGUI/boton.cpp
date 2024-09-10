#include "boton.h"

Boton::Boton()
{

}

Boton::~Boton()
{

}

void Boton::writeToXml(QDomDocument &doc, QDomElement &e)
{
    //qDebug("Creando XML botones");
    XMLOperator* xml_operator = XMLOperator::instance();
    QDomElement tag_boton = doc.createElement("boton");
    e.appendChild(tag_boton);
    tag_boton.appendChild(xml_operator->createTextElement(doc, "numboton", numBoton));
    QDomElement tag_listafunciones = doc.createElement("listafunciones");
    tag_boton.appendChild(tag_listafunciones);
    for(const auto &funcion: funciones) {
        QDomElement funcionElement = xml_operator->createTextElement(doc, "funcion", funcion);
        tag_listafunciones.appendChild(funcionElement);
    }
}

void Boton::readFromXML(QDomElement &e)
{
    XMLOperator* xml_operator = XMLOperator::instance();

    QDomElement e_numBoton = xml_operator->findTag("numboton", e);
    if(!e_numBoton.isNull())
        numBoton = e_numBoton.text();

    QDomElement e_listaFunciones = xml_operator->findTag("listafunciones", e);
    if(!e_listaFunciones.isNull()) {
        QDomNodeList funcionesList = e_listaFunciones.elementsByTagName("funcion");

        for (int i = 0; i < funcionesList.count(); ++i) {
            QDomElement funcionElement = funcionesList.at(i).toElement();
            QString funcion = funcionElement.text();
            funciones.append(funcion);
        }
    }
}
QList<QString> Boton::getFunciones() const
{
    return funciones;
}

void Boton::setFunciones(const QList<QString> &newFunciones)
{
    funciones = newFunciones;
}

QString Boton::getNumBoton() const
{
    return numBoton;
}

void Boton::setNumBoton(const QString &newNumBoton)
{
    numBoton = newNumBoton;
}
void Boton::addFuncion(const QString &newFuncion)
{
    funciones.append(newFuncion);
}
