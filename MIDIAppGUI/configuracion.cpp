#include "configuracion.h"
#include <QMessageBox>


QString Configuracion::ArduinoCLIPath = "";

Configuracion::Configuracion()
{

}

Configuracion::~Configuracion()
{

}

QString Configuracion::getId() const
{
    return id;
}

void Configuracion::setId(const QString &newId)
{
    id = newId;
}

QString Configuracion::getDescripcion() const
{
    return descripcion;
}

void Configuracion::setDescripcion(const QString &newDescripcion)
{
    descripcion = newDescripcion;
}

void Configuracion::writeToXml(QDomDocument &doc, QDomElement &e)
{
    //qDebug("Creando XML configuraciones");
    XMLOperator* xml_operator = XMLOperator::instance();
    QDomElement tag_configuracion = doc.createElement("configuracion");
    e.appendChild(tag_configuracion);
    tag_configuracion.appendChild(xml_operator->createTextElement(doc,
                  "configuracionID", id));
    tag_configuracion.appendChild(xml_operator->createTextElement(doc,"instrumentosFile", instrumentosFile));
    QDomElement tag_instrumentoscanal = doc.createElement("instrumentoscanal");
    tag_configuracion.appendChild(tag_instrumentoscanal);
    for(const auto &instCanal : instrumentos) {
        QDomElement instCanalElement = xml_operator->createTextElement(doc, "instCanal", instCanal->getCanal());
        instCanalElement.setAttribute("nombre", instCanal->getId());
        tag_instrumentoscanal.appendChild(instCanalElement);
    }
    tag_configuracion.appendChild(xml_operator->createMultiLineTextElement(doc,"descripcion", descripcion));
    QDomElement tag_botones = doc.createElement("botones");
    tag_configuracion.appendChild(tag_botones);
    for(int i = 0; i < botones.size(); i++) {
        Boton* boton = botones.at(i);
        boton->writeToXml(doc, tag_botones);
    }

}

void Configuracion::readFromXML(QDomElement &e)
{
    clearLists();
    XMLOperator* xml_operator = XMLOperator::instance();

    QDomElement e_id = xml_operator->findTag("configuracionID", e);
    if(!e_id.isNull())
        id = e_id.text();

    QDomElement e_instrumentosFile = xml_operator->findTag("instrumentosFile", e);
    if(!e_instrumentosFile.isNull())
        instrumentosFile = e_instrumentosFile.text();

    instrumentos = cargarNombresInstrumento();

    // Leer instrumentoscanal
    QDomElement e_instrumentosCanal = xml_operator->findTag("instrumentoscanal", e);
    if (!e_instrumentosCanal.isNull()) {
        QDomNodeList instrumentosCanalList = e_instrumentosCanal.elementsByTagName("instCanal");

        // Lista temporal para almacenar los instrumentos que sí están en instrumentoscanal
        QList<Instrumento*> instrumentosActualizados;

        for (int i = 0; i < instrumentosCanalList.count(); ++i) {
            QDomElement instCanalElement = instrumentosCanalList.at(i).toElement();

            QString nombreInstrumento = instCanalElement.attribute("nombre");
            QString canal = instCanalElement.text();  // Obtener el canal como QString

            // Buscar el instrumento en la lista y asignar el canal
            for (Instrumento* instrumento : instrumentos) {
                if (instrumento->getId() == nombreInstrumento) {
                    instrumento->setCanal(canal);
                    instrumentosActualizados.append(instrumento);  // Añadir a la lista actualizada
                    break;
                }
            }
        }

        // Reemplazar la lista de instrumentos con la lista actualizada
        instrumentos = instrumentosActualizados;
    }

    QDomElement e_descripcion = xml_operator->findTag("descripcion", e);
    if(!e_descripcion.isNull())
        descripcion = xml_operator->readMultiLineTextElement(e_descripcion);
    QDomElement e_botones = xml_operator->findTag("botones", e);
    QDomNodeList n_botones = e_botones.childNodes();
    int contador = n_botones.count();
    QDomElement e_conf;
    for(int i=0;i < contador; ++i)
    {
        QDomNode n = n_botones.item(i);
        e_conf = n.toElement();
        if(!e_conf.isNull())
        {
            Boton* boton = new Boton;
            boton->setXMLElement(e_conf);
            botones.append(boton);
        }
    }
}

QString Configuracion::getArduinoCLIPath()
{
    return ArduinoCLIPath;
}

void Configuracion::setArduinoCLIPath(const QString &newArduinoCLIPath)
{
    ArduinoCLIPath = newArduinoCLIPath;
}
QString Configuracion::getInstrumentosFile() const
{
    return instrumentosFile;
}

void Configuracion::setInstrumentosFile(const QString &newInstrumentosFile)
{
    instrumentosFile = newInstrumentosFile;
}

QList<Instrumento*> Configuracion::cargarNombresInstrumento()
{
    QList<Instrumento*> instrumentos;

    if (instrumentosFile.trimmed().isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("El camino del archivo XML no está establecido"));
        return instrumentos;
    }

    QFile file(instrumentosFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("No se puede abrir el archivo %1").arg(instrumentosFile));
        return instrumentos;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("No se puede analizar el archivo XML"));
        file.close();
        return instrumentos;
    }

    file.close();

    QDomNodeList instrumentNodes = doc.elementsByTagName("instrumento");
    for (int i = 0; i < instrumentNodes.count(); ++i) {
        QDomNode instrumentNode = instrumentNodes.at(i);
        if (instrumentNode.isElement()) {
            QDomElement instrumentElement = instrumentNode.toElement();

            Instrumento* instrumento = new Instrumento;
            instrumento->readFromXML(instrumentElement);

            instrumentos.append(instrumento);
        }
    }

    return instrumentos;
}

void Configuracion::clearLists()
{
    for(int i = 0; i < instrumentos.size(); i++)
        delete instrumentos.at(i);
    instrumentos.clear();
    for(int i = 0; i < botones.size(); i++)
        delete botones.at(i);
    botones.clear();
}

QList<Instrumento*> Configuracion::getInstrumentos()
{
    return instrumentos;
}

void Configuracion::setInstrumentos(QList<Instrumento*> newInstrumentos)
{
    instrumentos = newInstrumentos;
}

QList<Boton *>* Configuracion::getBotones()
{
    return &botones;
}

void Configuracion::setBotones(const QList<Boton *> &newBotones)
{
    botones = newBotones;
}
