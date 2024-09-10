#ifndef CONFIGURACION_H
#define CONFIGURACION_H
#include <xmlwritable.h>
#include "boton.h"
#include "instrumento.h"
#include <xmlwritable.h>
class Configuracion : public XMLWritable
{
public:
    Configuracion();
    virtual ~Configuracion();

    QString getId() const;
    void setId(const QString &newId);

    QString getDescripcion() const;
    void setDescripcion(const QString &newDescripcion);

    void writeToXml(QDomDocument& doc, QDomElement& e);
    QString generateHtml(int prevIndent) const{};

    QList<Boton *>* getBotones() ;
    void setBotones(const QList<Boton *> &newBotones);
    QList<Instrumento*> getInstrumentos();
    void setInstrumentos(QList<Instrumento*> newInstrumentos);

    QString getInstrumentosFile() const;
    void setInstrumentosFile(const QString &newInstrumentosFile);
    static QString getArduinoCLIPath();
    static void setArduinoCLIPath(const QString &newArduinoCLIPath);

private:
    QList<Instrumento*> cargarNombresInstrumento();

protected:
    void clearLists();
    void readFromXML(QDomElement& e);
    void write_constructor(QDomElement& ){}

protected:
    QString id;
    QString descripcion;
    QString instrumentosFile;
    QList<Boton*> botones;
    QList<Instrumento*> instrumentos;

    static QString ArduinoCLIPath;


};

#endif // CONFIGURACION_H
