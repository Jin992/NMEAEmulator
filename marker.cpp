#include "marker.h"
#include <ctime>

marker::marker(): _rmcStatus(false), _fix(1)
{

}

namespace  {

    std::string latToDegMin(double val) {
        int deg_int = 0;
        double res_int = 0;
        char direction = 'N';

        if (val < 0) {
            direction = 'S';
            val *= -1;
        }
        deg_int = static_cast<int>(val);
        double min = (val - deg_int) * 60;
        res_int = deg_int * 100 + min;
        std::string res;
        std::string value = std::to_string(res_int);
        value = value.substr(0, value.size() - 1);
        res.append(value);
        res.append(",");
        res.append(1, direction);
        return res;
    }

    std::string lonToDegMin(double val) {
        int deg_int = 0;
        double res_int = 0;
        char direction = 'E';

        if (val < 0) {
            direction = 'W';
            val *= -1;
        }
        deg_int = static_cast<int>(val);
        double min = (val - deg_int) * 60;
        res_int = deg_int * 100 + min;
        std::string res;
        if (deg_int < 100)
            res.append("0");
        std::string value = std::to_string(res_int);
        value = value.substr(0, value.size() - 1);
        res.append(value);
        res.append(",");
        res.append(1, direction);
        return res;
    }

    std::string generateCoords(QGeoCoordinate &point, const QString &suffix) {
        QString res = latToDegMin(point.latitude()).c_str();
        res.append(",");
        res.append(lonToDegMin(point.longitude()).c_str());
        //QString tmp = point.toString(QGeoCoordinate::DegreesMinutesWithHemisphere);
        //tmp.replace("° ", "");

        //tmp.replace("' ", suffix + ",");
        //tmp.replace("N, ", "N,0");
        //int pos = tmp.lastIndexOf(QChar(','));
        //tmp = tmp.left(pos);
        return res.toStdString();
    }

    std::string to_string(const char* format, tm* time) {
        std::vector<char> buf(100, '\0');
        buf.resize(std::strftime(buf.data(), buf.size(), format, time));
        return std::string(buf.begin(), buf.end());
    }

    std::string hex_checksum(int val) {
        std::stringstream stream;
        stream << std::hex << val;
        return std::string( stream.str());
    }

    int64_t _curt_utc() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    }

    int calc_NMEA_Checksum( const char *buf, size_t cnt )
    {
        char Character;
        int Checksum = 0;
        int i;              // loop counter

        //foreach(char Character in sentence)
        for (i=0;i<cnt;++i)
        {
            Character = buf[i];
            switch(Character)
            {
                case '$':
                    // Ignore the dollar sign
                    break;
                case '*':
                    // Stop processing before the asterisk
                    i = cnt;
                    continue;
                default:
                    // Is this the first value for the checksum?
                    if (Checksum == 0)
                    {
                        // Yes. Set the checksum to the value
                        Checksum = Character;
                    }
                    else
                    {
                        // No. XOR the checksum with this character's value
                        Checksum = Checksum ^ Character;
                    }
                    break;
            }
        }

        // Return the checksum
        return (Checksum);

    } // calc_NEMA_Checksum()
}


Q_INVOKABLE void marker::storeMarker(const QGeoCoordinate &coordinate){
    if (_point == coordinate)
        return;
    _point = coordinate;
}

QString marker::generateGGAmsg() {
    //$GNGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
    time_t sec = time(NULL);
    tm* curTime = localtime(&sec);
    std::string gga_msg = "$GNGGA,";
    gga_msg.append(to_string("%H%M%S.%S", curTime));
    gga_msg.append(",");
    gga_msg.append(generateCoords(_point, _gpsSufix));
    gga_msg.append(",");
    gga_msg.append(std::to_string(_fix));
    gga_msg.append(",08,0.9,178.4,M,46.9,M,,");
    int chksum = calc_NMEA_Checksum(gga_msg.c_str(), gga_msg.length());
    gga_msg.append("*");
    gga_msg.append(hex_checksum(chksum));
    gga_msg.append("\r\n");
    //qDebug() << latToDegMin(_point.latitude()).c_str() << " " << lonToDegMin(_point.longitude()).c_str();
    return gga_msg.c_str();
}

QString marker::generateRMCmsg() {
    //
    time_t sec = time(NULL);
    tm* curTime = localtime(&sec);
    std::string rmc_msg = "$GNRMC,";
    rmc_msg.append(to_string("%H%M%S.%S", curTime));
    rmc_msg.append(",");
    rmc_msg.append(1,_rmcStatus?'A':'V');
    rmc_msg.append(",");
    rmc_msg.append(generateCoords(_point, _gpsSufix));
    rmc_msg.append(",022.4,084.4,");
    rmc_msg.append(to_string("%d%m%y", curTime));
    rmc_msg.append(",");
    rmc_msg.append("003.1,W");
    int chksum = calc_NMEA_Checksum(rmc_msg.c_str(), rmc_msg.length());
    rmc_msg.append("*");
    rmc_msg.append(hex_checksum(chksum));
    rmc_msg.append("\r\n");

    return rmc_msg.c_str();
}

double marker::getLat() const {
    return _point.latitude();
}

double marker::getLon() const {
    return _point.longitude();
}

bool marker::rmcStatus() const {
    return _rmcStatus;
}

unsigned marker::gpsFix() const {
    return _fix;
}

void marker::setRmcStatus(bool c) {
    if (c == _rmcStatus)
        return;
    _rmcStatus = c;
    emit rmcStatusChanged();
}

void marker::setGpsFix(unsigned val) {
    if (val == _fix)
        return;
    _fix = val;
    emit gpsFixChanged();
}

QString marker::gpsSuffix() const {
    return _gpsSufix;
}

void marker::setGpsSuffix(QString suffix) {
    if (_gpsSufix == suffix)
        return;
    _gpsSufix = suffix;
    emit gpsSuffixChanged();
}
