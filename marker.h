#ifndef MARKER_H
#define MARKER_H

#include <QAbstractListModel>
#include <QGeoCoordinate>
#include <QDebug>
#include <chrono>
#include <string>
#include <sstream>

class marker:public QObject
{
   Q_OBJECT
   Q_PROPERTY(bool      rmcStatus    READ rmcStatus WRITE setRmcStatus  NOTIFY rmcStatusChanged)
   Q_PROPERTY(unsigned  gpsFix       READ gpsFix    WRITE setGpsFix     NOTIFY gpsFixChanged)
   Q_PROPERTY(QString   gpsSuffix     READ gpsSuffix WRITE setGpsSuffix  NOTIFY gpsSuffixChanged)

public:
    marker();

     Q_INVOKABLE void storeMarker(const QGeoCoordinate &coordinate);
    bool rmcStatus() const;
    unsigned gpsFix() const;
    QString gpsSuffix() const;

    void setRmcStatus(bool);
    void setGpsFix(unsigned);
    void setGpsSuffix(QString);

signals:
    void markerChanged();
    void rmcStatusChanged();
    void gpsFixChanged();
    void gpsSuffixChanged();

public slots:
    QString generateGGAmsg();
    QString generateRMCmsg();
    double getLat() const;
    double getLon() const;

private:
    QGeoCoordinate _point;
    bool           _rmcStatus;
    QString        _gpsSufix;
    unsigned       _fix;
};

#endif // MARKER_H
