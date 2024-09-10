#include "banco.h"

Banco::Banco()
{

}

Banco::~Banco()
{

}

void Banco::writeToXml(QDomDocument &doc, QDomElement &e)
{
    XMLOperator* xml_operator = XMLOperator::instance();
        QDomElement tag_banco = doc.createElement("banco");
        e.appendChild(tag_banco);
        tag_banco.appendChild(xml_operator->createTextElement(doc, "num", numBanco));
        tag_banco.appendChild(xml_operator->createTextElement(doc, "LSB", LSB));
        tag_banco.appendChild(xml_operator->createTextElement(doc, "MSB", MSB));
        QDomElement tag_programas = doc.createElement("programas");
        tag_banco.appendChild(tag_programas);
        for(const auto &programa : programas) {
            QDomElement programaElement = xml_operator->createTextElement(doc, "programa", QString::number(programa.first));
            programaElement.setAttribute("nombre", programa.second);
            tag_programas.appendChild(programaElement);
        }
}

void Banco::readFromXML(QDomElement &e)
{
    XMLOperator* xml_operator = XMLOperator::instance();

        QDomElement e_numBanco = xml_operator->findTag("num", e);
        if(!e_numBanco.isNull())
            numBanco = e_numBanco.text();
        QDomElement e_LSB = xml_operator->findTag("LSB", e);
        if(!e_LSB.isNull())
            LSB = e_LSB.text();
        QDomElement e_MSB = xml_operator->findTag("MSB", e);
        if(!e_MSB.isNull())
            MSB = e_MSB.text();
        QDomElement e_progs = xml_operator->findTag("programas", e);
        QDomNodeList n_progs = e_progs.childNodes();
        int contador = n_progs.count();
        programas.clear();
        for(int i = 0; i < contador; ++i){
            QDomNode n = n_progs.item(i);
            QDomElement e_prog = n.toElement();
            if (!e_prog.isNull())
            {
                int key = e_prog.text().toInt();
                QString name = e_prog.attribute("nombre");
                programas.append(qMakePair(key, name));
            }
        }
}


QString Banco::getNumBanco() const
{
    return numBanco;
}

int Banco::getNumBancoInt() const
{
    return numBanco.toInt();
}




void Banco::setNumBanco(const QString &newNumbanco)
{
    numBanco = newNumbanco;
}

QString Banco::getMSB() const
{
    return MSB;
}
int Banco::getLSBInt() const
{
    return LSB.toInt();
}

void Banco::setMSB(const QString &newMSB)
{
    MSB = newMSB;
}
int Banco::getMSBInt() const
{
    return MSB.toInt();
}
QString Banco::getLSB() const
{
    return LSB;
}

void Banco::setLSB(const QString &newLSB)
{
    LSB = newLSB;
}

QList<QPair<int, QString>>& Banco::getProgramas()
{
    return programas;
}

void Banco::setProgramas(const QList<QPair<int, QString>> &newProgramas)
{
    programas = newProgramas;
}
void Banco::generarProgramas(int numProgramas, QString nomenclatura)
{
    programas.clear(); // Limpiar cualquier dato existente en la lista
    for (int i = 0; i < numProgramas; ++i) {
        if (nomenclatura == "1-128")
        {
            programas.append(qMakePair(i + 1, QString("programa %1").arg(i + 1)));
        }
        else
        {
            programas.append(qMakePair(i, QString("programa %1").arg(i)));
        }
    }
}
