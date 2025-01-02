/*
    Copyright (c) Arne Bersch
    This file is part of dmxCanvas.
    dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    qInfo() << VERSION;
    qInfo() << COPYRIGHT;
    qInfo() << "dmxCanvas is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.";
    qInfo() << "dmxCanvas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.";
    qInfo() << "You should have received a copy of the GNU General Public License along with dmxCanvas. If not, see <https://www.gnu.org/licenses/>.";

    resize(700, 500);
    setWindowTitle("dmxCanvas");

    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *newFileAction = new QAction("New File (Ctrl+N)");
    fileMenu->addAction(newFileAction);
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openFileAction = new QAction("Open File (Ctrl+O)");
    fileMenu->addAction(openFileAction);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveFileAction = new QAction("Save File (Ctrl+S)");
    fileMenu->addAction(saveFileAction);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *saveFileAsAction = new QAction("Save File as (Ctrl+Shift+S)");
    fileMenu->addAction(saveFileAsAction);
    connect(saveFileAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

    fileMenu->addSeparator();

    QAction *quitAction = new QAction("Quit (Ctrl+Q)");
    fileMenu->addAction(quitAction);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    QMenu *objectMenu = menuBar()->addMenu("Objects");

    QAction *addObjectAction = new QAction("Add");
    objectMenu->addAction(addObjectAction);
    connect(addObjectAction, &QAction::triggered, this, &MainWindow::addObject);

    QAction *removeObjectAction = new QAction("Remove (Del)");
    objectMenu->addAction(removeObjectAction);
    connect(removeObjectAction, &QAction::triggered, this, &MainWindow::removeObject);

    QMenu *outputMenu = menuBar()->addMenu("Output");

    QAction *openFullscreenAction = new QAction("Open Canvas Fullscreen (F5)");
    outputMenu->addAction(openFullscreenAction);
    connect(openFullscreenAction, &QAction::triggered, this, &MainWindow::openFullscreen);

    QAction *openWindowAction = new QAction("Open Canvas Window (Shift+F5)");
    outputMenu->addAction(openWindowAction);
    connect(openWindowAction, &QAction::triggered, this, &MainWindow::openWindow);

    QMenu *helpMenu = menuBar()->addMenu("Help");

    QAction *aboutAction = new QAction("About dmxCanvas");
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    QAction *openGuideAction = new QAction("Manual");
    helpMenu->addAction(openGuideAction);
    connect(openGuideAction, &QAction::triggered, this, []{ QDesktopServices::openUrl(QUrl("https://github.com/arneBersch/dmxCanvas/blob/main/docs/manual.md")); });

    connect(new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this), &QShortcut::activated, this, &MainWindow::newFile); // New File
    connect(new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this), &QShortcut::activated, this, &MainWindow::openFile); // Open File
    connect(new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this), &QShortcut::activated, this, &MainWindow::saveFile); // Save File
    connect(new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S), this), &QShortcut::activated, this, &MainWindow::saveFileAs); // Save File As
    connect(new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this), &QShortcut::activated, this, &MainWindow::close); // Quit Application
    connect(new QShortcut(QKeySequence(Qt::Key_Delete), this), &QShortcut::activated, this, &MainWindow::removeObject); // Remove Object
    connect(new QShortcut(QKeySequence(Qt::Key_F5), this), &QShortcut::activated, this, &MainWindow::openFullscreen); // Open Canvas Fullscreen
    connect(new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F5), this), &QShortcut::activated, this, &MainWindow::openWindow); // Open Canvas Window

    QVBoxLayout* layout = new QVBoxLayout();
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);

    sacnServer = new SacnServer();
    layout->addWidget(sacnServer);

    objectTable = new QTableView();
    objectList = new ObjectList(VERSION);
    objectTable->setModel(objectList);
    objectTable->horizontalHeader()->setStretchLastSection(true);
    objectTable->verticalHeader()->hide();
    layout->addWidget(objectTable);

    this->show(); // Show window
    about(); // Open about window
}

MainWindow::~MainWindow() {
}

void MainWindow::addObject() {
    objectList->insertRows(objectList->rowCount(), 1);
}

void MainWindow::removeObject() {
    QModelIndexList selection = objectTable->selectionModel()->selectedIndexes();
    QList<int> rowsToRemove;
    for (QModelIndex index : selection) {
        if (!rowsToRemove.contains(index.row())) {
            rowsToRemove.append(index.row());
        }
    }
    std::sort(rowsToRemove.begin(), rowsToRemove.end());
    std::reverse(rowsToRemove.begin(), rowsToRemove.end());
    if (rowsToRemove.size() <= 0) {
        return;
    }
    QMessageBox messageBox;
    messageBox.setText("Do you want to delete " + QString::number(rowsToRemove.size()) + " Objects?");
    messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (messageBox.exec() != QMessageBox::Ok) {
        return;
    }
    for (const int row : rowsToRemove) {
        objectList->removeRows(row, 1);
    }
}

void MainWindow::openWindow() {
    new CanvasWindow(this, false, objectList, sacnServer);
}

void MainWindow::openFullscreen() {
    new CanvasWindow(this, true, objectList, sacnServer);
}

void MainWindow::openFile() {
    QString newFileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "dmxc Files (*.dmxc)");
    if (newFileName.isEmpty()) {
        return;
    }
    filename = newFileName;
    objectList->openFile(filename);
}

void MainWindow::newFile() {
    QMessageBox messageBox;
    messageBox.setText("Are you sure you want to open a new file?");
    messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (messageBox.exec() != QMessageBox::Ok) {
        return;
    }
    objectList->newFile();
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
    objectList->saveFile(filename);
}

void MainWindow::saveFileAs() {
    filename = QString(); // reset filename
    saveFile();
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
