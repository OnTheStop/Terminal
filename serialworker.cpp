#include "serialworker.h"

SerialWorker::SerialWorker(QObject *parent) : QObject(parent){
    serialPort = new QSerialPort();
}
SerialWorker::~SerialWorker() {
    delete serialPort;
}



void SerialWorker::setNamePort(const QString &portName){
    serialPort->setPortName(portName);
}
void SerialWorker::setBaudRate(qint32 baudRate) {
    serialPort->setBaudRate(baudRate);
}
void SerialWorker::setDataBits(QSerialPort::DataBits dataBitsIndex){
    serialPort->setDataBits(dataBitsIndex);
}
void SerialWorker::setStopBits(QSerialPort::StopBits stopBitsIndex){
    serialPort->setStopBits(stopBitsIndex);
}
void SerialWorker::setParity(QSerialPort::Parity parityIndex){
    serialPort->setParity(parityIndex);
}
void SerialWorker::setFlowControl(QSerialPort::FlowControl flowIndex){
    serialPort->setFlowControl(flowIndex);
}
void SerialWorker::connection(){
    if(serialPort->isOpen()){
        QMessageBox::warning(nullptr, "Error", "Port is already open.");
        return;
    }

    if (serialPort->open(QIODevice::ReadWrite)) {
        QObject::connect(serialPort, &QSerialPort::readyRead, this, &SerialWorker::readData);
    } else{
        QMessageBox::warning(nullptr, "Error", "Failed to open port.");
    }

}
void SerialWorker::disconnect(){
    if(serialPort->isOpen()){
        serialPort->close();
        QMessageBox::information(nullptr, "Disconnected", "Port disconnected successfully.");
    }  else{
        QMessageBox::warning(nullptr, "Error", "Port is not open.");
    }
}

void SerialWorker::readData(){
    QByteArray data = serialPort->readAll();
    emit dataReceived(data);
}
void SerialWorker::writeData(const QByteArray &data){
    if(serialPort->isOpen()){
        serialPort->write((data));
        emit dataSent(data);
    }
}
