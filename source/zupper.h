/*
* Copyright 2017 WoodenBell
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef ZUPPER_H
#define ZUPPER_H
#define MENU_ROWS 15
#include <3ds.h>
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
