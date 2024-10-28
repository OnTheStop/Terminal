#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QDesktopWidget>
#include <QMessageBox>
#include <unistd.h>
#include <QThread>
#include <QScrollBar>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        ui->portBox->addItem(info.portName());
    }

    QList<qint32> baudRates = QSerialPortInfo::standardBaudRates();

    for (int i = 0; i < baudRates.size(); ++i) {
        if(baudRates[i] == 9600){
            ui->baudRateBox->addItem(QString::number(baudRates[i]));
            ui->baudRateBox->setCurrentIndex(i);
            continue;
        }
        ui->baudRateBox->addItem(QString::number(baudRates[i]));
    }

    const QList<QPair<QString, QSerialPort::DataBits>> dataBitsOptions = {
        {QStringLiteral("5"), QSerialPort::Data5},
        {QStringLiteral("6"), QSerialPort::Data6},
        {QStringLiteral("7"), QSerialPort::Data7},
        {QStringLiteral("8"), QSerialPort::Data8}
    };

    for (const auto &option : dataBitsOptions) {
        ui->dataBitsBox->addItem(option.first, option.second);
    }

    ui->dataBitsBox->setCurrentIndex(3);

    const QList<QPair<QString, QSerialPort::Parity>> parityOptions = {
        {QStringLiteral("None"), QSerialPort::NoParity},
        {QStringLiteral("Even"), QSerialPort::EvenParity},
        {QStringLiteral("Odd"), QSerialPort::OddParity},
        {QStringLiteral("Mark"), QSerialPort::MarkParity},
        {QStringLiteral("Space"), QSerialPort::SpaceParity}
    };

    for (const auto &option : parityOptions) {
        ui->parityBox->addItem(option.first, option.second);
    }

    const QList<QPair<QString, QSerialPort::StopBits>> stopBitsOptions = {
        {QStringLiteral("1"), QSerialPort::OneStop},
#ifdef _WIN32
        {QStringLiteral("1.5"), QSerialPort::OneAndHalfStop},
#endif
        {QStringLiteral("2"), QSerialPort::TwoStop}
    };

    for (const auto &option : stopBitsOptions) {
        ui->stopBitsBox->addItem(option.first, option.second);
    }


    const QList<QPair<QString, QSerialPort::FlowControl>> flowControlOptions = {
        {QStringLiteral("None"), QSerialPort::NoFlowControl},
        {QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl},
        {QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl}
    };

    for (const auto &option : flowControlOptions) {
        ui->flowControlBox->addItem(option.first, option.second);
    }


    serialWorker = new SerialWorker();
    QThread *thread = new QThread;
    serialWorker->moveToThread(thread);

    QObject::connect(thread, &QThread::finished, serialWorker, &QObject::deleteLater);
    QObject::connect(serialWorker, &SerialWorker::dataReceived, this, &MainWindow::on_dataReceived);
    QObject::connect(serialWorker, &SerialWorker::dataSent, this, &MainWindow::on_sendData);



    thread->start();

}

MainWindow::~MainWindow()
{
    serialWorker->thread()->quit();
    serialWorker->thread()->wait();

    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    QString portName = ui->portBox->currentText();
    int baudRate = ui->baudRateBox->currentText().toInt();
    QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->currentText().toInt());
    QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->currentText().toInt());
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(ui->parityBox->currentIndex());
    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->currentIndex());

    qDebug() << "Selected parameters: "
             << "Port: " << portName
             << ", Baud rate: " << baudRate
             << ", Data bits: " << dataBits
             << ", Stop bits: " << stopBits
             << ", Parity: " << parity
             << ", Flow control: " << flowControl;


    serialWorker->setNamePort(portName);
    serialWorker->setBaudRate(baudRate);
    serialWorker->setDataBits(dataBits);
    serialWorker->setStopBits(stopBits);
    serialWorker->setParity(parity);
    serialWorker->setFlowControl(flowControl);

    serialWorker->connection();
}

void MainWindow::on_disconnectButton_clicked()
{
    if(serialWorker->serialPort->isOpen()){
        serialWorker->disconnect();
    } else {
        QMessageBox::warning(this, "Error", "Port is not open.");
    }
}

void MainWindow::on_sendTextButton_clicked(){
    if(serialWorker->serialPort->isOpen()){
        QString sendData = ui->inputTextArea->text();
        QByteArray data = sendData.toLocal8Bit();
        serialWorker->writeData(data);
    } else {
        QMessageBox::warning(this, "Error", "Port is not open.");
    }
}

void MainWindow::on_dataReceived(const QByteArray &data){
    QString asciiData = QString::fromLocal8Bit(data);
    ui->console->insertPlainText("Received: " + asciiData + '\n');
    ui->console->verticalScrollBar()->setValue(ui->console->verticalScrollBar()->maximum());
}
void MainWindow::on_sendData(const QByteArray &data) {
    QString asciiData = QString::fromLocal8Bit(data);
    ui->console->insertPlainText("Sent: " + asciiData + '\n');
    ui->console->verticalScrollBar()->setValue(ui->console->verticalScrollBar()->maximum());
}




