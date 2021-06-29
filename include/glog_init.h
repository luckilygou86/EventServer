/*
 * @Description: 
 * @version: 
 * @Author: sql
 * @Date: 2021-06-23 15:34:30
 * @LastEditors: sql
 * @LastEditTime: 2021-06-23 17:53:20
 */

#include "glog/logging.h"

static void log_info_init(const char *exec_path, const char *log_path, int v, bool err, bool alsoerr)
{
	FLAGS_log_dir = log_path;
    	//初始化glog
	google::InitGoogleLogging(exec_path);

	//设置输出日志文件名前缀
	char log_file[264] = {0};
	sprintf(log_file, "%s", "INFO_MESSAGE_");	
	google::SetLogDestination(google::INFO, log_file);
	memset(log_file, 0, 264);
	sprintf(log_file, "%s", "ERROR_MESSAGE_");
	google::SetLogDestination(google::ERROR, log_file);

	memset(log_file, 0, 264);
	sprintf(log_file, "%s", "WARNING_MESSAGE_");	
	google::SetLogDestination(google::WARNING, log_file);

    /* vlog */
    FLAGS_v = v;

	FLAGS_alsologtostderr = alsoerr;
	FLAGS_logtostderr = err;
	FLAGS_colorlogtostderr = true;

	//每行log加前缀
   	FLAGS_log_prefix = true;

	//设置logbuflevel，哪些级别立即写入文件还是先缓存,默认INFO级别以上都是直接落盘，会有io影响
	//日志级别 INFO, WARNING, ERROR, FATAL 的值分别为0、1、2、3。
	//FLAGS_logbuflevel = 2;

	//设置logbufsecs，最多延迟（buffer）多少秒写入文件，另外还有默认超过10^6 chars就写入文件。
	FLAGS_logbufsecs = 0;

	//设置日志文件大小，单位M，默认1800MB
	FLAGS_max_log_size = 50;	
}