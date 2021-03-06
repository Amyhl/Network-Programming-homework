#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_Port->setText("8765");
    ui->pushButton_Send->setEnabled(false);

    server = new QTcpServer();

    //连接信号槽
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);

    QObject::connect(ui->textEdit_Send,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,
                     SLOT(curFmtChanged(const QTextCharFormat)));
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_Listen_clicked()
{
    if(ui->pushButton_Listen->text() == tr("创建连接"))
    {
        //从输入框获取端口号
        int port = ui->lineEdit_Port->text().toInt();

        //监听指定的端口
        if(!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug()<<server->errorString();
            return;
        }
        //修改按键文字
        ui->pushButton_Listen->setText("断开连接");
        qDebug()<< "Listen succeessfully!";
    }
    else
    {
        //如果正在连接（点击侦听后立即取消侦听，若socket没有指定对象会有异常，应修正——2017.9.30）
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            //关闭连接
            socket->disconnectFromHost();
        }
        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("创建连接");
        //发送按键失能
        ui->pushButton_Send->setEnabled(false);
    }

}

void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
    //获取文本框内容并以ASCII码形式发送
    socket->write(ui->textEdit_Send->toPlainText().toUtf8());
    socket->flush();
}

void MainWindow::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);
    //发送按键使能
    ui->pushButton_Send->setEnabled(true);

    qDebug() << "A Client connect!";
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->textEdit_Recv->toPlainText();
        str+=tr(buffer)+"\n";
        //刷新显示
        ui->textEdit_Recv->setText(str);
    }
}

void MainWindow::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_Send->setEnabled(false);
    qDebug() << "Disconnected!";
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->textEdit_Send->setCurrentFont(f);
    ui->textEdit_Send->setFocus();
    ui->textEdit_Recv->setCurrentFont(f);
    ui->textEdit_Recv->setFocus();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->textEdit_Send->setFontPointSize(arg1.toDouble());
    ui->textEdit_Send->setFocus();
    ui->textEdit_Recv->setFontPointSize(arg1.toDouble());
    ui->textEdit_Recv->setFocus();
}

void MainWindow::on_toolButton_2_clicked(bool checked)
{
    if(checked)
    {
        ui->textEdit_Send->setFontWeight(QFont::Bold);
        ui->textEdit_Recv->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->textEdit_Send->setFontWeight(QFont::Normal);
        ui->textEdit_Recv->setFontWeight(QFont::Normal);
    }
    ui->textEdit_Send->setFocus();
    ui->textEdit_Recv->setFocus();
}


void MainWindow::on_toolButton_clicked(bool checked)
{
    ui->textEdit_Send->setFontItalic(checked);
    ui->textEdit_Send->setFocus();
    ui->textEdit_Recv->setFontItalic(checked);
    ui->textEdit_Recv->setFocus();
}

void MainWindow::on_toolButton_3_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->textEdit_Send->setTextColor(color);
        ui->textEdit_Send->setFocus();
        ui->textEdit_Recv->setTextColor(color);
        ui->textEdit_Recv->setFocus();
    }
}

void MainWindow::curFmtChanged(const QTextCharFormat &fmt)
{
    ui->fontComboBox->setCurrentFont(fmt.font());
    if(fmt.fontPointSize()<8){
        ui->comboBox->setCurrentIndex(1);
    }
    else
    {
        ui->toolButton_2->setChecked(fmt.font().bold());
        ui->toolButton->setChecked(fmt.font().italic());
        color=fmt.foreground().color();
    }
}
