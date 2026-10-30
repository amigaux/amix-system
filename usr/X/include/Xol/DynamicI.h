/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olmisc:DynamicI.h	1.4"
#endif

#ifndef __DynamicI_h__
#define __DynamicI_h__

/*
 *  Description:
 *		This file contains all private tables for Dynamic.c.
 */


static btn_mapping btn_mappings[] = {
   { Button1, Button1Mask },
   { Button2, Button2Mask },
   { Button3, Button3Mask },
   { Button4, Button4Mask },
   { Button5, Button5Mask },
};

static mapping mappings[] = {
   { "None",       None          },
   { "Button1",    Button1       },
   { "Button2",    Button2       },
   { "Button3",    Button3       },
   { "Button4",    Button4       },
   { "Button5",    Button5       },
   { "Shift",      ShiftMask     },
   { "s",          ShiftMask     },
   { "Ctrl",       ControlMask   },
   { "c",          ControlMask   },
   { "Lock",       LockMask      },
   { "l",          LockMask      },
   { "Alt",        Mod1Mask      },
   { "a",          Mod1Mask      },
   { "Meta",       Mod1Mask      },
   { "m",          Mod1Mask      },
   { "Mod1",       Mod1Mask      },
   { "1",          Mod1Mask      },
   { "Mod2",       Mod2Mask      },
   { "2",          Mod2Mask      },
   { "Mod3",       Mod3Mask      },
   { "3",          Mod3Mask      },
   { "Mod4",       Mod4Mask      },
   { "4",          Mod4Mask      },
   { "Mod5",       Mod5Mask      },
   { "5",          Mod5Mask      },
   { "Super",      Mod1Mask      },
   { "su",         Mod1Mask      },
   { "Hyper",      Mod1Mask      },
   { "h",          Mod1Mask      },
};

/*
 * The following two tables are used by StringToKey().
 * (I18N)
 */

static CharKeysymMap backslash_map[] = {
   { 'b',  XK_BackSpace    },
   { 't',  XK_Tab          },
   { 'n',  XK_Linefeed     },
   { 'v',  XK_Clear        },
   { 'f',  XK_Clear        },
   { 'r',  XK_Return       },
   {   0,  0               }
};

static CharKeysymMap singlechar_map[] = {
   { ' ',  XK_space        },
   { '!',  XK_exclam       },
   { '"',  XK_quotedbl     },
   { '#',  XK_numbersign   },
   { '$',  XK_dollar       },
   { '%',  XK_percent      },
   { '&',  XK_ampersand    },
#if	defined(XK_apostrophe)
   { '\'', XK_apostrophe   },
#else
   { '\'', XK_quoteright   },
#endif
   { '(',  XK_parenleft    },
   { ')',  XK_parenright   },
   { '*',  XK_asterisk     },
   { '+',  XK_plus         },
   { ',',  XK_comma        },
   { '-',  XK_minus        },
   { '.',  XK_period       },
   { '/',  XK_slash        },
   { ':',  XK_colon        },
   { ';',  XK_semicolon    },
   { '<',  XK_less         },
   { '=',  XK_equal        },
   { '>',  XK_greater      },
   { '?',  XK_question     },
   { '@',  XK_at           },
   { '[',  XK_bracketleft  },
   { '\\', XK_backslash    },
   { ']',  XK_bracketright },
   { '^',  XK_asciicircum  },
   { '_',  XK_underscore   },
#if	defined(XK_grave)
   { '`',  XK_grave        },
#else
   { '`',  XK_quoteleft    },
#endif
   { '{',  XK_braceleft    },
   { '|',  XK_bar          },
   { '}',  XK_braceright   },
   { '~',  XK_asciitilde   },
   {   0,  0               }
};

static OlKeyBinding OlCoreKeyBindings[] = {

/* button equivalents */

   { XtNselectKey,       "<space>,c<space>",  OL_SELECTKEY	},
   { XtNmenuKey,         "c<m>,n<F4>",        OL_MENUKEY	},
   { XtNmenuDefaultKey,  "c s<m>,s<F4>",      OL_MENUDEFAULTKEY	},
   { XtNhorizSBMenuKey,  "a c<r>",            OL_HSBMENU	},
   { XtNvertSBMenuKey,   "c<r>",              OL_VSBMENU	},
   { XtNadjustKey,       "c s<ampersand>",    OL_ADJUSTKEY	},
   { XtNduplicateKey,	 "s<space>",	      OL_DUPLICATEKEY	},

/* olwm + olwsm */

   { XtNworkspaceMenuKey,"c<w>",              OL_WORKSPACEMENU	},
   { XtNnextAppKey,      "a<Escape>",         OL_NEXTAPP	},
   { XtNnextWinKey,      "a<F6>",             OL_NEXTWINDOW	},
   { XtNprevAppKey,      "a s<Escape>",       OL_PREVAPP	},
   { XtNprevWinKey,      "a s<F6>",           OL_PREVWINDOW	},
   { XtNwindowMenuKey,   "s<Escape>",         OL_WINDOWMENU	},

/* traversal */

   { XtNdownKey,         "<Down>",            OL_MOVEDOWN	},
   { XtNleftKey,         "<Left>",            OL_MOVELEFT	},
   { XtNmultiDownKey,    "c<Down>",           OL_MULTIDOWN	},
   { XtNmultiLeftKey,    "c<Left>",           OL_MULTILEFT	},
   { XtNmultiRightKey,   "c<Right>",          OL_MULTIRIGHT	},
   { XtNmultiUpKey,      "c<Up>",             OL_MULTIUP	},
   { XtNnextFieldKey,    "n<Tab>,c<Tab>",     OL_NEXT_FIELD	},
   { XtNprevFieldKey,    "s<Tab>,c s<Tab>",   OL_PREV_FIELD	},
   { XtNrightKey,        "<Right>",           OL_MOVERIGHT	},
   { XtNupKey,           "<Up>",              OL_MOVEUP		},

/* selection */

#ifndef	sun
   { XtNcopyKey,         "c<Insert>",         OL_COPY		},
   { XtNcutKey,          "s<Delete>",         OL_CUT		},
   { XtNpasteKey,        "s<Insert>",         OL_PASTE		},
#else
   { XtNcopyKey,         "n<F16>",            OL_COPY		},
   { XtNcutKey,          "n<F20>",            OL_CUT		},
   { XtNpasteKey,        "n<F18>",            OL_PASTE		},
#endif
   { XtNselCharBakKey,   "s<Left>",           OL_SELCHARBAK	},
   { XtNselCharFwdKey,   "s<Right>",          OL_SELCHARFWD	},
   { XtNselLineKey,      "c a<Left>",         OL_SELLINE	},
   { XtNselLineBakKey,   "s<Home>",           OL_SELLINEBAK	},
   { XtNselLineFwdKey,   "s<End>",            OL_SELLINEFWD	},
   { XtNselWordBakKey,   "c s<Left>",         OL_SELWORDBAK	},
   { XtNselWordFwdKey,   "c s<Right>",        OL_SELWORDFWD	},
   { XtNselFlipEndsKey,  "a<Insert>",         OL_SELFLIPENDS	},

/* misc */

   { XtNcancelKey,       "n<Escape>",         OL_CANCEL		},
   { XtNdefaultActionKey,"<Return>,c<Return>",OL_DEFAULTACTION	},
   { XtNdragKey,         "n<F5>",             OL_DRAG		},
   { XtNdropKey,         "n<F2>",             OL_DROP		},
#ifndef	sun
   { XtNhelpKey,         "n<F1>",             OL_HELP		},
   { XtNpropertiesKey,   "c<p>",              OL_PROPERTY	},
   { XtNstopKey,         "c<s>",              OL_STOP		},
#else
   { XtNhelpKey,         "n<Help>",           OL_HELP		},
   { XtNpropertiesKey,   "n<F13>",            OL_PROPERTY	},
   { XtNstopKey,         "n<F11>",            OL_STOP		},
#endif
   { XtNtogglePushpinKey,"c<t>",              OL_TOGGLEPUSHPIN	},
#ifndef	sun
   { XtNundoKey,         "a<BackSpace>",      OL_UNDO		},
#else
   { XtNundoKey,         "n<F14>",            OL_UNDO		},
#endif

/* scrolling */

   { XtNpageDownKey,     "c<Next>",           OL_PAGEDOWN	},
   { XtNpageLeftKey,     "c<bracketleft>",    OL_PAGELEFT	},
   { XtNpageRightKey,    "c<bracketright>",   OL_PAGERIGHT	},
   { XtNpageUpKey,       "c<Prior>",          OL_PAGEUP		},
   { XtNscrollBottomKey, "a<Next>",           OL_SCROLLBOTTOM	},
   { XtNscrollDownKey,   "n<Next>",           OL_SCROLLDOWN	},
   { XtNscrollLeftKey,   "a<bracketleft>",    OL_SCROLLLEFT	},
   { XtNscrollLeftEdgeKey,"a s<braceleft>",   OL_SCROLLLEFTEDGE	},
   { XtNscrollRightKey,  "a<bracketright>",   OL_SCROLLRIGHT	},
   { XtNscrollRightEdgeKey,"a s<braceright>", OL_SCROLLRIGHTEDGE},
   { XtNscrollTopKey,    "a<Prior>",          OL_SCROLLTOP	},
   { XtNscrollUpKey,     "n<Prior>",          OL_SCROLLUP	},
};

static OlKeyBinding OlTextKeyBindings[] = {

/* text */

   { XtNdelCharBakKey,   "<BackSpace>",       OL_DELCHARBAK	},
   { XtNdelCharFwdKey,   "<Delete>",          OL_DELCHARFWD	},
   { XtNdelLineKey,      "a s<Delete>",       OL_DELLINE	},
   { XtNdelLineBakKey,   "c<BackSpace>",      OL_DELLINEBAK	},
   { XtNdelLineFwdKey,   "c<Delete>",         OL_DELLINEFWD	},
   { XtNdelWordBakKey,   "c s<BackSpace>",    OL_DELWORDBAK	},
   { XtNdelWordFwdKey,   "c s<Delete>",       OL_DELWORDFWD	},

/* text navigation */

   { XtNdocEndKey,       "c<End>",            OL_DOCEND		},
   { XtNdocStartKey,     "c<Home>",           OL_DOCSTART	},
   { XtNlineEndKey,      "n<End>",            OL_LINEEND	},
   { XtNlineStartKey,    "n<Home>",           OL_LINESTART	},
   { XtNpaneEndKey,      "c s<End>",          OL_PANEEND	},
   { XtNpaneStartKey,    "c s<Home>",         OL_PANESTART	},
   { XtNwordBakKey,      "c<Left>",           OL_WORDBAK	},
   { XtNwordFwdKey,      "c<Right>",          OL_WORDFWD	},

/* text backward compatibilities */

   { XtNcharBakKey,      "<Left>",            OL_CHARBAK	},
   { XtNcharFwdKey,      "<Right>",           OL_CHARFWD	},
   { XtNrowDownKey,      "<Down>",            OL_ROWDOWN	},
   { XtNrowUpKey,        "<Up>",              OL_ROWUP		},
   { XtNreturnKey,       "<Return>",          OL_RETURN		},

};

static OlBtnBinding OlCoreBtnBindings[] = {
   { XtNselectBtn,       "n<Button1>",        OL_SELECT      },
   { XtNadjustBtn,       "n<Button2>",        OL_ADJUST      },
   { XtNmenuBtn,         "n<Button3>",        OL_MENU        },
   { XtNconstrainBtn,    "c<Button1>",        OL_CONSTRAIN   },
   { XtNduplicateBtn,    "s<Button1>",        OL_DUPLICATE   },
   { XtNpanBtn,          "a<Button1>",        OL_PAN         },/*was C1*/
   { XtNmenuDefaultBtn,  "s<Button3>",        OL_MENUDEFAULT },
};

#endif /* __DynamicI_h__ */
