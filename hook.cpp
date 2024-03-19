#include "hook.h"

static HHOOK key_hook = nullptr; // 钩子对象
static Hook* hook;

// 钩子消息函数 系统消息队列会返回到该函数
LRESULT CALLBACK key_proc(int code,WPARAM wParam,LPARAM lParam){
    KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam; // 判断按键类型
    if(wParam == WM_KEYDOWN){
        if(pkbhs->vkCode == 0x1B){ // 0x1B是VK_CODE 对应ESC键
            hook->send_signal(Hook::EXIT);
        }
    }
    return CallNextHookEx(key_hook,code,wParam,lParam);
}

Hook::Hook(){}

Hook::~Hook(){}

void Hook::install_hook(){
    key_hook = SetWindowsHookEx(WH_KEYBOARD_LL,key_proc,nullptr,0);
    hook = this;
}

void Hook::uninstall_hook(){
    UnhookWindowsHookEx(key_hook);
    key_hook = nullptr;
}

void Hook::send_signal(Type type){
    emit send_key_type(type); // 发射信号
}
