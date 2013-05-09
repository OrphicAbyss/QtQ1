#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pakfile.h"
#include "bspfile.h"

#include <QTimer>
#include <QDebug>
#include <QFileDialog>

#include "gametime.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renderFrame()));
    timer->start(1);

    fpsTimer = new QTimer(this);
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
    fpsTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeFile()
{
    data.clear();
}

void MainWindow::renderFrame()
{
    GameTime *time = GameTime::instance();
    time->newFrame();

    ui->widget->repaint();
}

void MainWindow::updateFPS()
{
    int fps = GameTime::instance()->markFPS();
    ui->statusBar->showMessage(QString("FPS: %1").arg(fps));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.pak)"));
    qDebug("File: %s!", fileName.toStdString().data());

    QFile pakFile(fileName);
    if (pakFile.open(QIODevice::ReadOnly)) {
        qDebug("Opened file...");

        struct PakFile *header;

        data = pakFile.readAll();
        header = (PakFile *)data.data();

        if (0 == strncmp("PACK",&header->id[0], 4)) {
            struct PakEntry *pakEntries = header->getPackEntries();

            int fileSize = data.size();
            int numFiles = header->entityCount();

            qDebug("Pak id string: %c%c%c%c",header->id[0],header->id[1],header->id[2],header->id[3]);
            qDebug("Entities: %d Offset: %d Size: %d",
                    numFiles,
                    header->entryOffset,
                    fileSize);

            QHash<QString, void *> hash;

            for (long i=0; i<numFiles; i++) {
                qDebug("Pak Entry: %s", pakEntries[i].filename);
                hash.insert(pakEntries[i].filename, pakEntries[i].getFileData(header));
            }

            BSPFile *startMap = (BSPFile *)hash.value("maps/start.bsp");
            if (startMap->isSupportedFile()) {
                qDebug("Valid map.");
                qDebug("Number of models in map: %d", startMap->countOfModels());
                qDebug("Number of vertices: %d", startMap->countOfVertices());

                ui->widget->setMap(startMap);
                ui->widget->repaint();

                //char *entities = startMap->getEntities();
                //qDebug("%s",entities);
            } else {
                qDebug("Invalid map.");
            }
        } else {
            qDebug("Incorrect magic id code, expected: PACK found: %c%c%c%c",header->id[0],header->id[1],header->id[2],header->id[3]);
        }

        pakFile.close();
    } else {
        qDebug("Unable to open file.");
    }
}
