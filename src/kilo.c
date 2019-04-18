#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios orignalTermios;

void die(const char *s){
	perror(s);
	exit(1);
}
void disableRawMode(){{{
		tcsetattr(STDIN_FILENO,TCSAFLUSH,&orignalTermios);
}}}

void enableRawMode(){

		tcgetattr(STDIN_FILENO,&orignalTermios);
		atexit(disableRawMode);

		struct termios raw = orignalTermios;
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		raw.c_oflag &= ~(OPOST);
		raw.c_cflag &= (CS8);
		raw.c_lflag &= ~(ECHO | ICANON | IEXTEN| ISIG);
		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 1;
		tcsetattr(STDIN_FILENO,TCSAFLUSH, &raw);
}
int main(int argc, char *argv[])
{
		enableRawMode();
		while (1){

				char c = '\0';
				read(STDIN_FILENO,&c,1);
				if (iscntrl(c)){
						printf("%d\r\n",c);
				}else{
						printf("%d ('%c')\r\n",c,c);
				}
				if(c == 'q') break;
		}
		return 0;
}
