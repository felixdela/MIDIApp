#include "configuraciondialog.h"
#include "seleccioncanaldialog.h"
#include "ui_configuraciondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>  // Asegúrate de incluir QDir
#include <QTemporaryFile>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
ConfiguracionDialog::ConfiguracionDialog(QWidget *parent) :
    QewTreeViewDialog(parent),
    ui(new Ui::ConfiguracionDialog)
{
    ui->setupUi(this);
    configuracion = 0;
    configuracionItem = 0;
    dataItem = 0;
    botonesDialog = 0;
    titleBotones = tr("Botones");
    newConf = false;

    connect(ui->lineEditId, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    connect(ui->toolButtonXML, SIGNAL(clicked(bool)), this, SLOT(chooseInstFile()));
    connect(ui->pushButton_confirmInst, SIGNAL(clicked(bool)), this, SLOT(obtenerInstrumentosMarcados()));
    connect(ui->pushButtonGenerarArduino, &QPushButton::clicked, this, &ConfiguracionDialog::generarArchivoArduino);
    connect(ui->pushButtonCompilarArduino, &QPushButton::clicked, this, &ConfiguracionDialog::compilarProyectoArduino);
    connect(ui->pushButtonCargarArduino, &QPushButton::clicked, this, &ConfiguracionDialog::cargarProyectoArduino);
}

ConfiguracionDialog::~ConfiguracionDialog()
{
    delete ui;
}

bool ConfiguracionDialog::setUpDialog()
{
    bool ret = QewTreeViewDialog::setUpDialog();
    if(!ret)
        return ret;
    connectDialog(this);
    if (ret)
    {
//        prevCurrentItem = botonesItem;
//        dataItem = instrumentoItem->child(0);
        if(!botonesDialog) {
            botonesDialog = new AllBotonesDialog(this->configuracion,this);
            if(botonesDialog->setUpDialog()){}
            //title = tr("Botones");
            botonesDialog->setWindowTitle(titleBotones);
            // Si se añade sin singleShot aparece Bancos en dos posiciones del árbol
//            addExtensibleChild(bancosDialog, title);
//            bancosItem = bancosDialog->getItemTree();
//            connect(bancosDialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SLOT(changeCurrentItemId(QTreeWidgetItem*, QString)));
            if(newConf)
                QTimer::singleShot(100, this, SLOT(addBotonesDialog()));
            else
                addBotonesDialog();
            }

    }
    return ret;
}

Configuracion *ConfiguracionDialog::getConfiguracion() const
{
    return configuracion;
}

void ConfiguracionDialog::setConfiguracion(Configuracion *configuracion)
{
    this->configuracion = configuracion;
    configuracionID = configuracion->getId();
    instFile = configuracion->getInstrumentosFile();
    descripcion = configuracion->getDescripcion();
    ui->lineEditId->setText(configuracion->getId());
    if (!instFile.isEmpty()) // Arreglo para que no haya error al crear una nueva configuración
        fillUi();
    /*botonesDialog->setBotones(configuracion->getBotones());
    if(newConf)
            QTimer::singleShot(100, this, SLOT(addNewBotones()));*/

}

void ConfiguracionDialog::setBotones()
{

    botonesDialog->setBotones(configuracion->getBotones());
    /*if(newConf)
            QTimer::singleShot(100, this, SLOT(addNewBotones()));*/
}

void ConfiguracionDialog::setNewConf(bool newNewConf)
{
    newConf = newNewConf;
}


void ConfiguracionDialog::saveThis()
{
    //qDebug("Saving ConfiguracionDialog Data");
    configuracionID = ui->lineEditId->text().trimmed();
    configuracion->setId(configuracionID);
    instFile = ui->lineEditPathXML->text().trimmed();
    configuracion->setInstrumentosFile(instFile);

    //TODO instrumentos??
    descripcion = ui->textEditDescripcion->toPlainText();
    configuracion->setDescripcion(descripcion);

}

void ConfiguracionDialog::fillUi()
{
    ui->lineEditId->setText(configuracionID);
    ui->lineEditPathXML->setText(instFile);
    initializeInstrumentList();
    ui->textEditDescripcion->setText(descripcion);

}

void ConfiguracionDialog::cleanProject()
{
    botonesDialog->removeDialogs();
    for(int i = 0; i<botonesItem->childCount(); i++)
    {
        bool alsoWidgets=true;
        removeItemFromTree(botonesItem->child(0), alsoWidgets);
    }
}

void ConfiguracionDialog::editingFinished()
{
    if(prevId != ui->lineEditId->text()) {
            emit sendIdChanged(getItemTree(), ui->lineEditId->text().trimmed());
            prevId = ui->lineEditId->text().trimmed();
            setWindowTitle(prevId);
            setDataChangedThis(true);
    }
}

void ConfiguracionDialog::addBotonesDialog()
{
    addExtensibleChild(botonesDialog, titleBotones);
    botonesItem = botonesDialog->getItemTree();
    connect(botonesDialog, SIGNAL(sendIdChanged(QTreeWidgetItem*, QString)), this, SLOT(changeCurrentItemId(QTreeWidgetItem*, QString)));

}

void ConfiguracionDialog::changeCurrentItemId(QTreeWidgetItem *item, QString newId)
{
    changeItemText(item, newId);
}
QList<Instrumento*> ConfiguracionDialog::cargarNombresInstrumento()
{
    ui->listWidgetInst->clear();
    QList<Instrumento*> instrumentos;

    if (instFile.trimmed().isEmpty()) {
        QMessageBox::critical(this, tr("Error"), tr("El camino del archivo XML no está establecido"));
        return instrumentos;
    }

    QFile file(instFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("No se puede abrir el archivo %1").arg(instFile));
        return instrumentos;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::critical(this, tr("Error"), tr("No se puede analizar el archivo XML"));
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


void ConfiguracionDialog::initializeInstrumentList()
{
    instrumentosCD = cargarNombresInstrumento();

        for (Instrumento* inst : instrumentosCD) {
            QListWidgetItem *item = new QListWidgetItem(inst->getId(), ui->listWidgetInst);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }
}

void ConfiguracionDialog::obtenerInstrumentosMarcados()
{
    //Llamada a obtenerInstrumentosMarcados con el parámetro, para poder utilizarlo en un slot.
    obtenerInstrumentosMarcados(configuracion);
}

void ConfiguracionDialog::addNewBotones()
{
    for(int i=1;i<=8;i++)
        {
            botonesDialog->newBoton(i);
        }
}
void ConfiguracionDialog::obtenerInstrumentosMarcados(Configuracion* configuracion)
{
    QList<Instrumento*> checkedInstruments;

        // Obtener la lista completa de instrumentos
    if(instrumentosCD.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("La lista de instrumentos está vacía"));
        return;
    }

    // Recorrer todos los elementos en la QListWidget
    for (int i = 0; i < ui->listWidgetInst->count(); ++i) {
        QListWidgetItem *item = ui->listWidgetInst->item(i);

        // Verificar si el elemento está chequeado
        if (item->checkState() == Qt::Checked) {
            QString checkedId = item->text();

            // Buscar el instrumento correspondiente en la lista completa
            for (Instrumento* inst : instrumentosCD) {
                if (inst->getId() == checkedId) {
                    checkedInstruments.append(inst);
                    break;
                }
            }
        }
    }

    SeleccionCanalDialog dialog(this);
        dialog.setInstrumentos(checkedInstruments);

        if (dialog.exec() == QDialog::Accepted) {
            QList<QString> selectedCanales = dialog.getSelectedCanales();

            for (int i = 0; i < checkedInstruments.size(); ++i) {
                checkedInstruments[i]->setCanal(selectedCanales[i]);
                /*^qDebug() << "Instrumento ID:" << checkedInstruments[i]->getId()
                         << "Canal:" << checkedInstruments[i]->getCanal();*/
            }

            configuracion->setInstrumentos(checkedInstruments);
            if(botonesDialog->getDialogs().size()==0)
            {
                addNewBotones();
            }
            ui->pushButton_confirmInst->setEnabled(false);
        }

}

void ConfiguracionDialog::generarArchivoArduino()
{
    // Verificar si la configuración y los botones están configurados
    if (!configuracion || !botonesDialog) {
        QMessageBox::warning(this, tr("Error"), tr("Configuración o botones no están disponibles."));
        return;
    }

    // Mostrar diálogo para elegir la ruta y el nombre de la carpeta
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Seleccionar Carpeta para Guardar"),
                                                        QString(),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // Verificar si el usuario seleccionó una carpeta
    if (dirName.isEmpty()) {
        return;  // Si no se seleccionó ninguna carpeta, salir de la función
    }

    // Crear la ruta completa para el archivo botones.ino
    QString arduinoFile = dirName + "/botones.ino";

    // Crear y abrir el archivo de texto para escribir
    QFile file(arduinoFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo crear el archivo Arduino."));
        return;
    }

    QTextStream out(&file);

    // Obtener todos los BotonDialog usando getDialogs
    QList<BotonDialog *> dialogs = botonesDialog->getDialogs();
    for (int i = 0; i < dialogs.size(); ++i) {
        BotonDialog *botonDialog = dialogs[i];
        if (botonDialog) {
            // Comenzar la función para el botón
            out << QString("void boton%1()\n").arg(i + 1);
            out << "{\n";

            QStringList funciones = botonDialog->getFuncionesFromListWidget();
            for (const QString &funcion : funciones) {
                // Extraer el tipo de función (BYP, VOL, PAN)
                QString tipoFuncion = funcion.split("|").first().trimmed();
                QString contenido = funcion.split("|").last().trimmed();

                // Extraer el nombre del instrumento
                QString instrumentoId = contenido.split(":").first().trimmed();
                Instrumento* instrumento = nullptr;

                // Buscar el instrumento en la configuración
                for (Instrumento* inst : configuracion->getInstrumentos()) {
                    if (inst->getId() == instrumentoId) {
                        instrumento = inst;
                        break;
                    }
                }

                if (instrumento) {
                    QString canal = instrumento->getCanal();

                    if (tipoFuncion == "BYP") {
                        // Extraer el número de banco y programa
                        QString bancoString = contenido.split("banco ").last().split(":").first().trimmed();
                        int bancoNumero = bancoString.toInt();

                        Banco* banco = instrumento->getBanco(bancoString);
                        if (banco && banco->getNumBancoInt() == bancoNumero) {
                            int MSB = banco->getMSBInt();
                            int LSB = banco->getLSBInt();

                            QString programaString = contenido.split("al programa ").last().split(":").first().trimmed();
                            int programaNumero = programaString.toInt();

                            // Buscar el programa en la lista de programas
                            QList<QPair<int, QString>>& programas = banco->getProgramas();
                            bool programaEncontrado = false;
                            for (const QPair<int, QString>& programa : programas) {
                                if (programa.first == programaNumero) {
                                    programaEncontrado = true;

                                    // Escribir la línea con el número de programa ajustado
                                    int programaParaEscribir = programa.first;
                                    if (instrumento->getNomenclatura() == "0-127") {
                                        programaParaEscribir -= 1;  // Restar 1 si la nomenclatura es "0-127"
                                    }

                                    out << QString("    midiOut.sendControlChange(0, %1, %2); // seleccionar banco \n").arg(MSB).arg(canal);
                                    out << QString("    midiOut.sendControlChange(32, %1, %2); // seleccionar banco \n").arg(LSB).arg(canal);
                                    out << QString("    midiOut.sendProgramChange(%1, %2); // seleccionar programa \n").arg(programaParaEscribir).arg(canal);
                                    break;
                                }
                            }

                            if (!programaEncontrado) {
                                out << QString("    // Programa no encontrado: %1\n").arg(programaNumero);
                            }
                        } else {
                            out << QString("    // Banco no encontrado o no coincide: %1\n").arg(bancoNumero);
                        }

                    } else if (tipoFuncion == "VOL" || tipoFuncion == "PAN") {
                        int controlChange = (tipoFuncion == "VOL") ? 7 : 10;

                        // Extraer el valor (después del igual "=")
                        QString valorString = contenido.split("=").last().trimmed();
                        int valor = valorString.toInt();

                        // Generar la línea de código MIDI correspondiente
                        out << QString("    midiOut.sendControlChange(%1, %2, %3);\n").arg(controlChange).arg(valor).arg(canal);

                    } else {
                        out << QString("    // Tipo de función desconocido: %1\n").arg(tipoFuncion);
                    }

                } else {
                    out << QString("    // Instrumento no encontrado: %1\n").arg(instrumentoId);
                }
                out << "\n";
            }

            // Cerrar la función del botón
            out << "}\n\n";
        }
    }

    // Agregar la función encender_led al final del archivo
    out << "void encender_led(int ledout){\n";
    out << "    for (int j = 0 ; j<8; j++){\n";
    out << "        digitalWrite(leds[j], LOW); // apaga todos los LEDS\n";
    out << "        delay(20); // esperar \n";
    out << "    }\n";
    out << "    digitalWrite(leds[ledout], HIGH);\n";
    out << "}\n";

    file.close();

    // Copiar los archivos adicionales desde los recursos al directorio seleccionado
    QStringList filesToCopy = {":/resources/arduino/botones.h", ":/resources/arduino/pedalera_midi_conbotones.ino"};

    for (const QString& resourcePath : filesToCopy) {
        QFile resourceFile(resourcePath);
        if (resourceFile.exists()) {
            QString destinationPath;

            // Renombrar "pedalera_midi_conbotones.ino" para que coincida con el nombre de la carpeta
            if (resourcePath.endsWith("pedalera_midi_conbotones.ino")) {
                QString dirNameBase = QFileInfo(dirName).fileName(); // Obtener el nombre de la carpeta
                destinationPath = dirName + "/" + dirNameBase + ".ino";
            } else {
                destinationPath = dirName + "/" + QFileInfo(resourcePath).fileName();
            }

            if (QFile::copy(resourcePath, destinationPath)) {
                // Cambiar los permisos del archivo copiado a -rw-r--r--
                QFile::Permissions permissions = QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther;
                QFile::setPermissions(destinationPath, permissions);
            } else {
                QMessageBox::warning(this, tr("Error"), tr("No se pudo copiar el archivo %1.").arg(resourcePath));
            }
        } else {
            QMessageBox::warning(this, tr("Error"), tr("El archivo de recursos %1 no existe.").arg(resourcePath));
        }
    }

    // Mostrar mensaje con la ruta de la carpeta creada
    QMessageBox::information(this, tr("Éxito"), tr("Carpeta generada correctamente en: %1").arg(dirName));
}




void ConfiguracionDialog::compilarProyectoArduino()
{
    // Verificar si arduinoCLIPath está vacío
    if (arduinoCLIPath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("El path de arduino-cli no está configurado. Ve a las preferencias e indica la carpeta donde se encuentra el ejecutable de arduino-cli."));
       //qDebug() << "El path de arduino-cli no está configurado.";
        return;
    }

    // Agregar QDebug para la variable arduinoCLIPath
   //qDebug() << "Ruta del arduino-cli:" << arduinoCLIPath;

    // Abre un diálogo para seleccionar la carpeta del proyecto
    proyectoPath = QFileDialog::getExistingDirectory(this, tr("Seleccionar Carpeta del Proyecto"), "",
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // Agregar QDebug para la variable proyectoPath
   //qDebug() << "Ruta del Proyecto:" << proyectoPath;

    if (proyectoPath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No se ha seleccionado ninguna carpeta"));
        return;
    }

    QStringList argumentos;
    argumentos << "compile" << "--fqbn" << "arduino:avr:mega" << proyectoPath;

    // Crear un proceso para ejecutar el comando
    QProcess proceso;
    proceso.setProgram(arduinoCLIPath);
    proceso.setArguments(argumentos);

    // Agregar QDebug para mostrar el comando a ejecutar
    //qDebug() << "Comando a ejecutar:" << arduinoCLIPath << argumentos.join(" ");
    // Redirigir las salidas estándar y de error para diagnóstico
    QTemporaryFile tempOutputFile;
    if (!tempOutputFile.open()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo crear un archivo temporal para la salida del proceso."));
       //qDebug() << "Error al crear archivo temporal para salida.";
        return;
    }
    QString tempOutputFileName = tempOutputFile.fileName();

    QTemporaryFile tempErrorFile;
    if (!tempErrorFile.open()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo crear un archivo temporal para el error del proceso."));
       //qDebug() << "Error al crear archivo temporal para error.";
        return;
    }
    QString tempErrorFileName = tempErrorFile.fileName();

    // Redirigir salida estándar y error a los archivos temporales
    proceso.setStandardOutputFile(tempOutputFileName);
    proceso.setStandardErrorFile(tempErrorFileName);

    // Ejecutar el comando
    proceso.start();
    if (!proceso.waitForStarted()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo iniciar el proceso de compilación: %1").arg(proceso.errorString()));
       //qDebug() << "Error al iniciar el proceso:" << proceso.errorString();
        return;
    }

    if (!proceso.waitForFinished()) {
        QMessageBox::critical(this, tr("Error"), tr("El proceso de compilación no se completó correctamente: %1").arg(proceso.errorString()));
       //qDebug() << "Error durante la ejecución del proceso:" << proceso.errorString();
        return;
    }

    // Leer y mostrar el resultado de la compilación
    QFile salidaFile(tempOutputFileName);
    QFile errorFile(tempErrorFileName);

    QString salida, error;

    if (salidaFile.open(QIODevice::ReadOnly)) {
        salida = salidaFile.readAll();
        salidaFile.close();
    } else {
       //qDebug() << "Error al leer el archivo de salida.";
    }

    if (errorFile.open(QIODevice::ReadOnly)) {
        error = errorFile.readAll();
        errorFile.close();
    } else {
       //qDebug() << "Error al leer el archivo de error.";
    }

   //qDebug() << "Salida del proceso:" << salida;
   //qDebug() << "Error del proceso:" << error;

    if (!error.isEmpty()) {
        QMessageBox::critical(this, tr("Error de Compilación"), error);
    } else {
        QMessageBox::information(this, tr("Compilación Completa"), salida);
    }
}
void ConfiguracionDialog::cargarProyectoArduino()
{
    // Comprobar si la variable arduinoCLIPath está configurada
    if (arduinoCLIPath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("El path de arduino-cli no está configurado. Ve a las preferencias y selecciona el ejecutable de arduino-cli."));
        return;
    }

    // Verificar si el proyecto está configurado
    if (proyectoPath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No se ha seleccionado ninguna carpeta de proyecto."));
        return;
    }

    // Crear el proceso para listar los puertos
    QProcess procesoList;
    QStringList argumentosList;
    argumentosList << "board" << "list";

    procesoList.setProgram(arduinoCLIPath);
    procesoList.setArguments(argumentosList);

    // Ejecutar el comando para listar los puertos
    procesoList.start();
    if (!procesoList.waitForStarted() || !procesoList.waitForFinished()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo obtener la lista de puertos: %1").arg(procesoList.errorString()));
        return;
    }

    // Leer la salida del proceso para listar los puertos
    QString salidaList = procesoList.readAllStandardOutput();
   //qDebug() << "Puertos disponibles:" << salidaList;

    // Buscar el puerto conectado (se asume que el puerto aparece en la salida)
    QStringList lines = salidaList.split('\n');
    QString puerto;
    for (const QString &line : lines) {
        if (line.contains("Serial Port")) { // Busca una línea que contenga información sobre un puerto serial
            puerto = line.section(' ', 0, 0); // Extrae el puerto
            break;
        }
    }

    if (puerto.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No se detectó ningún controlador Arduino conectado a través de USB."));
        return;
    }

   //qDebug() << "Puerto encontrado:" << puerto;

    // Crear el proceso para subir el sketch
    QProcess procesoUpload;
    QStringList argumentosUpload;
    argumentosUpload << "upload" << "--fqbn" << "arduino:avr:mega" << "--port" << puerto << proyectoPath;

    procesoUpload.setProgram(arduinoCLIPath);
    procesoUpload.setArguments(argumentosUpload);

    // Ejecutar el comando para subir el proyecto
    procesoUpload.start();
    if (!procesoUpload.waitForStarted()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo iniciar el proceso de subida: %1").arg(procesoUpload.errorString()));
        return;
    }

    if (!procesoUpload.waitForFinished()) {
        QMessageBox::critical(this, tr("Error"), tr("El proceso de subida no se completó correctamente: %1").arg(procesoUpload.errorString()));
        return;
    }

    // Leer y mostrar el resultado de la subida
    QString salidaUpload = procesoUpload.readAllStandardOutput();
    QString errorUpload = procesoUpload.readAllStandardError();

   //qDebug() << "Salida del proceso de subida:" << salidaUpload;
   //qDebug() << "Error del proceso de subida:" << errorUpload;

    if (!errorUpload.isEmpty()) {
        QMessageBox::critical(this, tr("Error de Subida"), errorUpload);
    } else {
        QMessageBox::information(this, tr("Subida Completa"), salidaUpload);
    }
}

void ConfiguracionDialog::chooseInstFile()
{
    XMLOperator* xml_operator = XMLOperator::instance();
    QString filename = QDir::homePath();
    if(!ui->lineEditPathXML->text().trimmed().isEmpty())
        filename = ui->lineEditPathXML->text().trimmed();
    QString sender = tr("Main output file");
    QString type_file = "XML Files";  // Descripción del tipo de archivo
    QString extensions = "xml";     // Extensión de archivo para el filtro
    QFileDialog::FileMode filemode = QFileDialog::AnyFile;  // Permitir la selección de cualquier archivo

    bool ret = xml_operator->getFileDialog(filename, sender, QFileDialog::AcceptOpen,
                                           filemode, type_file, extensions);
    if(!ret)
        return;
    if(instFile != filename.trimmed()) {
        setDataChangedThis(true);
        instFile = filename.trimmed();
    }
    configuracion->setInstrumentosFile(instFile);
    ui->lineEditPathXML->setText(filename.trimmed());
    initializeInstrumentList();

}

QString ConfiguracionDialog::getArduinoCLIPath() const
{
    return arduinoCLIPath;
}

void ConfiguracionDialog::setArduinoCLIPath(const QString &newArduinoCLIPath)
{
    arduinoCLIPath = newArduinoCLIPath;
}

