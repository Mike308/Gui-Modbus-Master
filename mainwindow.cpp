#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QList<QSerialPortInfo> comPorts = QSerialPortInfo::availablePorts();
    QList<qint32> baudListInfo = QSerialPortInfo::standardBaudRates();


    foreach (QSerialPortInfo port, comPorts){

      ui->portList->addItem(port.portName());

    }

    foreach (qint32 baudList, baudListInfo) {

        ui->baudList->addItem(QString::number(baudList));

    }






}

MainWindow::~MainWindow()
{
    delete ui;
}
