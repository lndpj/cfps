#include "sys.h"

int main(int argc, char** argv)
{
	if(!sys.init()) exit(EXIT_FAILURE);

	while(term_getc() != KEY_ESC && sys.step() && sys.print());

	exit(EXIT_SUCCESS);
}
