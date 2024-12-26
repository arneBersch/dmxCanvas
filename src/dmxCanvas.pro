# Copyright (c) Arne Bersch
# This file is part of dmxCanvas.
# dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.

QT += core
QT += gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow/aboutwindow.cpp \
    canvaswindow/canvaswindow.cpp \
    main.cpp \
    mainwindow/mainwindow.cpp \
    objectlist/objectlist.cpp \
    server/sacnserver.cpp

HEADERS += \
    aboutwindow/aboutwindow.h \
    canvaswindow/canvaswindow.h \
    mainwindow/mainwindow.h \
    objectlist/objectlist.h \
    server/sacnserver.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/licenses.txt

RESOURCES += \
    resources.qrc
