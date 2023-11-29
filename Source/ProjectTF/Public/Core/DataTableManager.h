// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable.h"

typedef unsigned long long ulong_t;

/**
 * 
 */
class PROJECTTF_API DataTableManager
{
public:
	static DataTableManager* GetInstance();

	static DataTableManager* ms_pInstance;

	static void Destroy();
public:
	DataTableManager();
	~DataTableManager();

	DataTable::DataTableBase* GetDataTable(const char * data_name, ulong_t serialNo);
	std::map<ulong_t, DataTable::DataTableBase* >* GetDataTableList(const char* data_name);

	std::vector<DataTable::DataTableBase*> * GetActionDataTable(ulong_t serialNo);

	int8 GetCharacterCount();

protected:
	void CacheActionDataTable();

protected:
	DataTable::DataTableLoader* m_pDataTableLoader;

	std::map<ulong_t, std::vector<DataTable::DataTableBase*> * > m_characterActionMap;
};
