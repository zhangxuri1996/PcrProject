#pragma once
#include "stdafx.h"
#include <string>

using std::string;

// ����ʱ�䷭��
#define WIN_USER_INTEGRAL WM_USER +400

// ����ʱ�䷭���ص�֪ͨ
#define WIN_USER_INTEGRAL WM_USER +500

// ���û�����Ϣ
#define WIN_USER_RESET_INTEGRAL WM_USER +600

// ����ʱ�䷭���ص�֪ͨ
#define WIN_USER_MAIN_INTEGRAL WM_USER + 700

// ͨ����֤��Ϣ
#define WIN_USER_PASSAGEWAY WM_USER + 800

// ͨ��������Ϣ����
#define PASSAGEWAY_NUMBER_BY_CONDITION 0x130

// ���������ͨ���ж���Ϣ
#define PASSAGEWAY_NUMBER_BY_CONDITION_RECEIVE 0x150

// ����ͨ���ж���Ϣ�ж��Ƿ�����ִ��
#define PASSAGEWAY_NUMBER_BY_CONDITION_RUN_STATUS 0x170

// ������ת�������ս���
#define MAIN_INTERFACE_FORWARDING 0x180
