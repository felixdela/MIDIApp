#ifndef BANCO_H
#define BANCO_H

#include <QStringList>
#include <xmlwritable.h>
class Banco : public XMLWritable
{
public:
    Banco();
    virtual ~Banco();
    void writeToXml(QDomDocument& doc, QDomElement& e);
    QString generateHtml(int prevIndent) const{};

    QString getMSB() const;
    void setMSB(const QString &newMSB);
    QString getLSB() const;
    void setLSB(const QString &newLSB);
    QList<QPair<int, QString>>& getProgramas();
    void setProgramas(const QList<QPair<int, QString>> &newProgramas);

    int getNumBancoInt() const;
    int getLSBInt() const;
    int getMSBInt() const;
    QString getNumBanco() const;

    void setNumBanco(const QString &newNumbanco);
    void generarProgramas(int numProgramas, QString nomenclatura);

protected:
    void readFromXML(QDomElement& e);
    void write_constructor(QDomElement& /*e*/) {}



protected:
    QString numBanco;
    QString MSB;
    QString LSB;
    QList<QPair<int, QString>> programas;
};

#endif // BANCO_H
