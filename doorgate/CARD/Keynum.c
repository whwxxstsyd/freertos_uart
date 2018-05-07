#include "Keynum.h"	 
#include "globe.h"
#include "config.h"

PasswordStruct Password[4];
//u8 *doorNo[4]={DoorPara.Door1Password,DoorPara.Door2Password,DoorPara.Door3Password,DoorPara.Door4Password};

//////////////////////////////////////////////////////////////////////
void Keynum_timer_open(u8 Timer,u16 time)
{
  Password[Timer].Delay_ms = time;
}
void Keynum_timer_close(u8 Timer)
{
  Password[Timer].Delay_ms = 0xffff;
}
void Keynum_TimerDeal(void)
{
  u16 i;
  for(i=0;i<4;i++)
  {
    switch(Password[i].Delay_ms)
    {
      case 0xffff:
        break;
      case 0:
        memset(&Password[i].state,0x00,sizeof(Password[i]));
        memset(Password[i].buffer,0xff,sizeof(Password[i].buffer)+sizeof(Password[0].deal_buffer));
        break;
      default:
        if(Password[i].Delay_ms) Password[i].Delay_ms--;
        break;
    }
  }
}
////////////////////////////////////////////////////////////////////////
void Keynum_Init(void)
{
  u8 i;
  for(i=0;i<4;i++)
  {
    memset(&Password[i].state,0x00,sizeof(Password[i]));
    memset(Password[i].buffer,0xff,sizeof(Password[i].buffer)+sizeof(Password[0].deal_buffer));    
  }
}
void Keynum_Deal(u8 i,u8 Keynum,DoorParaStruct *DoorParaP,SysDataStruct *SysDataP)
{  
  u8 j;
	//keynum
  Password[i].keyNumber = Keynum;
  Keynum_timer_open(i,30000);
  if(Password[i].keyNumber>=0 && Password[i].keyNumber <= 9)//密码
  {
    Password[i].buffer[Password[i].end++] = Password[i].keyNumber;
    if(Password[i].end == Password_buffer_len) Password[i].end = 0;
    if(Password[i].count < Password_buffer_len) Password[i].count++;
    else Password[i].start = Password[i].end;
  }
  else if(Password[i].keyNumber == 10)//后退键
  {
    if(Password[i].end == 0) Password[i].end = Password_buffer_len-1;
    else Password[i].end--;
    Password[i].buffer[Password[i].end] = 0xff;
    if(Password[i].count) Password[i].count--;
    else Password[i].start = Password[i].end;
  }
  else if(Password[i].keyNumber == 11)//确定键
  {
    Password[i].right_flag = 0;
#ifdef 		Password_low //x[0] 低位
    for(j=0;j<Password[i].count;j++)
    {			
      if(Password[i].end == 0) Password[i].end = Password[i]_buffer_len-1;
      else Password[i].end--;
      Password[i].deal_buffer[j] = Password[i].buffer[Password[i].end];
    }
#else			//x[0] 高位
    for(j=0;j<Password[i].count;j++)
    {			
      Password[i].deal_buffer[j] = Password[i].buffer[Password[i].start];
      if(Password[i].start == Password_buffer_len-1) Password[i].start = 0;
      else Password[i].start++;			
    }
#endif
    
    ////////////////////////////////////////////////
    if( DoorParaP->DoorUseEnable )
    {
      //门i密码判断
      for(j=0;j<Password_buffer_len-sizeof(DoorParaP->Door1Password);j++)
      {
        if(memcmp(DoorParaP->Door1Password,Password[i].deal_buffer+j,6)==0)
        {
          Password[i].right_flag = 1;
          break;
        }
      }
    }
    else
    {	
      BeepNoPassword();
      return;				
    }
    
    if(Password[i].right_flag == 1)
    {
      SysDataP->Door_CurAction &= 0xff00;
		  SysDataP->Door_CurAction |= PasswordIn_ACT;
    }
    else
    {
      BeepPasswordError();
    }
    
    memset(&Password[i].state,0x00,sizeof(Password[i]));
    memset(Password[i].buffer,0xff,sizeof(Password[i].buffer)+sizeof(Password[0].deal_buffer));
  }
}


