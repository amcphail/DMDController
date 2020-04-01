#ifndef DMDCONTROLLER_H
#define DMDCONTROLLER_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QUdpSocket>
#include <QByteArray>

#include <DMDInterface.h>

namespace Ui {
    class DMDController;
}

class DMDController : public QMainWindow {
    Q_OBJECT
public:
    DMDController(QWidget *parent = 0);
    ~DMDController();

protected:
    void changeEvent(QEvent *e);

    void display(int,QImage);

    void scrollHorizontal(int);
    void scrollVertical(int);

    void updateResult(int);

    int processTheDatagram(QByteArray);

    QGraphicsPixmapItem* pixmap0;
    QGraphicsScene* scene0;
    QGraphicsView* view0;

    QGraphicsPixmapItem* pixmap1;
    QGraphicsScene* scene1;
    QGraphicsView* view1;

    int len0;
    char* buf0;

    int len1;
    char* buf1;

    char* image_map;
    char* image_sequence;

    int scroll_hor;
    int scroll_ver;

    DMDInterface* dmd;

    QString local_address;
    QHostAddress address;
    int port;

    QUdpSocket* socket;

    QByteArray recv_data;

    unsigned long int error;

private:
    Ui::DMDController *ui;

private slots:
    void on_pbPauseSequencer_clicked();
    void on_pbRunSequencer_clicked();
    void on_pbUploadImages_clicked();
    void on_leImage1Filename_returnPressed();
    void on_leImage0Filename_returnPressed();
    void on_pbImage1GetFile_clicked();
    void on_pbImage0GetFile_clicked();
    void on_cbSwapImages_stateChanged(int );
    void on_pbShutDown_clicked();
    void on_pbTest_clicked();
    void on_pbHScrollPlus_clicked();
    void on_pbHScrollMinus_clicked();
    void on_pbVScrollPlus_clicked();
    void on_pbVScrollMinus_clicked();

    void readPendingDatagrams();
    void on_cbMapImages_stateChanged(int arg1);
    void on_pbMapGetFile_clicked();
    void on_leMapFilename_returnPressed();
    void on_pbSeqGetFile_clicked();
    void on_leSeqFilename_returnPressed();
};

#endif // DMDCONTROLLER_H
