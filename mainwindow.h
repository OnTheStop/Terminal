#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <serialworker.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_sendTextButton_clicked();
    void on_dataReceived(const QByteArray &data);
    void on_sendData(const QByteArray &data);

private:
    Ui::MainWindow *ui;
    SerialWorker *serialWorker;



};
#endif // MAINWINDOW_H
