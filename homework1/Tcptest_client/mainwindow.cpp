#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();

    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);

    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("8765");

}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if(ui->pushButton_Connect->text() == tr("连接"))
    {
        QString IP;
        int port;

        //获取IP地址
        IP = ui->lineEdit_IP->text();
        //获取端口号
        port = ui->lineEdit_Port->text().toInt();

        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            return;
        }
        qDebug() << "Connect successfully!";

        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
   // qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
     //获取文本框内容并发送
    qDebug()<<"单次发送！";
    QTime startTime = QTime::currentTime();
    qDebug()<<startTime;
    QString str = startTime.toString("h:m:s.z");
    qDebug()<<str;
    socket->write(str.toUtf8());
    socket->flush();
}

void MainWindow::socket_Read_Data()
{
    qDebug()<<"执行了一次readbuffer的操作！";
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {

        QString str = ui->textEdit_Recv->toPlainText();
        str=tr(buffer);
        QTime startTime = QTime::fromString(str, "h:m:s.z");
        qDebug()<<startTime;
        QTime stopTime = QTime::currentTime();
        qDebug()<<stopTime;
        float elapsed = startTime.msecsTo(stopTime);
        //QString delay_time=QString::number(elapsed, 10);
        QString delay_time=QString("%1").arg(elapsed);
        qDebug()<<"Elapse Time is "<<elapsed<<"ms";
        //刷新显示
        ui->textEdit_Recv->setText(tr("The elapse time is ")+delay_time+tr("ms. "));
      }



        //存到txt文本里(这部分暂未测试)
        /*QFile data("data.txt");
        if(data.open(QFile::WriteOnly|QFile::Truncate))
        {
            QTextStream out(&data);
            QTime currenttime = QTime::currentTime();
            out<<QObject::tr("记录")<<currenttime;
            out<<qSetFieldWidth(10)<<left<<delay_time<<endl;
            data.close();
        }*/

}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
     ui->pushButton_2->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText("连接");
    qDebug() << "Disconnected!";
}

void MainWindow::on_pushButton_clicked()
{

    qDebug()<<"显示图像";
//    QCustomPlot *pricePlot = CreateAndInitPlotGraph(QString("Time(s)"),0,1200,QString("PRICE(RMB)"),0,20,
//                                                          QRect(0,0,800,475),QString("PRICE"),fifthPageWidget);
//    m_plotNameMap.insert("PRICE",pricePlot);
//    m_plotRowNumMap.insert(4,pricePlot);
//    m_plotNumAndName.insert(4,"PRICE");
//    pricePlot->graph(0)->addData(0,0);
}
/*
QCustomPlot* MainWindow::CreateAndInitPlotGraph(QString xLabel,int xRangeL,int xRangeR,QString yLabel,int yRangeL,int yRangeR,QRect rect,QString name,QWidget *parent)
{
    QCustomPlot *customPlot = new QCustomPlot(parent);
    customPlot->legend->setVisible(true);
    QFont lengendFont = font();
    lengendFont.setPointSize(9);
    customPlot->legend->setFont(lengendFont);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignRight);
    customPlot->setGeometry(rect);
    customPlot->xAxis->setLabel(xLabel);
    customPlot->xAxis->setRange(xRangeL,xRangeR);
    customPlot->yAxis->setLabel(yLabel);
    customPlot->yAxis->setRange(yRangeL,yRangeR);
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(QColor(Qt::black)));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsStepLeft);// li san de dian
    customPlot->graph(0)->setName(name);

    return customPlot;
}
*/

void MainWindow::on_pushButton_2_clicked()
{

    qDebug()<<"循环了10次测试！";
    for(int i=0;i<2;i++)
    {
        on_pushButton_Send_clicked();
        Sleep(5000);

    }

}