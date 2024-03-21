#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 设置字符编码
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    int font_id = QFontDatabase::addApplicationFont(":/fonts/宋.ttf");
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_id);
    qDebug()<<font_list;

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
