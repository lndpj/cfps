#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include "term.h"

typedef void (*FUNC_PTR_SIGNAL)(int sig);
typedef bool (*FUNC_PTR_INIT)(void);
typedef void (*FUNC_PTR_HALT)(void);
typedef bool (*FUNC_PTR_STEP)(void);
typedef bool (*FUNC_PTR_PRINT)(void);
typedef char* (*FUNC_PTR_FPS)(void);

static inline bool  sys_init();
static inline void  sys_halt();
static inline void  sys_sigalrm(int sig);
static inline void  sys_sigint(int sig);
static inline char* sys_time();
static inline char* sys_fps();
static inline bool  sys_step();
static inline bool  sys_print();

struct
{
	FUNC_PTR_SIGNAL        interrupt;
	FUNC_PTR_SIGNAL        alarm;
	FUNC_PTR_INIT          init;
	FUNC_PTR_HALT          halt;
	FUNC_PTR_STEP          step;
	FUNC_PTR_PRINT         print;
	FUNC_PTR_FPS           fps;
	struct
	{
		uint64_t     cnt;
		size_t       acc;
		size_t       fps;
		char cnt_str[32];
		char acc_str[32];
		char fps_str[32];
	} frame;
	struct
	{
		time_t       beg;
		time_t       now;
		size_t       sec;
		char     str[32];
		char     sec_str[32];
	} time;
} sys = { .interrupt = sys_sigint, .alarm = sys_sigalrm, .init = sys_init, .halt = sys_halt, .step = sys_step, .print = sys_print, .fps = sys_fps, .frame.acc = 0, .frame.cnt = 0, .frame.fps = 0, .frame.fps_str = "", .frame.cnt_str = "", .frame.acc_str = "", .time.now = 0, .time.beg = 0, .time.sec = 0, .time.str = "" };

static inline bool sys_print()
{
		static bool first = true;
		if(first)
		{
			printf(" %16s   %16s   %16s   %16s  %s", "uptime", "frames total", "frames sec", "fps", "calendar time");
			puts("");
			first = false;
		}
		printf("\r[%16s] [%16s] [%16s] [%16s] %s",
		sys.time.sec_str,
		sys.frame.cnt_str,
		sys.frame.acc_str,
		sys.frame.fps == 0 ? sys.frame.acc_str : sys.frame.fps_str,
		sys.time.str);
		return true;
}

static inline bool sys_step()
{
	sys.frame.acc++;
	snprintf(sys.frame.acc_str, 31, "%16zu", sys.frame.acc);
	return true;
}

static inline char* sys_fps()
{
	sys.frame.cnt += sys.frame.acc;
	sys.frame.fps = sys.frame.acc;
	sys.frame.acc = 0;

	snprintf(sys.frame.fps_str, 31, "%16zu", sys.frame.fps);
	snprintf(sys.frame.cnt_str, 31, "%16zu", sys.frame.cnt);
	return sys.frame.fps_str;
}

static inline char* sys_time()
{
	static bool first = true;
	sys.time.now = time(NULL);
	if(first)
	{
		sys.time.beg = sys.time.now; sys.time.sec = 0; first = false;
	}
	else
	{
		sys.time.sec++;
	}
	ctime_r(&sys.time.now, sys.time.str);
	sys.time.str[strlen(sys.time.str) - 1] = '\0';
	snprintf(sys.time.sec_str, 31, "%16zu", sys.time.sec);
	return sys.time.str;
}

static inline bool sys_init()
{
	sys_time();
	atexit(sys.halt);
	signal(SIGINT, sys.interrupt);
	signal(SIGALRM,sys.alarm);
	alarm(1);

	term_init();

	return true;
}

static inline void sys_sigalrm(int sig)
{
	sys_fps();
	sys_time();
	alarm(1);
}

static inline void sys_sigint(int sig)
{
	exit(EXIT_FAILURE);
}

static inline void sys_halt()
{
	term_halt();
	puts("");
}

