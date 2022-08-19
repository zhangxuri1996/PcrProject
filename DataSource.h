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

	// 实例
	static DataSource* instance;

	map<const char*, CString> data;

public:
	static DataSource* getInstance();

	// 初始化
	static void init();

	void addAttribute(const char* key, CString& value);

	void update(const char* key, CString& value);


	// 销毁
	void destory();
	~DataSource();
};

