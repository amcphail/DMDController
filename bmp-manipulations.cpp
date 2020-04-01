void DMDController::on_leImage0Filename_returnPressed()
{
    QString image_filename = ui->leImage0Filename->text();

    const char* filename = image_filename.toStdString().c_str();

    int flen, blen;
    char* fbuf;
    char* bbuf;

    loadFile(filename,flen,fbuf);
    getRawBmp(flen,fbuf,blen,bbuf);

    if (blen != IMAGE_COLS*IMAGE_ROWS/8) {
        printf("Incorrect Image Size");
        return;
    }

    //printf("blen:\t%d\n",blen);

    //    bool loaded = image0.load(image_filename);
    //QImage image_raw((uchar*)bbuf,48,24,QImage::Format_MonoLSB);
    image0.load(image_filename);
    QImage image_scaled = image0.scaled(ui->fImage0->size());

    /*
    printf("Depth:\t%d\n",image.depth());
    printf("Width:\t%d\n",image.width());
    printf("Height:\t%d\n",image.height());
    printf("Byte count:\t%d\n",image.byteCount());
    printf("Bytes per line:\t%d\n",image.bytesPerLine());

    uchar* sl = image.scanLine(0);

    int width = image.width();

    for (int k = 0; k < width; k++) printf("Byte %d:\t%d\n",k,sl[k]);

    int ROWS = 24;
    int COLS = 48;

    int pad = 4 - ((COLS/8)%4);

    int PAD_COLS = (COLS/8+pad);

    uchar* buf = image_raw.bits();

    for (int j = ROWS-1; j >= 0; j--) {
      for (int i = 0; i < PAD_COLS; i++) {
        printf("%d\t",buf[j*PAD_COLS+i]);
      }
      printf("\n");
      for (int i = 0; i < PAD_COLS; i++) {
        printf("%d\t",(unsigned char)bbuf[j*PAD_COLS+i]);
      }
      printf("\n\n");    }
    */

    //if (!image) QMessageBox::information(this,tr("Image Loader"),tr("Cannot load %1.").arg(image_filename));

    QPixmap pixmap = QPixmap::fromImage(image_scaled,0);

    scene0->removeItem(pixmap0);
    delete pixmap0;
    pixmap0 = scene0->addPixmap(pixmap);
    QSize image_size = view0->size();
    view0->show();



}
