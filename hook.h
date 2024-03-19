// 创建钩子处理类Hook
#pragma once

#ifndef HOOK_H
#define HOOK_H

#include <windows.h>
#include <QObject>
#include <QAction>
#include <QDebug>
#include <QLibrary>

class Hook:public QObject{
    Q_OBJECT
public:
    Hook();
    ~Hook();
    enum Type{EXIT}; // 枚举类型
    void install_hook();
    void uninstall_hook();
    void send_signal(Type type);
    Q_ENUM(Type);
    void test_hook();
signals:
    void send_key_type(Type);
};

#endif // HOOK_H
