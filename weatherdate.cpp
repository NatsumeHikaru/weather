#include "weatherdate.h"

Today::Today(){
    date = "0000-00-00";
    city = "null";
    wind_direction = u8"无数据";
    wind_power = u8"无数据";
    cold_index = u8"无数据";
    notice = u8"无数据";
    wet = u8"无数据";
    pm25 = u8"无数据";
    quality = u8"无数据";
    sunrise = "00:00";
    sunset = "00:00";
    temperature = "null";
    type = u8"无数据";
}

Today &Today::operator=(const QJsonObject &obj){
    QString date_str = obj.value("date").toString();
    date = QDate::fromString(date_str,"yyyyMMdd").toString("yyyy-MM-dd");
    city = obj.value("cityInfo").toObject().value("city").toString();

    // 解析数据
    QJsonObject data_obj = obj.value("data").toObject();
    wet = data_obj.value("shidu").toString();
    pm25 = QString::number(data_obj.value("pm25").toDouble());
    quality = data_obj.value("quality").toString();
    temperature = data_obj.value("wendu").toString()+u8"°";
    cold_index = data_obj.value("ganmao").toString();
    QJsonArray forecast_array = data_obj.value("forecast").toArray();
    QJsonObject today_obj = forecast_array.at(0).toObject();
    wind_direction = today_obj.value("fx").toString();
    wind_power = today_obj.value("fl").toString();
    type = today_obj.value("type").toString();
    sunrise = today_obj.value("sunrise").toString();
    sunset = today_obj.value("sunset").toString();
    notice = today_obj.value("notice").toString();

    return *this;
}

Forecast::Forecast(){
    quality = "0";
    date = u8"00日星期0";
    week = u8"星期0";
    high = u8"高温 0.0℃";
    low = u8"低温 0.0℃";
    type = "undefined";
}

Forecast &Forecast::operator=(const QJsonObject &obj){
    date = obj.value("date").toString();
    week = obj.value("week").toString();
    high = obj.value("high").toString();
    low = obj.value("low").toString();
    quality = QString::number(obj.value("aqi").toDouble());
    type = obj.value("type").toString();
    return *this;
}
