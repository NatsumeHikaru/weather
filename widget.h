#ifndef WIDGET_H
#define WIDGET_H

#pragma once

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>

#include "hook.h"

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
    void  contextMenuEvent(QContextMenuEvent* event);

private:
    Ui::Widget *ui;
    QMenu* exit_menu; // 右键退出菜单
    QAction *exit_action;
    Hook hook; // 钩子对象

private slots:
    void slot_exit_app();
    void slot_check_hook_type(Hook::Type);
};
#endif // WIDGET_H
