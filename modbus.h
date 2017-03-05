#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QDebug>

class QModbusReply;
class QModbusClient;


class Modbus : public QObject
{

    Q_OBJECT

public:
    Modbus();

    bool connectToSlave(QString port, int baud);
    void executeReadRequest(int slaveAdress, int regAdress, int regType); //passing adress of slave, adress of register and register type
    void executeWriteRequest(int slaveAdress, int regAdress, int regType, int value); //passing adress of slave, adress of register and register type
    void disconnectSlave();
    enum RegType { DiscreteInputs = 1, Coils = 2, InputRegisters = 3, HoldingRegisters = 4 };



private:
    QModbusReply *modbusReply;
    QModbusClient *modbusSlave;

    QModbusDataUnit prepareRequest(int regAdress, int regType);
    QModbusDataUnit prepareWriteRequest(int regAdress, int regType);


private slots:
    void readReady();
    void writeRequestFinished();

signals:

    void onFinishedWriteRequest(const QString & msg);
    void onReadError(const QString & msg);
    void onReadReady(const QModbusDataUnit & reg);











};

#endif // MODBUS_H
