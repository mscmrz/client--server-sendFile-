#include "widget.h"
#include "ui_widget.h"

#include <QTextCodec>

Widget::Widget(QWidget *parent):
  QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
  setWindowTitle("server");
  ui->progressLabel->hide();
// QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
}

Widget::~Widget()
{
  delete ui;
}

void  Widget::Acceptconnection()
{
  ui->receivedStatusLabel->setText(tr("Connected, preparing to receive files!"));

  receivedSocket = server->nextPendingConnection();
  connect(receivedSocket, &QTcpSocket::readyRead, this, &Widget::readClient);
// connect(receivedSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
}

void  Widget::readClient()
{
  ui->receivedStatusLabel->setText(tr("Receiving file ..."));

  if (byteReceived == 0)   // just started to receive data, this data is file information
  {
    ui->receivedProgressBar->setValue(0);

    QDataStream  in(receivedSocket);
    in >> totalSize >> byteReceived >> fileName;
// fileName = "Files /" + fileName;
    newFile = new QFile(fileName);
    newFile->open(QFile::WriteOnly);
  }
  else   // Officially read the file content
  {
    inBlock = receivedSocket->readAll();

    byteReceived += inBlock.size();
    newFile->write(inBlock);
    newFile->flush();
  }

  ui->progressLabel->show();
  ui->receivedProgressBar->setMaximum(totalSize);
  ui->receivedProgressBar->setValue(byteReceived);

  if (byteReceived == totalSize)
  {
    ui->receivedStatusLabel->setText(tr("% 1receive completed").arg(fileName));

    inBlock.clear();
    byteReceived = 0;
    totalSize    = 0;
  }
}

void  Widget::on_pushButton_clicked()
{
  totalSize    = 0;
  byteReceived = 0;

  server = new QTcpServer(this);
  server->listen(QHostAddress("127.0.0.1"), 10000);

  connect(server, &QTcpServer::newConnection, this, &Widget::Acceptconnection);
// connect(server, SIGNAL(newConnection()), this, SLOT(Acceptconnection()));

  ui->receivedProgressBar->setValue(0);
  ui->receivedStatusLabel->setText(tr("Start listening ..."));
}
