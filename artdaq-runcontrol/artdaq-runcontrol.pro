#-------------------------------------------------
#
# Project created by QtCreator 2018-05-29T16:26:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = artdaqRunControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    daqinterfacestate.cpp \
    xmlrpc_gui_comm.cpp \
    newexperimentdialog.cpp \
    newprofiledialog.cpp

HEADERS  += mainwindow.h \
    daqinterfacestate.h \
    xmlrpc_gui_comm.h \
    newexperimentdialog.h \
    newprofiledialog.h

FORMS    += mainwindow.ui \
    daqinterfacestate.ui \
    newexperimentdialog.ui \
    newprofiledialog.ui

INCLUDEPATH += /root/artdaq-demo-base/products/xmlrpc_c/v1_39_10d/Linux64bit+3.10-2.17-e15-prof/include

LIBS += -L/root/artdaq-demo-base/products/xmlrpc_c/v1_39_10d/Linux64bit+3.10-2.17-e15-prof/lib -lxmlrpc -lxmlrpc++ -lxmlrpc_abyss -lxmlrpc_abyss++ \
        -lxmlrpc_client -lxmlrpc_client++ -lxmlrpc_cpp -lxmlrpc_packetsocket -lxmlrpc_server -lxmlrpc_server++ -lxmlrpc_server_abyss -lxmlrpc_server_abyss++ \
        -lxmlrpc_server_cgi -lxmlrpc_server_cgi++ -lxmlrpc_server_pstream++ -lxmlrpc_util -lxmlrpc_util++ -lxmlrpc_xmlparse -lxmlrpc_xmltok \
        -L/usr/local/lib
