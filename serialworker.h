#ifndef SERIALWORKER_H
#define SERIALWORKER_H
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

class SerialWorker : public QObject{
    Q_OBJECT

public:
    SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

    QSerialPort *serialPort;

    void setNamePort(const QString &portName);
    void setBaudRate(qint32 baudRate);
    void setDataBits(QSerialPort::DataBits dataBitsIndex);
    void setStopBits(QSerialPort::StopBits stopBitsIndex);
    void setParity(QSerialPort::Parity parityIndex);
    void setFlowControl(QSerialPort::FlowControl flowIndex);


signals:
    void dataReceived(const QByteArray &data);
    void dataSent(const QByteArray &data);

public slots:
    void connection();
    void disconnect();
    void writeData(const QByteArray &data);

private slots:
    void readData();

private:

};


#endif // SERIALWORKER_H
