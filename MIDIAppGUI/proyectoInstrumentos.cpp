#include "proyectoInstrumentos.h"
#include <QObject>
ProyectoInstrumentos::ProyectoInstrumentos()
{

}

ProyectoInstrumentos::~ProyectoInstrumentos()
{

}

QList<Instrumento *> *ProyectoInstrumentos::getInstrumentos()
{
    return &instrumentos;
}

void ProyectoInstrumentos::setInstrumentos(const QList<Instrumento *> &newInstrumentos)
{
    instrumentos = newInstrumentos;
}

QString ProyectoInstrumentos::getProyectoID() const
{
    return proyectoID;
}

void ProyectoInstrumentos::setProyectoID(const QString &newProyectoID)
{
    proyectoID = newProyectoID;
}

QString ProyectoInstrumentos::getDescripcion() const
{
    return descripcion;
}

void ProyectoInstrumentos::setDescripcion(const QString &newDescripcion)
{
    descripcion = newDescripcion;
}

void ProyectoInstrumentos::addInstrumento(Instrumento *instrumento)
{
    instrumentos.append(instrumento);
}

void ProyectoInstrumentos::delInstrumento(Instrumento *instrumento)
{

}
//Función que lee los elementos del XML pertenecientes a proyectoInstrumentos, y los almacena en sus variables.
void ProyectoInstrumentos::readFromXML(QDomElement& e)
{
    clearLists();
    XMLOperator* xml_operator = XMLOperator::instance();

    QDomElement e_id = xml_operator->findTag("proyectoID", e);
    if(!e_id.isNull())
        proyectoID = e_id.text();

    QDomElement e_descripcion = xml_operator->findTag("descripcion", e);
    if(!e_descripcion.isNull())
        descripcion = xml_operator->readMultiLineTextElement(e_descripcion);

    QDomElement e_instrumentos = xml_operator->findTag("instrumentos", e);
    QDomNodeList n_instrumentos = e_instrumentos.childNodes();
    int contador = n_instrumentos.count();
    QDomElement e_inst;
    for(int i = 0; i < contador; ++i){
        QDomNode n = n_instrumentos.item(i);
        e_inst = n.toElement();
        if (!e_inst.isNull())
        {
            Instrumento* instrumento = new Instrumento;
            instrumento->setXMLElement(e_inst);
            instrumentos.append(instrumento);
        }
    }
}

void ProyectoInstrumentos::writeToXml(QDomDocument &doc, QDomElement &e) // Función que escribe en el XML el valor de los elementos correspondientes a las variables de proyectoInstrumento
{
    //qDebug("Creando XML proyectoInstrumentos");
    XMLOperator* xml_operator = XMLOperator::instance();
    doc.appendChild(e);
    e.appendChild(xml_operator->createTextElement(doc, "proyectoID", proyectoID));
    e.appendChild(xml_operator->createMultiLineTextElement(doc,"descripcion", descripcion));
    QDomElement tag_instrumentos = doc.createElement("instrumentos");
    e.appendChild(tag_instrumentos);
    int i = instrumentos.size();
    //qDebug() << i;
    for(int i = 0; i < instrumentos.size(); i++) {
        Instrumento* instrumento = instrumentos.at(i);
        instrumento->writeToXml(doc, tag_instrumentos);
    }
}

QString ProyectoInstrumentos::XMLDocName() //Función que establece el DOCTYPE de
{
    return "XMLInstrumentos";
}

void ProyectoInstrumentos::clearLists()
{
    for(int i = 0; i < instrumentos.size(); i++)
        delete instrumentos.at(i);
    instrumentos.clear();
}
