#include "proyectoconfiguraciones.h"
#include <QObject>
ProyectoConfiguraciones::ProyectoConfiguraciones()
{

}

ProyectoConfiguraciones::~ProyectoConfiguraciones()
{

}

QList<Configuracion *> *ProyectoConfiguraciones::getConfiguraciones()
{
    return &configuraciones;
}

void ProyectoConfiguraciones::setConfiguraciones(const QList<Configuracion *> &newConfiguraciones)
{
    configuraciones = newConfiguraciones;
}

QString ProyectoConfiguraciones::getProyectoID() const
{
    return proyectoID;
}

void ProyectoConfiguraciones::setProyectoID(const QString &newProyectoID)
{
    proyectoID = newProyectoID;
}

QString ProyectoConfiguraciones::getDescripcion() const
{
    return descripcion;
}

void ProyectoConfiguraciones::setDescripcion(const QString &newDescripcion)
{
    descripcion = newDescripcion;
}

void ProyectoConfiguraciones::writeToXml(QDomDocument &doc, QDomElement &e)
{
    //qDebug("Creando XML ProyectoConfiguraciones");
    XMLOperator* xml_operator = XMLOperator::instance();
    doc.appendChild(e);
    e.appendChild(xml_operator->createTextElement(doc, "proyectoID", proyectoID));
    e.appendChild(xml_operator->createMultiLineTextElement(doc, "descripcion", descripcion));
    QDomElement tag_configuraciones = doc.createElement("configuraciones");
    e.appendChild(tag_configuraciones);
    int i = configuraciones.size();
    //qDebug() << i;
    for(int i = 0; i < configuraciones.size(); i++) {
        Configuracion* configuracion = configuraciones.at(i);
        configuracion->writeToXml(doc, tag_configuraciones);
    }
}

QString ProyectoConfiguraciones::XMLDocName()
{
    return "configuracionesMIDI";
}

void ProyectoConfiguraciones::clearLists()
{
    for(int i = 0; i < configuraciones.size(); i++)
        delete configuraciones.at(i);
    configuraciones.clear();
}

void ProyectoConfiguraciones::readFromXML(QDomElement &e)
{
    clearLists();
    XMLOperator* xml_operator = XMLOperator::instance();

    QDomElement e_id = xml_operator->findTag("proyectoID", e);
    if(!e_id.isNull())
        proyectoID = e_id.text();

    QDomElement e_descripcion = xml_operator->findTag("descripcion", e);
    if(!e_descripcion.isNull())
        descripcion = e_descripcion.text();
    QDomElement e_configuraciones = xml_operator->findTag("configuraciones", e);
    QDomNodeList n_configuraciones = e_configuraciones.childNodes();
    int contador = n_configuraciones.count();
    QDomElement e_inst;
    for(int i = 0; i < contador; ++i){
        QDomNode n = n_configuraciones.item(i);
        e_inst = n.toElement();
        if (!e_inst.isNull())
        {
            Configuracion* configuracion = new Configuracion;
            configuracion->setXMLElement(e_inst);
            configuraciones.append(configuracion);
        }
    }
}

