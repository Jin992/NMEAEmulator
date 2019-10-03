TEMPLATE = app

QT += location
QT += serialport

SOURCES += main.cpp \
    marker.cpp \
    serial.cpp

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/location/minimal_map
INSTALLS += target

HEADERS += \
    marker.h \
    serial.h
