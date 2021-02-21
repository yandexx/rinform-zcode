!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!       AltMenu                 An Inform 6 Library extension to create 
!  Version 6                    object-oriented menus.  By L. Ross Raszewski
!                               (rraszews@acm.org)
!  Requires DoMenu.h and utility.h
!
!
!  New in this version: Main code rewrite. The low memory "footprint"
!  of altmenu-related objects has been greatly reduced. This _should_
!  be invisible to the programmer, but whatever you do, don't go messing
!  with the Menu_Controller.
!
!  See the NOTES section for more information about this update
!
!    In brief, this is an object oriented front-end to my modified domenu
!    library.  It is designed for use with toggle options and dynamic menus
!    It supports menus over a page long.  It's easy to use and yet
!    greatly versatile.
!
! Usage:
!       To activate a menu, send the select(); message to the appropriate 
!       Menu-Class object.  The menu will automatically manipulate menus
!       longer than one screen.
!       Set the "nolook" attribute to suppress the <<look>> action generated
!       when quitting the menu
!       the short name:
!               if a title bar is given, printed on the second line of the
!               banner, else printed on the first line.
!       the description property:
!               Holds the description test to be printed at the top of the
!               menu.
!       the number property:
!               Holds the number of lines which the description takes up
!               or a function to determine them.  If someone can provide
!               me with a reliable function, which can account for
!               differing screen widths, variable pitch font,
!               and manual new_lines, please send it this way!.
!       the title_bar property:
!               If given, is printed on the top line of the banner
!       the sup_bar property:
!               if given, is printed on the second line of the banner.
!               DO NOT use both a sup_bar and a title_bar.
!       the banner_tags property:
!               if given, this array contains the top bar tags to be used
!               by domenu (see domenu.h for the format of the tagarray
!
!    The children of a menu are the items of that menu. All children
!    of a menu must inherit from class Option.
!       the select property:
!               Performs the task of the option.  Returns 1 to wait for a
!               keypress, 2 to return with no keypress, and 3 to return and
!               exit the current menu. By default, this prints the description
!               of the object.
!       the "tag" property:
!               if given, this is used as the name given in menu itself
!       the label property: if given, this is used as the name placed in the
!               top bar when the option is selected
!
!   This library contains 2 additional varities of option classes:
!       Separator: A menu item which cannot be selected.  Useful for
!               grouping options together within a menu.
!               An option with the "locked" attribute also acts like a
!               separator
!       SwitchOption: A menu item designed to toggle, as in game settings
!          the label property: Printed in the menu.  Should be changed
!                to reflect the setting of the switch
!          (synonymous to tag property.  If both a tag and a label are given
!           preference is given to the tag)
!          the toggle property: Does the work of the switch.  This may be
!                as simple as turning the object from ~on to on, but might
!                involve global variables or otherwise affect the behavior
!                of the game
!
!
!  ONCE-MODE EMULATION: Set the "menu_style" property of the menu object to
!       "TRADITIONAL", "ONCE", or a positive integer to define the menu style.
!       The default value (-1) will cause the menu to be presented in the
!       same style as the _last menu activated_
!
!  NOTES:
!       For adaptive menus, in order to keep items in the same order,
!       use the Pmove function (from utility.h) to preserve lineage.
!
!       Menu objects may not make use of the "door" attribute. I can't see why
!       you would anyway, but if you need to for some reason, define
!       MENUS_USE_DOOR before inclusion (this will cost you an attribute)
!
!       In order to keep the memory footprint low, the old stubs have been
!       removed. If you are not using all of the features of Altmenu, you
!       may recieve an error while compiling, to the effect of "no such
!       constant as ..." attributed to altmenu.h. Define ALTMENU_PROP_STUBS
!       before inclusion to suppress these errors
!
!  This is part of a series of inter-dependant libraries.
!  For this to work, you have to include:
!  DoMenu       -> A modified DoMenu routine to support extended functionality
!  Utility      -> A set of generic utility functions
!
! For maximum enjoyment, add on (not required)
!  Hints.H      -> A system for making object-oriented hints which appear
!                      formatted in the same way as Infocom's InvisiClues
!
System_file;
iffalse DOMENU_LIBRARY>=62;
message error "AltMenu 6.0 requires version 6.2 or greater of the DoMenu library";
endif;
ifndef ALTMENU_LIBRARY;
Constant ALTMENU_LIBRARY 60;
ifndef MENUS_USE_DOOR;
Attribute nolook alias door;
ifnot;
Attribute nolook;
endif;
!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! Option Class (Menu options)
!        select             (calls self.description to perform menu
!                                       function.)
!                               returns: self.description();
!       Usage:
! Option "Option One"
!       with description "Some text here.";
!       Note - description returns 1 to run and wait for a keypress,
!               2 to run and return to the menu with no keypress
!               3 to run and quit the current menu.
!
!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Class Option
        with    !tag [; print (name) self;],
                select [;
                          if (self provides description) 
                           return self.description();
                          else rtrue;
                       ];

!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! Separator Class (Null Menu Item)
!      Usage:
! Separator "Game Commands";
!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Class separator;

!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! SwitchOption Class (Toggle options)
!       with sup_bar (this is only a stub property.  Switchoptions will never 
!                        use a sup_bar.)
!           short_name (prints self.label, if provided.)    
!           select (runs self.toggle();)    
!           title_bar (this is also a stub.)
!
!       Usage:
!  SwitchOption sound
!        with label [; print "Sound     ";
!                       if (self has on) style reverse; print "ON";
!                       style roman; print "   ";
!                       if (self hasnt on) style reverse; print "OFF";
!                       style roman;],
!               toggle [; if (self has on) give self ~on; else give self on;];
!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
Class SwitchOption              
  with    
       tag [; if (self provides label) 
              { self.label(); return 1;}
           ],
       select [; self.toggle(); return 2;];


!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! Menu class (This is the big one.)
!       class option (A menu can also be an option of another menu.)
!       with select (The main menu loop. throws the results back to Domenu.)
!            label (Stub for the option label property.)
!            toggle (Stub for the switchoption toggle property)  
!            emblazon (prints the menu text, self.description, and the 
!               list of children.  This is the first argument sent to Domenu.)
!            titles (generates the option names sent to Domenu.)
!            execute (runs the selected option.  Sent as the third argument 
!                       to Domenu)  
!            selection_name (generates the name needed by self.titles)
!            doname (prints the name of the menu.  A separate property, 
!               because of the form of argument required by Domenu.)
!
!       Usage:
! Menu "Main Menu"
!       with description "This is the main Menu",
!            number 1, ! not strictly required, because in this case it is 1,
!                      !but provided for clarity
!            title_bar "The big game",  !will be printed on the first line 
!                               ! of the menu bar
!            sup_bar "A menu"; !will be printed on the second line.  Use one,
!                               ! or the other, not both.
!
!
!++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Object Menu_Controller ""
  with select [m i j k l;
                l=self.current_menu;
                self.current_menu=m;
                if (self.current_menu has nolook) i=1;
                else i=0;
                k=menu_mode;
                if (self.current_menu provides menu_style && self.current_menu.menu_style~= -1)
                 menu_mode=self.current_menu.menu_style;
                if (menu_mode~=TRADITIONAL && self.current_menu provides description)
                 self.current_menu.description();
                if (self.current_menu provides banner_tags && self.current_menu.banner_tags ~=0)
                 if (self.current_menu.#banner_tags~=2) j=self.current_menu.&banner_tags;
                 else j=self.current_menu.banner_tags;
                DoMenu(self.emblazon,self.titles,self.execute,i,j);
                menu_mode=k;
                self.current_menu=l;
                return 2;
              ],
       current_menu 0,
       emblazon [ doFrom doTo o;
                  if (doFrom==0)
                   {
                    if (self.current_menu provides description)
                     self.current_menu.description();
                    else new_line;
                    dofrom=1;
                   }

                  o=Scion(self.current_menu,dofrom);
                  if (doto==0) doto==children(self.current_menu)+1;
                  while (o ofclass object && doTo>=0)
                   {
                    doTo--;
                    font off;
                    #ifdef V6DEFS_H;
                    StatusWin.SetFontStyle(ST_FIXED);
                    #endif;
                    spaces(5);
                    font on;
                    #ifdef V6DEFS_H;
                    StatusWin.SetFontStyle(ST_ROMAN);
                    #endif;
                    if (Menu_Mode~=Traditional)
                     style reverse;
                    if (o provides tag) o.tag();
                    else print (name) o;
                    new_line;
                    o=sibling(o);
                    if (Menu_Mode~=Traditional)
                     style reverse;

                   }
                   
                ],
       titles [ o;
                if (menu_item==-5)
                {
                  o=Scion(self.current_menu,SkipItem);
                  if (o ofclass separator || o has locked)
                   return SKIP;
                }
                else if (menu_item==-1)
                {
                 if (self.current_menu provides sup_bar)
                  item_name=self.current_menu.sup_bar;
                 if (self.current_menu provides title_bar) 
                  item_name=amtag;
                 if (self.current_menu provides number)
                  return self.current_menu.number;
                 else return 1;
                }
                else if (menu_item==0)
                {
                 if (self.current_menu provides title_bar)
                  item_name=self.current_menu.title_bar;
                 else item_name=amtag;
                 return children(self.current_menu);
                }
                else
                {
                 o=Scion(self.current_menu,menu_item);
                 if (o ofclass object)
                 { item_width=o;
                   item_name=selection_name;
                 }
                }
              ],
       execute [ o j;
                 j=2;
                 o=Scion(self.current_menu,menu_item);
                 if (o ofclass object)
                  j=o.select();
                 return j;
               ];
Class Menu
  with select [; return Menu_Controller.select(self);],
       tag [; print (name) self;];
       
[ amtag; print (name) Menu_controller.current_menu;];
[ selection_name;
                        if (item_width provides label && item_width.label~=NULL)
                         if (item_width.label ofclass string) print (string) item_width.label;
                         else item_width.label();
                        else print (name) item_width;
                      ];
#ifdef ALTMENU_PROP_STUBS;
Object "" with label 0, toggle 0, menu_style 0, banner_tags 0, sup_bar 0, title_bar 0;
#endif;
endif;
