#include "save.h"

#include <libmcrd.h>
#include "../scenes/stage/stage.h"

static const u8 saveIconPalette[32] = 
{
	0x00, 0x00, 0x24, 0xED, 0xAD, 0xFD, 0x84, 0xC8, 0xB6, 0xA5, 0xDF, 0xCA,
    0xFF, 0xFF, 0xC6, 0xBC, 0x08, 0xA5, 0x84, 0x90, 0x1F, 0x80, 0xD6, 0xEE,
    0x0F, 0x80, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00
};

static const u8 saveIconImage[128] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x21, 0x12,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x33, 0x13, 0x21, 0x12, 0x01, 0x00, 0x00,
    0x00, 0x30, 0x43, 0x14, 0x22, 0x12, 0x30, 0x00, 0x00, 0x00, 0x43, 0x45,
    0x22, 0x12, 0x31, 0x00, 0x00, 0x30, 0x43, 0x15, 0x22, 0x26, 0x11, 0x00,
    0x00, 0x13, 0x33, 0x11, 0x22, 0x12, 0x71, 0x03, 0x10, 0x11, 0x13, 0x81,
    0x29, 0x12, 0x81, 0x03, 0x12, 0x33, 0x13, 0x97, 0x99, 0x11, 0x97, 0x03,
    0x33, 0x33, 0x13, 0x97, 0x9A, 0x99, 0x9A, 0x02, 0x00, 0x3B, 0x13, 0x71,
    0x9A, 0xDC, 0x99, 0x02, 0x00, 0x20, 0x31, 0xC4, 0xDC, 0x95, 0x99, 0x00,
    0x00, 0x10, 0x31, 0x54, 0x5D, 0x55, 0x44, 0x00, 0x00, 0x12, 0x33, 0x43,
    0x44, 0x44, 0x04, 0x00, 0x00, 0x31, 0x03, 0x30, 0x33, 0x33, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static void toShiftJIS(u8 *buffer, const char *text)
{
    int pos = 0;
    for (u32 i = 0; i < strlen(text); i++) 
    {
        u8 c = text[i];
        if (c >= '0' && c <= '9') { buffer[pos++] = 0x82; buffer[pos++] = 0x4F + c - '0'; }
        else if (c >= 'A' && c <= 'Z') { buffer[pos++] = 0x82; buffer[pos++] = 0x60 + c - 'A'; }
        else if (c >= 'a' && c <= 'z') { buffer[pos++] = 0x82; buffer[pos++] = 0x81 + c - 'a'; }
        else if (c == '(') { buffer[pos++] = 0x81; buffer[pos++] = 0x69; }
        else if (c == ')') { buffer[pos++] = 0x81; buffer[pos++] = 0x6A; }
        else /* space */ { buffer[pos++] = 0x81; buffer[pos++] = 0x40; }
    }
}

static void initSaveFile(SaveFile *file, const char *name) 
{
	file->id = 0x4353;
 	file->iconDisplayFlag = 0x11;
 	file->iconBlockNum = 1;
  	toShiftJIS(file->title, name);
 	memcpy(file->iconPalette, saveIconPalette, 32);
 	memcpy(file->iconImage, saveIconImage, 128);
}

void defaultSettings()
{
	stage.prefs.hidehud = false;
	stage.prefs.timebar = true;
	stage.prefs.camerazooms = true;
	stage.prefs.combostack = true;
	
	stage.prefs.mode = 0;
	stage.prefs.downscroll = true;
	stage.prefs.middlescroll = false;
	stage.prefs.ghost = true;
	stage.prefs.botplay = false;
	
	stage.prefs.control_keys[0] = PAD_SQUARE;
	stage.prefs.control_keys[1] = PAD_CROSS;
	stage.prefs.control_keys[2] = PAD_TRIANGLE;
	stage.prefs.control_keys[3] = PAD_CIRCLE;
}

boolean readSaveFile()
{
	int fd = open("bu00:BASCUS-00000funkinlucky", 0x0001);
	if (fd < 0) // file doesnt exist 
		return false;

	SaveFile file;
	if (read(fd, (void *) &file, sizeof(SaveFile)) == sizeof(SaveFile)) 
		printf("ok\n");
	else {
		printf("read error\n");
		return false;
	}
	memcpy((void *) &stage.prefs, (const void *) file.saveData, sizeof(stage.prefs));
	close(fd);
	return true;
}

void writeSaveFile()
{	
	int fd = open("bu00:BASCUS-00000funkinlucky", 0x0002);

	if (fd < 0) // if save doesnt exist make one
		fd =  open("bu00:BASCUS-00000funkinlucky", 0x0202 | (1 << 16));

	SaveFile file;
	initSaveFile(&file, "PSXF Lucky's Engine");
  	memcpy((void *) file.saveData, (const void *) &stage.prefs, sizeof(stage.prefs));
	
	if (fd >= 0) {
	  	if (write(fd, (void *) &file, sizeof(SaveFile)) == sizeof(SaveFile)) 
	  		printf("ok\n");
	 	else 
	 		printf("write error\n");  // if save doesnt exist do a error
		close(fd);
	} 
	else 
		printf("open error %d\n", fd);  // failed to save
}