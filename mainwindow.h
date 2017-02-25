#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QString>
#include <QTimer>
#include <QLabel>

#include "modbus.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Modbus * modbusMaster;
    QTimer * timer;

private:

    QLabel * statusLabel;

private slots:
    void on_connectButton_clicked();
    void onReadReadySlot(QModbusDataUnit);
    void onTimeoutSlot();
    void onReadError(QString);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
