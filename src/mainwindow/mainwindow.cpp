/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    qInfo() << "dmxCanvas " + VERSION;
    qInfo() << COPYRIGHT;
    qInfo() << "dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.";
    qInfo() << "dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.";
    qInfo() << "You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.";

    resize(700, 500);
    setWindowTitle("dmxCanvas");

    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *newFileAction = new QAction("New File (Ctrl+N)");
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newFileAction);

    QAction *openFileAction = new QAction("Open File (Ctrl+O)");
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openFileAction);

    QAction *saveFileAction = new QAction("Save File (Ctrl+S)");
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveFileAction);

    QAction *saveFileAsAction = new QAction("Save File as (Ctrl+Shift+S)");
    connect(saveFileAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    fileMenu->addAction(saveFileAsAction);

    fileMenu->addSeparator();

    QAction *quitAction = new QAction("Quit (Ctrl+Q)");
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(quitAction);

    QMenu *outputMenu = menuBar()->addMenu("Output");

    QAction *openFullscreenAction = new QAction("Open Canvas Fullscreen (F5)");
    connect(openFullscreenAction, &QAction::triggered, this, [this]{ openWindow(true); });
    outputMenu->addAction(openFullscreenAction);

    QAction *openWindowAction = new QAction("Open Canvas Window (Shift+F5)");
    connect(openWindowAction, &QAction::triggered, this, [this]{ openWindow(false); });
    outputMenu->addAction(openWindowAction);

    QMenu *helpMenu = menuBar()->addMenu("Help");

    QAction *aboutAction = new QAction("About dmxCanvas");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction("About Qt");
    connect(aboutQtAction, &QAction::triggered, this, []{ QApplication::aboutQt(); });
    helpMenu->addAction(aboutQtAction);

    QAction *openManualAction = new QAction("Manual");
    connect(openManualAction, &QAction::triggered, this, []{ QDesktopServices::openUrl(QUrl("https://github.com/arneBersch/dmxCanvas/blob/main/docs/manual.md")); });
    helpMenu->addAction(openManualAction);

    QAction *openDmxChartsAction = new QAction("DMX Charts");
    connect(openDmxChartsAction, &QAction::triggered, this, []{ QDesktopServices::openUrl(QUrl("https://github.com/arneBersch/dmxCanvas/blob/main/docs/dmxCharts.md")); });
    helpMenu->addAction(openDmxChartsAction);

    new QShortcut(Qt::CTRL | Qt::Key_N, this, [this]{ newFile(); });
    new QShortcut(Qt::CTRL | Qt::Key_O, this, [this]{ openFile(); });
    new QShortcut(Qt::CTRL | Qt::Key_S, this, [this]{ saveFile(); });
    new QShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_S, this, [this]{ saveFileAs(); });
    new QShortcut(Qt::CTRL | Qt::Key_Q, this, [this]{ close(); });
    new QShortcut(Qt::Key_F5, this, [this]{ openWindow(true); });
    new QShortcut(Qt::SHIFT | Qt::Key_F5, this, [this]{ openWindow(false); });

    QTabWidget *tabs = new QTabWidget();
    tabs->setTabPosition(QTabWidget::South);
    this->setCentralWidget(tabs);

    QVBoxLayout *objectsLayout = new QVBoxLayout();
    QWidget *objects = new QWidget;
    objects->setLayout(objectsLayout);
    objectTable = new QTableView();
    objectList = new ObjectList();
    objectTable->setModel(objectList);
    objectTable->horizontalHeader()->setStretchLastSection(true);
    objectTable->verticalHeader()->hide();
    objectTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    objectTable->setItemDelegateForColumn(ObjectListColumns::AddressColumn, new AddressItemDelegate(objectTable));
    objectTable->setItemDelegateForColumn(ObjectListColumns::TypeColumn, new ObjectTypeItemDelegate(objectTable));
    objectsLayout->addWidget(objectTable);
    QPushButton *addObjectButton = new QPushButton("Add Object");
    connect(addObjectButton, &QPushButton::clicked, this, &MainWindow::addObject);
    objectsLayout->addWidget(addObjectButton);
    QPushButton *removeObjectButton = new QPushButton("Remove Object");
    connect(removeObjectButton, &QPushButton::clicked, this, &MainWindow::removeObject);
    objectsLayout->addWidget(removeObjectButton);
    tabs->addTab(objects, "Objects");

    mediaSources = new MediaSources();
    tabs->addTab(mediaSources, "Media");

    sacnServer = new SacnServer();
    tabs->addTab(sacnServer, "Input");

    this->show(); // Show window
    about(); // Open about window
}

MainWindow::~MainWindow() {
}

void MainWindow::addObject() {
    objectList->insertRows(objectList->rowCount(), 1);
}

void MainWindow::removeObject() {
    QModelIndexList selection = objectTable->selectionModel()->selectedRows();
    if (selection.size() <= 0) {
        return;
    }
    QMessageBox messageBox;
    messageBox.setText("Do you want to delete " + QString::number(selection.size()) + " Objects?");
    messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (messageBox.exec() != QMessageBox::Ok) {
        return;
    }
    std::sort(selection.begin(), selection.end(), [](QModelIndex a, QModelIndex b) { return a.row() > b.row(); });
    for (QModelIndex index : selection) {
        objectList->removeRows(index.row(), 1);
    }
}

void MainWindow::openWindow(bool fullscreen) {
    new CanvasWindow(this, fullscreen, objectList, mediaSources, sacnServer);
}

void MainWindow::openFile() {
    QString newFileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "dmxc Files (*.dmxc)");
    if (newFileName.isEmpty()) {
        return;
    }
    QFile file(newFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox errorBox;
        errorBox.setText("Can't open file.");
        errorBox.exec();
        return;
    }
    reset();
    QXmlStreamReader fileStream(&file);
    if ((fileStream.readNextStartElement()) && (fileStream.name().toString() == "Workspace")) {
        while (fileStream.readNextStartElement()) {
            if (fileStream.name().toString() == "Creator") {
                while (fileStream.readNextStartElement()) {
                    if ((fileStream.name().toString() == "Name") && (fileStream.readElementText() != "dmxCanvas")) {
                        QMessageBox errorBox;
                        errorBox.setText("This is not a dmxCanvas file.");
                        errorBox.exec();
                        return;
                    } else if ((fileStream.name().toString() == "Version") && (fileStream.readElementText() != VERSION)) {
                        QMessageBox errorBox;
                        errorBox.setText("This dmxCanvas version isn't compatible to the current version (" + VERSION + ").");
                        errorBox.exec();
                        return;
                    }
                }
            } else if (fileStream.name().toString() == "Objects") {
                while (fileStream.readNextStartElement()) {
                    if (fileStream.name().toString() == "Object") {
                        objectList->insertRows(objectList->rowCount(), 1);
                        while (fileStream.readNextStartElement()) {
                            if (fileStream.name().toString() == "Name") {
                                objectList->setData(objectList->index((objectList->rowCount() - 1), ObjectListColumns::NameColumn), fileStream.readElementText());
                            } else if (fileStream.name().toString() == "Address") {
                                objectList->setData(objectList->index((objectList->rowCount() - 1), ObjectListColumns::AddressColumn), fileStream.readElementText());
                            } else if (fileStream.name().toString() == "Type") {
                                objectList->setData(objectList->index((objectList->rowCount() - 1), ObjectListColumns::TypeColumn), fileStream.readElementText());
                            }
                        }
                    }
                }
            } else if (fileStream.name().toString() == "Media") {
                while (fileStream.readNextStartElement()) {
                    if (fileStream.name().toString() == "Images") {
                        mediaSources->setImageDirectory(fileStream.readElementText());
                    }
                }
            } else if (fileStream.name().toString() == "Input") {
                while (fileStream.readNextStartElement()) {
                    if (fileStream.name().toString() == "Universe") {
                        bool ok = true;
                        int universe = fileStream.readElementText().toInt(&ok);
                        if (!ok) {
                            QMessageBox errorBox;
                            errorBox.setText("Invalid Input Universe.");
                            errorBox.exec();
                            return;
                        }
                        sacnServer->setUniverse(universe);
                    }
                }
            }
        }
    }
    filename = newFileName;
    if (fileStream.hasError()) {
        QMessageBox errorBox;
        errorBox.setText("Can't open File because a XML parsing Error occured in Line " + QString::number(fileStream.lineNumber()) + ": " + fileStream.errorString() + " (" + QString::number(fileStream.error()) + ")");
        errorBox.exec();
        return;
    }
    qDebug() << "Opened File " << filename;
}

void MainWindow::newFile() {
    QMessageBox messageBox;
    messageBox.setText("Are you sure you want to open a new File?");
    messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (messageBox.exec() != QMessageBox::Ok) {
        return;
    }
    reset();
    qDebug() << "Opened new File.";
}

void MainWindow::saveFile() {
    if (filename.isEmpty()) {
        QString filenameFilter = "dmxc Files (*.dmxc)";
        filename = QFileDialog::getSaveFileName(this, "Save File", QString(), filenameFilter, &filenameFilter);
        if (filename.isEmpty()) {
            return; // don't save if no valid file name was given
        }
        if (!filename.endsWith(".dmxc")) {
            filename += ".dmxc";
        }
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox errorBox;
        errorBox.setText("Unable to save File.");
        errorBox.exec();
        return;
    }
    QXmlStreamWriter fileStream(&file);
    fileStream.setAutoFormatting(true);
    fileStream.writeStartDocument();
    fileStream.writeStartElement("Workspace");

    fileStream.writeStartElement("Creator");
    fileStream.writeTextElement("Name", "dmxCanvas");
    fileStream.writeTextElement("Version", VERSION);
    fileStream.writeEndElement();

    fileStream.writeStartElement("Objects");
    for (int objectRow = 0; objectRow < objectList->rowCount(); objectRow++) {
        fileStream.writeStartElement("Object");
        fileStream.writeTextElement("Name", objectList->data(objectList->index(objectRow, ObjectListColumns::NameColumn), Qt::DisplayRole).toString());
        fileStream.writeTextElement("Address", objectList->data(objectList->index(objectRow, ObjectListColumns::AddressColumn), Qt::DisplayRole).toString());
        fileStream.writeTextElement("Type", objectList->data(objectList->index(objectRow, ObjectListColumns::TypeColumn), Qt::DisplayRole).toString());
        fileStream.writeEndElement();
    }
    fileStream.writeEndElement();

    fileStream.writeStartElement("Media");
    fileStream.writeTextElement("Images", mediaSources->imageDirectory);
    fileStream.writeEndElement();

    fileStream.writeStartElement("Input");
    fileStream.writeTextElement("Universe", QString::number(sacnServer->getUniverse()));
    fileStream.writeEndElement();

    fileStream.writeEndElement();
    fileStream.writeEndDocument();
    qDebug() << "Saved File" << filename;
}

void MainWindow::saveFileAs() {
    filename = QString(); // reset filename
    saveFile();
}

void MainWindow::reset() {
    objectList->removeRows(0, objectList->rowCount(), QModelIndex());
    mediaSources->resetSources();
    sacnServer->reset();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    QMessageBox confirmBox;
    confirmBox.setText("Do you really want to quit dmxCanvas?");
    confirmBox.setWindowTitle("Quit dmxCanvas?");
    confirmBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    confirmBox.setDefaultButton(QMessageBox::Cancel);
    if (confirmBox.exec() == QMessageBox::Ok) {
        event->accept();
    }
}

void MainWindow::about() {
    AboutWindow about(VERSION, COPYRIGHT);
    about.exec();
}
