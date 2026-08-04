#pragma once

#include <errno.h>
#include <fcntl.h>
#include <sys/param.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef KEY_ESC
#define KEY_ESC 27
#endif

void  term_halt()
{
	struct termios info;
	int flags = fcntl(STDIN_FILENO,F_GETFL,0);
	fcntl(STDIN_FILENO, F_SETFL,flags & ~O_NONBLOCK);
	tcgetattr(STDIN_FILENO, &info);
	info.c_lflag |= ICANON;
	info.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &info);
}

bool term_init()
{
	struct termios info;
	tcgetattr(STDIN_FILENO, &info);
	info.c_lflag &= ~ICANON;
	info.c_lflag &= ~ECHO;
	info.c_cc[VMIN] = 1;
	info.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &info);
	int flags = fcntl(STDIN_FILENO,F_GETFL,0);
	if (flags == -1)
	{
		perror("fcntl: F_GETFL failed");
		term_halt();
		return false;
	}
	if(fcntl(STDIN_FILENO, F_SETFL,flags | O_NONBLOCK) == -1)
	{
		perror("fcntl: F_SETFL failed");
		term_halt();
		return false;
	}

	return true;
}

int term_getc()
{
	int c = '\0';
	/* use read here because of slow performance getc and fgetc */
	if(read(STDIN_FILENO, &c, 1) == 1)
		return c;

	if(errno == EAGAIN || errno == EWOULDBLOCK)
		return c;

	perror("getc: read failed\n");
	exit(EXIT_FAILURE);
}

