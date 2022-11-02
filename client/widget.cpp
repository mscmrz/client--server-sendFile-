#include "widget.h"
#include "ui_widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QTextCodec>
#include <QFileDialog>
#include <QTextCodec>

Widget::Widget(QWidget *parent):
  QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
  setWindowTitle("client");

  ui->progressLabel->hide();

// QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
  tcpClient = new QTcpSocket(this);
  sendTimes = 0;

  connect(tcpClient, &QTcpSocket::connected, this, &Widget::send);
  connect(tcpClient, &QTcpSocket::bytesWritten, this, &Widget::goOnSend);
// connect(tcpClient, SIGNAL(connected()), this, SLOT(send()));      // When the connection is successful, start to transfer files
// connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
}

Widget::~Widget()
{
  delete ui;
}

void  Widget::send()   // Send file header information
{
  byteToWrite = localFile->size();     // The size of the remaining data
  totalSize   = localFile->size();

  loadSize = 4 * 1024;   // The size of data sent each time

  QDataStream  out(&outBlock, QIODevice::WriteOnly);
  QString      currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);

  out << qint64(0) << qint64(0) << currentFileName;

  totalSize   += outBlock.size(); // The total size is the file size plus the size of the file name and other information
  byteToWrite += outBlock.size();

  out.device()->seek(0);      // Go back to the beginning of the byte stream to write a qint64 in front, which is the total size and file name and other
                              // information size
  out << totalSize << qint64(outBlock.size());

  tcpClient->write(outBlock);     // Send the read file to the socket

  ui->progressLabel->show();
  ui->sendProgressBar->setMaximum(totalSize);
  ui->sendProgressBar->setValue(totalSize - byteToWrite);
}

void  Widget::goOnSend(qint64 numBytes)   // Start sending file content
{
  byteToWrite -= numBytes;  // Remaining data size
  outBlock     = localFile->read(qMin(byteToWrite, loadSize));
  tcpClient->write(outBlock);

  ui->sendProgressBar->setMaximum(totalSize);
  ui->sendProgressBar->setValue(totalSize - byteToWrite);

  if (byteToWrite == 0)   // Send completed
  {
    ui->sendStatusLabel->setText(tr("File sending completed!"));
  }
}

void  Widget::on_openpushbutton_clicked()
{
  ui->sendStatusLabel->setText(tr("Opening file ..."));
  ui->sendProgressBar->setValue(0);    // Not sent for the first time

  loadSize    = 0;
  byteToWrite = 0;
  totalSize   = 0;
  outBlock.clear();

  fileName  = QFileDialog::getOpenFileName(this);
  localFile = new QFile(fileName);
  localFile->open(QFile::ReadOnly);

  ui->sendStatusLabel->setText(tr("File% 1 opened").arg(fileName));
}

void  Widget::on_sendpushbutton_clicked()
{
  if (sendTimes == 0) // Only the first time it is sent, it happens when the connection generates the signal connect
  {
    tcpClient->connectToHost(QHostAddress("127.0.0.1"), 10000);
    sendTimes = 1;
  }
  else
  {
    send();    // When sending for the first time, connectToHost initiates the connect signal to call send, and you need to call send after the second time
  }

  ui->sendStatusLabel->setText(tr("Sending file% 1").arg(fileName));
}
