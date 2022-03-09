#include "serial_port_writer.h"

SerialPortWriter::SerialPortWriter(QObject *parent)
    : QObject{parent}
{
    serial_port = new QSerialPort(this);
}

SerialPortWriter::SerialPortWriter(QString serial_port_name, int baud_rate, QObject *parent)
    : QObject{parent}
{
    this->serial_port_name = serial_port_name;
    this->baud_rate = baud_rate;
    serial_port = new QSerialPort(this);
}

void SerialPortWriter::setSerialPortName(QString serial_port_name) {
    this->serial_port_name = serial_port_name;
}

QString SerialPortWriter::getSerialPortName() {
    return this->serial_port_name;
}

void SerialPortWriter::setBaudRate(int baud_rate) {
    this->baud_rate = baud_rate;
}

int SerialPortWriter::getBaudRate() {
    return this->baud_rate;
}

void SerialPortWriter::openPort() {
    serial_port->setPortName(serial_port_name);
    serial_port->setBaudRate(baud_rate);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    if(!serial_port->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port error";
    }
}

void SerialPortWriter::closePort() {
    if(serial_port->isOpen())
        serial_port->close();
}

void SerialPortWriter::writeMessage(QString message) {
    serial_port->write(message.toUtf8());
}

//QString SerialPortMiddleware::readMessage() {
//    buffer = serial_port->readAll();
//}
