#include "my_ui.h"	
#include "ugui.h"	


UG_WINDOW g_calling_window;

static void calling_window_callback(UG_MESSAGE *msg)
{
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
	
        }
    }
}


void calling_window_create(void)
{
    static UG_BUTTON button;
    static UG_TEXTBOX textboxs[2];
    static UG_OBJECT objects[3];
	
    UG_WindowCreate(&g_calling_window, objects, 3, calling_window_callback);

	UG_WindowResize( &g_calling_window, 20 , 20 , 119-20, 119-20 ) ;	
	
	UG_WindowSetTitleText(&g_calling_window , "hello" ) ;
	UG_WindowSetTitleTextFont(&g_calling_window , &FONT_6X8 ) ;
				
    UG_ButtonCreate(&g_calling_window, &button, BTN_ID_0, 	
					UG_WindowGetInnerWidth(&g_calling_window)-60, 
					UG_WindowGetInnerHeight(&g_calling_window)-60,	 
					UG_WindowGetInnerWidth(&g_calling_window)-20 , 
					UG_WindowGetInnerHeight(&g_calling_window)-20);			
	
    UG_ButtonSetFont(&g_calling_window, BTN_ID_0, &FONT_6X8);			
    UG_ButtonSetText(&g_calling_window, BTN_ID_0, "OK");			

}

	

void calling_window_show(void)
{	
	UG_WindowSetBackColor(&g_calling_window,C_WHITE);		
		
    UG_WindowShow(&g_calling_window);	
}



UG_WINDOW g_time_window;

uint8_t g_time_set_focus = 4;
	
char g_time_set_string[5][5] = {0,};	


u16 year = 2018;
u8	month = 6;
u8	day = 11;
u8 	hour = 9;	
u8 	minute = 30;	


void time_window_update()
{
    g_time_set_string[0][0] = '0' + (hour / 10) % 3;
    g_time_set_string[0][1] = '0' + (hour % 10);
    UG_ButtonSetText(&g_time_window, 4, g_time_set_string[0]);

    g_time_set_string[1][0] = '0' + (minute / 10) % 6;
    g_time_set_string[1][1] = '0' + (minute % 10);
    UG_ButtonSetText(&g_time_window, 6, g_time_set_string[1]);

    g_time_set_string[2][0] = '0' + (year / 1000) % 10;
    g_time_set_string[2][1] = '0' + (year / 100) % 10;
    g_time_set_string[2][2] = '0' + (year / 10) % 10;
    g_time_set_string[2][3] = '0' + (year % 10);
    UG_ButtonSetText(&g_time_window, 7, g_time_set_string[2]);

    g_time_set_string[3][0] = '0' + (month / 10) % 2;
    g_time_set_string[3][1] = '0' + (month % 10);
    UG_ButtonSetText(&g_time_window, 8, g_time_set_string[3]);
	
    g_time_set_string[4][0] = '0' + (day / 10) % 4;
    g_time_set_string[4][1] = '0' + (day % 10);	
    UG_ButtonSetText(&g_time_window, 9, g_time_set_string[4]);
}



static void time_window_callback(UG_MESSAGE *msg)
{
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            switch (msg->sub_id) {
                case 0: // return
                    break;
                case 1: // +
                    break;
                case 2: // -
                    break;

                case 4: // hour
                case 6: // minute
                case 7: // year	
                case 8: // month
                case 9: // day	
                    break;
                default:
                    break;
            }
        }
    }
}


static void time_window_create(void)
{	
    static UG_BUTTON buttons[10];
    static UG_OBJECT objects[10];	
    char *button_icons[] = {"1", "5", "J"}; // <, +, -
    int id = 0;
    int i = 0;
    int j = 0;

    UG_WindowCreate(&g_time_window, objects, sizeof(objects) / sizeof(*objects), time_window_callback);
    UG_WindowSetStyle(&g_time_window, WND_STYLE_2D);
		
	UG_WindowSetForeColor(&g_time_window,C_GREEN) ;							
		
    for (i = 0; i < 3; i++) {	
        id = i;
        UG_ButtonCreate(&g_time_window, buttons + id, id, 80 * i,
                200, 80 * i + 79, 239);
        UG_ButtonSetFont(&g_time_window, id, &FONT_ICON24);
        UG_ButtonSetText(&g_time_window, id, button_icons[id]);	
        UG_ButtonSetStyle(&g_time_window, id,BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);
    }	

    id++;
    UG_ButtonCreate(&g_time_window, buttons + id, id, 0, 0, 239, 39);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_time_window, id, "Time & Date");
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);

    id++;	
    UG_ButtonCreate(&g_time_window, buttons + id, id, 40, 40, 109, 79);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_time_window, id, "00");
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                        | BTN_STYLE_NO_BORDERS);

    id++;
    UG_ButtonCreate(&g_time_window, buttons + id, id, 110, 40, 129, 79);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_time_window, id, ":");
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);

    id++;
    UG_ButtonCreate(&g_time_window, buttons + id, id, 130, 40, 199, 79);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_time_window, id, "00");
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                        | BTN_STYLE_NO_BORDERS);

    id++;
    UG_ButtonCreate(&g_time_window, buttons + id, id, 10, 80, 109, 119);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_time_window, id, "2015");
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                        | BTN_STYLE_NO_BORDERS);

    id++;
    UG_ButtonCreate(&g_time_window, buttons + id, id, 120, 80, 169, 119);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_time_window, id, "06");
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                        | BTN_STYLE_NO_BORDERS);	

    id++;
    UG_ButtonCreate(&g_time_window, buttons + id, id, 180, 80, 229, 119);
    UG_ButtonSetFont(&g_time_window, id, &FONT_12X20);	
    UG_ButtonSetText(&g_time_window, id, "16");	
    UG_ButtonSetStyle(&g_time_window, id,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                        | BTN_STYLE_NO_BORDERS);	

	

}

static void time_window_show(void)
{	
    UG_WindowShow(&g_time_window);	

	UG_WindowSetBackColor( &g_time_window , C_YELLOW) ;		
}







UG_WINDOW g_home_window;

char *g_entry_name[7] = { "3", "6", "7", "2", "1", "5", "12:00" }; // icon font

char g_system_time_string[17] = {'2', '0', '1', '8', '-', '0', '6', '-', '1', '1', '\n', '1', '1', ':', '2', '1', 0 };

char *g_battery_info[] = { ";", "<", "=", ">", "?" }; // charging, empty, full, half, low

char g_no_sim_string[4] = {'s', 'i', 'm', 0};				
	
static void home_window_callback(UG_MESSAGE *msg)
{
    /* output log to monitor or catcher */
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            switch (msg->sub_id){

			}
        }
    }	
}	

static void home_window_create(void)
{
    static UG_BUTTON buttons[11];
    static UG_OBJECT objects[11];
    uint32_t colors[] = {0x00865a, 0x4edc4, 0xc7f464, 0xff6b6b, 0xc44d58, 0xf7cf10};
    int i, j;		
    int index;

    UG_WindowCreate(&g_home_window, objects, sizeof(objects) / sizeof(*objects),
            home_window_callback);
    UG_WindowSetStyle(&g_home_window, WND_STYLE_2D);	

	

    index = 0;
    for (i = 0; i < 2; i++) {	
        for (j = 0; j < 3; j++) {
            UG_ButtonCreate(&g_home_window, buttons + index, index, 80 * j,
                    80 * i + 80, 80 * (j + 1) - 1, 80 * i + 160 - 1);
            UG_ButtonSetFont(&g_home_window, index, &FONT_ICON48);
            UG_ButtonSetText(&g_home_window, index, g_entry_name[index]);
            UG_ButtonSetBackColor(&g_home_window, index, colors[index]);
            UG_ButtonSetStyle(&g_home_window, index,
                    		BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                            | BTN_STYLE_NO_BORDERS);	

            index++;
        }		
    }	
		
    UG_ButtonCreate(&g_home_window, buttons + index, index, 40, 24, 199, 79);
    UG_ButtonSetFont(&g_home_window, index, &FONT_12X20);
    UG_ButtonSetStyle(&g_home_window, index,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);

    index++;

    UG_ButtonCreate(&g_home_window, buttons + index, index, 200, 0, 239, 26);
    UG_ButtonSetFont(&g_home_window, index, &FONT_ICON24);
    UG_ButtonSetStyle(&g_home_window, index,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);

    index++;

    UG_ButtonCreate(&g_home_window, buttons + index, index, 0, 0, 70, 26);
    UG_ButtonSetFont(&g_home_window, index, &FONT_12X20);
    UG_ButtonSetStyle(&g_home_window, index,	
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);				

	
	UG_ButtonSetText(&g_home_window, 6, g_system_time_string);

	UG_ButtonSetText(&g_home_window, 7, g_battery_info[0]);			

	UG_ButtonSetText(&g_home_window, 8, g_no_sim_string);							

}


static void home_window_show(void)
{	
    UG_WindowShow(&g_home_window);					
	
	UG_WindowSetBackColor(&g_home_window , C_BLACK);		
}














UG_WINDOW g_vibrator_window;
UG_WINDOW *g_vibrator_window_prev;
static uint32_t *g_vibrator_parameters = 0;
static uint8_t g_vibrator_mode = 0;
static uint32_t g_vibrator_time = 0;
static char g_vibrator_time_str[8] = {0,};



void vibrator_window_callback(UG_MESSAGE *msg)

{
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            switch (msg->sub_id) {
				
			}
        }
    }
}



void vibrator_window_create(void)

{
    static UG_BUTTON buttons[6];
    static UG_OBJECT objects[6];
    char *button_texts[] = { "Run Time (s)", ""};
    int offset;

    UG_WindowCreate(&g_vibrator_window, objects,
            sizeof(objects) / sizeof(*objects), vibrator_window_callback);
    UG_WindowSetStyle(&g_vibrator_window, WND_STYLE_2D);

	
    for (offset = 0; offset < 2; offset++) {
        UG_ButtonCreate(&g_vibrator_window, buttons + offset, offset, 0,
                40 * offset + 40, 239, 40 * offset + 80 - 1);
		UG_ButtonSetForeColor(&g_vibrator_window, offset, C_WHITE);
        UG_ButtonSetFont(&g_vibrator_window, offset, &FONT_12X20);

        if (offset % 2) {
            UG_ButtonSetStyle(&g_vibrator_window, offset,
                BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);
        } else {
            UG_ButtonSetStyle(&g_vibrator_window, offset,
                            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
            UG_ButtonSetBackColor(&g_vibrator_window, offset, 0);
        }

        UG_ButtonSetAlignment(&g_vibrator_window, offset, ALIGN_CENTER_LEFT);
        UG_ButtonSetHSpace(&g_vibrator_window, offset, 8);

        UG_ButtonSetText(&g_vibrator_window, offset,
                button_texts[offset]);
    }	

    UG_ButtonCreate(&g_vibrator_window, buttons + offset, offset, 0,
            200, 119, 239);
    UG_ButtonSetFont(&g_vibrator_window, offset, &FONT_ICON24);
    UG_ButtonSetText(&g_vibrator_window, offset, "1");
    UG_ButtonSetStyle(&g_vibrator_window, offset,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);	

    offset++;
    UG_ButtonCreate(&g_vibrator_window, buttons + offset, offset, 120,
            200, 239, 239);
    UG_ButtonSetFont(&g_vibrator_window, offset, &FONT_ICON24);
    UG_ButtonSetText(&g_vibrator_window, offset, ":");
    UG_ButtonSetStyle(&g_vibrator_window, offset,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);

    offset++;
    UG_ButtonCreate(&g_vibrator_window, buttons + offset, offset, 0, 0, 239, 39);
    UG_ButtonSetFont(&g_vibrator_window, offset, &FONT_12X20);	
    UG_ButtonSetText(&g_vibrator_window, offset, "Vibrator");		
    UG_ButtonSetStyle(&g_vibrator_window, offset,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
}


static void vibrator_window_show(void)
{	
    UG_WindowShow(&g_vibrator_window);					
	
	UG_WindowSetBackColor(&g_vibrator_window , C_BLACK);			
}






















UG_WINDOW *g_led_matrix_window_prev;
UG_WINDOW g_led_matrix_window;
UG_WINDOW g_led_matrix_time_window;
char *g_led_matrix_button_text[15] = { "0", "1", "2", "3", "4", "5", "6",
        "7", "8", "9", "a", "b"};	
static char g_led_matrix_display_char = ' ';
static uint32_t *g_led_matrix_parameters = 0;
char g_led_matrix_time_str[8] = {0,};



static void led_matrix_window_callback(UG_MESSAGE *msg)
{
    char ch;
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            switch (msg->sub_id) {
				
			}
        }
    }
}


void led_matrix_time_window_callback(UG_MESSAGE *msg)
{
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            int duration;
            switch (msg->sub_id) {
	
			}
        }
    }
}





void led_matrix_time_window_create(void)
{
    static UG_BUTTON buttons[6];
    static UG_OBJECT objects[6];
    char *button_texts[] = { "Run Time (s)", ""};
    int offset;
	
    UG_WindowCreate(&g_led_matrix_time_window, objects,
            sizeof(objects) / sizeof(*objects), led_matrix_time_window_callback);
    UG_WindowSetStyle(&g_led_matrix_time_window, WND_STYLE_2D);
	
	
    for (offset = 0; offset < 2; offset++) {
        UG_ButtonCreate(&g_led_matrix_time_window, buttons + offset, offset, 0,
                40 * offset + 40, 239, 40 * offset + 80 - 1);
		UG_ButtonSetForeColor(&g_led_matrix_time_window, offset, C_WHITE);
        UG_ButtonSetFont(&g_led_matrix_time_window, offset, &FONT_12X20);

        if (offset % 2) {	
            UG_ButtonSetStyle(&g_led_matrix_time_window, offset,
                BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);
        } else {
            UG_ButtonSetStyle(&g_led_matrix_time_window, offset,
                            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
            UG_ButtonSetBackColor(&g_led_matrix_time_window, offset, C_GRAY);	
        }

        UG_ButtonSetAlignment(&g_led_matrix_time_window, offset, ALIGN_CENTER_LEFT);
        UG_ButtonSetHSpace(&g_led_matrix_time_window, offset, 8);

        UG_ButtonSetText(&g_led_matrix_time_window, offset, button_texts[offset]);
		UG_ButtonSetBackColor(&g_led_matrix_time_window, offset, 0x000000);		
    }
	
    UG_ButtonCreate(&g_led_matrix_time_window, buttons + offset, offset, 0,
            200, 119, 239);
	UG_ButtonSetForeColor(&g_led_matrix_time_window, offset, C_WHITE);
    UG_ButtonSetFont(&g_led_matrix_time_window, offset, &FONT_ICON24);
    UG_ButtonSetText(&g_led_matrix_time_window, offset, "1");
    UG_ButtonSetStyle(&g_led_matrix_time_window, offset,
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);	

    offset++;
    UG_ButtonCreate(&g_led_matrix_time_window, buttons + offset, offset, 120,
            200, 239, 239);
	UG_ButtonSetForeColor(&g_led_matrix_time_window, offset, C_WHITE);
    UG_ButtonSetFont(&g_led_matrix_time_window, offset, &FONT_ICON24);
    UG_ButtonSetText(&g_led_matrix_time_window, offset, ":");	
    UG_ButtonSetStyle(&g_led_matrix_time_window, offset,	
            BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);

    offset++;
    UG_ButtonCreate(&g_led_matrix_time_window, buttons + offset, offset, 0, 0, 239, 39);
	UG_ButtonSetForeColor(&g_led_matrix_time_window, offset, C_WHITE);
    UG_ButtonSetFont(&g_led_matrix_time_window, offset, &FONT_12X20);
    UG_ButtonSetText(&g_led_matrix_time_window, offset, "LED Matrix");
    UG_ButtonSetStyle(&g_led_matrix_time_window, offset,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
    UG_ButtonSetBackColor(&g_led_matrix_time_window, offset, 0x000000);
}





void led_matrix_window_create(void)
{
    static UG_BUTTON buttons[16];
    static UG_OBJECT objects[16];
    char *icons[] = {"1", "3", "4"};
    int i = 0;
    int j = 0;
	
    UG_WindowCreate(&g_led_matrix_window, objects, 16,
            led_matrix_window_callback);
    UG_WindowSetStyle(&g_led_matrix_window, WND_STYLE_2D);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            int index = i * 3 + j;
            UG_ButtonCreate(&g_led_matrix_window, buttons + index, index,
                    80 * j, 40 * (i + 1), 80 * (j + 1) - 1, 40 * (i + 2) - 1);
            UG_ButtonSetFont(&g_led_matrix_window, index, &FONT_12X20);
            UG_ButtonSetText(&g_led_matrix_window, index,
                    g_led_matrix_button_text[index]);	
            UG_ButtonSetStyle(&g_led_matrix_window, index,	
                    BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);
        }
    }

    for (j = 0; j < 3; j++) {
        int index = 4 * 3 + j;
        UG_ButtonCreate(&g_led_matrix_window, buttons + index, index, 80 * j,
                40 * (i + 1), 80 * (j + 1) - 1, 40 * (i + 2) - 1);
        UG_ButtonSetFont(&g_led_matrix_window, index, &FONT_ICON24);
        UG_ButtonSetText(&g_led_matrix_window, index,
                icons[j]);
        UG_ButtonSetStyle(&g_led_matrix_window, index,
                BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);
    }

    UG_ButtonCreate(&g_led_matrix_window, buttons + 15, 15, 0, 0, 239, 39);
	UG_ButtonSetForeColor(&g_led_matrix_window, 15, C_WHITE);
    UG_ButtonSetFont(&g_led_matrix_window, 15, &FONT_12X20);	
    UG_ButtonSetText(&g_led_matrix_window, 15, "led matrix");
    UG_ButtonSetStyle(&g_led_matrix_window, 15,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
    UG_ButtonSetBackColor(&g_led_matrix_window, 15, 0);

    //led_matrix_time_window_create();
}


void led_matrix_window_show(void)
{
		
    UG_WindowShow(&g_led_matrix_time_window);	

	UG_WindowSetBackColor(&g_led_matrix_time_window , C_BLACK);				
}















UG_WINDOW g_settings_window;
uint8_t g_settings_brightness = 3;
uint8_t g_settings_sound = 3;
char g_settings_brightness_str[4] = {'3', 0, 0 };
char g_settings_sound_str[2] = { '3', 0 };
uint8_t g_settings_is_changed = 0;

uint8_t g_brightness_table[] = {2, 20, 40, 50, 60, 80, 100};

extern UG_WINDOW g_home_window;



static void settings_window_callback(UG_MESSAGE *msg)
{
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            switch (msg->sub_id) {
				
			}
        }
    }
}



void settings_window_create(void)
{
    char *button_texts[] = { "1", //icon font
                "", "", "-", "3", "+", "-", "3", "+"};
    static UG_BUTTON buttons[16];
    static UG_OBJECT objects[16];
    int id = 0;
    int i = 0;
    int j = 0;

    UG_WindowCreate(&g_settings_window, objects, 16, settings_window_callback);
    UG_WindowSetStyle(&g_settings_window, WND_STYLE_2D);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            id = i * 3 + j;
            UG_ButtonCreate(&g_settings_window, buttons + id, id, 80 * j,
                    200 - i * 80, 80 * j + 79, 239 - i * 80);
            UG_ButtonSetFont(&g_settings_window, id, &FONT_12X20);
            UG_ButtonSetText(&g_settings_window, id,
                    button_texts[id]);
            UG_ButtonSetStyle(&g_settings_window, id,
                    BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS
                            | BTN_STYLE_NO_BORDERS);
        }
    }

    UG_ButtonSetFont(&g_settings_window, 0, &FONT_ICON24);

    id++;	
    UG_ButtonCreate(&g_settings_window, buttons + id, id, 0, 0, 239, 39);
    UG_ButtonSetFont(&g_settings_window, id, &FONT_12X20);	
    UG_ButtonSetText(&g_settings_window, id, "Brightness");
    UG_ButtonSetStyle(&g_settings_window, id,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
    UG_ButtonSetBackColor(&g_settings_window, id, 0);

    id++;
    UG_ButtonCreate(&g_settings_window, buttons + id, id, 0, 80, 239, 119);
    UG_ButtonSetFont(&g_settings_window, id, &FONT_12X20);
    UG_ButtonSetText(&g_settings_window, id, "Sound");
    UG_ButtonSetStyle(&g_settings_window, id,
            BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
    UG_ButtonSetBackColor(&g_settings_window, id, 0);

   UG_ButtonSetText(&g_settings_window, 7, g_settings_brightness_str);
   UG_ButtonSetText(&g_settings_window, 4, g_settings_sound_str);
   
}


void settings_window_show(void)
{
	UG_WindowShow(&g_settings_window);		
	
	UG_WindowSetBackColor(&g_settings_window , C_YELLOW);		
}













UG_WINDOW g_music_window;
char g_music_filename[4][32] = {0,};
int g_music_filename_selected = -1;
uint32_t *g_music_data_ptr = NULL;





void music_window_callback(UG_MESSAGE *msg)
{
    if (msg->type == MSG_TYPE_OBJECT) {
        if (msg->id == OBJ_TYPE_BUTTON && msg->event == OBJ_EVENT_RELEASED) {
            int index;
            switch (msg->sub_id) {

			}	
        }
    }
}


void music_window_create(void)
{
    static UG_BUTTON buttons[8];
    static UG_OBJECT objects[8];
    char *icons[] = { "1", "D", "C" }; // back, stop, play
    int i = 0;
    int id = 0;

    UG_WindowCreate(&g_music_window, objects,
            sizeof(objects) / sizeof(*objects), music_window_callback);
    UG_WindowSetStyle(&g_music_window, WND_STYLE_2D);

    for (i = 0; i < 3; i++) {
        UG_ButtonCreate(&g_music_window, buttons+ i, i, 80 * i,
                200, 80 * i + 80 - 1, 239);
        UG_ButtonSetFont(&g_music_window, i , &FONT_ICON24);
        UG_ButtonSetText(&g_music_window, i, icons[i]);
        UG_ButtonSetStyle(&g_music_window, i,
                BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);

    }

   
    for (i = 0; i < 4; i++) {
        id = 3 + i;	
        UG_ButtonCreate(&g_music_window, buttons + id, id, 0,
                40 * i + 40, 239, 40 * i + 80 - 1);
        UG_ButtonSetFont(&g_music_window, id, &FONT_12X20);	
        UG_ButtonSetText(&g_music_window, id, g_music_filename[i]);
        UG_ButtonSetStyle(&g_music_window, id,
                BTN_STYLE_2D | BTN_STYLE_TOGGLE_COLORS | BTN_STYLE_NO_BORDERS);

    }


    if (g_music_filename[0][0] != 0) {
        g_music_filename_selected = 0;

        UG_ButtonSetForeColor(&g_music_window, 3, C_RED);	
        UG_ButtonSetBackColor(&g_music_window, 3, C_GREEN);	
    }	
	
    id = 7;
    UG_ButtonCreate(&g_music_window, buttons + id, id, 0, 0, 239, 39);
    UG_ButtonSetFont(&g_music_window, id, &FONT_12X20);	
    UG_ButtonSetText(&g_music_window, id, "Music");	
    UG_ButtonSetStyle(&g_music_window, id, BTN_STYLE_2D | BTN_STYLE_NO_BORDERS);
}




void music_window_show(void)
{
	UG_WindowShow(&g_music_window);		
		
	UG_WindowSetBackColor(&g_music_window , C_YELLOW);		
}
			
		







void windows_create(void)
{		
	music_window_create();					
		
	music_window_show();					
}	





