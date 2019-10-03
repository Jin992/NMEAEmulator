#include "serial.h"

Serial::Serial(): _timerTrigger(false)
{}

void Serial::setSerialConnMsg(const QString &msg) {
    if (_serialConnMsg == msg)
        return;
    _serialConnMsg = msg;
    emit serialConnMsgChanged();
}

QStringList Serial::scanSerial() {
    _availablePorts = QSerialPortInfo::availablePorts();
       for (const QSerialPortInfo &info : _availablePorts) {
           QString s = info.portName();
           if (!info.isBusy()){
                _serialNamesList.append(s);
           }
           //qDebug() << s;
       }
   return _serialNamesList;
}

void Serial::close() {
    _serial.close();
     setSerialConnMsg("Port has been closed.");
}

QString Serial::serialConnMsg() const {
    return _serialConnMsg;
}

void Serial::openSerial(const QString &serialName, int baudrate, int dataBits, const QString & parity, double stopBit) {
    //qDebug() << "Port " << serialName << " baud " << baudrate << " data " << dataBits << " parity " << parity << " stop " << stopBit;
    _serial.setPortName(serialName);
    _serial.setBaudRate(baudrate);
    //dataBits
    switch (dataBits) {
    case 5 :
        _serial.setDataBits(QSerialPort::Data5);
        break;
    case 6 :
        _serial.setDataBits(QSerialPort::Data6);
        break;
    case 7 :
        _serial.setDataBits(QSerialPort::Data7);
        break;
    case 8 :
        _serial.setDataBits(QSerialPort::Data8);
        break;
    default:
        _serial.setDataBits(QSerialPort::Data8);
        break;
    }
    // parity
    if (parity == "NoParity")
        _serial.setParity(QSerialPort::NoParity);
    else if (parity == "EvenParity")
        _serial.setParity(QSerialPort::EvenParity);
    else if (parity == "OddParity")
        _serial.setParity(QSerialPort::OddParity);
    else if (parity == "SpaceParity")
        _serial.setParity(QSerialPort::SpaceParity);
    else if (parity == "MarkParity")
        _serial.setParity(QSerialPort::MarkParity);
    else
        _serial.setParity(QSerialPort::NoParity);
    // stopbit
    if (stopBit == 1.0)
        _serial.setStopBits(QSerialPort::OneStop);
    else if (stopBit == 1.5)
        _serial.setStopBits(QSerialPort::OneAndHalfStop);
    else if (stopBit == 2.0)
        _serial.setStopBits(QSerialPort::TwoStop);
    else
        _serial.setStopBits(QSerialPort::OneStop);

    if (!_serial.open(QIODevice::WriteOnly)) {
        setSerialConnMsg("Failed to open port " + serialName + _serial.error());
        return;
    }
    std::string end = std::to_string(stopBit);

    setSerialConnMsg("Connected to "  + serialName +
                      " baud " + std::to_string(baudrate).c_str() +
                      " data " + std::to_string(dataBits).c_str() +
                      " parity " + parity + " stop " + std::string(end.begin(), end.begin() +3).c_str());
}

void Serial::writeToSerial(const QString &msg) {
    if (_serial.write(msg.toStdString().c_str(), msg.toStdString().length()) == -1) {
        setTimerTrigger(false);
    }
}

void Serial::setTimerTrigger(bool status) {
    if (_timerTrigger == status)
        return;
    _timerTrigger = status;
    emit timerTriggerChanged();
}

bool Serial::timerTrigger() const {
    return _timerTrigger;
}
