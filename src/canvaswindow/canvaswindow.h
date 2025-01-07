/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CANVASWINDOW_H
#define CANVASWINDOW_H

#include <QtWidgets>

#include "objects/objectlist.h"
#include "server/sacnserver.h"

class CanvasWindow : public QWidget {
    Q_OBJECT
public:
    CanvasWindow(QWidget* parent, bool fullscreen, ObjectList* objectList, SacnServer* sacnServer);
    void paintEvent(QPaintEvent *event) override;
private:
    ObjectList *objects;
    SacnServer *sacn;
};

#endif // CANVASWINDOW_H
