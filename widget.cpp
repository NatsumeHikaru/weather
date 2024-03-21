#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 设置字符编码
    // QTextCodec* codec = QTextCodec::codecForName("GBK");
    // QTextCodec::setCodecForLocale(codec);

    // int font_id = QFontDatabase::addApplicationFont(":/fonts/宋.ttf");
    // QStringList font_list = QFontDatabase::applicationFontFamilies(font_id);
    // qDebug()<<font_list;

    // 设置窗口属性 无边框窗口
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(width(),height());

    // 设置退出菜单和活动
    exit_menu = new QMenu(this);
    exit_action = new QAction;
    exit_action->setText(u8"退出");
    exit_action->setIcon(QIcon(":/icons/close.ico"));
    exit_menu->addAction(exit_action);
    connect(exit_action,SIGNAL(triggered(bool)),this,SLOT(slot_exit_app()));

    // 钩子监听键盘
    hook.install_hook();
    qRegisterMetaType<Hook::Type>("Type"); // 为了信号能传递自定义类型
    connect(&hook,SIGNAL(send_key_type(Type)),this,SLOT(slot_check_hook_type(Type)));

    // UI初始化
    forecast_week_list << ui->day0_label << ui->day1_label << ui->day2_label \
                       << ui->day3_label << ui->day4_label << ui->day5_label;
    forecast_date_list << ui->date0_label << ui->date1_label << ui->date2_label \
                       << ui->date3_label << ui->date4_label << ui->date5_label;
    forecast_quality_list << ui->quality0_label << ui->quality1_label << ui->quality2_label \
                          << ui->quality3_label << ui->quality4_label << ui->quality5_label;
    forecast_type_list << ui->type0_label << ui->type1_label << ui->type2_label \
                       << ui->type3_label << ui->type4_label << ui->type5_label;
    forecast_type_icon_list << ui->icon0 << ui->icon1 << ui->icon2 \
                            << ui->icon3 << ui->icon4 << ui->icon5;
    forecast_high_list << ui->high0 << ui->high1 << ui->high2 \
                       << ui->high3 << ui->high4 << ui->high5;
    forecast_low_list << ui->low0 << ui->low1 << ui->low2 \
                      << ui->low3 << ui->low4 << ui->low5;

    url = "http://t.weather.itboy.net/api/weather/city/";
    city = u8"北京";
    city_tmp = city;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_reply_finished(QNetworkReply*)));
    get_weather_info(manager);

}

Widget::~Widget()
{
    delete ui;
    hook.uninstall_hook();
}

void  Widget::contextMenuEvent(QContextMenuEvent* event){
    exit_menu->exec(QCursor::pos()); // 获取当前位置
    event->accept();
}

void Widget::slot_exit_app(){
    qApp->exit(0);
}

void Widget::slot_check_hook_type(Hook::Type type){
    switch(type){
    case Hook::EXIT:
        qApp->exit(0);
        break;
    default:
        break;
    }
}

void Widget::mouseMoveEvent(QMouseEvent* event){
    move(event->globalPos() - position);
}

void Widget::mousePressEvent(QMouseEvent* event){
    position = event->globalPos() - this->pos(); // 计算坐标差值
}

void Widget::slot_reply_finished(QNetworkReply* reply){
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(reply->error() != QNetworkReply::NoError || status_code != 200){
        QMessageBox::warning(this, "ERROR!", u8"天气：请求数据错误",QMessageBox::Ok);
        return;
    }

    QByteArray bytes = reply->readAll();

    parse_json(bytes);
}

void Widget::get_weather_info(QNetworkAccessManager* manager){
    QString citycode = tool[city];
    if(citycode == "000000000"){
        QMessageBox::warning(this, "ERROR!", u8"天气:指定的城市不存在",QMessageBox::Ok);
        return;
    }

    QUrl json_url = url + citycode;
    manager->get(QNetworkRequest(json_url));
}

// 解析Json数据
void Widget::parse_json(QByteArray& bytes){
    QJsonParseError err;
    QJsonDocument json_doc = QJsonDocument::fromJson(bytes,&err); // 检测json格式
    if(err.error != QJsonParseError::NoError){
        return;
    }

    QJsonObject json_obj = json_doc.object();
    QString message = json_obj.value("message").toString();
    if(message.contains("success") == false){
        QMessageBox::warning(this,tr("The information of Json_desc"), u8"天气:城市错误", QMessageBox::Ok);
        city = city_tmp;
        return;
    }
    today = json_obj;

    QString date_str = json_obj.value("data").toString();
    today.date = QDate::fromString(date_str,"yyyyMMdd").toString("yyyy-MM-dd");
    today.city = json_obj.value("cityInfo").toObject().value("city").toString();

    // 解析data
    QJsonObject data_obj = json_obj.value("data").toObject();
    today.wet = data_obj.value("shidu").toString();
    today.pm25 = QString::number(data_obj.value("pm25").toDouble());
    today.quality = data_obj.value("quality").toString();
    today.temperature = data_obj.value("wendu").toString() + u8"°";

    // 解析yesterday
    QJsonObject y_obj = data_obj.value("yesterday").toObject();
    forecast[0].date = y_obj.value("date").toString();
    forecast[0].high = y_obj.value("high").toString();
    forecast[0].low = y_obj.value("low").toString();
    forecast[0].quality = QString::number(y_obj.value("aqi").toDouble());
    forecast[0].type = y_obj.value("type").toString();

    // 解析forecast
    QJsonArray forecast_arr = data_obj.value("forecast").toArray();
    for(int i=1,j=0;i<6;++i,++j){
        QJsonObject t = forecast_arr.at(j).toObject();
        forecast[i].week = t.value("week").toString();
        forecast[i].date = t.value("date").toString();
        forecast[i].high = t.value("high").toString();
        forecast[i].low = t.value("low").toString();
        forecast[i].quality = QString::number(t.value("aqi").toDouble());
        forecast[i].type = t.value("type").toString();
    }

    // 取得今日数据
    QJsonObject today_obj = forecast_arr.at(0).toObject();
    today.wind_direction = today_obj.value("fx").toString();
    today.wind_power = today_obj.value("fl").toString();
    today.type = today_obj.value("type").toString();
    today.sunrise = today_obj.value("sunrise").toString();
    today.sunset = today_obj.value("sunset").toString();

    set_label_content();
}

// 更新UI数据
void Widget::set_label_content(){
    // 今日数据
    ui->dateLabel->setText(today.date);
    ui->temperature->setText(today.temperature);
    ui->city_label->setText(today.city);
    ui->typeLabel->setText(today.type);
    ui->wet_label->setText(today.wet);
    ui->PM_label->setText(today.pm25);
    ui->direction_label->setText(today.wind_direction);
    ui->power_label->setText(today.wind_power);
    ui->browser->setText(today.cold_index);

    for(int i=0;i<6;++i){
        forecast_week_list[i]->setText(forecast[i].week);
        forecast_date_list[i]->setText(forecast[i].date.left(3));
        forecast_type_list[i]->setText(forecast[i].type);
        forecast_high_list[i]->setText(forecast[i].high.split(" ").at(1));
        forecast_low_list[i]->setText(forecast[i].low.split(" ").at(1));
        forecast_type_icon_list[i]->setStyleSheet(tr("image:url(:weatherIcons/day/%1.png);").arg(forecast[i].type));

        if(forecast[i].quality.toInt() >= 0 && forecast[i].quality.toInt() <= 50){
            forecast_quality_list[i]->setText(u8"优");
            ui->quality->setText(u8"优");
        }
        else if(forecast[i].quality.toInt() > 50 && forecast[i].quality.toInt() <= 100){
            forecast_quality_list[i]->setText(u8"良好");
            ui->quality->setText(u8"良好");
        }
        else if(forecast[i].quality.toInt() >=100 && forecast[i].quality.toInt() <= 150){
            forecast_quality_list[i]->setText(u8"轻度污染");
            ui->quality->setText(u8"污染");
        }
        else if(forecast[i].quality.toInt() > 150 && forecast[i].quality.toInt() <= 200){
            forecast_quality_list[i]->setText(u8"重度污染");\
            ui->quality->setText(u8"污染");
        }
        else {
            forecast_quality_list[i]->setText(u8"严重污染");
            ui->quality->setText(u8"污染");
        }
    }

    ui->day0_label->setText(u8"昨天");
    ui->day1_label->setText(u8"今天");
    ui->day2_label->setText(u8"明天");

    ui->curve_label->update();
}
