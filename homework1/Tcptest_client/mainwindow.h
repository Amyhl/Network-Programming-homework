#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_Connect_clicked();

    void on_pushButton_Send_clicked();

    void socket_Read_Data();

    void socket_Disconnected();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    bool flag=1;
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QCustomPlot *timePlot;    //绘图功能还仍待添加
    QCustomPlot *CreateAndInitPlotGraph(QString xLabel,int xRangeL,int xRangeR,QString yLabel,int yRangeL,int yRangeR,QRect rect,QString name,QWidget *parent);
};

#endif // MAINWINDOW_H