#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <unordered_map>
#include <QFile>
#include <QCoreApplication>
#include <QDateTime>

class WeatherTool{
public:
    WeatherTool(){
        QString filename = QCoreApplication::applicationDirPath();
        filename += "/citycode.json";
        QFile file(filename);
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QByteArray json = file.readAll();
        file.close();
        QJsonParseError err;
        QJsonDocument json_doc = QJsonDocument::fromJson(json,&err);
        QJsonArray citys = json_doc.array();
        // 遍历城市
        for(int i=0;i<citys.size();++i){
            QString code = citys.at(i).toObject().value("city_code").toString(); // 取出city_code
            QString name = citys.at(i).toObject().value("city_name").toString();
            if(code.size() > 0)
                city2code.insert(std::pair<QString,QString>(name,code));
        }
    }

    QString operator[](const QString& city){
        std::unordered_map<QString,QString>::iterator it = city2code.find(city);
        if(it == city2code.end()){
            it = city2code.find(city+u8"市");
        }
        if(it != city2code.end()){
            return it->second;
        }
        return "000000000";
    }

private:
    std::unordered_map<QString,QString> city2code;
};

#endif // WEATHERTOOL_H
