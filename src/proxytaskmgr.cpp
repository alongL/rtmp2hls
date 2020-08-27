#include "proxytaskmgr.h"
#include "srs_app_process.hpp"




using namespace std;




IngestTask::~IngestTask()
{
	free(ffmpeg);
}


int IngestTask::start()
{
	return ffmpeg->start();
}

void IngestTask::stop()
{
	ffmpeg->stop();
}

srs_error_t IngestTask::cycle()
{
	return ffmpeg->cycle();
}

void IngestTask::fast_stop()
{
	ffmpeg->fast_stop();
}

void IngestTask::fast_kill()
{
	ffmpeg->fast_kill();
}

std::string replaceAll(const std::string& instr, const std::string& from, const std::string& to) {
	if (from.empty())
		return instr;
	std::string str = instr;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return str;
}
void IngestTask::init(std::string src, std::string dest)
{
	this->src = src;
	this->dest = dest;
	std::string rtmp_port = ProxytaskMgr::getinstance().get_rtmp_port();
	std::string hls_port = ProxytaskMgr::getinstance().get_hls_port();
	this->rtmp = "rtmp://127.0.0.1:" + rtmp_port + dest;					        //rtmp://localhost/live/my
	this->hls = "http://127.0.0.1:" + hls_port + dest + std::string("/hls.m3u8");   //http://localhost/live/my/hls.m3u8

	std::string  m3u8_dir = "./html" + dest;
	std::string  m3u8 = m3u8_dir + std::string("/hls.m3u8");
	string cmd = "mkdir  -p " + string(m3u8_dir);
	system(cmd.c_str());

	string ffmpeg_path = "./bin/ffmpeg";
	string cmd1 = "chmod +x " + ffmpeg_path;
	system(cmd.c_str());

	ffmpeg = new SrsFFMPEG(ffmpeg_path);
	auto name = replaceAll(dest, "/", "_");
	string log_file = "./logs/ffmpeg" + name;// 日志文件./logs/ffmpeg_live_my
	
	ffmpeg->initialize(src, m3u8, log_file);
}





//////////////////////////////////////////////////////////////////////////////////////

int ProxytaskMgr::init()
{


	return 0;
}

int ProxytaskMgr::load_from_db()
{
	return 0;
}

int ProxytaskMgr::start(std::string src, std::string dest)
{
	return add_task(src, dest);
}


//第一次强制加载配置文件
void ProxytaskMgr::loadConfig(bool force)
{
	
}


int ProxytaskMgr::check(int timecnt)
{
	//检查srs是否正在运行
	if (m_srs_process)
	{
		m_srs_process->cycle();//检查
		m_srs_process->start();//启动。内部有标志，如果未在运行才运行程序
	}


	for (auto &item : m_taskMap)
	{	
		int err = 0;
		auto task = item.second;
		
		// check ffmpeg status.
		if ((err = task->cycle()) != srs_success) {
			printf("ingest cycle. err:%d\n", err);
		}
		
		if ((err = task->start()) != srs_success) {
			printf("ingester start. err:%d\n", err);
		}
	}

	if (timecnt % 10 == 0)//every 10 seconds
	{
		loadConfig(); //读取加载配置文件
	}

	return 0;
}




