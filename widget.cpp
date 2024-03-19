#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
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
