/*!
  Slot. Prepares to send image to LUXBEAM on signal from Export tab
*/
void CommunicationTab::sigSendImg(QStringList fileNames, int inumBase, int imagemode, int inumSize, bool jumbo, bool stream = false){
    int dataPacketSize = 0;
    int addoffset = 0;
     ushort seq_no;
    if(jumbo == true){
        dataPacketSize = 8880;
        addoffset = 37;
    }else{
        dataPacketSize = 1440;
        addoffset = 6;
    }
    transferError = false;
    uint inum = inumBase;
         seq_no= 1;
         timer.start(1000);
         comobj->ResetSeqNo();
         loop->exec();
         timer.stop();

             uint offset =0;
             QByteArray imageData;
             QByteArray imageData1[24];
             QByteArray imageData2[8];
             QByteArray imageData3[8];
              ushort seq_no_in = seq_no;
              ushort inum_in = inum;
	if(fileNames[i].endsWith(".bmp",Qt::CaseSensitive) || fileNames[i].endsWith(".jpg",Qt::CaseSensitive) && transferError == false){

            QImage img(QString("%1").arg(fileNames[i]));
            QRgb color;
            if(img.width() != 1920){
                QMessageBox msgBox;
                msgBox.setText(QString("Image %1 does not have the correct width. Images uploaded need to have 1920 px width. Please rescale the image").arg(fileNames[i]));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Critical);
                transferError= true;
                int ret = msgBox.exec();
            }
            if(img.height()!= inumSize){
                QMessageBox msgBox;
                msgBox.setText(QString("Image %1 does not have the correct height with your Image size in rows setting. Please rescale the image or change the image size in rows spinbox to match your image height").arg(fileNames[i]));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Critical);
                transferError = true;
                int ret = msgBox.exec();
            }


             if(!transferError){
                for ( int row = img.height()-1; row >= 0; --row ){
                    for ( int col = 0; col < img.width(); col=col+8 )
                    {
                        quint8 msg[8]= {0,0,0,0,0,0,0,0};
                        quint8 msg2[8]={0,0,0,0,0,0,0,0};
                        quint8 msg3[8]={0,0,0,0,0,0,0,0};


                        for(int n=0;n<8;n++){
                         if(imagemode == 2){
                          color = img.pixel(col+n,row);
                          int grayvalue = qGray(color);
                          for(int bit=0;bit<8;bit++){
                             if((grayvalue >> ((bit)) & 1) == 1){
                              msg[bit] |= (1 << (7-n));
                             }
                           }


                         }else if(imagemode == 3){
                             color = img.pixel(col+n,row);
                             int grayvalue = qGray(color);
                              if(grayvalue > 127){
                                 msg[0] |= (1 << (7-n));
                                }
                             //  QColor pxl(img.pixel(col+n,row));
                            // if(pxl.red()!=0){
                             //   msg[0] |= (1 << n);
                           // }
                         }else if(imagemode == 1){
                             color = img.pixel(col+n,row);
                             int redvalue = qRed(color);
                             int greenvalue = qGreen(color);
                             int bluevalue = qBlue(color);
                             for(int bit=0;bit<8;bit++){
                                 if((redvalue >> ((bit)) & 1) == 1){
                                        msg[bit] |= (1 << (7-n));
                                 }
                                 if((greenvalue >> ((bit)) & 1) == 1){
                                     msg2[bit] |= (1 << (7-n));
                                 }
                                 if((bluevalue >> ((bit)) & 1) == 1){
                                    msg3[bit] |= (1 << (7-n));
                                 }
                              }
                         }
                        }

                      if(imagemode == 2){
                        for(int bit=0;bit<8;bit++){
                          imageData1[bit].append(msg[bit]);
                         }
                      }else if(imagemode == 1){
                       for(int bit=0;bit<8;bit++){
                         imageData1[bit].append(msg[bit]);
                         imageData1[bit+8].append(msg2[bit]);
                         imageData1[bit+16].append(msg3[bit]);
                        }
                      }else if(imagemode == 3){
                          imageData.append(msg[0]);
                        }

                    }
                }
              }
              }

 if(!transferError){
            if(imagemode == 2){
             // for(int bit=0;bit<8;bit++){
               // imageData.append(imageData1[bit]);
            //  }
            }else if(imagemode == 1){
             for(int bit=0;bit<8;bit++){
               imageData.append(imageData1[bit]);
             }
              for(int bit=0;bit<8;bit++){
                imageData.append(imageData2[bit]);
              }
              for(int bit=0;bit<8;bit++){
                imageData.append(imageData3[bit]);
              }
              }
 }




            if(imagemode == 1 && transferError == false){
                for(int bit=0;bit<24;bit++){
                offset = 0;
                if(imageData1[bit].size()>dataPacketSize){
                    int pos = 0, arrsize = imageData1[bit].size(), sizeInArray = dataPacketSize;
                    QList<QByteArray> arrays;
                    while(pos<arrsize){
                        QByteArray arr = imageData1[bit].mid(pos, sizeInArray);
                        arrays << arr;
                        pos+=arr.size();
                    }
                    for(int j=0; j<arrays.size(); j++){
                       //  QThread::usleep(1);
                       // QThread::msleep(100);
                        timer.start(1000);
                        while(comobj->LoadImageData(seq_no,inum,offset,arrays[j]) == -1){}
                        timer.stop();
                         offset = offset+addoffset;

                          seq_no++;
                    }
                    inum++;
                }else{

                }
                }
            if(stream == false){
            //CHECK FOR ERROR
            if(!transferError){
                QThread::msleep(1);
                timer.start(1000);
                comobj->RequestSeqNoError();
                loop->exec();
                timer.stop();
                while(is_error == 1){
                    timer.start(5000);
                    comobj->ResetSeqNo();
                    loop->exec();
                    timer.stop();
                    seq_no = 1;
                    QThread::usleep(1);
                    int start;
                    if(seq_no_in<seq_no){
                        start = (seq_no - seq_no_in)/30;
                        inum = inum_in + start;
                    }else{
                        start = (65535 - seq_no_in + seq_no)/30;
                        inum = inum_in + start;
                    }

                       for(int bit=start;bit<8;bit++){
                           offset = 0;
                        if(imageData.size()>dataPacketSize){
                            int pos = 0, arrsize = imageData1[bit].size(), sizeInArray = dataPacketSize;
                            QList<QByteArray> arrays;
                            while(pos<arrsize){
                                QByteArray arr = imageData1[bit].mid(pos, sizeInArray);
                                arrays << arr;
                                pos+=arr.size();
                            }
                            for(int j=last_seq_no; j<arrays.size(); j++){
                            // QThread::msleep(100);
                                timer.start(5000);
                                while(comobj->LoadImageData(seq_no,inum,offset,arrays[j]) == -1){}
                                timer.stop();
                                 offset = offset+addoffset;

                                  seq_no++;
                            }
                        }else{

                        }

                        inum++;
                       }

                    QThread::msleep(10);
                    timer.start(5000);
                    comobj->RequestSeqNoError();
                    loop->exec();
                    timer.stop();
                }
            }



            else if(imagemode == 2 && transferError == false){
                for(int bit=0;bit<8;bit++){
                offset = 0;
                if(imageData1[bit].size()>dataPacketSize){
                    int pos = 0, arrsize = imageData1[bit].size(), sizeInArray = dataPacketSize;
                    QList<QByteArray> arrays;
                    while(pos<arrsize){
                        QByteArray arr = imageData1[bit].mid(pos, sizeInArray);
                        arrays << arr;
                        pos+=arr.size();
                    }
                    for(int j=0; j<arrays.size(); j++){
                        // QThread::usleep(1);
                        timer.start(5000);
                        while(comobj->LoadImageData(seq_no,inum,offset,arrays[j]) == -1){}
                        timer.stop();


                         offset = offset+addoffset;

                          seq_no++;
                    }
                    inum++;
                }else{

                }
                }
                // qDebug() << addoffset << ":::" << dataPacketSize << endl;
 if(stream == false){
            //CHECK FOR ERROR
          if(transferError == true){
               i = fileNames.size();
          }else{
            QThread::msleep(1);
            timer.start(5000);
            comobj->RequestSeqNoError();
            loop->exec();
            timer.stop();
            if(transferError == true){
                i = fileNames.size();
            }else{
            while(is_error == 1){
                timer.start(5000);
                comobj->ResetSeqNo();
                loop->exec();
                timer.stop();
                seq_no = 1;
                QThread::usleep(1);
                inum = inum_in;
                int start;
               /* if(seq_no_in<seq_no){
                    start = (seq_no - seq_no_in)/30;
                    qDebug() << "START: " << start << "INUMIN: " << inum_in << "SEQ_NO: " << seq_no << "SEQ_NO_IN"  << seq_no_in << endl;

                    inum = inum_in;// + start;
                }else{
                    start = (65535 - seq_no_in + seq_no)/30;
                    inum = inum_in + start;
                }*/

                   for(int bit=0;bit<8;bit++){
                       offset = 0;
                    if(imageData.size()>dataPacketSize){
                        int pos = 0, arrsize = imageData1[bit].size(), sizeInArray = dataPacketSize;
                        QList<QByteArray> arrays;
                        while(pos<arrsize){
                            QByteArray arr = imageData1[bit].mid(pos, sizeInArray);
                            arrays << arr;
                            pos+=arr.size();
                        }
                        for(int j=0; j<arrays.size(); j++){
                            //comobj->LoadImageData(seq_no,inum,offset,arrays[j]);
                            timer.start(5000);
                            while(comobj->LoadImageData(seq_no,inum,offset,arrays[j]) == -1){}
                            timer.stop();
                             offset = offset+addoffset;

                              seq_no++;
                        }
                    }else{

                    }

                    inum++;
                   }

                QThread::msleep(10);
                timer.start(1000);
                comobj->RequestSeqNoError();
                loop->exec();
                timer.stop();
            }
             //qDebug() << "We have seq error gone" << endl;
                }
          }
             emit updateProgressBar(progress);
            }
            }
            else if(imagemode == 3 && transferError == false){
                     offset = 0;
                     if(imageData.size()>dataPacketSize){
                         int pos = 0, arrsize = imageData.size(), sizeInArray = dataPacketSize;
                         QList<QByteArray> arrays;
                         while(pos<arrsize){
                             QByteArray arr = imageData.mid(pos, sizeInArray);
                             arrays << arr;
                             pos+=arr.size();
                         }
                         for(int j=0; j<arrays.size(); j++){
                              //QThread::usleep(1);
                             timer.start(500);
                              while(comobj->LoadImageData(seq_no,inum,offset,arrays[j]) == -1){}
                              timer.stop();
                              offset = offset+addoffset;

                               seq_no++;

                         }
                     }else{

                     }
                      if(stream == false){
            if(transferError == false){
                 //CHECK FOR ERROR
                 QThread::msleep(10);
                 timer.start(5000);
                 comobj->RequestSeqNoError();
                 loop->exec();
                 timer.stop();
                 while(is_error == 1){
                     timer.start(5000);
                     comobj->ResetSeqNo();
                     loop->exec();
                     timer.stop();
                     seq_no = 1;
                     QThread::usleep(1);
                     inum = inum_in;


                         offset = 0;
                         if(imageData.size()>dataPacketSize){
                             int pos = 0, arrsize = imageData.size(), sizeInArray = dataPacketSize;
                             QList<QByteArray> arrays;
                             while(pos<arrsize){
                                 QByteArray arr = imageData.mid(pos, sizeInArray);
                                 arrays << arr;
                                 pos+=arr.size();
                             }
                             for(int j=last_seq_no; j<arrays.size(); j++){
                                 timer.start(1000);
                                 while(comobj->LoadImageData(seq_no,inum,offset,arrays[j]) == -1){}
                                 timer.stop();
                                  offset = offset+addoffset;

                                   seq_no++;
                             }
                         }else{

                         }

                     QThread::msleep(10);
                     timer.start(1000);
                     comobj->RequestSeqNoError();
                     loop->exec();
                     timer.stop();
                 }
            }
                 inum++;
                 emit updateProgressBar(progress);
                 }
            }
    }
      stream_seq_no = seq_no;
      transferError = false;
    }

