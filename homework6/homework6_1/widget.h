#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QUdpSocket;
namespace Ui {
class Widget;
}

enum MsgType{Msg, UsrEnter, UsrLeft, FileName, Refuse};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString usrname);
    ~Widget();

protected:
    void usrEnter(QString usrname,QString ipaddr); //新用户加入
    void usrLeft(QString usrname, QString time);  //用户离开
    void sndMsg(MsgType type, QString srvaddr=""); //广播UDP消息

    QString getIP();  //获取IP地址
    QString getUsr(); //获取用户名
    QString getMsg(); //获取聊天信息

private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;

private slots:
    void processPendingDatagrams();  //接收UDP消息
    void on_sendBtn_clicked();
};

#endif // WIDGET_H
