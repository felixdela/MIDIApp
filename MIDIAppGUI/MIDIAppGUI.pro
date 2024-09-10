QT       += core gui
QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    allbancosdialog.cpp \
    allbotonesdialog.cpp \
    allconfiguracionesdialog.cpp \
    allinstrumentosdialog.cpp \
    banco.cpp \
    bancodialog.cpp \
    boton.cpp \
    botondialog.cpp \
    configuracion.cpp \
    configuraciondialog.cpp \
    instrumento.cpp \
    instrumentoDialog.cpp \
    main.cpp \
    mainwindow.cpp \
    newbancodialog.cpp \
    newconfiguracionesdialog.cpp \
    newinstrumentosdialog.cpp \
    preferencesdialog.cpp \
    proyectoInstrumentos.cpp \
    proyectoconfiguraciones.cpp \
    proyectoconfiguracionesdialog.cpp \
    proyectoinstrumentosdialog.cpp \
    seleccioncanaldialog.cpp \
    seleccioninstrumentosdialog.cpp \
    seleccionpanningdialog.cpp \
    seleccionvolumendialog.cpp

HEADERS += \
    allbancosdialog.h \
    allbotonesdialog.h \
    allconfiguracionesdialog.h \
    allinstrumentosdialog.h \
    banco.h \
    bancodialog.h \
    boton.h \
    botondialog.h \
    configuracion.h \
    configuraciondialog.h \
    instrumento.h \
    instrumentoDialog.h \
    mainwindow.h \
    newbancodialog.h \
    newconfiguracionesdialog.h \
    newinstrumentosdialog.h \
    preferencesdialog.h \
    proyectoInstrumentos.h \
    proyectoconfiguraciones.h \
    proyectoconfiguracionesdialog.h \
    proyectoinstrumentosdialog.h \
    qvariantptr.h \
    seleccioncanaldialog.h \
    seleccioninstrumentosdialog.h \
    seleccionpanningdialog.h \
    seleccionvolumendialog.h

FORMS += \
    allbancosdialog.ui \
    allbotonesdialog.ui \
    allconfiguracionesdialog.ui \
    allinstrumentosdialog.ui \
    bancodialog.ui \
    botondialog.ui \
    configuraciondialog.ui \
    instrumentoDialog.ui \
    mainwindow.ui \
    newbancodialog.ui \
    newconfiguracionesdialog.ui \
    newinstrumentosdialog.ui \
    preferencesdialog.ui \
    proyectoconfiguracionesdialog.ui \
    proyectoinstrumentosdialog.ui \
    seleccioncanaldialog.ui \
    seleccioninstrumentosdialog.ui \
    seleccionpanningdialog.ui \
    seleccionvolumendialog.ui

TRANSLATIONS += \
    MIDIAppGUI_es_ES.ts
CONFIG += lrelease
CONFIG += embed_translations
INCLUDEPATH += /usr/include/qew6
DEPENDPATH += /usr/include/qew6
LIBS += -L$(QTDIR)/plugins/designer -lqewextensibledialogs6
INCLUDEPATH += /usr/include/xmloperator
DEPENDPATH +=/usr/include/xmloperator
LIBS += -L/usr/lib/ -lxmloperator6
# Disable optimization in debug mode
QMAKE_CXXFLAGS_DEBUG -= -O2
unix {
    UI_DIR = .gui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../../../Descargas/flecha_abajo.jpg
