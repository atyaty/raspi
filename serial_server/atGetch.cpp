#include "atGetch.h"
#include <sys/ioctl.h>



//----------------------------------------------------------------
atGetch::atGetch(int echo) {
  tcgetattr(0, &oldterm);                      /* grab old terminal i/o settings */
  newterm = oldterm;                           /* make new settings same as old settings */
  newterm.c_lflag &= ~ICANON;                  /* disable buffered i/o */
  if (echo) { newterm.c_lflag |= ECHO;  }
  else {      newterm.c_lflag &= ~ECHO;   }
  tcsetattr(0, TCSANOW, &newterm);             /* use these new terminal i/o settings now */
}
//-----------------------------------------------------------------
atGetch::~atGetch(void){  tcsetattr(0, TCSANOW, &oldterm); }
//-----------------------------------------------------------------
int atGetch::kbhint(){
    int n;
    ioctl(0,FIONREAD,&n);
    return n;
}
//-----------------------------------------------------------------
char atGetch::get_char(){
    if(kbhint()){
        return getchar();
    }
    return 0;
}
/*
char atGetch::getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
//-----------------------------------------------------------------
char getch(void){  return getch_(0); }
//-----------------------------------------------------------------
char getche(void){  return getch_(1); }
*/
