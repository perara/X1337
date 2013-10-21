/*
 * Implements a simple set of logging macros that prints out file name and
 * line numbers, together with the logging level and the user message.
 * Feel free to steal and modify.
 */

#ifndef __log_h__
#define __log_h__

#include <iostream>
#include <cstring>

#define LOG(line) (strrchr(__FILE__, '\\') + 1) << "(" << __LINE__ << \
	"): " << line
#ifdef _DEBUG
#define LOGD(line) std::cout << "D/" << LOG(line) << std::endl
#else
#define LOGD(line)
#endif
#define LOGI(line) std::cout << "I/" << LOG(line) << std::endl
#define LOGW(line) std::cout << "W/" << LOG(line) << std::endl
#define LOGE(line) std::cout << "E/" << LOG(line) << std::endl


#endif
