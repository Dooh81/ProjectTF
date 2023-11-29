// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DataTableManager.h"


DataTableManager* DataTableManager::ms_pInstance = NULL;

DataTableManager* DataTableManager::GetInstance()
{
	if (ms_pInstance == NULL)
	{
		ms_pInstance = new DataTableManager();
	}

	return ms_pInstance;
}

void DataTableManager::Destroy()
{
	if (ms_pInstance == NULL)
	{
		return;
	}

	delete ms_pInstance;
}

DataTableManager::DataTableManager()
{
	const char * pPath = StringCast<ANSICHAR>(*(FPaths::ProjectContentDir() + "/Data")).Get();

	FString LogpPath = ANSI_TO_TCHAR(pPath);
	UE_LOG(LogTemp, Warning, TEXT("DataTableManager Init pPath: %s"), *LogpPath);

	m_pDataTableLoader = new DataTable::DataTableLoader(pPath);

	CacheActionDataTable();
}

DataTableManager::~DataTableManager()
{
}

void DataTableManager::CacheActionDataTable()
{
	DataTable::ActionDataTable_List* pActionDataTableList = (DataTable::ActionDataTable_List*)m_pDataTableLoader->GetDataTableList("Action");// DataTable::Action_DATAFILENAME);

	std::map<ulong_t, DataTable::DataTableBase* >* pActionList = pActionDataTableList->GetDataTableList();
	for (std::map<ulong_t, DataTable::DataTableBase* >::iterator iter = pActionList->begin(); iter != pActionList->end(); ++iter)
	{
		DataTable::ActionDataTable *pActionDataTable = (DataTable::ActionDataTable * )iter->second;

		ulong_t ownerSerialNo = pActionDataTable->SerialNo / 10000;

		std::map<ulong_t, std::vector<DataTable::DataTableBase*>* >::iterator find = m_characterActionMap.find(ownerSerialNo);
		std::vector<DataTable::DataTableBase*>* pCharacterActionDataTableVec = NULL;
		if (find == m_characterActionMap.end())
		{
			pCharacterActionDataTableVec = new std::vector<DataTable::DataTableBase*>();
			m_characterActionMap.insert(std::make_pair(ownerSerialNo, pCharacterActionDataTableVec));
		}
		else
		{
			pCharacterActionDataTableVec = find->second;
		}

		pCharacterActionDataTableVec->push_back(pActionDataTable);

	}
}

DataTable::DataTableBase* DataTableManager::GetDataTable(const char* data_name, ulong_t serialNo)
{
	return m_pDataTableLoader->GetDataTable(data_name, serialNo);
}

std::map<ulong_t, DataTable::DataTableBase*> * DataTableManager::GetDataTableList(const char* data_name)
{
	return m_pDataTableLoader->GetDataTableList(data_name)->GetDataTableList();
}

std::vector<DataTable::DataTableBase*> * DataTableManager::GetActionDataTable(ulong_t serialNo)
{
	std::map<ulong_t, std::vector<DataTable::DataTableBase*> *>::iterator it = m_characterActionMap.find(serialNo);
	if (it == m_characterActionMap.end())
	{
		return NULL;
	}
	return it->second;
}

int8 DataTableManager::GetCharacterCount()
{
	int8 Count = m_pDataTableLoader->GetDataTableList("Character")->GetDataTableList()->size();

	return Count;
}

