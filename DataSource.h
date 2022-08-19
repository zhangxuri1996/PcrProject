#pragma once
#include "stdafx.h"
#include <map>

using std::map;
using std::pair;
class DataSource
{
private:
	DataSource& operator = (DataSource &) = delete;

	DataSource();

	// ʵ��
	static DataSource* instance;

	map<const char*, CString> data;

public:
	static DataSource* getInstance();

	// ��ʼ��
	static void init();

	void addAttribute(const char* key, CString& value);

	void update(const char* key, CString& value);


	// ����
	void destory();
	~DataSource();
};

