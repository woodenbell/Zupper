# Zupper   
![Zupper Icon](https://woodenbell.github.io/static/images/Zupper.png)  
*A simple c lib for building menus in 3DS Homebrew*

## Getting started
*The code below will explain the basic use of Zupper while the documentation is not ready yet*

    #include "zupper.h"
    
    
    //Declaring the functions to be called when an item is chosen
    
    //This method will be called when the user presses A on with the message item selected (explained below)
     
    void handle_msg(ZupperMenu *zppm, ZOptionList **o) {
        
    /* The first argument is a reference to the ZupperMenu (the structure which controls
    * the menu system).
    * The second one is an array containing the selected value in each OPTION item
    * in the menu (the index is the same as the one used to add the OPTION item 
    * with addZMenuItem() method which appears later)
    */
    
	   printf("\x1b[22;5H                                         \n");
	   printf("\x1b[22;5HMESSAGE: %s  %i\n", o[6]->str, o[5]->num);
    }

    // Called when Exit is chosen
    void handle_exit(ZupperMenu *zppm, ZOptionList **o) {
	    gfxExit();
	    exit(0);
    }

    int main(int argc, char **argv)
    {
    
        //Using ctrulib
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	    
	// Creates ZupperMenu instance
	    
	ZupperMenu *zpmenu;
    	zpmenu = zupperMenu();
    	
    	// Creates a menu (the only one used in this example)
    	
    	ZMenu *zmenu;
    	
    	/* '>' will be the selecion symbol
    	* The value 1 means that the menu will use infinite scrolling (when going down on the last
    	* item, the first item will be selected and vice-versa)
    	*/
    	
    	zmenu = createZMenu('>', 1);
    	
    	// Creates the items which will be used in the menu
    	
    	ZMenuItem *title, *msg, *exit_option, *msg_num, *msg_txt;
    	
    	
    	// Initializes each one of the items
    	
    	// Text items are not selectable, and will be used as the title of the menu
    	
    	title = createZMenuTextItem("Zupper menu");
    	
    	
    	// Action items are selectable and will call (in this case) the method handle_msg() when chosen
    	
    	msg = createZMenuActionItem("Message", handle_msg);
    	
    	
    	// Just like msg, but will cause the application to exit by calling handle_exit()
    	
    	exit_option = createZMenuActionItem("Exit", handle_exit);
    	
    	
    	// Creates the option wrapper symbols for the OPTION items values
    	// Examples:
	// {'(', ')'}  => (SelectedOption)
    	// {'%', '%'}  => %SelectedOption%
    	
    	char msg_num_optwrap[2] = { '<', '>' };
    	char msg_str_optwrap[2] = { ':', ':' };
    	
    	// Creates the string options for the first item
    	
    	char *msg_str_opt1 = "Hello";
    	char *msg_str_opt2 = "Hey";
    	char *msg_str_opt3 = "Hi";
    	
    	// Creates an array for the string options
    	char **msg_str_opts = (char **) malloc(sizeof(char *) * 3);
    	msg_str_opts[0] = msg_str_opt1;
    	msg_str_opts[1] = msg_str_opt2;
    	msg_str_opts[2] = msg_str_opt3;
    	
    	// Creates a STR_OPTION item, which will have the three options above
    	// 3 is the number of options (compiler doesn't know the size of a pointer)
    	// 1 is the index of the default option -> In this case, "Hey"
    	// msg_str_optwrap is the array containing the 2 option wrappers, which are optional
    	
    	msg_txt = createZMenuStrOptionsItem("Message: ", msg_str_opts, 3, 1, msg_str_optwrap);
    	
    	// Creates a NUM_OPTION item, which will have numbers from 1 to 15 as options
    	// 1 and 15 are the min and max of options (number of options is not needed, since a range is supplied)
    	// 9 is the index of the default number -> In this case, 10
    	
    	msg_num = createZMenuNumOptionsItem("Number: ", 1, 15, 9, msg_num_optwrap);
    	
    	// Adds the items to the menu
    	
    	/* 0 is the index of the item, and will determine its order in the menu
    	* Example: index 2 will make the item go on line 2
    	* Also, if the item is an OPTION item, when the handler method is called,
	* the selected option in each item can be accessed through an array
	* containing each of those values on the index of their respective items
	* Example: if there is an option item in index 6 on the menu, the option
	* selected on it will be available on the index 6 of the array passed as argument.
    	*/
	
	// Adds the title text
    	addZMenuItem(zmenu, title, 0, 6);
    	
    	// Adds the other items just like the one above
    	addZMenuItem(zmenu, msg, 2, 7);
    	addZMenuItem(zmenu, exit_option, 3, 9);
    	addZMenuItem(zmenu, msg_num, 5, 9);
    	addZMenuItem(zmenu, msg_txt, 6, 9);
    	
    	// Sets the menu as current menu on ZupperMenu (zpmenu)
	
    	/* 0 - Means that the menu will not keep previous menus if they exist.
    	* This means that when the user press B or returnToLastMenu() is called,
    	* nothing will happen
    	* 1 - Will keep previous menus in order to be able to return to them
    	* 2 - Internal use, when a previous menu is set as current one, neither clearing nor
	* adding a new menu to the stack (used to return to previous menus).
    	*/
    	
    	setZMenu(zpmenu, zmenu, 0);
    	
    	//Prints the menu for the first time
    	
    	/* Note that the menu will be reprinted only when necessary,
    	* increasing performance and avoiding the screen blinks effect of multiple
	* ConsoleClear() calls
	*/
    	
    	/* For instance: 
	* - When the selection changes, only the symbols are cleared and
    	* printed again.
    	* - When the option selection changes, only the options will be cleared and reprinted.
    	* - When a new menu is called or the user returns to a menu, printZMenu() is called
    	* to fully print the menu
    	*/
    	
    	printZMenu(zpmenu);
    	
	
	// Application loop
	while (aptMainLoop())
	{
	    hidScanInput();
	    u32 kDown = hidKeysDown();
	    	
	    //Processes the key inputs
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
	    	
	    // ...
	    gfxFlushBuffers();
	    gfxSwapBuffers();
	    gspWaitForVBlank();
    	}

    	gfxExit();
    	return 0;
    }


