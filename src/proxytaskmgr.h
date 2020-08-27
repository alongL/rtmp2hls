#pragma once

#include <string>
#include <map>
#include <string>
#include <vector>

#include "srs_common.h"
#include "srs_app_process.hpp"
#include "srs_app_ffmpeg.hpp"



class IngestTask
{
public:
	virtual ~IngestTask();
	int start();
	void stop();
	int cycle();
	void fast_stop();
	void fast_kill();

	void init(std::string src, std::string dest);


	std::string src;
	std::string dest;// 类似于/live/my
	std::string rtmp;// rtmp://127.0.0.1:1936/live/my
	std::string hls;// http://127.0.0.1:8081/live/my.m3u8
	bool enable=true;

	time_t starttime=time(0);
	SrsFFMPEG* ffmpeg=nullptr;
};




class ProxytaskMgr
{
private:
	ProxytaskMgr() {}

	std::map<std::string, IngestTask*> m_taskMap;
	SrsProcess* m_srs_process = nullptr;
	std::string  m_errmsg;

	//这两个端口号是srs配置的
	std::string m_hls_port = "8081";
	std::string m_rtmp_port = "1936";
public:
	static ProxytaskMgr& getinstance()
	{
		static ProxytaskMgr instance;
		return instance;
	}
	int init();
	~ProxytaskMgr()
	{
		delete m_srs_process;
	}

	int load_from_db();
	//int save_task_to_db(IngestTask *ptask);
	//int del_task_from_db(const std::string dest);
	std::string get_hls_port() { return m_hls_port; }
	std::string get_rtmp_port() { return m_rtmp_port; }


	int add_task(std::string src, std::string dest)
	{
		if (src.empty() || dest.empty())
		{
			m_errmsg = "failed. parameter is empty.";
			return -1;
		}

		//如果已经存在，不添加
		if (m_taskMap.find(dest) != m_taskMap.end())
		{
			m_errmsg = "failed." + dest + " exists.";
			return -1;
		}

		//目前没有此任务，添加
		IngestTask* ptask = new IngestTask();
		ptask->init(src, dest);
		m_taskMap[dest] = ptask;
		ptask->start();
		return 0;
	 }


	//删除某个任务
	int del_task(std::string dest)
	{
		//如果已经存在，不添加
		auto iter = m_taskMap.find(dest);
		if (iter == m_taskMap.end())
		{
			m_errmsg = "dest not found. " + dest;
			return -1;
		}

		auto ptask = iter->second;
		ptask->stop();
		m_taskMap.erase(iter);
		//del_task_from_db(dest);
		delete ptask;
		
		return 0;
	}

	const decltype(m_taskMap)&  get_task_list()
	{
		return m_taskMap;
	}

	//获取对应的hls地址
	std::string get_hls_path(const std::string &dest)
	{
		auto iter = m_taskMap.find(dest);
		if (iter == m_taskMap.end())
			return "";

		auto ptask = iter->second;
		if (ptask)
			return ptask->hls;
		else
			return "";
	}

	std::string get_errmsg() const
	{
		return m_errmsg;
	}




	int startAll()
	{
		for (auto &item : m_taskMap)
		{
			auto task = item.second;
			task->start();
		}
		return 0;
	}

	int start(std::string src, std::string dest);
	

	//停止某个url
	int stop(std::string dest)
	{
		auto iter = m_taskMap.find(dest);
		if (iter != m_taskMap.end())
			return 0;

		auto ptask = iter->second;
		ptask->stop();
	}

	int stopAll(std::string src)
	{
		for (auto &item : m_taskMap)
		{
			auto ptask = item.second;
			ptask->stop();
			//删除此task;
		}
	}


	void loadConfig(bool force=false);

	//1 秒check 一次
	int check(int timecnt);
	


};