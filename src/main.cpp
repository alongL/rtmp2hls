
#include <cstdio>
#include "httplib.h"
#include <iostream>

#include "timer.hpp"
#include "proxytaskmgr.h"




using namespace httplib;
using namespace std;


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
	if(timer_cnt % 3==0)
		ProxytaskMgr::getinstance().check(timer_cnt);

}




int main(int argc, const char **argv)
{

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
	if (argc > 1) { port = atoi(argv[1]); }

	auto base_dir = "./html";
	if (argc > 2) { base_dir = argv[2]; }
	string cmd = "mkdir  -p " + string(base_dir);
	system(cmd.c_str());

	system("mkdir -p logs");


	if (!svr.set_mount_point("/", base_dir)) {
		cout << "The specified base directory doesn't exist...";
		return 1;
	}
	


	ProxytaskMgr::getinstance().init();
	ProxytaskMgr::getinstance().add_task("rtmp://yunyuntx.com/live/my", "/live/my");


	Timer m_timer;
	m_timer.StartTimer(1000, std::bind(check));//周期性执行定时任务;



	cout << "The server started at port " << port  << endl;
	svr.listen("0.0.0.0", port);

	return 0;
}
