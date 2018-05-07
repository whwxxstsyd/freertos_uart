#include "Password.h"

PasswordStruct Password[door_count];
u8 *doorNo[4]={DoorPara.DoorPassword,DoorPara.DoorPassword,DoorPara.DoorPassword,DoorPara.DoorPassword};

void Password_Deal(void)
{
	u16 i,j;
  for(i=0;i<door_count;i++)
	{
		if(Password[i].state == receive_keyNumber)
		{
			Password[i].state = 0;
			if((Password[i].keyNumber+1)>0 && Password[i].keyNumber <= 9)//ÃÜÂë
			{
				Password[i].buffer[Password[i].end++] = Password[i].keyNumber;
				if(Password[i].end == Password_buffer_len) Password[i].end = 0;
				if(Password[i].count < Password_buffer_len) Password[i].count++;
				else Password[i].start = Password[i].end;
			}
			else if(Password[i].keyNumber == 10)//ºóÍË¼ü
			{
				if(Password[i].end == 0) Password[i].end = Password_buffer_len-1;
				else Password[i].end--;
				Password[i].buffer[Password[i].end] = 0xff;
				if(Password[i].count) Password[i].count--;
				else Password[i].start = Password[i].end;
			}
			else if(Password[i].keyNumber == 11)//È·¶¨¼ü
			{
				Password[i].right_flag = 0;
			#ifdef 		Password_low //x[0] µÍÎ»
				for(j=0;j<Password[i].count;j++)
				{			
					if(Password[i].end == 0) Password[i].end = Password[i]_buffer_len-1;
					else Password[i].end--;
					Password[i].deal_buffer[j] = Password[i].buffer[Password[i].end];
				}
			#else			//x[0] ¸ßÎ»
				for(j=0;j<Password[i].count;j++)
				{			
					Password[i].deal_buffer[j] = Password[i].buffer[Password[i].start];
					if(Password[i].start == Password_buffer_len-1) Password[i].start = 0;
					else Password[i].start++;			
				}
			#endif
				if(DoorPara.DoorUseEnable & 0x01)//¿ªÆôÃÜÂë
				{
					//ÃÅiÃÜÂëÅÐ¶Ï
					for(j=0;j<Password_buffer_len-Passwore_Digit;j++)
					{
						//if(memcmp(doorNo[i],Password[i].deal_buffer+j,sizeof(*doorNo[i]))==0)//problem
						if(memcmp(doorNo[i],Password[i].deal_buffer+j,6)==0)
						{
							Password[i].right_flag = 1;
							break;
						}
					}
				}
				else
				{	
					BeepNoPassword;
					break;				
				}
				if(Password[i].right_flag == 1)
				{
					switch(i)
					{
						case 0:
							SysData.Door_CurAction =	PasswordIn_ACT;
						break;
						case 1:
							SysData2.Door_CurAction =	PasswordIn_ACT;
						break;
						case 2:
							SysData3.Door_CurAction =	PasswordIn_ACT;
						break;
						case 3:
							SysData4.Door_CurAction =	PasswordIn_ACT;
						break;
						default:
						break;
					}
				}
				else
					BeepPasswordError;
				memset(&Password[i].state,0x00,sizeof(Password[i]));
				memset(Password[i].buffer,0xff,sizeof(Password[i].buffer)+sizeof(Password[0].deal_buffer));
			}
		}
	}
}
