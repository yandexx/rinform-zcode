!
! WhoWhat.H - Modified from WhatIs.H by A.C. Murie, by David A. Cornelson
!
! Note: I felt I abused it enough so that I would call it another name and add different
!       instructions. WhatIs.H ambiguously answered who and what questions and I wanted
!       them to be answered appropriately. So, I 'fixed' it.
!
! USAGE
!
! Include this file after "parser.h".
!
! Add the attribute 'queryable' to your object to allow for WHO am/is/are or WHAT is/am/are
! queries.
!
! Add the whatisit property with a string or function to describe what the object is.
!
! Add whoisit property to describe who the object is.
!
! Additionally, objects that do not appear in the game may be stored in the object
! 'QueryObjs'. They will be in scope all the time, and may be used for general game
! concepts
!
! As a final option, the actions WhoIs and WhatIs may be used in an objects 'before' code
! to print the whoisit or whatisit text.
!
! COMMENTS...
!
! If you intend to allow the player to change to different player objects using ChangePlayer,
! then make sure you write functions to determine which response is appropriate for whatisit
! and whoisit.
!
! The WhoIs routine check's if the object is animate before answering.
!
! I put smartass comments in for defaults.
!

Attribute queryable;
property  whoisit;
property  whatisit;

[ IsAmAre w; w=NextWord(); if (w=='такое' or 'такой' or 'такая' or 'такие') return 0; return -1; ];
[ WhatIsWhatSub; "Что именно?"; ];
[ QueryTopic; while( NextWordStopped() ~= -1 ) ; return 1; ];
[ WhatisQSub; "Тебе об этом ничего не известно."; ];

[ WhoAmISub;
    if( ZRegion(player.whoisit) == 2 or 3 )
    {
        PrintOrRun(player,whoisit);
        rtrue;
    }
    "Серьёзный вопрос, на который у тебя нет ответа.";
];

[ WhatAmISub;
    if( ZRegion(player.whatisit) == 2 or 3 )
    {
        PrintOrRun(player,whatisit);
        rtrue;
    }
    "Ты скорее всего человек, но кто знает?";
];

[ WhatIsSub;
    if( noun hasnt queryable ) "Тебе об этом ничего не известно.";
    if( ZRegion(noun.whatisit) == 2 or 3 )
    {
        PrintOrRun(noun,whatisit);
        rtrue;
    }
    "Ты не знаешь.";
];

[ WhoIsSub;
  if( noun hasnt animate) "Это не @<<кто@>>, а @<<что@>>.";
    if( noun hasnt queryable ) "Тебе об этом ничего не известно.";
    if( ZRegion(noun.whoisit) == 2 or 3 )
    {
        PrintOrRun(noun,whoisit);
        rtrue;
    }
    "Ты не знаешь.";
];

[ QueryScope;
    if( scope_stage == 1 ) rfalse;
    if( scope_stage == 2 )
    {
        ScopeWithin( QueryObjs );
        rfalse;
    }
];

Object QueryObjs "Queryable objects";

verb meta "кто"
           * IsAmAre                      -> WhatIsWhat
           * "я"                          -> WhoAmI
           * IsAmAre "я"                  -> WhoAmI
           * "я" IsAmAre                  -> WhoAmI
           * "я?"                 -> WhoAmI
           * IsAmAre "я?"                 -> WhoAmI
           * "я" "?"              -> WhoAmI
           * IsAmAre "я" "?"              -> WhoAmI
           * IsAmAre scope=QueryScope     -> WhoIs
           * IsAmAre scope=QueryScope "?" -> WhoIs
           * QueryTopic                   -> WhatisQ;

verb meta "что" 
           * IsAmAre                      -> WhatIsWhat
           * IsAmAre "я"                  -> WhatAmI
           * "я"                  -> WhatAmI
           * IsAmAre "я?"                 -> WhatAmI
           * IsAmAre "я" "?"              -> WhatAmI
           * IsAmAre scope=QueryScope     -> WhatIs
           * IsAmAre scope=QueryScope "?" -> WhatIs
           * QueryTopic                   -> WhatisQ;
