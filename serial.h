#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QGeoCoordinate>
#include <chrono>



class Serial:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  serialConnMsg READ serialConnMsg WRITE setSerialConnMsg NOTIFY serialConnMsgChanged)
    Q_PROPERTY(bool     timerTrigger  READ timerTrigger  WRITE setTimerTrigger  NOTIFY timerTriggerChanged)

public:
    Serial();
    void setSerialConnMsg(const QString &msg);
    void setTimerTrigger(bool);
    QString serialConnMsg() const;
    bool timerTrigger() const;
signals:
    void serialConnMsgChanged();
    void timerTriggerChanged();

public slots:
    QStringList scanSerial();
    void openSerial(const QString &serialName, int baudrate, int dataBits, const QString & parity, double stopBit);
    void writeToSerial(const QString &msg);
    void close();


private:
    QList<QSerialPortInfo> _availablePorts;
    QSerialPort            _serial;
    QStringList            _serialNamesList;
    QString                _serialConnMsg;
    bool                   _timerTrigger;
};

#endif // SERIAL_H
