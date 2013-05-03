#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pakfile.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.pak)"));
    qDebug("File: %s!", fileName.toStdString().data());

    QFile pakFile(fileName);
    if (pakFile.open(QIODevice::ReadOnly)) {
        qDebug("Opened file...");

        struct PakFile *header;

        QByteArray data = pakFile.readAll();
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

            void *startMap = hash.value("maps/start.bsp");
        } else {
            qDebug("Incorrect magic id code, expected: PACK found: %c%c%c%c",header->id[0],header->id[1],header->id[2],header->id[3]);
        }

        pakFile.close();
    } else {
        qDebug("Unable to open file.");
    }
}
