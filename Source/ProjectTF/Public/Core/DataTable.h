#ifndef DATA_TABLE_H_
#define DATA_TABLE_H_

#include <string>
#include <iostream>
#include <fstream>
#include <map>

namespace DataTable
{
typedef unsigned char byte_t;
typedef signed char sbyte_t;
typedef unsigned int uint_t;
typedef unsigned long long ulong_t;
typedef unsigned short ushort_t;

using namespace std;

class DataTableBase
{
public:
	DataTableBase()
	{
	}

	virtual ~DataTableBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
	}
};

class DataTableListBase
{
protected:
	ushort_t Version;
	std::map<ulong_t, DataTableBase * > datatable_list_;

public:
	DataTableListBase()
	{
	}

	virtual ~DataTableListBase()
	{
		std::map<ulong_t, DataTableBase*>::iterator it;
		for(it = datatable_list_.begin(); it != datatable_list_.end(); ++it)
		{
			delete it->second;
		}

		datatable_list_.clear();
	}

	virtual void Load(std::ifstream &is)
	{
	}

	std::map<ulong_t, DataTableBase * > * GetDataTableList()
	{
		return &datatable_list_;
	}

	DataTableBase* GetDataTable(ulong_t serialNo)
	{
		std::map<ulong_t, DataTableBase*>::iterator it = datatable_list_.find(serialNo);
		if(it == datatable_list_.end())
		{
			return NULL;
		}
		return it->second;
	}

	ushort_t GetVersion()
	{
		return Version;
	}

};

class ActionDataTable : public DataTableBase
{
public:
	ulong_t SerialNo;
	std::list<string> Command;
	std::list<string> Interval;
	string AnimName;
	byte_t Loop;
	byte_t CutScene;
	ushort_t Damage;
	ushort_t B_Damage;
	ushort_t MP_Add_OH;
	ushort_t MP_Add_OB;
	ushort_t MP_Self_Add_OH;
	ushort_t MP_Self_Add_OB;
	ulong_t DamageActionNo;
	byte_t Block_C;
	byte_t Upper_Atk;
	byte_t Launcher_Atk;
	ushort_t Upper_Force;
	byte_t Wall_Stun;
	byte_t Push_Atk;
	ushort_t Push_Force;
	string C_Hit_Motion;
	byte_t Down_Atk;
	string Add_FX;
	ushort_t HitInterval;
	ushort_t HitStun;
	ushort_t BlockStun;
	ushort_t React;
	byte_t ChargeAction;
	ushort_t MaxChargeTime;
	ushort_t Invincible;
	std::list<ulong_t> Chain_Atk_List;
	ulong_t Chain_Action_Append;
	byte_t Finish;
	ulong_t Juggle;
	byte_t DownCheck;
	byte_t Cancel_Act;
	ulong_t NextActionNo;
	ushort_t NextActionInterval;
	ulong_t PrevActionNo;
	byte_t EndType;
	byte_t MoveType;
	byte_t RotateType;
	byte_t CatchType;
	byte_t BlockUseType;
	byte_t PlayerState;
	byte_t AttackState;

public:
	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&SerialNo, sizeof(ulong_t));
		uint_t Command_count = 0;
		is.read((char*)&Command_count, sizeof(uint_t));
		for( uint_t i = 0; i < Command_count; ++i)
		{
			unsigned int temp_count = 0;
			is.read((char*)&temp_count, sizeof(uint_t));
			char* temp_string = new char[temp_count];
			temp_string[0] = { 0, };
			is.read(temp_string, temp_count);
			string temp = "";
			temp.append(temp_string, temp_count);

			Command.push_back(temp);
		}
		uint_t Interval_count = 0;
		is.read((char*)&Interval_count, sizeof(uint_t));
		for( uint_t i = 0; i < Interval_count; ++i)
		{
			unsigned int temp_count = 0;
			is.read((char*)&temp_count, sizeof(uint_t));
			char* temp_string = new char[temp_count];
			temp_string[0] = { 0, };
			is.read(temp_string, temp_count);
			string temp = "";
			temp.append(temp_string, temp_count);

			Interval.push_back(temp);
		}
		uint_t AnimName_count = 0;
		is.read((char*)&AnimName_count, sizeof(uint_t));
		char* AnimName_temp = new char[AnimName_count];
		AnimName_temp[0] = { 0, };
		is.read(AnimName_temp, AnimName_count);
		AnimName = "";
		AnimName.append(AnimName_temp, AnimName_count);
		is.read((char*)&Loop, sizeof(byte_t));
		is.read((char*)&CutScene, sizeof(byte_t));
		is.read((char*)&Damage, sizeof(ushort_t));
		is.read((char*)&B_Damage, sizeof(ushort_t));
		is.read((char*)&MP_Add_OH, sizeof(ushort_t));
		is.read((char*)&MP_Add_OB, sizeof(ushort_t));
		is.read((char*)&MP_Self_Add_OH, sizeof(ushort_t));
		is.read((char*)&MP_Self_Add_OB, sizeof(ushort_t));
		is.read((char*)&DamageActionNo, sizeof(ulong_t));
		is.read((char*)&Block_C, sizeof(byte_t));
		is.read((char*)&Upper_Atk, sizeof(byte_t));
		is.read((char*)&Launcher_Atk, sizeof(byte_t));
		is.read((char*)&Upper_Force, sizeof(ushort_t));
		is.read((char*)&Wall_Stun, sizeof(byte_t));
		is.read((char*)&Push_Atk, sizeof(byte_t));
		is.read((char*)&Push_Force, sizeof(ushort_t));
		uint_t C_Hit_Motion_count = 0;
		is.read((char*)&C_Hit_Motion_count, sizeof(uint_t));
		char* C_Hit_Motion_temp = new char[C_Hit_Motion_count];
		C_Hit_Motion_temp[0] = { 0, };
		is.read(C_Hit_Motion_temp, C_Hit_Motion_count);
		C_Hit_Motion = "";
		C_Hit_Motion.append(C_Hit_Motion_temp, C_Hit_Motion_count);
		is.read((char*)&Down_Atk, sizeof(byte_t));
		uint_t Add_FX_count = 0;
		is.read((char*)&Add_FX_count, sizeof(uint_t));
		char* Add_FX_temp = new char[Add_FX_count];
		Add_FX_temp[0] = { 0, };
		is.read(Add_FX_temp, Add_FX_count);
		Add_FX = "";
		Add_FX.append(Add_FX_temp, Add_FX_count);
		is.read((char*)&HitInterval, sizeof(ushort_t));
		is.read((char*)&HitStun, sizeof(ushort_t));
		is.read((char*)&BlockStun, sizeof(ushort_t));
		is.read((char*)&React, sizeof(ushort_t));
		is.read((char*)&ChargeAction, sizeof(byte_t));
		is.read((char*)&MaxChargeTime, sizeof(ushort_t));
		is.read((char*)&Invincible, sizeof(ushort_t));
		uint_t Chain_Atk_List_count = 0;
		is.read((char*)&Chain_Atk_List_count, sizeof(uint_t));
		for( uint_t i = 0; i < Chain_Atk_List_count; ++i)
		{
			ulong_t temp;
			is.read((char*)&temp, sizeof(ulong_t));
			Chain_Atk_List.push_back(temp);
		}
		is.read((char*)&Chain_Action_Append, sizeof(ulong_t));
		is.read((char*)&Finish, sizeof(byte_t));
		is.read((char*)&Juggle, sizeof(ulong_t));
		is.read((char*)&DownCheck, sizeof(byte_t));
		is.read((char*)&Cancel_Act, sizeof(byte_t));
		is.read((char*)&NextActionNo, sizeof(ulong_t));
		is.read((char*)&NextActionInterval, sizeof(ushort_t));
		is.read((char*)&PrevActionNo, sizeof(ulong_t));
		is.read((char*)&EndType, sizeof(byte_t));
		is.read((char*)&MoveType, sizeof(byte_t));
		is.read((char*)&RotateType, sizeof(byte_t));
		is.read((char*)&CatchType, sizeof(byte_t));
		is.read((char*)&BlockUseType, sizeof(byte_t));
		is.read((char*)&PlayerState, sizeof(byte_t));
		is.read((char*)&AttackState, sizeof(byte_t));
	}

};

#define Action_NAME "Action"
#define Action_DATAFILENAME "ActionData.bytes"
class ActionDataTable_List : public DataTableListBase
{
public:
	ActionDataTable_List() : DataTableListBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&Version, sizeof(ushort_t));
		ushort_t data_count = 0;
		is.read((char*)&data_count, sizeof(ushort_t));
		for(ushort_t i = 0; i < data_count; ++i)
		{
			ActionDataTable * data = new ActionDataTable();
			data->Load(is);
			datatable_list_.insert(std::make_pair(data->SerialNo, data));
		}
	}

};

class CharacterDataTable : public DataTableBase
{
public:
	ulong_t SerialNo;
	string Name;
	byte_t Default;
	ushort_t HP;
	ushort_t MP;
	uint_t Foward_Walk;
	uint_t Back_Walk;
	uint_t Foward_Dash;
	uint_t Back_Dash;
	uint_t Jump;
	uint_t Forward_Jump;
	uint_t Back_Jump;
	uint_t Evade;
	uint_t Parry;
	ulong_t DefaultActionNo;

public:
	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&SerialNo, sizeof(ulong_t));
		uint_t Name_count = 0;
		is.read((char*)&Name_count, sizeof(uint_t));
		char* Name_temp = new char[Name_count];
		Name_temp[0] = { 0, };
		is.read(Name_temp, Name_count);
		Name = "";
		Name.append(Name_temp, Name_count);
		is.read((char*)&Default, sizeof(byte_t));
		is.read((char*)&HP, sizeof(ushort_t));
		is.read((char*)&MP, sizeof(ushort_t));
		is.read((char*)&Foward_Walk, sizeof(uint_t));
		is.read((char*)&Back_Walk, sizeof(uint_t));
		is.read((char*)&Foward_Dash, sizeof(uint_t));
		is.read((char*)&Back_Dash, sizeof(uint_t));
		is.read((char*)&Jump, sizeof(uint_t));
		is.read((char*)&Forward_Jump, sizeof(uint_t));
		is.read((char*)&Back_Jump, sizeof(uint_t));
		is.read((char*)&Evade, sizeof(uint_t));
		is.read((char*)&Parry, sizeof(uint_t));
		is.read((char*)&DefaultActionNo, sizeof(ulong_t));
	}

};

#define Character_NAME "Character"
#define Character_DATAFILENAME "CharacterData.bytes"
class CharacterDataTable_List : public DataTableListBase
{
public:
	CharacterDataTable_List() : DataTableListBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&Version, sizeof(ushort_t));
		ushort_t data_count = 0;
		is.read((char*)&data_count, sizeof(ushort_t));
		for(ushort_t i = 0; i < data_count; ++i)
		{
			CharacterDataTable * data = new CharacterDataTable();
			data->Load(is);
			datatable_list_.insert(std::make_pair(data->SerialNo, data));
		}
	}

};

class GameModeDataTable : public DataTableBase
{
public:
	ulong_t SerialNo;
	ushort_t RoundTime;
	byte_t RoundLimit;
	std::list<ulong_t> StageList;

public:
	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&SerialNo, sizeof(ulong_t));
		is.read((char*)&RoundTime, sizeof(ushort_t));
		is.read((char*)&RoundLimit, sizeof(byte_t));
		uint_t StageList_count = 0;
		is.read((char*)&StageList_count, sizeof(uint_t));
		for( uint_t i = 0; i < StageList_count; ++i)
		{
			ulong_t temp;
			is.read((char*)&temp, sizeof(ulong_t));
			StageList.push_back(temp);
		}
	}

};

#define GameMode_NAME "GameMode"
#define GameMode_DATAFILENAME "GameModeData.bytes"
class GameModeDataTable_List : public DataTableListBase
{
public:
	GameModeDataTable_List() : DataTableListBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&Version, sizeof(ushort_t));
		ushort_t data_count = 0;
		is.read((char*)&data_count, sizeof(ushort_t));
		for(ushort_t i = 0; i < data_count; ++i)
		{
			GameModeDataTable * data = new GameModeDataTable();
			data->Load(is);
			datatable_list_.insert(std::make_pair(data->SerialNo, data));
		}
	}

};

class GlobalSettingDataTable : public DataTableBase
{
public:
	ulong_t SerialNo;
	ushort_t MultiCommandInterval;
	ushort_t ContinueActionInterval;

public:
	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&SerialNo, sizeof(ulong_t));
		is.read((char*)&MultiCommandInterval, sizeof(ushort_t));
		is.read((char*)&ContinueActionInterval, sizeof(ushort_t));
	}

};

#define GlobalSetting_NAME "GlobalSetting"
#define GlobalSetting_DATAFILENAME "GlobalSettingData.bytes"
class GlobalSettingDataTable_List : public DataTableListBase
{
public:
	GlobalSettingDataTable_List() : DataTableListBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&Version, sizeof(ushort_t));
		ushort_t data_count = 0;
		is.read((char*)&data_count, sizeof(ushort_t));
		for(ushort_t i = 0; i < data_count; ++i)
		{
			GlobalSettingDataTable * data = new GlobalSettingDataTable();
			data->Load(is);
			datatable_list_.insert(std::make_pair(data->SerialNo, data));
		}
	}

};

class StageDataTable : public DataTableBase
{
public:
	ulong_t SerialNo;
	string Name;
	byte_t Default;
	string BGM;

public:
	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&SerialNo, sizeof(ulong_t));
		uint_t Name_count = 0;
		is.read((char*)&Name_count, sizeof(uint_t));
		char* Name_temp = new char[Name_count];
		Name_temp[0] = { 0, };
		is.read(Name_temp, Name_count);
		Name = "";
		Name.append(Name_temp, Name_count);
		is.read((char*)&Default, sizeof(byte_t));
		uint_t BGM_count = 0;
		is.read((char*)&BGM_count, sizeof(uint_t));
		char* BGM_temp = new char[BGM_count];
		BGM_temp[0] = { 0, };
		is.read(BGM_temp, BGM_count);
		BGM = "";
		BGM.append(BGM_temp, BGM_count);
	}

};

#define Stage_NAME "Stage"
#define Stage_DATAFILENAME "StageData.bytes"
class StageDataTable_List : public DataTableListBase
{
public:
	StageDataTable_List() : DataTableListBase()
	{
	}

	virtual void Load(std::ifstream &is)
	{
		is.read((char*)&Version, sizeof(ushort_t));
		ushort_t data_count = 0;
		is.read((char*)&data_count, sizeof(ushort_t));
		for(ushort_t i = 0; i < data_count; ++i)
		{
			StageDataTable * data = new StageDataTable();
			data->Load(is);
			datatable_list_.insert(std::make_pair(data->SerialNo, data));
		}
	}

};


class DataTableLoader
{
protected:
	map<string, DataTableListBase* > datatablelist_list_;

public:
	DataTableLoader(const char* dst_path)
	{
		Load(dst_path, Action_DATAFILENAME, Action_NAME, new ActionDataTable_List());
		Load(dst_path, Character_DATAFILENAME, Character_NAME, new CharacterDataTable_List());
		Load(dst_path, GameMode_DATAFILENAME, GameMode_NAME, new GameModeDataTable_List());
		Load(dst_path, GlobalSetting_DATAFILENAME, GlobalSetting_NAME, new GlobalSettingDataTable_List());
		Load(dst_path, Stage_DATAFILENAME, Stage_NAME, new StageDataTable_List());
	}

	~DataTableLoader()
	{
		map<string, DataTableListBase*>::iterator it;
		for(it = datatablelist_list_.begin(); it != datatablelist_list_.end(); ++it)
		{
			delete it->second;
		}

		datatablelist_list_.clear();
	}

	void Load(const char* dst_path, const char* file_name, const char* data_name, DataTableListBase* datatable_list)
	{
		try {
			char temp_path[512] = {0,};
			sprintf(temp_path, "%s/%s", dst_path, file_name);
			string path = temp_path;
			std::ifstream is (path, std::ios::in | std::ios::binary);
			if (is.is_open())
			{
				datatable_list->Load(is);
				map<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);
				if(it != datatablelist_list_.end())
				{
					delete it->second;
					datatablelist_list_.erase(data_name);
				}
				datatablelist_list_.insert(std::make_pair(data_name, datatable_list));
			}
			else
			{
				delete datatable_list;
			}

			is.close();
		}
		catch (exception&)
		{
			delete datatable_list;
		}
	}

	DataTableBase* GetDataTable(const char* data_name, ulong_t serialNo)
	{
		map<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);
		if (it == datatablelist_list_.end())
		{
			return NULL;
		}
		return it->second->GetDataTable(serialNo);
	}

	DataTableListBase* GetDataTableList(const char* data_name)
	{
		map<string, DataTableListBase*>::iterator it = datatablelist_list_.find(data_name);
		if(it == datatablelist_list_.end())
		{
			return NULL;
		}
		return it->second;
	}

};

}
#endif

