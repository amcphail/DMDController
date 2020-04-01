#include <QFileDialog>
#include <QBitmap>
#include <QMessageBox>
#include <QDebug>

#include <QNetworkInterface>
#include <QByteArray>

#include <string.h>

#include "DMDController.h"
#include "ui_DMDController.h"

#include "dmd_file.h"
#include "DMDInterface.h"

#define DMD_ADDRESS "192.168.0.10"
#define DMD_IMAGE_FILES "/home/lab/data/Programming/DMDController/DMDImages"
#define DMD_SEQUENCE_FILES "/home/lab/data/Programming/DMDController/DMDSequences"

#define CONTROLLER_PORT 52987

DMDController::DMDController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DMDController)
{
    ui->setupUi(this);

    QImage image0(IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);
    QImage image1(IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);

    scene0 = new QGraphicsScene(ui->fImage0);
    view0 = new QGraphicsView(ui->fImage0);
    pixmap0 = scene0->addPixmap(QBitmap::fromImage(image0 ,0));
    view0->setScene(scene0);

    scene1 = new QGraphicsScene(ui->fImage1);
    view1 = new QGraphicsView(ui->fImage1);
    pixmap1 = scene1->addPixmap(QBitmap::fromImage(image1 ,0));
    view1->setScene(scene1);

    image_map = 0;
    image_sequence = 0;

    scroll_hor = 0;
    scroll_ver = 0;

    dmd = new DMDInterface(DMD_ADDRESS);

    dmd->oneTimeInitialiseDMD();

    QNetworkInterface net_if = QNetworkInterface::interfaceFromName(QString("eno1"));
    QList<QNetworkAddressEntry> net_list = net_if.addressEntries();

    address = net_list[0].ip();

    socket = new QUdpSocket(this);
    socket->bind(address,CONTROLLER_PORT);

    connect(socket,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));

    error = 0;

}

DMDController::~DMDController()
{
    socket->close();

    delete socket;

    delete dmd;

    if (image_map) free(image_map);
    if (image_sequence) free(image_sequence);

    delete view0;
    delete scene0;

    delete view1;
    delete scene1;

    delete ui;
}

void DMDController::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DMDController::on_cbSwapImages_stateChanged(int newValue)
{
    if (newValue == Qt::Checked) {
        ui->fImage1->setEnabled(true);
        ui->leImage1Filename->setEnabled(true);
        ui->pbImage1GetFile->setEnabled(true);

        ui->leMapFilename->setEnabled(false);
        ui->pbMapGetFile->setEnabled(false);
        ui->leSeqFilename->setEnabled(false);
        ui->pbSeqGetFile->setEnabled(false);

    }
    else if (newValue == Qt::Unchecked) {
        ui->fImage1->setEnabled(false);
        ui->leImage1Filename->setEnabled(false);
        ui->pbImage1GetFile->setEnabled(false);
    }
    else {// Qt::PartiallyChecked
    }
}


void DMDController::on_cbMapImages_stateChanged(int newValue)
{
    if (newValue == Qt::Checked) {
        ui->fImage0->setEnabled(false);
        ui->leImage0Filename->setEnabled(false);
        ui->pbImage0GetFile->setEnabled(false);

        ui->fImage1->setEnabled(false);
        ui->leImage1Filename->setEnabled(false);
        ui->pbImage1GetFile->setEnabled(false);

        ui->leMapFilename->setEnabled(true);
        ui->pbMapGetFile->setEnabled(true);
        ui->leSeqFilename->setEnabled(true);
        ui->pbSeqGetFile->setEnabled(true);
    }
    else if (newValue == Qt::Unchecked) {
        ui->leMapFilename->setEnabled(false);
        ui->pbMapGetFile->setEnabled(false);
        ui->leSeqFilename->setEnabled(false);
        ui->pbSeqGetFile->setEnabled(false);
    }
    else {// Qt::PartiallyChecked
    }
}

void DMDController::on_pbImage0GetFile_clicked()
{
    QString image_filename = QFileDialog::getOpenFileName( this, tr("Image File"), DMD_IMAGE_FILES, tr("Image files (*.*)" ));

    if ( !image_filename.isEmpty() ) {
        ui->leImage0Filename->setText(image_filename);
        on_leImage0Filename_returnPressed();
    }
}

void DMDController::on_pbImage1GetFile_clicked()
{
    QString image_filename = QFileDialog::getOpenFileName( this, tr("Image File"), DMD_IMAGE_FILES, tr("Image files (*.bmp)" ));

    if ( !image_filename.isEmpty() ) {
        ui->leImage1Filename->setText(image_filename);
        on_leImage1Filename_returnPressed();
    }
}

void DMDController::on_pbMapGetFile_clicked()
{
    QString map_filename = QFileDialog::getOpenFileName( this, tr("Map File"), DMD_IMAGE_FILES, tr("Map files (*.txt)" ));

    if ( !map_filename.isEmpty() ) {
        ui->leMapFilename->setText(map_filename);
        on_leMapFilename_returnPressed();
    }
}

void DMDController::on_pbSeqGetFile_clicked()
{
    QString seq_filename = QFileDialog::getOpenFileName( this, tr("Sequence File"), DMD_SEQUENCE_FILES, tr("Sequence files (*.txt)" ));

    if ( !seq_filename.isEmpty() ) {
        ui->leSeqFilename->setText(seq_filename);
        on_leSeqFilename_returnPressed();
    }
}

void DMDController::on_leImage0Filename_returnPressed()
{
    QString image_filename = ui->leImage0Filename->text();
    std::string filename1 = image_filename.toStdString();
    const char* filename = filename1.c_str();

    int flen = 0;
    char* fbuf;

    int result;

    result = loadFile(filename,flen,fbuf);

    if (result) {
        QMessageBox::information(this,tr("Image Loader"),tr("File note loaded %1.").arg(filename));
        return;
    }

    getRawBmp(flen,fbuf,len0,buf0);
    free(fbuf);

    if (len0 != IMAGE_COLS*IMAGE_ROWS/8) {
        QMessageBox::information(this,tr("Image Loader"),tr("Incorrect image size for %1.").arg(image_filename));
        return;
    }

    QImage image0((uchar*)buf0,IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);
    this->display(0,image0);
}

void DMDController::on_leImage1Filename_returnPressed()
{
    QString image_filename = ui->leImage1Filename->text();
    std::string filename1 = image_filename.toStdString();
    const char* filename = filename1.c_str();

    int flen = 0;
    char* fbuf;

    int result;

    result = loadFile(filename,flen,fbuf);

    if (result) {
        QMessageBox::information(this,tr("Image Loader"),tr("File not loaded %1.").arg(filename));
        printf("Could not load file:\t%s\n",filename);
        return;
    }

    getRawBmp(flen,fbuf,len1,buf1);
    free(fbuf);

    if (len1 != IMAGE_COLS*IMAGE_ROWS/8) {
        QMessageBox::information(this,tr("Image Loader"),tr("Incorrect image size for %1.").arg(image_filename));
        return;
    }

    QImage image1((uchar*)buf1,IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);
//    image1.load(image_filename);
    this->display(1,image1);
}


void DMDController::on_leMapFilename_returnPressed()
{
    QString map_filename = ui->leMapFilename->text();
    std::string filename1 = map_filename.toStdString();

    if (image_map) free(image_map);
    image_map = (char*)malloc(sizeof(char)*strlen(filename1.c_str()));
    strcpy(image_map,filename1.c_str());
}

void DMDController::on_leSeqFilename_returnPressed()
{
    QString seq_filename = ui->leSeqFilename->text();
    std::string filename1 = seq_filename.toStdString();

    if (image_sequence) free(image_sequence);
    image_sequence = (char*)malloc(sizeof(char)*strlen(filename1.c_str()));
    strcpy(image_sequence,filename1.c_str());
}

void DMDController::on_pbUploadImages_clicked()
{
    int error;

    if (ui->cbSwapImages->isChecked()) { // swap images
        error = dmd->setupSwapImage(buf0,buf1);
    }
    else if (ui->cbMapImages->isChecked()) { // image map
        error = dmd->setupMultipleImages(image_map,image_sequence);
    }
    else {
        error = dmd->setupSingleImage(buf0);
    }
    this->updateResult(error);
}

void DMDController::on_pbRunSequencer_clicked()
{
    int error;

    error = dmd->runSequencer();

    this->updateResult(error);
}

void DMDController::on_pbPauseSequencer_clicked()
{
    int error;

    error = dmd->pauseSequencer();

    this->updateResult(error);
}

void DMDController::on_pbShutDown_clicked()
{
    dmd->quit();

    QApplication::quit();
}

void DMDController::on_pbTest_clicked()
{
    int error;

    error = dmd->test();

    this->updateResult(error);
}

void DMDController::on_pbHScrollPlus_clicked()
{
    this->scrollHorizontal(ui->leHScroll->text().toInt());
}

void DMDController::on_pbHScrollMinus_clicked()
{
    this->scrollHorizontal(-ui->leHScroll->text().toInt());
}

void DMDController::on_pbVScrollPlus_clicked()
{
    this->scrollVertical(ui->leVScroll->text().toInt());
}

void DMDController::on_pbVScrollMinus_clicked()
{
    this->scrollVertical(-ui->leVScroll->text().toInt());
}

void DMDController::display(int num, QImage image)
{
    QImage image_scaled;

    QPixmap pixmap;

    switch (num) {
    case 0:
        image_scaled = image.scaled(ui->fImage0->size());

        pixmap = QPixmap::fromImage(image_scaled,0);

        scene0->removeItem(pixmap0);
        delete pixmap0;
        pixmap0 = scene0->addPixmap(pixmap);
        view0->show();
        break;
    case 1:
        image_scaled = image.scaled(ui->fImage1->size());

        pixmap = QPixmap::fromImage(image_scaled,0);

        scene1->removeItem(pixmap1);
        delete pixmap1;
        pixmap1 = scene1->addPixmap(pixmap);
        view1->show();
        break;
    default:
        break;
    }
}

void DMDController::scrollHorizontal(int pixels)
{
    if (ui->cbSwapImages->isChecked()) { // disable
        ui->cbSwapImages->setChecked(false);
    }

    scroll_hor += pixels;

    if ((scroll_hor >= IMAGE_COLS) || (scroll_hor < 0)) scroll_hor %= IMAGE_COLS;

    ui->lbHorShiftValue->setText(QString::number(scroll_hor));

    scrollHor(IMAGE_COLS,IMAGE_ROWS,-pixels,buf0);

    QImage image((uchar*)buf0,IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);

    this->display(0,image);
}

void DMDController::scrollVertical(int pixels)
{
    if (ui->cbSwapImages->isChecked()) { // disable
        ui->cbSwapImages->setChecked(false);
    }

    scroll_ver += pixels;

    if ((scroll_ver >= IMAGE_ROWS) || (scroll_ver < 0)) scroll_ver %= IMAGE_ROWS;

    ui->lbVerShiftValue->setText(QString::number(scroll_ver));

    scrollVer(IMAGE_COLS,IMAGE_ROWS,pixels,buf0);

    QImage image((uchar*)buf0,IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);

    this->display(0,image);
}

void DMDController::updateResult(int error)
{
    if (error) {
        ui->lbResult->setText(QString("Error: %1").arg(error));
    }
    else {
        ui->lbResult->setText("Success");
    }
}

void DMDController::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        processTheDatagram(datagram);
    }
}

int DMDController::processTheDatagram(QByteArray data)
{
    int error;

    bool horizontal;

    fprintf(stderr,"%s\n",data.data());

    if (data[0] == 'H') horizontal = true;
    else if (data[0] == 'V') horizontal = false;
    else return -1;

    data.remove(0,1);

    int pixels = data.toInt();

    if (horizontal) this->scrollHorizontal(pixels);
    else this->scrollVertical(pixels);

    QImage image((uchar*)buf0,IMAGE_COLS,IMAGE_ROWS,QImage::Format_Mono);
    this->display(0,image);

    error = dmd->setupSingleImage(buf0);

    this->updateResult(error);

    return error;
}


