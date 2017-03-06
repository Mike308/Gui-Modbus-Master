#include "modbus.h"

Modbus::Modbus(){

     modbusSlave = new QModbusRtuSerialMaster(this);


}

Modbus::~Modbus(){

    qDebug () << "Modbus die";
}


bool Modbus::connectToSlave(const QString port, int baud){



    qDebug () << "Connection state: " << modbusSlave->state();
    if (!modbusSlave) return false;


    if (modbusSlave->state() != QModbusDevice::ConnectedState){

        qDebug () << "Setting Paramaters...";

        modbusSlave->setConnectionParameter(QModbusDevice::SerialPortNameParameter,port);
        modbusSlave->setConnectionParameter(QModbusDevice::SerialParityParameter,QSerialPort::NoParity);
        modbusSlave->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,baud);
        modbusSlave->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,QSerialPort::Data8);
        modbusSlave->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,QSerialPort::OneStop);
        modbusSlave->setTimeout(1000);
        modbusSlave->setNumberOfRetries(3);

    }


    if (!modbusSlave->connectDevice()){

        qDebug () << "Error: "<< modbusSlave->errorString();

        return false;

    }else{

        qDebug () << "Connection state: " << modbusSlave->state();
        return true;


    }


}


void Modbus::disconnectSlave(){

    qDebug () << "Connection state: " << modbusSlave->state();

    modbusSlave->disconnectDevice();
   // modbusSlave->blockSignals(true);
    this->~Modbus();

    qDebug () << "Connection state: " << modbusSlave->state();

}





QModbusDataUnit Modbus::prepareRequest(int regAdress, int regType){

    return QModbusDataUnit( (QModbusDataUnit::RegisterType)regType, regAdress,10);

}

void Modbus::executeReadRequest(int slaveAdress, int regAdress, int regType){

    if (!modbusSlave) return;

    if (auto *reply = modbusSlave->sendReadRequest(prepareRequest(regAdress,regType),slaveAdress)){

        if (!reply->isFinished())


            connect(reply,&QModbusReply::finished,this,&Modbus::readReady);


            else

            delete reply;



    }else{

        qDebug () << "Error while executing read request: " << modbusSlave->errorString();

    }

}

QModbusDataUnit Modbus::prepareWriteRequest(int regAdress, int regType){

    return QModbusDataUnit((QModbusDataUnit::RegisterType)regType,regAdress,1);

}

void Modbus::executeWriteRequest(int slaveAdress, int regAdress, int regType, int value){

    QModbusDataUnit writeRequest = prepareWriteRequest(regAdress,regType);

    for (uint i=0; i<writeRequest.valueCount(); i++){

        writeRequest.setValue(i,value);
    }

    if (auto * reply = modbusSlave->sendWriteRequest(writeRequest,slaveAdress)){

        if (!reply->isFinished()){

            //TODO: conncet signal finished to slot
            connect(reply,&QModbusReply::finished,this,&Modbus::writeRequestFinished);

        }else{

            reply->deleteLater();
        }
    }else{

        qDebug () << "Error while executing write request: " << reply->errorString();
    }


}


void Modbus::readReady(){

    auto reply = qobject_cast<QModbusReply*>(sender());

    if (!reply) return;

    if (reply->error()==QModbusDevice::NoError){

        const QModbusDataUnit modbusDataUnit = reply->result();
        emit onReadReady(modbusDataUnit);

//        for (uint i=0; i<modbusDataUnit.valueCount();i++){

//          //  qDebug () << "Value: " << modbusDataUnit.value(i);

//        }

    }

    else if (reply->error()==QModbusDevice::ProtocolError){

        qDebug () << "Protocol error: " << reply->errorString();
        emit onReadError(reply->errorString());

    }else {

        qDebug () << "Other Error: " << reply->errorString();
        emit onReadError(reply->errorString());

    }


}

void Modbus::writeRequestFinished(){

    auto reply  = qobject_cast<QModbusReply*>(sender());

    if (!reply) return;

    if (reply->error()==QModbusDevice::ProtocolError){

        qDebug () << "Protocol error: " << reply->errorString();
        emit onFinishedWriteRequest("Protocol error: "+reply->errorString());

    }else if (!reply->error()==QModbusDevice::NoError){

        qDebug () << "Other error: " << reply->errorString();
         emit onFinishedWriteRequest("Other error: "+reply->errorString());
    }
    else{

        qDebug () << "Send request status: ok ";
         emit onFinishedWriteRequest("OK!");
    }

}










