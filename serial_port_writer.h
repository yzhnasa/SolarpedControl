#ifndef SERIALPORTINTERFACE_H
#define SERIALPORTINTERFACE_H

#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialPortWriter : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortWriter(QObject *parent = nullptr);
    explicit SerialPortWriter(QString serial_port_name, int baud_rate, QObject *parent = nullptr);
    void setSerialPortName(QString serial_port_name);
    QString getSerialPortName();
    void setBaudRate(int baud_rate);
    int getBaudRate();
    void openPort();
    void closePort();
    void writeMessage(QString message);
//    QString readMessage();

private:
    QSerialPort *serial_port;
    QString serial_port_name;
    int baud_rate;
//    QString buffer;

public Q_SLOTS:


Q_SIGNALS:

};

#endif // SERIALPORTINTERFACE_H
