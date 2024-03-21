#pragma once

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <windows.h>
#include <QList>
#include <QLabel>
#include <QTextCodec>
#include <QFontDatabase>
#include <QStringList>
#include <QPalette>
#include <QStringLiteral>
#include <QFont>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QByteArray>
#include <QUrl>
#include <QJsonParseError>

#include "hook.h"
#include "weatherTool.h"
#include "weatherdate.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


protected:
    void contextMenuEvent(QContextMenuEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void get_weather_info(QNetworkAccessManager* manager);
    void parse_json(QByteArray& bytes);
    void set_label_content();

private:
    Ui::Widget *ui;
    QMenu* exit_menu; // 右键退出菜单
    QAction *exit_action;
    Hook hook; // 钩子对象
    QPoint position;

    // UI
    QList<QLabel *> forecast_week_list, forecast_date_list, forecast_quality_list, \
                    forecast_type_list, forecast_type_icon_list, forecast_high_list, forecast_low_list;

    QString url; // 接口链接
    QString city, city_tmp; // 城市
    WeatherTool tool;
    QNetworkAccessManager* manager;

    Today today;
    Forecast forecast[7];

private slots:
    void slot_exit_app();
    void slot_check_hook_type(Hook::Type);
    void slot_reply_finished(QNetworkReply* reply);
};
#endif // WIDGET_H
