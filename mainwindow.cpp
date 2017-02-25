#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    modbusMaster = new Modbus();
    timer = new QTimer();
    statusLabel = new QLabel();





    QList<QSerialPortInfo> comPorts = QSerialPortInfo::availablePorts();
    QList<qint32> baudListInfo = QSerialPortInfo::standardBaudRates();


    foreach (QSerialPortInfo port, comPorts){

      ui->portList->addItem(port.portName());

    }

    foreach (qint32 baudList, baudListInfo) {

        ui->baudList->addItem(QString::number(baudList));

    }

    statusLabel->setText("Waiting..");
    ui->statusBar->addWidget(statusLabel);

    connect(modbusMaster,SIGNAL(onReadReady(QModbusDataUnit)),this,SLOT(onReadReadySlot(QModbusDataUnit)));
    connect(modbusMaster,SIGNAL(onReadError(QString)),this,SLOT(onReadError(QString)));









}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked(){

    modbusMaster->connectToSlave(ui->portList->currentText(),ui->baudList->currentText().toInt());
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeoutSlot()));
    timer->start(1000);


}

void MainWindow::onReadReadySlot(QModbusDataUnit reg){

    timer->stop();

    int t1 = reg.value(0);
    int t2 = reg.value(1);

    ui->t1Label->setText(QString::number(t1));
    ui->t2Label->setText(QString::number(t2));
    ui->t1Gauge->setValue(t1);
    ui->t2Gauge->setValue(t2);

    timer->start();




}

void MainWindow::on_pushButton_clicked(){



}

void MainWindow::onTimeoutSlot(){

    modbusMaster->executeReadRequest(10,100,Modbus::InputRegisters);
}

void MainWindow::onReadError(QString msg){

    statusLabel->setText(msg);


}
