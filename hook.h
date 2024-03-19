// 创建钩子处理类Hook
#pragma once

#ifndef HOOK_H
#define HOOK_H

#include <windows.h>
#include <QObject>
#include <QAction>
#include <QDebug>

#pragma commenit(lib,"user32.lib");
#pragma commenit(lib,"kernel32.lib");

class Hook:public QObject{
    Q_OBJECT
public:
    Hook();
    enum Type{EXIT}; // 枚举类型
    void install_hook();
    void uninstall_hook();
    void send_signal(Type type);
    Q_ENUM(Type);
signals:
    void send_key_type(Type);
};

#endif // HOOK_H
