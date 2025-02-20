/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtNetwork>

#include "canvaswindow/canvaswindow.h"
#include "aboutwindow/aboutwindow.h"
#include "objects/objectlist.h"
#include "objects/objecttypeitemdelegate.h"
#include "media/mediasources.h"
#include "server/sacnserver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void addObject();
    void removeObject();
    void openWindow();
    void openFullscreen();
    void openFile();
    void newFile();
    void saveFile();
    void saveFileAs();
    void reset();
    void closeEvent(QCloseEvent *event) override;
    void about();
    ObjectList *objectList;
    MediaSources *mediaSources;
    SacnServer *sacnServer;
    QTableView *objectTable;
    QString filename;
    const QString VERSION = "0.1.0";
    const QString COPYRIGHT = "Copyright (c) 2025 Arne Bersch (zoeglfrex-dmx@web.de)";
};
#endif // MAINWINDOW_H
