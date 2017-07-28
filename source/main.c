#include <stdlib.h>
#include <3ds.h>
#include "zupper.h"

void handle_msg(char *copt, int iopt) {
	printf("\x1b[21;16H MESSAGE");
}

void handle_exit(char *copt, int iopt) {
	gfxExit();
	exit(0);
}

int main(int argc, char **argv)
{
	gfxInitDefault();

	consoleInit(GFX_TOP, NULL);
	
	ZupperMenu *zpmenu;
	zpmenu = zupperMenu();
	ZMenu *zmenu;
	zmenu = createZMenu('>', 1);
	ZMenuItem *title, *msg, *exit_option;
	title = createZMenuTextItem("Zupper menu");
	msg = createZMenuSelectItem("Message", handle_msg);
	exit_option = createZMenuSelectItem("Exit", handle_exit);
	addZItem(zmenu, title, 0, 6);
	addZItem(zmenu, msg, 2, 7);
	addZItem(zmenu, exit_option, 3, 9);
	setZMenu(zpmenu, zmenu, 0);
	printZMenu(zpmenu);
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		zupperKeysDown(zpmenu, kDown);
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
