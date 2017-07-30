#include "zupper.h"

enum menu_item_type {
	TEXT = 1,
	STR_OPTION = 2,
	NUM_OPTION = 3,
	ACTION_ITEM = 4
};


struct menu_item {
	char *text;
	char opt_wrapper[2];
	enum menu_item_type type;
	char **opt_select;
	int *opt_n_select;
	unsigned int default_index;
	unsigned int opt_len;
	void (*onA)(ZupperMenu *, ZOptionList **);
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
	unsigned int *last_len;
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

unsigned int strlength(char *s) {
	unsigned int i = 0;
	while (1) {
		if (s[i] == '\0') {
			//printf("Strlength of %s: %i\n", s, i);
			return i;
		}
		i++;
	}
}

char *multiplyChar(char ch, int n_times) {
	if (n_times <= 0) {
		return " \0";
	}
	else {
		char *res = (char *) malloc(sizeof(char) * (n_times + 1));
		int i;
		for (i = 0; i < n_times; i++) {
			res[i] = ch;
		}
		res[n_times] = '\0';
		return res;
	}
}

unsigned int integerLength(int n) {
	double res = 1;
	unsigned int i = 1;
	unsigned int l;
	if (n < 0) {
		n = n * -1;
	}
	if (n < 10) {
		return 1;
	}
	else {
		l = 10;
		while (res >= 1) {
			i++;
			l *= 10;
			res = n / l;
		}
		return i;
	}
}

void initCurrZMenuSystem(ZupperMenu *zppm) {
	if (zppm) {
		if (zppm->curr_menu->curr_zmenu->s_n_items > 0) {
			unsigned int i;
			for (i = 0; i < MENU_ROWS; i++) {
				if (zppm->curr_menu->curr_zmenu->init_items[i]) {
					if (zppm->curr_menu->curr_zmenu->items[i]->item) {
						if (zppm->curr_menu->curr_zmenu->items[i]->item->type != TEXT) {
							zppm->curr_menu->select_y = i;
							break;
						}
					}
				}
			}
			for (i = 0; i < MENU_ROWS; i++) {
				if (zppm->curr_menu->curr_zmenu->init_items[i]) {
					if (zppm->curr_menu->curr_zmenu->items[i]->item->type == STR_OPTION ||
						zppm->curr_menu->curr_zmenu->items[i]->item->type == NUM_OPTION) {
						zppm->curr_menu->curr_selection[i] =
							zppm->curr_menu->curr_zmenu->items[i]->item->default_index;
						if (zppm->curr_menu->curr_zmenu->items[i]->item->type == STR_OPTION) {
							zppm->curr_menu->curr_zmenu->last_len[i] =
								strlength(zppm->curr_menu->curr_zmenu->items[i]->
									item->opt_select[zppm->curr_menu->curr_zmenu->items[i]->item->default_index]);
						}
						else {
							zppm->curr_menu->curr_zmenu->last_len[i] =
								integerLength(zppm->curr_menu->curr_zmenu->items[i]->item->
									opt_n_select[zppm->curr_menu->curr_zmenu->items[i]->item->default_index]);
						}
					}


				}
			}
		}
	}

}

void printZMenuOptions(ZupperMenu *zppm) {
	int i;
	struct menu_item_onmenu *m_item;
	for (i = 0; i < MENU_ROWS; i++) {
		m_item = zppm->curr_menu->curr_zmenu->items[i];
		if (zppm->curr_menu->curr_zmenu->init_items[i]) {
			if (m_item->item->type == STR_OPTION) {
				//printf("Last len: %i curr len: %i\n", zppm->curr_menu->curr_zmenu->last_len[i], strlength(m_item->item->opt_select[zppm->curr_menu->curr_selection[i]]));
				char *filler = multiplyChar(' ', zppm->curr_menu->curr_zmenu->last_len[i] -
					strlength(m_item->item->opt_select[zppm->curr_menu->curr_selection[i]]));
				zppm->curr_menu->curr_zmenu->last_len[i] = strlength(m_item->item->opt_select[zppm->curr_menu->curr_selection[i]]);
				printf("\x1b[%i;%iH%c%s%c%s", i + 7, m_item->pos + 1 + strlength(m_item->item->text),
					m_item->item->opt_wrapper[0], m_item->item->opt_select[zppm->curr_menu->curr_selection[i]],
					m_item->item->opt_wrapper[1], filler);
			}

			if (m_item->item->type == NUM_OPTION) {
				//printf("Last len: %i curr len: %i\n", zppm->curr_menu->curr_zmenu->last_len[i], integerLength(m_item->item->opt_n_select[zppm->curr_menu->curr_selection[i]]));
				char *filler = multiplyChar(' ', zppm->curr_menu->curr_zmenu->last_len[i] -
					integerLength(m_item->item->opt_n_select[zppm->curr_menu->curr_selection[i]]));
				zppm->curr_menu->curr_zmenu->last_len[i] = integerLength(m_item->item->opt_n_select[zppm->curr_menu->curr_selection[i]]);
				printf("\x1b[%i;%iH%c%i%c%s", i + 7, m_item->pos + 1 + strlength(m_item->item->text),
					m_item->item->opt_wrapper[0], m_item->item->opt_n_select[zppm->curr_menu->curr_selection[i]],
					m_item->item->opt_wrapper[1], filler);
			}
		}
	}
}
void printZMenuSelection(ZupperMenu *zppm) {
	if (zppm->curr_menu->curr_zmenu->s_n_items == 0) {
		return;
	}
	int i;
	struct menu_item_onmenu *m_item;
	char ss = ' ';
	for (i = 0; i < MENU_ROWS; i++) {
		m_item = zppm->curr_menu->curr_zmenu->items[i];
		if (zppm->curr_menu->curr_zmenu->init_items[i]) {
			if (zppm->curr_menu->select_y == m_item->ym) {
				ss = zppm->curr_menu->curr_zmenu->selection_symbol;
			}
			else {
				ss = ' ';
			}
			printf("\x1b[%i;%iH%c", i + 7, m_item->pos, ss);
		}

	}
}
void printZMenu(ZupperMenu *zppm) {
	clearMenu();
	unsigned int i;
	struct menu_item_onmenu *m_item;
	if (zppm->curr_menu->curr_zmenu->s_n_items == 0) {
		for (i = 0; i < MENU_ROWS; i++) {
			m_item = zppm->curr_menu->curr_zmenu->items[i];
			if (zppm->curr_menu->curr_zmenu->init_items[i]) {
				printf("\x1b[%i;%iH%s", i + 7, m_item->pos + 1, m_item->item->text);
			}
		}
		return;
	}
	char ss = ' ';
	for (i = 0; i < MENU_ROWS; i++) {
		m_item = zppm->curr_menu->curr_zmenu->items[i];
		if (zppm->curr_menu->curr_zmenu->init_items[i]) {
			if (zppm->curr_menu->select_y == m_item->ym) {
				ss = zppm->curr_menu->curr_zmenu->selection_symbol;
			}
			else {
				ss = ' ';
			}
			if (m_item->item->type == TEXT) {
				printf("\x1b[%i;%iH%s", i + 7, m_item->pos + 1, m_item->item->text);
			}
			if (m_item->item->type == ACTION_ITEM) {
				printf("\x1b[%i;%iH%c", i + 7, m_item->pos, ss);
				printf("\x1b[%i;%iH%s", i + 7, m_item->pos + 1, m_item->item->text);
			}
			if (m_item->item->type == STR_OPTION) {
				printf("\x1b[%i;%iH%c", i + 7, m_item->pos, ss);
				printf("\x1b[%i;%iH%s%c%s%c", i + 7, m_item->pos + 1, m_item->item->text,
					m_item->item->opt_wrapper[0], m_item->item->opt_select[zppm->curr_menu->curr_selection[i]],
					m_item->item->opt_wrapper[1]);
			}

			if (m_item->item->type == NUM_OPTION) {
				printf("\x1b[%i;%iH%c", i + 7, m_item->pos, ss);
				printf("\x1b[%i;%iH%s%c%i%c", i + 7, m_item->pos + 1, m_item->item->text,
					m_item->item->opt_wrapper[0], m_item->item->opt_n_select[zppm->curr_menu->curr_selection[i]],
					m_item->item->opt_wrapper[1]);
			}

		}

	}
}
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

ZMenuItem * createZMenuActionItem(char *txt, void (*handler)(ZupperMenu *, ZOptionList **)) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->type = ACTION_ITEM;
		item->text = txt;
		item->onA = handler;
		return item;
	} else {
		return 0;
	}
}

ZMenuItem *createZMenuStrOptionsItem(char *txt, char **opts, unsigned int opts_length, unsigned int def_index, char opt_wrap[2]) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->text = txt;
		item->opt_len = opts_length;
		if (def_index < opts_length) {
			item->default_index = def_index;
		}
		else {
			return 0;
		}
		item->opt_select = opts;
		item->type = STR_OPTION;
		item->opt_wrapper[0] = opt_wrap[0];
		item->opt_wrapper[1] = opt_wrap[1];
		return item;
	} else {
		return 0;
	}
}

ZMenuItem *createZMenuNumOptionsItem(char *txt, unsigned int min, unsigned int max, unsigned int def_index, char opt_wrap[2]) {
	ZMenuItem *item = (ZMenuItem *) malloc(sizeof(ZMenuItem));
	if(item) {
		item->text = txt;
		item->default_index = def_index;

		if (def_index < (max - min + 1)) {
			item->opt_len = max - min + 1;
		}
		else {
			return 0;
		}
		int *narray = (int *)malloc(sizeof(int) * item->opt_len);
		if (narray) {
			unsigned int i;
			for (i = 0; i < item->opt_len; i++) {
				narray[i] = min + i;
			}
			item->opt_n_select = narray;
			item->opt_wrapper[0] = opt_wrap[0];
			item->opt_wrapper[1] = opt_wrap[1];
			item->type = NUM_OPTION;
			return item;
		}
		else {
			return 0;
		}
	} else {
		return 0;
	}
}

void clearMenu() {
	consoleClear();
}


ZupperMenu *zupperMenu() {
	ZupperMenu *zppm = (ZupperMenu *) malloc(sizeof(ZupperMenu));
	if(zppm) {
		struct menu_curr_system *zmsystem = (struct menu_curr_system *) malloc(sizeof(struct menu_curr_system));
		if (zmsystem) {
			zmsystem->curr_selection = (unsigned int *) malloc(sizeof(unsigned int) * MENU_ROWS);
			zppm->llmenus = linkedListZMenu();
			zppm->curr_menu = zmsystem;
			return zppm;
		}
		else {
			return 0;
		}
	} else {
		return 0;
	}
}

struct menu_struct *llMenuGetLast(struct ll_menu *llm) {
	if(llm->size > 0) {
		return llm->last->val;
	} else {
		return 0;
	}
}

struct menu_struct *llMenuPopLast(struct ll_menu *llm) {
	if(llm->size > 0) {
		//printf("Clearing current menu aka index %i\n", (llm->size - 1));
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
					if (node) {
						return node->val;
					}
					else {
						return 0;
					}
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
			llm->size--;
			return r->val;
		} else {
			int i = 1;
			while(1) {
				if(i == index) {
					//printf("Found pop item at index %i\n", i);
					r = node->next;
					if (node->next->next) {
						node->next = 0;
					}
					else {
						node->next = node->next->next;
					}

					if(index == (llm->size - 1)) {
						llm->last = node;
					}
					llm->size--;
					return r->val;
				}
				node = node->next;
				i++;
			}
		}
	} else {
		return 0;
	}
}


void llMenuClear(struct ll_menu *llm) {
	llm->first = 0;
	llm->last = 0;
	llm->size = 0;
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
unsigned int returnToLastMenu(ZupperMenu *zppm) {
	if (zppm->llmenus->size > 1) {
		llMenuPopLast(zppm->llmenus);
		setZMenu(zppm, llMenuGetLast(zppm->llmenus), 1);
		return 1;
	}
	return 0;
}

	void zupperSTARTK(ZupperMenu *zppm) {
		gfxExit();
		exit(0);
	}
	void zupperAK(ZupperMenu *zppm) {
		if (zppm->curr_menu->curr_zmenu->s_n_items == 0) {
			return;
		}
		if (zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y]->item->type == ACTION_ITEM) {
			struct menu_item_onmenu *mit;
			union option_type **opt_args;
			opt_args = (union option_type **) malloc(sizeof(union option_type *) * MENU_ROWS);
			int i;
			for (i = 0; i < MENU_ROWS; i++) {
				opt_args[i] = (union option_type *) malloc(sizeof(union option_type));
				if (zppm->curr_menu->curr_zmenu->init_items[i]) {
					if (zppm->curr_menu->curr_zmenu->items[i]->item->type == STR_OPTION) {
						opt_args[i]->str = zppm->curr_menu->curr_zmenu->items[i]->item->opt_select[zppm->curr_menu->curr_selection[i]];
					}
					else {
						if (zppm->curr_menu->curr_zmenu->items[i]->item->type == NUM_OPTION) {
							opt_args[i]->num = zppm->curr_menu->curr_zmenu->items[i]->item->opt_n_select[zppm->curr_menu->curr_selection[i]];
						}
						else {
							opt_args[i] = 0;
						}
					}
				}
				else {
					opt_args[i] = 0;
				}
			}
			mit = zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y];
			(*mit->item->onA)(zppm, opt_args);
		}
	}
	void zupperBK(ZupperMenu *zppm) {
		if (returnToLastMenu(zppm)) {
			printZMenu(zppm);
		}
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
								printZMenuSelection(zppm);
								//printf("\x1b[15;10HNow selecting %i\n", i);
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
								printZMenuSelection(zppm);
								//printf("\x1b[15;10HNow selecting %i\n", i);
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
				if(i == (MENU_ROWS - 1)) {
					i = 0;
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
							printZMenuSelection(zppm);
							//printf("\x1b[15;10HNow selecting %i\n", i);
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
							printZMenuSelection(zppm);
							//printf("\x1b[15;10HNow selecting %i\n", i);
							break;
						}
					}
				}
				i++;
			}
		}
	}

void zupperDLEFTK(ZupperMenu *zppm) {
	if (zppm->curr_menu->curr_zmenu->s_n_items == 0) {
		return;
	}
	if (zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y]->item->type == STR_OPTION ||
		zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y]->item->type == NUM_OPTION) {
		unsigned int y = zppm->curr_menu->select_y;
		unsigned int oxpos = zppm->curr_menu->curr_selection[y];
		if (oxpos > 0) {
			zppm->curr_menu->curr_selection[y] = oxpos - 1;
			printZMenuOptions(zppm);
		}
	}
}

void zupperDRIGHTK(ZupperMenu *zppm) {
	if (zppm->curr_menu->curr_zmenu->s_n_items == 0) {
		return;
	}
	if (zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y]->item->type == STR_OPTION ||
		zppm->curr_menu->curr_zmenu->items[zppm->curr_menu->select_y]->item->type == NUM_OPTION) {
	unsigned int y = zppm->curr_menu->select_y;
	unsigned int oxpos = zppm->curr_menu->curr_selection[y];
		if (oxpos < (zppm->curr_menu->curr_zmenu->items[y]->item->opt_len - 1)) {
			zppm->curr_menu->curr_selection[y] = oxpos + 1;
			printZMenuOptions(zppm);
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
		if (kd & KEY_DLEFT) {
			zupperDLEFTK(zppm);
		}
		if (kd & KEY_DRIGHT) {
			zupperDRIGHTK(zppm);
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

void addZMenuItem(ZMenu *zm, ZMenuItem *zitem, unsigned int posy, unsigned int position) {
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
		zm->last_len = (unsigned int *)malloc(sizeof(unsigned int) * MENU_ROWS);
		zm->items = mi;
		zm->infinite_scroll = inf_scroll;
		zm->s_n_items = 0;
		zm->selection_symbol = select_symbol;
		return zm;
	} else {
		return 0;
	}
}
