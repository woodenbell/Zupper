#ifndef ZUPPER_H
#define ZUPPER_H
#define MENU_ROWS 15
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>

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
ZMenuItem *createZMenuSelectItem(char *, void (*)(char *, int));
struct ll_menu *linkedListZMenu();
struct menu_struct *llMenuGet(struct ll_menu *, unsigned int);
struct menu_struct *llMenuPop(struct ll_menu *, unsigned int);
void printZMenu(ZupperMenu *);
void addZItem(ZMenu *, ZMenuItem *, unsigned int, unsigned int);
void setZMenu(ZupperMenu *, ZMenu *, unsigned int);
void zupperKeysDown(ZupperMenu *, u32);
#endif
