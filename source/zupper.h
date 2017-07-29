#ifndef ZUPPER_H
#define ZUPPER_H
#define MENU_ROWS 15
#include <3ds.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
union option_type {
	char *str;
	int num;
};

typedef union option_type ZOptionList;
struct menu_item;
struct menu_struct;
struct menu_system;
struct ll_menu;
typedef struct menu_struct ZMenu;
typedef struct menu_item ZMenuItem;
typedef struct menu_system ZupperMenu;
ZMenu *createZMenu(char, unsigned int);
ZupperMenu *zupperMenu();
ZMenuItem *createZMenuTextItem(char *);
ZMenuItem *createZMenuActionItem(char *, void (*)(ZupperMenu *, ZOptionList **));
ZMenuItem *createZMenuNumOptionsItem(char *, unsigned int, unsigned int, unsigned int, char[2]);
ZMenuItem *createZMenuStrOptionsItem(char *, char **, unsigned int, unsigned int, char[2]);
struct ll_menu *linkedListZMenu();
struct menu_struct *llMenuGet(struct ll_menu *, unsigned int);
struct menu_struct *llMenuPop(struct ll_menu *, unsigned int);
void printZMenu(ZupperMenu *);
void addZMenuItem(ZMenu *, ZMenuItem *, unsigned int, unsigned int);
void setZMenu(ZupperMenu *, ZMenu *, unsigned int);
void zupperKeysDown(ZupperMenu *, u32);
void clearMenu();
unsigned int returnToLastMenu(ZupperMenu *zppm);
#endif
