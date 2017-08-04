# Zupper ![Zupper Icon](https://woodenbell.github.io/static/images/Zupper.png)  
*A simple c lib for building menus in 3DS Homebrew*

## Getting started
*The code below will explain the basic use of Zupper while the documentation is not ready yet*

    #include "zupper.h"
    
    
    //Declaring the functions to be called when an item is chosen
    
     //Will be called when user press A on the Message item (explained below)
     
    void handle_msg(ZupperMenu *zppm, ZOptionList **o) {
        
     /* The first argument is a reference to the ZupperMenu (the strcuture wich controls
     * the menu system).
    * The second one is an array containing the selected value in each OPTION item
    * in the menu (the index is the same as the used to add the OPTION item 
    * with addZMenuItem() (used below)
    */
    
	   printf("\x1b[22;5H                                         \n");
	   printf("\x1b[22;5HMESSAGE: %s  %i\n", o[6]->str, o[5]->num);
    }

    //Called when Exit is chosen
    void handle_exit(ZupperMenu *zppm, ZOptionList **o) {
	    gfxExit();
	    exit(0);
    }

    int main(int argc, char **argv)
    {
    
        //Using ctrulib
	    gfxInitDefault();
	    consoleInit(GFX_TOP, NULL);
	    
	    //Creates ZupperMenu (only 1 is needed)
	    
	    ZupperMenu *zpmenu;
    	zpmenu = zupperMenu();
    	
    	//Creates a menu (the only used in this example)
    	
    	ZMenu *zmenu;
    	
    	/* '>' will be the selecion symbol
    	* The 1 means the menu will use infinite scrolling (when going down on the last
    	* item, the first item will be selected and vice versa)
    	*/
    	
    	zmenu = createZMenu('>', 1);
    	
    	//Creates the items that will be used in the menu
    	
    	ZMenuItem *title, *msg, *exit_option, *msg_num, *msg_txt;
    	
    	
    	//Initialize each of the items
    	
    	//Text item is not selectable, will be used as the title of the menu
    	
    	title = createZMenuTextItem("Zupper menu");
    	
    	
    	//Action item is selectable and will call the method handle_msg() when chosen
    	
    	msg = createZMenuActionItem("Message", handle_msg);
    	
    	
    	//Just like msg, but will cause the application to exit by calling handle_exit()
    	
    	exit_option = createZMenuActionItem("Exit", handle_exit);
    	
    	
    	//Creates the option wrappers for the OPTION items values
    	//Example {'(', ')'}  =>    (SimpleOption)
    	// {'%', '%'} =>  %33%
    	
    	char msg_num_optwrap[2] = { '<', '>' };
    	char msg_str_optwrap[2] = { ':', ':' };
    	
    	//Creates the string options for the first item
    	
    	char *msg_str_opt1 = "Hello";
    	char *msg_str_opt2 = "Hey";
    	char *msg_str_opt3 = "Hi";
    	
    	//Creates an array to the string options
    	char **msg_str_opts = (char **)malloc(sizeof(char *) * 3);
    	msg_str_opts[0] = msg_str_opt1;
    	msg_str_opts[1] = msg_str_opt2;
    	msg_str_opts[2] = msg_str_opt3;
    	
    	//Creates a STR_OPTION item, which will have the three options above
    	// 3 is the number of options (compiler doesn't know the size of a pointer)
    	// 1 is the index of the default option -> "Hey"
    	// msg_str_optwrap is the array containing the 2 option wrappers
    	
    	msg_txt = createZMenuStrOptionsItem("Message: ", msg_str_opts, 3, 1, msg_str_optwrap);
    	
    	//Creates a NUM_OPTION item, which will have numbers from 1 to 15 as options
    	// 1 and 15 are the min and max of options (num of options not needed)
    	// 9 is the index of the default number -> 10
    	
    	msg_num = createZMenuNumOptionsItem("Number: ", 1, 15, 9, msg_num_optwrap);
    	
    	//Adds the items to the menu
    	
    	//zmenu is the reference to the menu
    	//title is ZMenuItem 
    	/* 0 is the index of the item, and will determine it's order in the menu
    	* Example: index 2 will make the item go on line 2
    	* Also, if the item is an OPTION item, when handle_msg() is called, the ZOptionList
    	* array argument will contain on index 2 it's value, string or number.
    	*/
    	addZMenuItem(zmenu, title, 0, 6);
    	
    	//Adds the other items just like the one above
    	addZMenuItem(zmenu, msg, 2, 7);
    	addZMenuItem(zmenu, exit_option, 3, 9);
    	addZMenuItem(zmenu, msg_num, 5, 9);
    	addZMenuItem(zmenu, msg_txt, 6, 9);
    	
    	//Sets the menu as current menu on ZupperMenu (zpmenu)
    	/* 0 - Means that the menu will not keep previous menus if they exists.
    	* This means that when the user press B or returnToLastMenu() is called,
    	* nothing will happen
    	* 1 - Will keep previous menus in order to be able to return to them
    	* 2 - Internal use, when a previous menu is set to a current one in order to
    	* neither clear nor add a new menu to the stack.
    	*/
    	
    	setZMenu(zpmenu, zmenu, 0);
    	
    	//Prints the menu for the first time
    	
    	/* Please note that the menu will be reprinted only when necessary,
    	* increasing performance and avoiding the screen blinks from COnsoleClear()
    	
    	/* For example, when the selection changes, only the symbols are cleared and
    	* printed again.
    	* When the option selection changes, only the options will be cleared and reprinted.
    	* When a new menu is called or the user returns to a menu, printZMenu() is called
    	* to fully print the menu
    	*/
    	
    	printZMenu(zpmenu);
    
	    while (aptMainLoop())
	    {
	    	hidScanInput();
	    	u32 kDown = hidKeysDown();
	    	
	    	//Process the keys
	    	
	    	/*
	    	* A -> if current item is ACTION, will call it's function, if not, does nothing
	    	*
	    	* B -> if a previous menu exits, returns to it
	    	*
	    	* START -> exits from the application with code 0
	    	*
	    	* DUP -> Will go to the next selectable item above, and may go to the
	    	* last item if inifinite scroll is enabled.
	    	*
	    	* DDOWN -> same as DUP but goes down.
	    	*
	    	* DLEFT -> Selects the next value on an OPTION item, but will have no
	    	* effect in other items (OPTION items doesn't have inifinite scrolling
	    	*
	    	* DRIGHT -> Same as DLEFT but goes right
	    	*/
	    	
	    	zupperKeysDown(zpmenu, kDown);
	    	
	    	//....
	    	gfxFlushBuffers();
	    	gfxSwapBuffers();
	    	gspWaitForVBlank();
    	}

    	gfxExit();
    	return 0;
    }


