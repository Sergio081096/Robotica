#include <ros/ros.h>
#include <iostream>
#include <wiringPi.h>
#include <conio.h>

int main()
{
  if (wiringPiSetupGpio() == \-1) {
    std::cout << "cannot setup gpio." << std::endl;
    return 1;
  }

  pinMode(18, PWM_OUTPUT);
  pinMode(19, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  pwmSetClock(266);
  pwmSetRange(1024);
  pwmWrite(18,110);
  pwmWrite(19,105);

  while (1) {
    pwmWrite(18,110);
    if (kbhit() && getch() == 'w'){
      while(1){
      pwmWrite(18,117); //前進
      if(kbhit() && getch() == 'x') break;
       }
    }
    if (kbhit() && getch() == 's'){
      while(1){
      pwmWrite(18,100);//後退
      if(kbhit() && getch() == 'x') break;
       }
    }
    if (kbhit() && getch() == 'd'){
      pwmWrite(19,90);//右旋回
    }
    if (kbhit() && getch() == 'a'){
      pwmWrite(19,120);//左旋回
    }
    if (kbhit() && getch() == 'e'){
      pwmWrite(19,105); //直進
     }
    if (kbhit() && getch() == 'q'){ //停止
    pwmWrite(18,110);
    pwmWrite(19,105);
    break;
    }
  }
  pwmWrite(18,110);
  pwmWrite(19,105);
  return 0;
}
