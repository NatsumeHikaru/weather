#pragma once

#ifndef WEATHERDATE_H
#define WEATHERDATE_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QCoreApplication>
#include <QDateTime>

class Today{ // 今日数据
public:
    Today();
    Today& operator=(const QJsonObject& obj);
    QString date; // 日期
    QString temperature; // 温度
    QString city; // 城市
    QString wet; // 湿度
    QString pm25; // PM2.5
    QString quality; // 空气质量
    QString cold_index; // 感冒指数
    QString wind_direction; // 风向
    QString wind_power; // 风力
    QString type; // 天气
    QString sunrise; // 日出时间
    QString sunset; // 日落时间
    QString notice; // 注意事项
};

class Forecast{ // 预报数据
public:
    Forecast();
    Forecast& operator=(const QJsonObject& obj);
    QString date; // 日期
    QString week; // 星期
    QString high; // 最高温
    QString low; // 最低温
    QString quality; // 空气质量
    QString type; // 天气

};

#endif // WEATHERDATE_H
