#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui
{
class Widget;
}
QT_END_NAMESPACE

class Widget: public QWidget
{
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);

  ~Widget();

public slots:
private slots:
  void  on_pushButton_clicked();

  void  Acceptconnection();

  void  readClient();

private:
  Ui::Widget *ui;
  QTcpServer *server;
  QTcpSocket *receivedSocket;
  QFile      *newFile;
  QByteArray  inBlock;
  QString     fileName;
  quint64     totalSize;// Total file size to send (file contents & filename information)
  quint64     byteReceived;// The size that has been sent
};

#endif // WIDGET_H
