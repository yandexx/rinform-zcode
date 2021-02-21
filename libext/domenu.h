!    DOMENU             A replacement for Inform's standard DoMenu
!  version 6.2         built for Inform 6 by L. Ross Raszewski
!                       (rraszews@skipjack.bluecrab.org)
!
!
!  Requires version 3.1 or greater of the Utility.h library
!  To install:
!  Add the following lines BEFORE the inclusion of the parser header file:
!       Replace DoMenu;
!       Replace LowKey_Menu;
!  Include this library AFTER the inclusion of parser.h and utility.h     
!  New in 6.2:  Fixed a bug that sometimes causes odd behavior when removing 
!               options from a menu. Also revised some of the comments.
!  New in 6.1:  It is now legal for the first option on a menu to be a
!               skipped-option. The menu will behave properly.
!               NOTE::: A menu consisting of ONLY separators is ILLEGAL
!                       So don't do it.
!  new in 6.0:  Once-Mode-Emulation!  Domenu can now create menus in the
!               style of Chris Klimas's "Once"
!               See details below!
!
!  Other features:
!       Custom Menu Marker                                      (v.5)
!       User-defined top-bar tags                               (v.5)
!       V6lib support (Thanks to Jason C. Penny)                (v.5)
!       Complete code revision                                  (v.5)
!       User-Defined MENU_MARKER to indicate current choice     (v.5)
!       Multi-page support                                      (v.4)
!       Multilingual (Inform 6.3+) support                      (v.3)
!       Null-menu objects                                       (v.3)
!       dynamic menu support                                    (v.2)
!       multi-line descriptions                                 (v.1)
!       Second title bar line                                   (v.1)
!       Suppression of terminating <look>                       (v.1)
!       Automatic centering of titles                           (v.1)
!
!  This menu system was designed to support the Altmenu object based
!  menu system.  It does still, and always will support old style domenus.
!
! Differences in usage from standard Domenu:
!   Supression of terminating <look>:
!       to prevent the library from executing a <<look>> command upon exiting
!       the menu, add ,1 to the end of the argument list:
!       DoMenu(menu_choices,entryr,choicer,1);
!   Null-objects:
!       When EntryR is called with a menu_item of -5,
!       the global skipitem will contain the number equivalent to the current 
!       selection.  To force a skip of that selection, make EntryR return SKIP:
!               (from EntryR:)
!               if (menu_item==-5 && skipitem==3) return skip;
!               will skip the third option.
!   Multi-line descriptions:
!      When EntryR() is called with menu_item=-1, return the number of lines
!                in the description:
!               (from EntryR:)
!               if (menu_item==-1) return x;
!       where x is the number of lines of description
!    Title Bars:
!       When EntryR() is called with menu_item=-1, set item_name to the
!         title bar string:
!          (from EntryR)
!          if (menu_item==-1) { item_name="Second Line"; return 4;}
!               (where 4 is the number of lines of description)
!
!     Long menus:
!       menu_choices (which must be a function for multipage menus) is now
!       called with a parameter.  This parameter is the number of the menu
!       item to start with.  Descriptive text should only be printed if
!       the parameter is zero:
!       [ Menu_choices doFrom;
!               if (doFrom==0) print "Introductory text";
!               if (doFrom<=1) print "Item 1";
!               if (doFrom<=2) print "item 2";
!               ...
!       ]
!      Inserts the word [More] at the top and bottom to
!      indicate additional selections.
!     Tagarray:
!       to change the default messages displayed on the domenu top
!       banner, pass an array containing replacement tags as the 5th argument:
!       DoMenu(menu_choices,entryr,choicer,0,MY_TAGS);
!       the structure of the tag array is as follows:
!       -->0: top-left tag
!       -->1: top-right tag
!       -->2: bottom-left tag
!       -->3: bottom-right tag: unnested menu
!       -->4: bottom-right tag: nested menu
!       -->5: right-margin offset for top-right tag
!       -->6: right-margin offset for bottom-right tag
!    Custom menu-marker
!       Define MENU_MARKER before inclusion to a single character.
!               ex: Constant MENU_MARKER '*';
!               Replaces the traditional > with a *
!
!  NEW FEATURE --  ONCE-MODE-EMULATION
!  Domenu now uses the global variable "Menu_Mode" to determine the menu style
!  A Menu_Mode of TRADITIONAL activates "Traditional" menus (the default)
!  A menu_mode of ONCE activates full Once-Emulation-Mode.  the menu
!   will dynamically resize to fit the menu, up to the screen height-5
!   lines.  Banner tags are not supported, nor are menu titles.  
! An integer value of Menu_Mode, greater than 1 will create a menu
!  in partial Once-Mode-Emulation, which will resize to a maximum of
!  menu_mode lines.  Multipage is still supported in Once-Mode
!
!
!
! Comments?  e-mail me!
!
! For maximum enjoyment, add on (not required)
!  AltMenu.H   -> An alternative to menus, object oriented menu system
!                  (inspired by Graham Nelson's attempt to do the same
!                   thing.  Mine makes use of the nifty abilities of this
!                   library)
Iffalse UTILITY_LIBRARY>=31;
message error "DoMenu 6.2 requires version 3.1 or greater of the Utility.h library.";
endif;
ifndef DOMENU_LIBRARY;
Constant DOMENU_LIBRARY 62;
! Global Skipitem, used for specifying items to be skipped.
global Skipitem;
! SKIP  The return value for skip objects.
constant SKIP = -55;
! tagarray: holds the text-tags used in the domenu banner
global tagarray;
! DM_TAG is the default tagarray
Array DM_TAG--> NKEY__TX PKEY__TX RKEY__TX QKEY1__TX QKEY2__TX 12 17;
! Language Block
Default MORE__TX "[Далее]";      ! Message displayed to indicate multipage
Default MENU_MARKER '>';        ! Marks the current selection
Constant TRADITIONAL 0;
Constant ONCE 1;
Global Menu_Mode=TRADITIONAL;
! Loykey version of Domenu.  NOTE: skipped options do not work in lowkey
! mode.

[ LowKey_Menu menu_choices EntryR ChoiceR inflag lines main_title i j;
  menu_nesting++;
 .LKRD;
  menu_item=0;
  lines=indirect(EntryR);
  main_title=item_name;
  print "--- "; print (string) main_title; print " ---^";
  menu_item=-1;
  item_name=-1;
  indirect(EntryR);
  if (item_name ofclass string) print (string) item_name;
  if (menu_choices ofclass Routine) menu_choices.call();
  else print (string) menu_choices;
  for (::)
  {   L__M(##Miscellany, 52, lines);
      print "> ";

      #IFV3; read buffer parse;
      #IFNOT; read buffer parse DrawStatusLine;
      #ENDIF;

      i=parse-->1;
      if (i==QUIT1__WD or QUIT2__WD || parse->1==0)
      {   menu_nesting--; if (menu_nesting>0) rfalse;
          if (deadflag==0 && inflag==0) <<Look>>;
          rfalse;
      }
      i=TryNumber(1);
      if (i==0) jump LKRD;
      if (i<1 || i>lines) continue;
      menu_item=i;
      j=indirect(ChoiceR);
      if (j==2) jump LKRD;
      if (j==3) rfalse;
  }
];



#IFV3;
[ DoMenu menu_choices EntryR ChoiceR inflag;
  LowKey_Menu(menu_choices,EntryR,ChoiceR,inflag);
];
#ENDIF;

! Domenu: syntax is the same as always

[ DoMenu Menu_choices EntryR ChoiceR inflag D_tagarray cl;
 #ifdef V6DEFS_H;
  give ActiveZWinStyle ~general;
  StatusWin.HideCursor();
 #Endif;

 if (D_tagarray==0) D_tagarray=DM_TAG;
 menu_nesting++;
 cl=1;
 while(cl~=-1)
 {
  tagarray=D_Tagarray;
  cl=DM_Menu(Menu_choices,EntryR,ChoiceR,cl);

 }
 menu_nesting--;
 if (menu_nesting==0)
 {
  if (Menu_Mode==traditional) @erase_window -1;
  else
  #ifdef V6DEFS_H;
   StatusWin.Erase();
  #ifnot;
   @erase_window 1;
  #endif;
  #ifdef V6DEFS_H;
  MainWin.Activate();
  ActiveZWinStyle.Activate();
  #ifnot;
  font on; @set_cursor 1 1;
  #endif;
  if (deadflag==0 && inflag==0)
  {
   DrawStatusline();
   <<Look>>;
  }
 }
];


! Domenu internal functions: DO NOT CALL SEPARATELY
[ DM_Menu Menu_choices EntryR ChoiceR cl menu_title
          sub_title lines d_lines oldcl offset dofrom i cursor_move height;

 menu_item=0;
 lines=indirect(EntryR);
 if (cl>lines) cl=lines;
 menu_title=item_name;
 item_name=NULL;
 menu_item=-1;
 d_lines=indirect(EntryR);
 sub_title=item_name;
 if (Menu_Mode==TRADITIONAL) height=d_lines+lines+5;
 else height=lines;
 if (Menu_Mode~=ONCE or TRADITIONAL && height>Menu_Mode) height=Menu_Mode;
 if (Menu_Mode==ONCE && height>((0->32)-5)) height=(0->32)-5;
 if (height>(0->32)) height=0->32;
 skipitem=cl;
 menu_item=-5;
 while (indirect(EntryR)==SKIP)
 {
   cl=cl+1;
   if (cl>lines) cl=1;
   else if (cl<1) cl=lines;
   menu_item=-5;
   skipitem=cl;
  }
 oldcl=cl;

 dofrom=DM_CheckDofrom(cl, dofrom, d_lines,height);
 if (Menu_Mode==Traditional)
 {
  if (dofrom==0) offset=d_lines+4;
  else {
         dofrom=dofrom+5;
         offset=5-dofrom;
       }
 }
 else offset=1-dofrom;
 DM_DrawMenu(menu_choices,menu_title,sub_title,lines,d_lines,
             dofrom,height);
 DM_PutCursor(offset,cl,oldcl,height);
 do {
  LocateCursor(0,0);
  @read_char 1 0 0 i;
  cursor_move=0;
  if (i=='n' or 'N' or 'т' or 'Т' or 130) cursor_move=1;
  else if (i=='p' or 'P' or 'з' or 'З' or 129) cursor_move=-1;
  else if (i=='q' or 'Q' or 'й' or 'Й' or 27) return -1;
  else if (i==10 or 13 or 132)
  {
   menu_item=cl;
   indirect(EntryR);
   if (Menu_Mode==traditional)
   {
    @erase_window -1;
    #ifdef V6DEFS_H;
    i = StatusWin.GetCharHeight();
    @split_window i;
    #ifnot;
    @split_window 1;
    #endif;
    i = 0->33; if (i==0) i=80;
    #ifndef V6DEFS_H;
    @set_window 1;
    LocateCursor(1,1);
    style reverse; spaces(i);
    #ifnot;
    StatusWin.Activate();
    LocateCursor(1,1);
    StatusWin.SetColours(MainWin.GetBGColour(),
                         MainWin.GetFGColour());
    StatusWin.Erase();
    #endif;
    CenterU(item_name,1);
   }
   #ifndef V6DEFS_H;
   style roman; @set_window 0;
   #ifnot;
   MainWin.Activate();
   #endif;
   new_line;
   i = indirect(ChoiceR);
   if (i==3) return -1;
   else if (i==2) return cl;
   L__M(##Miscellany,53);
   WaitForKey(" ");
   return cl;
  }
  if (cursor_move~=0)
  {
   do {
    cl=cl+cursor_move;
    if (cl>lines) cl=1;
    else if (cl<1) cl=lines;
    menu_item=-5;
    skipitem=cl;
   } until (indirect(EntryR)~=SKIP);
   i=DM_CheckDofrom(cl, dofrom, d_lines,height);
   if (i~=dofrom)
   {
    dofrom=i;
    DM_DrawMenu(menu_choices,menu_title,sub_title,lines,d_lines,
                dofrom,height);
   }
   if (Menu_Mode==Traditional)
    if (dofrom==0) offset=d_lines+4;
    else offset=5-dofrom;
   else offset=1-dofrom;
   DM_PutCursor(offset,cl,oldcl,height);
   oldcl=cl;
  }
 } until (false);
];

[ DM_CheckDofrom cl dofrom d_lines height offset i;
 if (Menu_Mode~=Traditional && dofrom==0) dofrom=1;
 if (menu_mode==traditional)
  if (dofrom==0) offset=d_lines+4;
  else offset=5-dofrom;
 else offset=1-dofrom;
 if ((offset+cl)>height)
  return DM_CheckDofrom(cl,dofrom+1,d_lines,height);
 if (Menu_Mode==Traditional) i=5; else i=0;
 if ((offset+cl)<=i)
  return DM_CheckDofrom(cl,dofrom-1,d_lines,height);
 return dofrom;
];

[ DM_DrawMenu menu_choices menu_title sub_title lines d_lines dofrom
              height width i j;
 if (Menu_Mode==TRADITIONAL) @erase_window -1;
 else
  #ifdef V6DEFS_H;
   StatusWin.Erase();
  #ifnot;
   @erase_window 1;
  #endif;
 

 #Ifdef V6DEFS_H;
 i=height*StatusWin.GetCharHeight();
 @split_window i;
 StatusWin.Activate();
 StatusWin.SetColours(MainWin.GetFGColour(),
                      MainWin.GetBGColour());
 StatusWin.Erase();
 StatusWin.SetFontStyle(ST_FIXED|ST_REVERSE);
 width = StatusWin.GetXSize() / StatusWin.GetCharWidth();
 #Ifnot;
 @split_window height;
 @set_window 1;
 style reverse;
 width=(0->33); if (width==0) width=80;
 #Endif;
 if (Menu_Mode==TRADITIONAL) j=3;
 else j=height;
 for (i=1:i<=j:i++)
 {
  LocateCursor(i,1);
  spaces(width);
 }
 if (Menu_Mode==TRADITIONAL)
 {
  CenterU(menu_title,1);
  LocateCursor(2,2);
  print (string) tagarray-->0;
  j=width-tagarray-->5;
  LocateCursor(2,j);
  print (string) tagarray-->1;
  LocateCursor(3,2);
  print (string) tagarray-->2;
  j=width-tagarray-->6;
  LocateCursor(3,j);
  if (menu_nesting==1)
   print (string) tagarray-->3;
  else print (string) tagarray-->4;
  if (sub_title ~= NULL)
  {
   style bold;
   CenterU(sub_title,2);
  }
  style roman;
  LocateCursor(5,1);
 }
 else LocateCursor(1,1); 
 if (Menu_Mode==TRADITIONAL)
 {
  j=height-5;
  if (dofrom==0) j=j-d_lines;
 }
 else j=height-1;
 if (menu_choices ofclass String) print (string) menu_choices;
 else indirect(menu_choices,doFrom,j);
 j=width-7;
 if ((dofrom==1 && Menu_Mode==Traditional) ||  dofrom > 1)
 {
  if (Menu_Mode==Traditional) i=d_lines+4;
  else i=1;
  LocateCursor(i,j);
  print (string) MORE__TX;
 }
 if (Menu_Mode==Traditional) i=lines+5-dofrom;
 else i=lines-dofrom;
 if (i>height)
 {
  i=height;
  LocateCursor(i,j);
  print (string) MORE__TX;
 }

];

[ DM_PutCursor offset cl oldcl height i j;
  i=offset+oldcl;
  LocateCursor(i,4);
  if (Menu_Mode==TRADITIONAL) j=5; else j=1;
  if (i>=1 && i<=height)
  print "  ";
  i=offset+cl;
  LocateCursor(i,4);
  print (char) MENU_MARKER;
];
Endif;
