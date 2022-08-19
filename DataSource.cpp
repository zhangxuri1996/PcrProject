#include "stdafx.h"
#include "DataSource.h"

DataSource* DataSource::instance = NULL;

DataSource::DataSource()
{

}

DataSource * DataSource::getInstance()
{
	
	if (NULL == DataSource::instance) {
		DataSource::instance = new DataSource();
	}

	return DataSource::instance;
}

void DataSource::init()
{
	DataSource::instance = new DataSource();
}

void DataSource::addAttribute(const char * key, CString& value)
{
	this->data.insert(pair<const char*, CString&>(key, value));
}

void DataSource::update(const char * key, CString & value)
{
	this->data[key] = value;
}

void DataSource::destory()
{
	this->data.clear();
	delete DataSource::instance;
}

DataSource::~DataSource()
{
	//this->data.clear();
}
