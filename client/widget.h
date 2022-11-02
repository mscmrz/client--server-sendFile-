#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <string>

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
protected:
private slots:
  void  goOnSend(qint64 numBytes);

  void  send();  // Transfer file header information

// void  Goonsend(quint64);// Transfer file contents

  void  on_openpushbutton_clicked();

  void  on_sendpushbutton_clicked();

private:
  Ui::Widget *ui;
  QTcpSocket *tcpClient;
  QFile      *localFile;
  QString     fileName;        // FileName
  QByteArray  outBlock;
  quint64     loadSize;        // of Sub-pass  The size of each send data
  quint64     byteToWrite;     // The remaining data size
  quint64     totalSize;       // Total file size
  int         sendTimes;
};

#endif // WIDGET_H
