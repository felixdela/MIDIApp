#include "instrumento.h"

Instrumento::Instrumento()
{

}

Instrumento::~Instrumento()
{

}

void Instrumento::writeToXml(QDomDocument &doc, QDomElement &e)
{
    //qDebug("Creando XML Instrumentos");

    XMLOperator* xml_operator = XMLOperator::instance();
    QDomElement tag_instrumento = doc.createElement("instrumento");
    e.appendChild(tag_instrumento);
    tag_instrumento.appendChild(xml_operator->createTextElement(doc,
                  "instrumentoID", id));
    tag_instrumento.appendChild(xml_operator->createTextElement(doc,
                  "canal", canal));
    tag_instrumento.appendChild(xml_operator->createTextElement(doc,
                  "nomenclatura", nomenclatura));
    QDomElement tag_bancos = doc.createElement("bancos");
    tag_instrumento.appendChild(tag_bancos);


    for(int i = 0; i < bancos.size(); i++) {
        Banco* subject = bancos.at(i);
        subject->writeToXml(doc, tag_bancos);
    }
}

void Instrumento::readFromXML(QDomElement &e)
{
    clearLists();
    XMLOperator* xml_operator = XMLOperator::instance();
    QDomElement e_id = xml_operator->findTag("instrumentoID", e);
    if(!e_id.isNull())
        id = e_id.text();
    QDomElement e_canal = xml_operator->findTag("canal", e);
    if (!e_canal.isNull())
        canal = e_canal.text();
    QDomElement e_nomen = xml_operator->findTag("nomenclatura", e);
    if (!e_nomen.isNull())
        nomenclatura = e_nomen.text();
    QDomElement e_bancos = xml_operator->findTag("bancos", e);
    QDomNodeList n_bancos = e_bancos.childNodes();
    int contador = n_bancos.count();
    QDomElement e_inst;
    for(int i = 0; i < contador; ++i){
        QDomNode n = n_bancos.item(i);
        e_inst = n.toElement();
        if (!e_inst.isNull())
        {
            Banco* banco = new Banco;
            banco->setXMLElement(e_inst);
            bancos.append(banco);
        }
    }

}

void Instrumento::clearLists()
{
    for(int i = 0; i < bancos.size(); i++)
        delete bancos.at(i);
    bancos.clear();
}

Banco *Instrumento::getBanco(QString id)
{
    for (Banco* banco : bancos) {
            if (banco->getNumBanco() == id) {
                return banco;  // Devolver el banco si el ID coincide
            }
        }
        return nullptr;
}

QString Instrumento::getNomenclatura() const
{
    return nomenclatura;
}

void Instrumento::setNomenclatura(const QString &newNomenclatura)
{
    nomenclatura = newNomenclatura;
}

QString Instrumento::getId() const
{
    return id;
}

void Instrumento::setId(const QString &newId)
{
    id = newId;
}

QString Instrumento::getCanal() const
{
    return canal;
}

void Instrumento::setCanal(const QString &newCanal)
{
    canal = newCanal;
}

QList<Banco *>* Instrumento::getBancos()
{
    return &bancos;
}

void Instrumento::setBancos(const QList<Banco *> &newBancos)
{
    bancos = newBancos;
}


