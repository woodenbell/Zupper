#include "zupper.h"

void handle_msg(ZupperMenu *zppm, ZOptionList **o) {
	printf("\x1b[22;5HMESSAGE: %s  %i\n", o[6]->str, o[5]->num);
}

void handle_exit(ZupperMenu *zppm, ZOptionList **o) {
	gfxExit();
	exit(0);
}
void nmenu_btn1(ZupperMenu *zppm, ZOptionList **o) {
	ZMenu *nzm = createZMenu('%', 0);
	ZMenuItem *m_title;
	m_title = createZMenuTextItem("Last menu, no return");
	addZMenuItem(nzm, m_title, 3, 5);
	setZMenu(zppm, nzm, 0);
	printZMenu(zppm);
}
void nmenu_btn2(ZupperMenu *zppm, ZOptionList **o) {
	returnToLastMenu(zppm);
	printZMenu(zppm);
}
void nmenu(ZupperMenu *zppm, ZOptionList **o) {
	ZMenu *nzm = createZMenu('#', 0);
	ZMenuItem *btn1, *btn2;
	btn1 = createZMenuActionItem("New menu", nmenu_btn1);
	btn2 = createZMenuActionItem("Return to main menu", nmenu_btn2);
	addZMenuItem(nzm, btn1, 5, 4);
	addZMenuItem(nzm, btn2, 6, 3);
	setZMenu(zppm, nzm, 1);
	printZMenu(zppm);
}
int main(int argc, char **argv)
{
	gfxInitDefault();

	consoleInit(GFX_TOP, NULL);
	ZupperMenu *zpmenu;
	zpmenu = zupperMenu();
	ZMenu *zmenu;
	zmenu = createZMenu('>', 1);
	ZMenuItem *title, *msg, *exit_option, *msg_num, *msg_txt, *new_menu;
	title = createZMenuTextItem("Zupper menu");
	msg = createZMenuActionItem("Message", handle_msg);
	exit_option = createZMenuActionItem("Exit", handle_exit);
	char msg_num_optwrap[2] = { '<', '>' };
	char msg_str_optwrap[2] = { ':', ':' };
	char *msg_str_opt1 = "Hello";
	char *msg_str_opt2 = "Hey";
	char *msg_str_opt3 = "Hi";
	char **msg_str_opts = (char **)malloc(sizeof(char *) * 3);
	msg_str_opts[0] = msg_str_opt1;
	msg_str_opts[1] = msg_str_opt2;
	msg_str_opts[2] = msg_str_opt3;
	msg_num = createZMenuNumOptionsItem("Number: ", 1, 15, 9, msg_num_optwrap);
	msg_txt = createZMenuStrOptionsItem("Message: ", msg_str_opts, 3, 1, msg_str_optwrap);
	new_menu = createZMenuActionItem("New menu", nmenu);
	addZMenuItem(zmenu, title, 0, 6);
	addZMenuItem(zmenu, msg, 2, 7);
	addZMenuItem(zmenu, exit_option, 3, 9);
	addZMenuItem(zmenu, msg_num, 5, 9);
	addZMenuItem(zmenu, msg_txt, 6, 9);
	addZMenuItem(zmenu, new_menu, 8, 6);
	setZMenu(zpmenu, zmenu, 0);
	//printf("No exceptions ocurred");
	printZMenu(zpmenu);

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
