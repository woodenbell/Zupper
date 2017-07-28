#include "zupper.h"


enum menu_item_type {
	TEXT = 1,
	STR_OPTION = 2,
	NUM_OPTION = 3,
	SIMPLE_ITEM = 4
};


struct menu_item {
	char *text;
	enum menu_item_type type;
	char **opt_select;
	int *opt_n_select;
	unsigned int default_index;
	void (*onA)(char *, int);
};

struct menu_item_onmenu {
	unsigned int ym;
	unsigned int pos;
	struct menu_item *item;
};

struct ll_menu_node {
	struct menu_struct *val;
	struct ll_menu_node *next;
};

struct ll_menu {
	struct ll_menu_node *first;
	struct ll_menu_node *last;
	unsigned int size;
};

struct menu_struct {
	struct menu_item_onmenu **items;
	unsigned short *init_items;
	unsigned int s_n_items;
	char selection_symbol;
	unsigned int infinite_scroll:1;
};

struct menu_curr_system {
		struct menu_struct *curr_zmenu;
		unsigned int *curr_selection;
		unsigned int select_y;
};
struct menu_system {
	struct ll_menu *llmenus;
	struct menu_curr_system *curr_menu;
};

ZMenuItem *createZMenuTextItem(char *txt) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->type = TEXT;
		item->text = txt;
		return item;
	} else {
		return 0;
	}
}

ZMenuItem *createZMenuSelectItem(char *txt, void (*handler)(char *, int)) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->type = SIMPLE_ITEM;
		item->text = txt;
		item->onA = handler;
		return item;
	} else {
		return 0;
	}
}

//=================
ZMenuItem *createZMenuOptionsItem(char *txt, char **opts, unsigned int def_index) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->text = txt;
		return item;
	} else {
		return 0;
	}
}

//=============
ZMenuItem *createZMenuNumOptionsItem(char *txt, unsigned int min, unsigned int max) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->text = txt;
		return item;
	} else {
		return 0;
	}
}

void clearMenu() {
	int i;
	for(i = 0; i < MENU_ROWS; i++) {
		consoleClear();
	}
}


ZupperMenu *zupperMenu() {
	ZupperMenu *zppm = (ZupperMenu *) malloc(sizeof(ZupperMenu));
	if(zppm) {
		zppm->llmenus = linkedListZMenu();
		zppm->curr_menu = (struct menu_curr_system *) malloc(sizeof(struct menu_curr_system));
		return zppm;
	} else {
		return 0;
	}
}

struct menu_struct *llMenuGetLast(struct ll_menu *llm) {
	if(llm->size > 0) {
		return llMenuGet(llm, (llm->size - 1));
	} else {
		return 0;
	}
}

struct menu_struct *llMenuPopLast(struct ll_menu *llm) {
	if(llm->size > 0) {
		return llMenuPop(llm, (llm->size - 1));
	} else {
		return 0;
	}
}

struct menu_struct *llMenuGet(struct ll_menu *llm, unsigned int index) {
	if(index < llm->size) {
		struct ll_menu_node *node;
		if(llm->size == 1) {
			return llm->first->val;
		}
		if(index == 0) {
			return llm->first->val;
		} else {
			node = llm->first->next;
			int i = 1;
			while(1) {
				if(i == index) {
					return node->val;
					}
				}
				node = node->next;
				i++;
			}
	} else {
		return 0;
	}
}

struct menu_struct *llMenuPop(struct ll_menu *llm, unsigned int index) {
	if(index < llm->size) {
		struct ll_menu_node *node;
		struct ll_menu_node *r;
		if(llm->size == 1) {
			r = llm->first;
			llm->first = 0;
			llm->last = 0;
			llm->size--;
			return r->val;
		}
		node = llm->first;
		if(index == 0) {
			r = llm-> first;
			llm->first = node->next;
			return r->val;
		} else {
			int i = 1;
			while(1) {
				if(i == index) {
					r = node->next;
					node->next = node->next->next;
					if(index == (llm->size - 1)) {
						llm->last = node;
					}
					return r->val;
				}
				node = node->next;
				i++;
			}
		}
		llm->size--;
	} else {
		return 0;
	}
}


void llMenuClear(struct ll_menu *llm) {
	while(llm->size > 0) {
		llMenuPopLast(llm);
	}
}

void llMenuAppend(struct ll_menu *llm, struct menu_struct *value) {
	struct ll_menu_node *node = (struct ll_menu_node *) malloc(sizeof(struct ll_menu_node));
	if(node) {
		node->next = 0;
		node->val = value;
		if(llm->size > 0) {
			llm->last->next = node;
			llm->last = node;
		} else {
			llm->first = node;
			llm->last = node;
		}
		llm->size++;
		}
	}
void llMenuSetLast(struct ll_menu *llm, struct menu_struct *value) {
	struct ll_menu_node *node = (struct ll_menu_node *) malloc(sizeof(struct ll_menu_node));
	if(node) {
		if(llMenuPopLast(llm)) {
			llMenuAppend(llm, value);
		} else {
			llm->first = node;
			llm->last = node;
			llm->size++;
		}
	}
}

struct ll_menu *linkedListZMenu() {
	struct ll_menu *llm = (struct ll_menu *) malloc(sizeof(struct ll_menu));
	if(llm) {
		llm->first = 0;
		llm->last = 0;
		llm->size = 0;
		return llm;
	} else {
		return 0;
	}
}
void goBackToLastMenu(ZupperMenu *zppm) {
	if (zppm->llmenus->size > 1) {
		llMenuPopLast(zppm->llmenus);
		setZMenu(zppm, llMenuGetLast(zppm->llmenus), 1);
	}
}
void initCurrZMenuSystem(ZupperMenu *zppm) {
	if(zppm) {
		if(zppm->curr_menu->curr_zmenu->s_n_items > 0) {
			int i;
			for(i = 0; i < MENU_ROWS; i++) {
				if(zppm->curr_menu->curr_zmenu->init_items[i]) {
					if (zppm->curr_menu->curr_zmenu->items[i]->item) {
						if (zppm->curr_menu->curr_zmenu->items[i]->item->type != TEXT) {
							zppm->curr_menu->select_y = i;
							break;
						}
					}
				}
			}
			for(i = 0; i < MENU_ROWS; i++) {
					if(zppm->curr_menu->curr_zmenu->items[i]) {
					/*
					*		if(zppm->curr_menu->curr_zmenu->items[i]->item->type == STR_OPTION ||
					*				zppm->curr_menu->curr_zmenu->items[i]->item->type == NUM_OPTION) {
					*			zppm->curr_menu->curr_selection[i] =
					*					zppm->curr_menu->curr_zmenu->items[i]->item->default_index;
					*	}
					*
					*/
					}
				}
	}
}

}
	void zupperSTARTK(ZupperMenu *zppm) {
		exit(0);
	}
	void zupperAK(ZupperMenu *zppm) {
		struct menu_item_onmenu *mit;
		mit = zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y];
		char *copt_arg = 0;
		int iopt_arg = 0;
		if(mit->item->type == STR_OPTION) {
			copt_arg = mit->item->opt_select[zppm->curr_menu->curr_selection[zppm->curr_menu->select_y]];
		}

		if(mit->item->type == NUM_OPTION) {
			iopt_arg =  mit->item->opt_n_select[zppm->curr_menu->curr_selection[zppm->curr_menu->select_y]];
		}
		(*mit->item->onA)(copt_arg, iopt_arg);
	}
	void zupperBK(ZupperMenu *zppm) {
		goBackToLastMenu(zppm);
	}
	void zupperDUPK(ZupperMenu *zppm) {
			if(zppm->curr_menu->curr_zmenu->s_n_items <= 1) {
				return;
			}
			int i = zppm->curr_menu->select_y;
			if(zppm->curr_menu->curr_zmenu->infinite_scroll) {
				struct menu_item_onmenu *curr_item;
				while(1) {
					if(i < 0) {
						i = MENU_ROWS - 1;
					} else {
						if (zppm->curr_menu->curr_zmenu->init_items[i]) {
							curr_item = zppm->curr_menu->curr_zmenu->items[i];
						}
						else {
							i--;
							continue;
						}
						if(curr_item) {
							if(curr_item->item->type != TEXT && i != zppm->curr_menu->select_y) {
								zppm->curr_menu->select_y = i;
								printZMenu(zppm);
								printf("\x1b[15;10HNow selecting %i\n", i);
								break;
							}
						}
					}
					i--;
				}
			} else {
				struct menu_item_onmenu *curr_item;
				while(1) {
					if(i< 0) {
						return;
					} else {
						if(zppm->curr_menu->curr_zmenu->init_items[i]) {
							curr_item = zppm->curr_menu->curr_zmenu->items[i];
						}
			else {
				i--;
				continue;
			}
						if(curr_item) {
							if(curr_item->item->type != TEXT && i != zppm->curr_menu->select_y) {
								zppm->curr_menu->select_y = i;
								printZMenu(zppm);
								printf("\x1b[15;10HNow selecting %i\n", i);
								break;
							}
						}
					}
					i--;
				}
			}
		}
	void zupperDDOWNK(ZupperMenu *zppm) {
		if(zppm->curr_menu->curr_zmenu->s_n_items <= 1) {
			return;
		}
		int i = zppm->curr_menu->select_y;
		if(zppm->curr_menu->curr_zmenu->infinite_scroll) {
			struct menu_item_onmenu *curr_item;
			while(1) {
				if(i == MENU_ROWS) {
				} else {
					if (zppm->curr_menu->curr_zmenu->init_items[i]) {
						curr_item = zppm->curr_menu->curr_zmenu->items[i];
					} else {
						i++;
						continue;
					}
					if(curr_item) {
						if(curr_item->item->type != TEXT && i != zppm->curr_menu->select_y) {
							zppm->curr_menu->select_y = i;
							printZMenu(zppm);
							printf("\x1b[15;10HNow selecting %i\n", i);
							break;
						}
					}
				}
				i++;
			}
		} else {
			struct menu_item_onmenu *curr_item;
			while(1) {
				if(i == MENU_ROWS) {
					return;
				} else {
					if (zppm->curr_menu->curr_zmenu->init_items[i]) {
						curr_item = zppm->curr_menu->curr_zmenu->items[i];
					}
					else {
						i++;
						continue;
					}
					if(curr_item) {
						if(curr_item->item->type != TEXT && i != zppm->curr_menu->select_y) {
							zppm->curr_menu->select_y = i;
							printZMenu(zppm);
							printf("\x1b[15;10HNow selecting %i\n", i);
							break;
						}
					}
				}
				i++;
			}
		}
	}

void zupperKeysDown(ZupperMenu *zppm, u32 kd) {
		if (kd & KEY_START) {
			zupperSTARTK(zppm);
		}
		if (kd & KEY_A) {
			zupperAK(zppm);
		}
		if (kd & KEY_B) {
			zupperBK(zppm);
		}
		if (kd & KEY_DUP) {
			zupperDUPK(zppm);
		}
		if (kd & KEY_DDOWN) {
			zupperDDOWNK(zppm);
		}
	}

void printZMenu(ZupperMenu *zppm) {
				clearMenu();
				int i;
				struct menu_item_onmenu *m_item;
				char ss = ' ';
				for(i = 0; i < MENU_ROWS; i++) {
						m_item = zppm->curr_menu->curr_zmenu->items[i];
						if(zppm->curr_menu->curr_zmenu->init_items[i]) {
							if(zppm->curr_menu->select_y == m_item->ym) {
								ss = zppm->curr_menu->curr_zmenu->selection_symbol;
							} else {
								ss = ' ';
							}
							if(m_item->item->type == TEXT) {
								printf("\x1b[%i;%iH%c", i + 7, m_item->pos, ss);
								printf("\x1b[%i;%iH%s", i + 7, m_item->pos + 1, m_item->item->text);
							}
							if(m_item->item->type == SIMPLE_ITEM) {
								printf("\x1b[%i;%iH%c", i + 7, m_item->pos, ss);
								printf("\x1b[%i;%iH%s", i + 7, m_item->pos + 1, m_item->item->text);
							}
							/*
							if(m_item->item->type == STR_OPTION) {

							}

							if(m_item->item->type == NUM_OPTION) {

							}
							*/

						}

				}
}

void setZMenu(ZupperMenu *zppm, ZMenu *zm, unsigned int maintain_last) {
	if(zppm) {
		if(zm) {
			if(maintain_last == 0) {
				llMenuClear(zppm->llmenus);
			}
				llMenuAppend(zppm->llmenus, zm);
				zppm->curr_menu->curr_zmenu = zm;
				initCurrZMenuSystem(zppm);
		}
	}
}

void addZItem(ZMenu *zm, ZMenuItem *zitem, unsigned int posy, unsigned int position) {
	if(posy < MENU_ROWS && zitem && zm) {
		zm->init_items[posy] = 1;
		zm->items[posy] = (struct menu_item_onmenu *) malloc(sizeof(struct menu_item_onmenu));
		zm->items[posy]->item = (struct menu_item *) malloc(sizeof(struct menu_item));
		zm->items[posy]->item = zitem;
		zm->items[posy]->ym = posy;
		zm->items[posy]->pos = position;
		if(zitem->type != TEXT) {
			zm->s_n_items++;
		}
	}
}

ZMenu *createZMenu(char select_symbol, unsigned int inf_scroll) {
	ZMenu *zm;
	zm = (ZMenu *) malloc(sizeof(ZMenu));
	if(zm) {
	    struct menu_item_onmenu **mi =
	    		(struct menu_item_onmenu **) malloc(sizeof(struct menu_item_onmenu *) * MENU_ROWS);
		zm->init_items = (unsigned short *)malloc(sizeof(unsigned short) * MENU_ROWS);
		int i;
		for (i = 0; i < MENU_ROWS; i++) {
			zm->init_items[i] = 0;
		}
		zm->items = mi;
		zm->infinite_scroll = inf_scroll;
		zm->s_n_items = 0;
		zm->selection_symbol = select_symbol;
		return zm;
	} else {
		return 0;
	}
}
