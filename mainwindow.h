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

    void on_led1_stateChanged(int arg1);

    void on_led2_stateChanged(int arg1);

    void on_rDial_actionTriggered(int action);

    void on_gDial_actionTriggered(int action);

    void on_MainWindow_destroyed();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
