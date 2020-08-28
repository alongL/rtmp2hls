
#include <cstdio>
#include "httplib.h"
#include <iostream>

#include "timer.hpp"
#include "proxytaskmgr.h"

#include "csv.hpp"



using namespace httplib;
using namespace std;




#ifndef WIN32
#include <signal.h>
void signalDeal(int sig)
{
	//ctrl+c, pkill
	if (sig == SIGINT || sig == SIGTERM)    
	{
		ProxytaskMgr::getinstance().fast_kill();
		printf("receive signal %d, and kill sub process.\n", sig);
		exit(0);
	}
	else
	{
		printf("receive signal %d. but ignore.\n", sig);
	}
}

#endif





string log(const Request &req, const Response &res) {
	string s;
	char buf[BUFSIZ];

	snprintf(buf, sizeof(buf), "%s %s %s\n", req.method.c_str(),
		req.version.c_str(), req.path.c_str());
	s += buf;

	return s;
}


static int timer_cnt = 0;
void check()
{
	timer_cnt++;
	if(timer_cnt%3 == 0)
		ProxytaskMgr::getinstance().check(timer_cnt);

}


const string CSV_FILE = "tasks.csv";
map<string,string> load_task_from_csv()
{
	map<string, string> taskmap;
	try {
		csv::CSVFormat format;
		format.trim({ ' ', '\t' });
		csv::CSVReader reader(CSV_FILE, format);
		csv::CSVRow row;
		while (reader.read_row(row))
		{
			if (!row["dest"].is_null() &&
				!row["src"].is_null()
				)
			{
				auto dest = row["dest"].get();
				auto src = row["src"].get();

				taskmap[src] = dest;
			}
		}
	}
	catch (exception& ex)
	{
		printf("exception:%s\n", ex.what());
	}
	return taskmap;
}



int main(int argc, const char **argv)
{

#ifndef WIN32
	signal(SIGINT, signalDeal);   //注册SIGINT对应的处理函数
	signal(SIGTERM, signalDeal);  //注册SIGTERM对应的处理函数 pkill默认
#endif

	httplib::Server svr;



	svr.set_error_handler([](const Request & /*req*/, Response &res) {
		const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
		char buf[BUFSIZ];
		snprintf(buf, sizeof(buf), fmt, res.status);
		res.set_content(buf, "text/html");
	});

	svr.set_logger(
		[](const Request &req, const Response &res) { cout << log(req, res); });

	auto port = 8086;
	if (argc > 1) 
	{ port = atoi(argv[1]); }

	auto base_dir = "./html";
	if (argc > 2) 
	{ base_dir = argv[2]; }

	string cmd = "mkdir  -p " + string(base_dir);
	system(cmd.c_str());

	system("mkdir -p logs");


	if (!svr.set_mount_point("/", base_dir)) {
		cout << "The specified base directory doesn't exist...";
		return 1;
	}
	


	map<string,string> taskmap = load_task_from_csv();
	for (auto item : taskmap)
	{
		auto src = item.first;
		auto dest = item.second;
		ProxytaskMgr::getinstance().add_task(src, dest);
	}


	Timer m_timer;
	m_timer.StartTimer(1000, std::bind(check));//check the ffmpeg process;



	cout << "The server started at port " << port  << endl;
	svr.listen("0.0.0.0", port);

	return 0;
}
