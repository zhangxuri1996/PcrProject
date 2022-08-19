#pragma once
#include "stdafx.h"
#include <string>

using std::string;

// 积分时间翻倍
#define WIN_USER_INTEGRAL WM_USER +400

// 积分时间翻倍回调通知
#define WIN_USER_INTEGRAL WM_USER +500

// 重置积分消息
#define WIN_USER_RESET_INTEGRAL WM_USER +600

// 积分时间翻倍回调通知
#define WIN_USER_MAIN_INTEGRAL WM_USER + 700

// 通道验证消息
#define WIN_USER_PASSAGEWAY WM_USER + 800

// 通道数量消息参数
#define PASSAGEWAY_NUMBER_BY_CONDITION 0x130

// 主界面接收通道判断消息
#define PASSAGEWAY_NUMBER_BY_CONDITION_RECEIVE 0x150

// 接收通道判断消息判断是否往下执行
#define PASSAGEWAY_NUMBER_BY_CONDITION_RUN_STATUS 0x170

// 主界面转发至拍照界面
#define MAIN_INTERFACE_FORWARDING 0x180
