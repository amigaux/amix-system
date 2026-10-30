/*	Copyright (c) 1990 UNIX System Laboratories, Inc.	*/
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF     	*/
/*	UNIX System Laboratories, Inc.                     	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef NOIDENT
#ident	"@(#)olexamples:tutorial/s_sampler.c	1.33"
#endif

/************************************************************************/
/*	OPEN LOOK WIDGET SAMPLER : prototype widgets and use of form	*/
/*									*/
/*	Copyright (c) 1989 AT&T						*/
/*	Copyright (c) 1988 Hewlett-Packard Company 			*/
/*	Copyright (c) 1988 Massachusetts Institute of Technology	*/
/************************************************************************/

#include <stdio.h>

/*
 * Headers required for all OPEN LOOK applications.
 */

#include <X11/Intrinsic.h>	
#include <X11/StringDefs.h>
#include <Xol/OpenLook.h>

/*
 * Headers required for creating widget instances.
 */

#include <Xol/AbbrevMenu.h>
#include <Xol/BulletinBo.h>
#include <Xol/Button.h>
#include <Xol/Caption.h>
#include <Xol/CheckBox.h>
#include <Xol/ControlAre.h>
#include <Xol/Exclusives.h>
#include <Xol/FooterPane.h>
#include <Xol/Form.h>
#include <Xol/Gauge.h>
#include <Xol/MenuButton.h>
#include <Xol/Nonexclusi.h>
#include <Xol/Notice.h>
#include <Xol/OblongButt.h>
#include <Xol/PopupWindo.h>
#include <Xol/RectButton.h>
#include <Xol/Scrollbar.h>
#include <Xol/ScrolledWi.h>
#include <Xol/ScrollingL.h>
#include <X11/Shell.h>
#include <Xol/Slider.h>
#include <Xol/StaticText.h>
#include <Xol/Stub.h>
#include <Xol/TextEdit.h>
#include <Xol/TextField.h>

/*
 * Headers required for creating flat widget instances.
 */

#include <Xol/FExclusive.h>
#include <Xol/FNonexclus.h>
#include <Xol/FCheckBox.h>

/*
 * icon.xpm is the pixmap for the application's icon
 */

#include "icon.xpm"

/*
 * Defines.
 */

#define MAXTEXT 1000 
#define MAXBUF  100 

/*
 * Ol_PointToPixel() scales to the current screen resolution.
 */

#define N1_H_PIXEL Ol_PointToPixel(OL_HORIZONTAL,1) 
#define N1_V_PIXEL Ol_PointToPixel(OL_VERTICAL,1)
#define N10_H_PIXELS Ol_PointToPixel(OL_HORIZONTAL,10) 
#define N10_V_PIXELS Ol_PointToPixel(OL_VERTICAL,10)
#define N50_V_PIXELS Ol_PointToPixel(OL_VERTICAL,50)
#define N100_H_PIXELS Ol_PointToPixel(OL_HORIZONTAL,100)
#define N100_V_PIXELS Ol_PointToPixel(OL_VERTICAL,100)
#define N150_H_PIXELS Ol_PointToPixel(OL_HORIZONTAL,150)
#define N150_V_PIXELS Ol_PointToPixel(OL_VERTICAL,150)
#define N200_H_PIXELS Ol_PointToPixel(OL_HORIZONTAL,200)

/*
 * Global variables.
 */

static Pixel	red_pixel,blue_pixel,purple_pixel,green_pixel, 
		yellow_pixel,orange_pixel,skyblue_pixel;

static int	i;

static Boolean 	rainbow = FALSE;

/*
 * Note: In many cases, a widget is labelled by making it the child of a 
 * caption, which itself is a child of the form.
 */

static Widget	toplevel,
		controlarea,cabutton,
		form,
		gauge,
		abbmenu_caption,	/* caption for abbreviatedmenubutton */
		bb_caption,		/* caption for bulletinboard */
		ca_caption,		/* caption for controlarea */
		caption,		/* "True" caption */
		cb_caption,		/* caption for checkbox */
		f_caption,		/* caption for flats */
		fm_caption,		/* caption for form */
		gd_caption,		/* caption for gadgets */
		g_caption,		/* caption for gauge */
		slist_caption,		/* caption for scrollinglist */
		slider_caption,		/* caption for slider */
		te_caption,		/* caption for textedit */
		sw_caption,		/* caption for scrolledwindow */
		nonexclusives,nebutton2,nebutton3,
		noticeshell,noticebox,
		popupshell0,
		popupshell1,
		popupshell2,
		scrollbar,
		stub,
		footer_text;

/* 
 * Form constraint resources used for most widgets in application.
 */

static Arg genericARGS[] = {

	{ XtNxRefName, NULL },
	{ XtNyRefName, NULL },
	{ XtNxOffset,(XtArgVal) 0 },	/* to be initialized below */
	{ XtNyOffset,(XtArgVal) 0 },	/* to be initialized below */
	{ XtNxAddWidth,(XtArgVal) TRUE },
	{ XtNyAddHeight,(XtArgVal) TRUE },
};

/* 
 * UTILITY FUNCTIONS
 */

/*
 * Function to vary value displayed in gauge widget.
 */

static void 
ChangeGauge(widget)
	Widget widget;
{
	Arg arg;
	int current_value;

	XtSetArg(arg,XtNsliderValue,&current_value);
	XtGetValues(widget,&arg,1);

	current_value+=10;
	if(current_value>100)
		current_value=0;

/*
 * The following convenience routine is faster than a SetValues().
 */
	OlSetGaugeValue(gauge,current_value);
}

/* 
 * Xt and Xlib examples to give functionality to the stub widget.
 */

static void 
DrawAndPrint(widget,xevent,region)
	Widget widget;
	XEvent *xevent; /* not used */
	Region region;  /* not used */
{
	Display *display; 
	static Window window; 
	static Boolean done1 = FALSE, done2 = FALSE;
	static GC gc[6];
	XGCValues values;
	int i, ix5, ix10, coord, axis, finish = 180*64;
	static int hpixel,vpixel;
	Font fid = (Font)0;
	char *fontname = "*12bluci";
	XtGCMask mask;

	display = XtDisplay(widget);
	window = XtWindow(widget);

	if(!done1) {
		hpixel = (int) N1_H_PIXEL;
		vpixel = (int) N1_V_PIXEL;
		if(hpixel==0)
			hpixel=1;
		if(vpixel==0)
			vpixel=1;
		done1 = TRUE;
	}

	if(rainbow) {
	
		values.line_width = 5*hpixel;

		if(!done2) {
			mask = (XtGCMask) (GCForeground | GCLineWidth);

			values.foreground = red_pixel;
			gc[0]= XtGetGC(widget,mask, &values);
			values.foreground = orange_pixel;
			gc[1]= XtGetGC(widget,mask, &values);
			values.foreground = yellow_pixel;
			gc[2]= XtGetGC(widget,mask, &values);
			values.foreground = green_pixel;
			gc[3]= XtGetGC(widget,mask, &values);
			values.foreground = blue_pixel;
			gc[4]= XtGetGC(widget,mask, &values);
			values.foreground = purple_pixel;
			gc[5]= XtGetGC(widget,mask, &values);
		}

		for(i=0;i<6;i++) {

			ix5 = i*5;
			ix10 = i*10;

			axis = 95 - ix10;
			coord = ix5 + 5;	

			XDrawArc(	display, window, gc[i],
					coord*hpixel,
					coord*vpixel,
					(unsigned int) (axis*hpixel),
					(unsigned int) (axis*vpixel),
					0,finish);
		}
	}
	else
		XClearWindow(display,window);

/* 
 * Get font for this screen resolution.
 */

	fontname[0] = OlGetResolution(XtScreen(widget));
	fid = XLoadFont(display,fontname);

/* 
 * If successful, label the widget.
 */

	if(fid != (Font)0) {
	values.font = fid;
	gc[0] = XtGetGC(widget,(XtGCMask) GCFont, &values);
	XDrawString(display,window,gc[0],5*hpixel,
			70*vpixel,"STUB WIDGET",11);
	XtReleaseGC(stub,gc[0]);
	XUnloadFont(display,fid);
	}
/*
 * This function merely varies the value displayed in the gauge.
 */

	ChangeGauge(gauge);
}

/* 
 * Function for changing the footer message.
 */

static void FooterMessage(footer_text,buf)
Widget footer_text;
char buf[];
{
	Arg arg;

	XtSetArg(arg,XtNstring,buf);
	XtSetValues(footer_text,&arg,1);
}

/* 
 * Function for getting color values for the display.
 */

static void 
GetColors()	
{
	XrmValue fromValue,toValue;

	static char *colors[] = {
		"purple","blue","green","yellow","orange","red","skyblue"
	};

	int ncolors = 7;

	for(i=0;i<7;i++) {
		fromValue.size = sizeof(colors[i]);
		fromValue.addr = colors[i];
		XtConvert(toplevel,XtRString,&fromValue,XtRPixel,&toValue);
		switch(i) {
			case 0: 
				purple_pixel = *((Pixel *) toValue.addr);
				break;
			case 1: 
				blue_pixel = *((Pixel *) toValue.addr);
				break;
			case 2: 
				green_pixel = *((Pixel *) toValue.addr);
				break;
			case 3: 
				yellow_pixel = *((Pixel *) toValue.addr);
				break;
			case 4: 
				orange_pixel = *((Pixel *) toValue.addr);
				break;
			case 5: 
				red_pixel = *((Pixel *) toValue.addr);
				break;
			case 6: 
				skyblue_pixel = *((Pixel *) toValue.addr);
				break;
		}
	}
}

/*
 * Function for positioning widgets on the form.
 *
 * Note: XtNx[y]RefName are used instead of XtNx[y]RefWidget so
 * that resources for each widget can be set in an .Xdefaults 
 * file.  In addition, this also allows specifying the placement 
 * of any widget on the form without first having to place the 
 * corresponding reference widget(s) on the form.  (This would 
 * NOT be the case with the XtNx[y]RefWidget resource).
 */

static void 
SetPosition(widget, xwidget, ywidget)
Widget widget;
char *xwidget, *ywidget;
{
	static int nargs;

	if(nargs == 0)
		nargs = XtNumber(genericARGS);

	genericARGS[0].value = (XtArgVal) xwidget; 
	genericARGS[1].value = (XtArgVal) ywidget; 
	XtSetValues(widget,genericARGS,nargs);
}

/* 
 * Function for creating a custom icon for the application.
 */

static void 
SetProgramIcon(toplevel)
	Widget toplevel;
{
	Pixmap program_icon;
	Arg arg[2];
	int i;
	Screen *screen;

	screen = XtScreen(toplevel);

/* 
 * "icon_" values from icon.xpm
 */

	program_icon = XCreatePixmapFromData( XtDisplay(toplevel),
				RootWindowOfScreen(screen),
				DefaultColormapOfScreen(screen),
				icon_width,icon_height,
				DefaultDepthOfScreen(screen),
				icon_ncolors,
				icon_chars_per_pixel,
				icon_colors, 
				icon_pixels); 
	i=0;
	XtSetArg(arg[i],XtNiconPixmap,(XtArgVal) program_icon); i++;
	XtSetArg(arg[i],XtNiconName,(XtArgVal) "s_sampler"); i++;
	XtSetValues(toplevel,arg,i);
}

/* 
 * Function for creating a special cursor for the stub widget.
 */

static void 
SetStubCursor(widget)
	Widget widget;
{
	static Cursor cursor;

/*
 * See OlCorsor.c for other cursor possibilities.
 */

	cursor = GetOlQuestionCursor(XtScreen(widget));
	XDefineCursor(XtDisplay(widget),XtWindow(widget),cursor);
}

/* 
 *  Event handler example to give functionality to the stub widget.
 */

static void 
StubEventHandler(widget,clientData,event)
	Widget widget;
	XtPointer clientData;
	XEvent *event;
{
	XCrossingEvent *xce;

/*
 * The xce pointer allows referencing to event specifics - see Xlib.h
 */

	if(event->type==EnterNotify || event->type==LeaveNotify)
		xce = (XCrossingEvent *) &(event->xcrossing);
	else 
		return;

	if(event->type ==EnterNotify)
		FooterMessage(footer_text,
			"Footerpanel: Pointer entered STUB widget");
	else
		FooterMessage(footer_text,
			"Footerpanel: Pointer left STUB widget");
}

/* 
 * CALLBACKS FOR WIDGETS
 */

/* 
 * With this callback, each widget passes its index as 
 * clientData and thus maps to its own footerpanel message. 
 */

static void 
genericCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	int n = (int) clientData;

	switch(n) {
		case 1:

		FooterMessage(footer_text,
			"Footerpanel: OBLONGBUTTON callback");
		break;

		case 2:
		case 3:
		case 4:

		{
		char buf[MAXBUF];
		
		sprintf(buf,
		"Footerpanel: [NON] EXCLUSIVES callback for button %d",n-1);
		FooterMessage(footer_text,buf);
		}
		break;

		case 5:

		FooterMessage(footer_text,
			"Footerpanel: POPUP apply callback");
		break;

		case 6:

		FooterMessage(footer_text,
			"Footerpanel: POPUP setdefaults callback");
		break;

		case 7:

		FooterMessage(footer_text,
			"Footerpanel: POPUP reset callback");
		break;

		case 8:

		FooterMessage(footer_text,
			"Footerpanel: POPUP resetfactory callback");
		break;

		case 9:

		FooterMessage(footer_text,
			"Footerpanel: OBLONGBUTTONGADGET callback");
		break;

		case 10:

		FooterMessage(footer_text,
			"Footerpanel: FLAT select callback");
		break;
	}
}

static void 
checkboxCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData, callData;
{
	Arg arg;
	OlDefine position;
/* 
 * Get the current value.
 */
	XtSetArg(arg,XtNposition,&position);
	XtGetValues(widget,&arg,1);

/* 
 * Toggle it.
 */
	if(position == (OlDefine) OL_LEFT)
		XtSetArg(arg,XtNposition,OL_RIGHT);
	else 
		XtSetArg(arg,XtNposition,OL_LEFT);

	XtSetValues(widget,&arg,1);
}

/* 
 * The MenuSelectCB callback is used by the pulldown menu
 * popup menu, and abbreviatedmenubutton widget and gadget. 
 */

static void 
menuSelectCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	int n = (int) clientData;
	char buf[MAXBUF];
		
	sprintf(buf,"Footerpanel: Button %d selected",n);
	FooterMessage(footer_text,buf);
}

static void 
nonexclusivesCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	Widget parent = (Widget) clientData;

/* 
 * If only "More" button, create "Fewer" button.
 */
	if(widget == nebutton2 && nebutton3 == (Widget) 0) {
		nebutton3 = XtCreateManagedWidget
			("Fewer",rectButtonWidgetClass,parent,NULL,0);
		XtAddCallback(nebutton3,XtNselect,
				nonexclusivesCB,(XtPointer) nonexclusives);
		XtAddCallback(nebutton3,XtNselect,
				genericCB,(XtPointer) 4);

	}

/* 
 * If all three buttons, delete "Fewer" button.
 */

	else if(widget == nebutton3 && nebutton3 != (Widget)0) {
		XtDestroyWidget(nebutton3);
		nebutton3 = (Widget) 0;
	}
}

static void 
noticeCB1(widget,callData,clientData) 
	Widget widget;	/* w = emanating button: where notice does popup */
	XtPointer callData, clientData;
{
	Arg arg;

	XtSetArg(arg, XtNemanateWidget, (XtArgVal)widget);
	XtSetValues(noticebox, &arg, 1);
	XtPopup(noticeshell, XtGrabExclusive); 
}

static void 
noticeCB2(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	FooterMessage(footer_text,"Footerpanel: ADIOS !!!");
	exit(0);
}

static void 
popupCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	XtPopup(clientData,XtGrabNone);
}

/*
 * The rainbowCB callback is for the RAINBOW button.
 */

static void 
rainbowCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	if(rainbow)
		rainbow=FALSE;
	else 
		rainbow=TRUE;

	DrawAndPrint(stub,(XEvent *)0,(Region)0);
}

/* 
 * This callback is used by all three scrollbars.
 */

static void 
scrollbarCB(widget,clientData,callData)
    Widget widget;
    XtPointer clientData,callData;
{
	OlScrollbarVerify *sbv = (OlScrollbarVerify *) callData;
	int n = (int) clientData;
	char buf[MAXBUF];

	sbv->ok = TRUE;

	switch(n) {
		case 0:

		sprintf(buf,
		"Footerpanel: Form scrollbar moved to %d%%",
			sbv->new_location);

		break;

		case 1:

		sprintf(buf,
		"Footerpanel: Scrolledwindow vertical scrollbar moved");

		break;

		case 2:

		sprintf(buf,
		"Footerpanel: Scrolledwindow horizontal scrollbar moved");

		break;
	}

	FooterMessage(footer_text,buf);

/*
 * Update the form's scrollbar page indicator.
 */
	if(widget==scrollbar)
		sbv->new_page = (int) (sbv->new_location/10) + 1;
}

static void 
scrollinglistCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	OlListToken token = (OlListToken) callData;
	OlListItem *selected_listitem;
	char buf[MAXBUF];

/* 
 * This macro identifies the item selected.
 */

	selected_listitem = OlListItemPointer(token);

		
	sprintf(buf,
		"Footerpanel: SCROLLING LIST : Item label %s Data stored %d",
		selected_listitem->label,selected_listitem->user_data);
	FooterMessage(footer_text,buf);
}

static void 
sliderCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	OlSliderVerify *sv = (OlSliderVerify *) callData;
	Arg arg;

/*
 * Slider returns current value.
 */
	arg.value = (XtArgVal) sv->new_location;
	XtSetArg(arg,XtNbackground,arg.value);
	XtSetValues(stub,&arg,1);
}

static void 
textfieldCB(widget,clientData,callData)
	Widget widget;
	XtPointer clientData,callData;
{
	OlTextFieldVerify *tfv = (OlTextFieldVerify *) callData;
	char buf[MAXBUF];
	Arg arg;
		

	sprintf(buf,"Footerpanel: TEXTFIELD User Input: %s\n",tfv->string);
	FooterMessage(footer_text,buf);

	XtSetArg(arg,XtNstring,(XtArgVal) "");
	XtSetValues(widget,&arg,1);
}

/*
 * THE WIDGET TREE: 
 *
 * 				Toplevel 
 *				   |
 * 				Footerpanel
 *				/     \
 * 			     Form     Statictext
 *			      /
 * 			Remaining OPEN LOOK widgets, gadgets, and flats
 */

/*
 * MAIN
 */

int main(argc,argv)
	int argc;
	char *argv[];
{

	Arg arg[10];

/*
 * Initialize the environment.
 */

{
	toplevel = OlInitialize(	"s_sampler",
					"s_sampler", 
					(XrmOptionDescRec *) NULL, 
					(Cardinal) 0, 
					&argc, 
					argv);

	i=0;
	XtSetArg(arg[i],XtNtitle,(XtArgVal) "s_sampler");i++;
	XtSetValues(toplevel,arg,i);

/*
 * Get colors to use later.
 */
	GetColors();		

/* 
 * Set the pixmap for program icon window 
 */
	SetProgramIcon(toplevel);

/*
 * Set FORM resources for the specific screen.
 */

	genericARGS[2].value = (XtArgVal) N10_H_PIXELS; 
	genericARGS[3].value = (XtArgVal) N10_V_PIXELS; 
}

/* 
 * Make all the widgets and then do placement on the form last.
 */

/* 
 * FOOTERPANEL: This will be the child of toplevel, with
 * the FORM as the topchild and STATICTEXT as the footer child.
 */

{
	Widget footerpanel;

	footerpanel = XtCreateManagedWidget("footerpanel",
		footerPanelWidgetClass,toplevel,NULL,0);

/* 
 * FORM: Form's caption is the top child of the footerpanel.
 */

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Form"); i++;
	fm_caption = XtCreateManagedWidget("fm_caption",
		captionWidgetClass,footerpanel,arg,i);

	form = XtCreateManagedWidget("form",
			formWidgetClass,fm_caption,NULL,0);

/*
 * Register help for the form, using the file "form.help"
 */

	(void) OlRegisterHelp(	OL_WIDGET_HELP,(XtPointer) form, 
		"Form Widget",OL_DISK_SOURCE,"/usr/X/lib/tutorial/Xol/form.help");

/*
 * Make STATICTEXT as the footer child of the footerpanel.
 */

	i = 0;
	XtSetArg(arg[i], XtNstring,
		(XtArgVal) "Statictext (read only) for the Footerpanel . . . ");
	i++;
	footer_text = XtCreateManagedWidget("statictext",
		staticTextWidgetClass,footerpanel,arg,i);
}

/*
 * All the remaining widgets will be on the form.
 */

/*
 * CAPTION: to label the controlarea, latter to be created next.
 */

	i = 0;
	XtSetArg(arg[i],XtNxResizable,(XtArgVal) TRUE); i++;
	XtSetArg(arg[i],XtNxAttachRight,(XtArgVal) TRUE); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Controlarea: "); i++;
	ca_caption = XtCreateManagedWidget("ca_caption",
		captionWidgetClass,form,arg,i);

/*
 * CONTROLAREA: the control area contains a README oblongbutton with
 * with a popup statictext message, a popup command window, a popup 
 * property window, and an EXIT button with a notice widget which pops 
 * up when the button is selected.
 */

{
	Widget widget,popupca01;
	static char stext[MAXTEXT];
	FILE *fp = (FILE *) NULL;

	controlarea = XtCreateManagedWidget("controlarea",
		controlAreaWidgetClass,ca_caption,NULL,0);

	widget = XtCreateManagedWidget("README",
		oblongButtonWidgetClass,controlarea,NULL,0);
	i=0;
	XtSetArg(arg[i],XtNtitle,"README"); i++;
	popupshell0 = XtCreatePopupShell("popupshell0",
		popupWindowShellWidgetClass,widget,arg,i);

	XtAddCallback(widget,XtNselect,popupCB,popupshell0);

	i = 0;
	XtSetArg(arg[i], XtNupperControlArea, &popupca01); i++;
	XtGetValues (popupshell0, arg, i);

/* 
 * STATICTEXT: This is used as a README for the application.
 */


	if((fp=fopen("/usr/X/lib/tutorial/Xol/statictext.text","r"))) {
		fread(stext,sizeof(char),MAXTEXT,fp); 
		fclose(fp);
	}
	else 
		strcpy(stext,"Statictext message for README\n");

	i = 0;
	XtSetArg(arg[i], XtNstring, stext); i++;
	XtCreateManagedWidget("statictext",
		staticTextWidgetClass,popupca01,arg,i);
}

/* 
 * POPUP: Used for Command Window.
 */

{
	Widget widget,popupca11,popupca12,popupfooter1,popupbutton1;

	popupbutton1 = XtCreateManagedWidget("Popup: Command Window",
		oblongButtonWidgetClass,controlarea,NULL,0);

/* 
 * Make the popup shell first.
 */
	i = 0;
	XtSetArg(arg[i],XtNtitle,"Command Window"); i++;
	popupshell1 = XtCreatePopupShell("POPUPSHELL", 
		popupWindowShellWidgetClass,popupbutton1,arg,i);

/* 
 * Add callback to popup button now that we have popupshell widget ID. 
 */
	XtAddCallback(popupbutton1, XtNselect, popupCB, popupshell1);

/*
 * The popup window automatically makes three children: upper and
 * lower control areas and footer: get widget IDs to populate them.
 */
	i = 0;
	XtSetArg(arg[i], XtNupperControlArea, &popupca11); i++;
	XtSetArg(arg[i], XtNlowerControlArea, &popupca12); i++;
	XtSetArg(arg[i], XtNfooterPanel, &popupfooter1); i++;
	XtGetValues (popupshell1, arg, i);

/* 
 * Populate popup upper control area.
 */

/* 
 * Make a caption as a label/prompt for the TEXTFIELD.
 */

	i=0;
	XtSetArg(arg[i],XtNlabel,
		(XtArgVal) "Textfield: type & type <return> :"); i++;
	widget = XtCreateManagedWidget("caption",
		captionWidgetClass,popupca11,arg,i);

	widget = XtCreateManagedWidget("textfield",
		textFieldWidgetClass,widget,NULL,0);

/* 
 * Callback to "read" user input when <return> typed. 
 */

	XtAddCallback(widget,XtNverification,textfieldCB,NULL);

/* 
 * Populate popup lower controlarea with buttons: one must be a default.
 */
	i = 0;
	XtSetArg(arg[i], XtNdefault,(XtArgVal) TRUE); i++;
	XtCreateManagedWidget("Option 1",
		oblongButtonWidgetClass,popupca12,arg,i);

	XtCreateManagedWidget("Option 2",
		oblongButtonWidgetClass,popupca12,NULL,0);

/* 
 * Add text to the popup footer.
 */
	i = 0;
	XtSetArg(arg[i], XtNborderWidth, 0); i++;
	XtSetArg(arg[i], XtNstring, "Footer widget for messages"); i++;
	XtCreateManagedWidget("footer",
		staticTextWidgetClass,popupfooter1,arg,i);
}

/*
 * End of POPUP: Command Window.
 */

/*
 * POPUP: Used for Property Sheet.
 */

{
	Widget exclusives,widget,popupca21,popupfooter2,popupbutton2;

/*
 * XtCallbackRec used for widget SetValues().
 */

	static XtCallbackRec popup_applyCBR[] = {
		{ genericCB, (XtPointer) 5 },
		{ (XtCallbackProc) NULL, (XtPointer) NULL },
	};

	static XtCallbackRec popup_setdefaultsCBR[] = {
		{ genericCB, (XtPointer) 6 },
		{ (XtCallbackProc) NULL, (XtPointer) NULL },
	};

	static XtCallbackRec popup_resetCBR[] = {
		{ genericCB, (XtPointer) 7 },
		{ (XtCallbackProc) NULL, (XtPointer) NULL },
	};

	static XtCallbackRec popup_resetfactoryCBR[] = {
		{ genericCB, (XtPointer) 8 },
		{ (XtCallbackProc) NULL, (XtPointer) NULL },
	};

	popupbutton2 = XtCreateManagedWidget("Popup: Property Window",
		oblongButtonWidgetClass,controlarea,NULL,0);

/* 
 * Make the popup shell first:
 * NOTE: callbacks must be set for creation of the automatic buttons.
 * In this example, mapping is to one function but does not have to be. 
 */
	i = 0;
	XtSetArg(arg[0],XtNtitle,"Property Window"); i++;
	XtSetArg(arg[i],XtNreset,(XtArgVal) popup_resetCBR); i++;
	XtSetArg(arg[i],XtNapply,(XtArgVal) popup_applyCBR); i++;
	XtSetArg(arg[i],XtNresetFactory,(XtArgVal) popup_resetfactoryCBR);i++;
	XtSetArg(arg[i],XtNsetDefaults, (XtArgVal) popup_setdefaultsCBR);i++;
	popupshell2 = XtCreatePopupShell("POPUPSHELL", 
		popupWindowShellWidgetClass,popupbutton2,arg,i);

/* 
 * Add callback to popup button now that we have popupshell widget ID. 
 */
	XtAddCallback(popupbutton2, XtNselect, popupCB, popupshell2);

/*
 * Get widget IDs of popup children needed.  
 * Note that lower control area ID not needed 
 * since automatic buttons are created by code above.
 */
	i = 0;
	XtSetArg(arg[i], XtNupperControlArea, &popupca21); i++;
	XtSetArg(arg[i], XtNfooterPanel, &popupfooter2); i++;
	XtGetValues (popupshell2, arg, i);

/* 
 * Populate popup upper control area with EXCLUSIVES and NONEXLCUSIVES.
 * Note that there is no need to populate popup lower controlarea.
 */
	i=0;
	XtSetArg(arg[i],XtNlayoutType,OL_FIXEDCOLS);i++;
	XtSetValues(popupca21,arg,i);

	widget =XtCreateManagedWidget("Exclusives: ",
		captionWidgetClass,popupca21,NULL,0);

	i = 0;
	XtSetArg(arg[0],XtNlayoutType,(XtArgVal) OL_FIXEDROWS); i++;
	XtSetArg(arg[0],XtNmeasure,(XtArgVal) 1); i++;
	exclusives = XtCreateManagedWidget("exclusives",
		exclusivesWidgetClass,widget,arg,i);

	i = 0;
	XtSetArg(arg[0],XtNlabel,(XtArgVal) "Choice 1"); i++;
	widget = XtCreateManagedWidget("rbutton",
		rectButtonWidgetClass,exclusives,arg,i);
	XtAddCallback(widget,XtNselect,genericCB,(XtPointer) 2);

	widget = XtCreateManagedWidget("Choice 2",
		rectButtonWidgetClass,exclusives,NULL,0);
	XtAddCallback(widget,XtNselect,genericCB,(XtPointer) 3);

	widget = XtCreateManagedWidget("Choice 3",
		rectButtonWidgetClass,exclusives,NULL,0);
	XtAddCallback(widget,XtNselect,genericCB,(XtPointer) 4);

	widget = XtCreateManagedWidget("Nonexclusives: ",
		captionWidgetClass,popupca21,NULL,0);

	i = 0;
	XtSetArg(arg[i],XtNlayoutType,(XtArgVal) OL_FIXEDROWS); i++;
	XtSetArg(arg[i],XtNmeasure,(XtArgVal) 1); i++;
	nonexclusives = XtCreateManagedWidget("nonexclusives",
		nonexclusivesWidgetClass,widget,arg,i);

	widget = XtCreateManagedWidget("Choice 1",
		rectButtonWidgetClass,nonexclusives,NULL,0);
	XtAddCallback(widget,XtNselect,genericCB,(XtPointer) 2);

	nebutton2 = XtCreateManagedWidget("More",
		rectButtonWidgetClass,nonexclusives,NULL,0);
	XtAddCallback(nebutton2,XtNselect,nonexclusivesCB,nonexclusives);
	XtAddCallback(nebutton2,XtNselect,genericCB,(XtPointer)3);

/* 
 * Add text to the popup footer.
 */
	i = 0;
	XtSetArg(arg[i], XtNborderWidth, 0); i++;
	XtSetArg(arg[i], XtNstring, "Footer widget for messages"); i++;
	XtCreateManagedWidget("footer",
		staticTextWidgetClass,popupfooter2,arg,i);
}

/*
 * End of POPUP: Property Sheet.
 */

/*
 * TO MAKE A MENU:
 *
 * Widget Tree for creating menu:
 *
 *		( parent widget )
 *			|
 *		menubutton ( visible/mouse sensitive symbol for menu )
 *			|
 *		pane ( used for placement of menu button set )
 *			|
 *		       / \
 *		     /	   \
 *	        button1   button2
 */

/*
 * Create a menubutton and menu with a pushpin.
 */

{
	static Widget menubutton,menupane,widget;

	i = 0;
	XtSetArg(arg[i],XtNpushpin,(XtArgVal) OL_OUT); i++;
	XtSetArg(arg[i],XtNlabelType,(XtArgVal) OL_STRING); i++;
	XtSetArg(arg[i],XtNlabelJustify,(XtArgVal) OL_LEFT); i++;
	XtSetArg(arg[i],XtNrecomputeSize,(XtArgVal) TRUE); i++;
	menubutton = XtCreateManagedWidget("Menubutton",
		menuButtonWidgetClass,controlarea,arg,i);

/* 
 * Get the Widget id of the menupane of the menubutton. 
 */
	i = 0;
	XtSetArg(arg[i], XtNmenuPane,(XtArgVal) &menupane); i++;
	XtGetValues(menubutton, arg, i);

/* 
 * Make two oblongbuttons on the menupane, with select callbacks.
 */

	widget = XtCreateManagedWidget("One",
		oblongButtonWidgetClass,menupane,NULL,0);
	XtAddCallback(widget,XtNselect,menuSelectCB,(XtPointer) 1);

	widget = XtCreateManagedWidget("Two",
		oblongButtonWidgetClass,menupane,NULL,0);
	XtAddCallback(widget,XtNselect,menuSelectCB,(XtPointer) 2);
}

/*
 * End of MENU
 */

/* 
 * NOTICE: attach to an EXIT button in the control area.
 */

{

	static Widget noticetext;
	Widget widget;

	static Arg noticeARGS[] = {
		{XtNtextArea, (XtArgVal) &noticetext},
		{XtNcontrolArea, (XtArgVal) &noticebox},
	};

	cabutton = XtCreateManagedWidget("Exit",
		oblongButtonWidgetClass,controlarea,NULL,0);

/* 
 * Attach notice popup callback to "EXIT" controlarea button. 
 */

	XtAddCallback(cabutton, XtNselect, noticeCB1,NULL);

/* 
 * Create notice popup shell. 
 */

	noticeshell = XtCreatePopupShell("notice",
		noticeShellWidgetClass,cabutton,NULL,0);

/* 
 * Get the widget ids of noticebox and textarea of noticebox.
 */

	XtGetValues(noticeshell, noticeARGS, XtNumber(noticeARGS));

/* 
 * Add text to text area of noticebox. 
 */
 
	i = 0;
	XtSetArg(arg[i],XtNstring,(XtArgVal)"NOTICE WIDGET:\n Confirm EXIT");
	i++;
	XtSetValues(noticetext,arg,i);

/* 
 * Add two buttons to noticebox.
 */

/* 
 * First button has an exit() callback.
 */

	widget = XtCreateManagedWidget("Okay", 
		oblongButtonWidgetClass,noticebox,NULL,0);
	XtAddCallback(widget, XtNselect,noticeCB2,noticebox);

/* 
 * Second button is a no-op that pops down notice widget without exiting.
 */

	XtCreateManagedWidget("Cancel",
		oblongButtonWidgetClass,noticebox,NULL,0);
}

/* 
 * CAPTION
 */

{
	static Widget widget; 

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Caption"); i++;
	caption = XtCreateManagedWidget("caption",
		captionWidgetClass,form,arg,i);

	widget = XtCreateManagedWidget("*** Rainbow ***",
		oblongButtonWidgetClass,caption,NULL,0);
	XtAddCallback(widget, XtNselect, rainbowCB, NULL);
}

/* 
 * STUB WIDGET: this will be used as a drawing canvas.
 */

/* 
 * First two arguments scale widget to resolution of screen.
 */

	i = 0;
	XtSetArg(arg[i],XtNheight,(XtArgVal) N100_V_PIXELS); i++;
	XtSetArg(arg[i],XtNwidth,(XtArgVal) N100_H_PIXELS); i++;
	XtSetArg(arg[i],XtNbackground, skyblue_pixel); i++;
/*
 * DrawAndPrint() will be called with an Expose event;
 */
	XtSetArg(arg[i],XtNexpose, DrawAndPrint); i++; 
	stub = XtCreateManagedWidget("stub",stubWidgetClass,form,arg,i);

/*
 * Add an eventhandler to track when the pointer 
 * enters and leaves the stub widget window.
 */

	XtAddEventHandler(stub,EnterWindowMask | LeaveWindowMask,
		FALSE, StubEventHandler, (XtPointer)NULL);

/* 
 * Set the special cursor for stub widget window 
 * below once the widget tree realized. 
 */

/* 
 * BULLETINBOARD: with ABBREVIATEDMENUBUTTON, CHECKBOX, 
 * and TEXTEDIT widgets in it, each within a caption 
 * labelling the widget.
 */

{
	Widget widget,bulletinboard,abbmenubutton,abbmenupane,checkbox;
	Dimension	height,yvalue,
			xpad = (Dimension) N10_H_PIXELS,
			ypad = (Dimension) N10_V_PIXELS;
	
	static char	textedit_string[] = 
		"This text is in a textedit\nwidget and can be edited.";

/* 
 * BULLETINBOARD.
 */

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Bulletinboard"); i++;
	bb_caption = XtCreateManagedWidget("bb_caption",
		captionWidgetClass,form,arg,i);

	bulletinboard = XtCreateManagedWidget("bulletinboard",
	bulletinBoardWidgetClass,bb_caption,NULL,0);

/*
 * Set some resources for the bulletinboard.
 */

	i = 0;
	XtSetArg(arg[i],XtNborderWidth, (XtArgVal) 2); i++;
	XtSetArg(arg[i],XtNborderColor, (XtArgVal) orange_pixel); i++;
	XtSetValues(bulletinboard,arg,i);

/*
 * ABBREVIATEDMENUBUTTON.
 */

	yvalue = ypad;

	i = 0;
	XtSetArg(arg[i],XtNx,(XtArgVal) xpad); i++;
	XtSetArg(arg[i],XtNy,(XtArgVal) yvalue); i++;
	XtSetArg(arg[i],XtNlabel, (XtArgVal) "Abbreviatedmenubutton: "); i++;
	abbmenu_caption = XtCreateManagedWidget("abbmenu_caption",
		captionWidgetClass,bulletinboard,arg,i);

	abbmenubutton = XtCreateManagedWidget("abbmenubutton",
		abbrevMenuButtonWidgetClass,abbmenu_caption,arg,i);

/* 
 * Get the Widget ID of the menupane of the abbreviated menu button.
 */

	i = 0;
	XtSetArg(arg[i], XtNmenuPane,(XtArgVal) &abbmenupane); i++;
	XtGetValues(abbmenubutton, arg, i);

	i = 0;
	widget = XtCreateManagedWidget("Button 1",
		oblongButtonWidgetClass,abbmenupane,arg,i);
 	XtAddCallback(widget,XtNselect,menuSelectCB,(XtPointer) 1);

	i = 0;
	widget = XtCreateManagedWidget("Button 2",
		oblongButtonWidgetClass,abbmenupane,arg,i);
 	XtAddCallback(widget,XtNselect,menuSelectCB,(XtPointer) 2);

/*
 * CHECKBOX.
 */

	i=0;
	XtSetArg(arg[i],XtNheight, &height); i++;
	XtGetValues(abbmenubutton,arg,1);

	yvalue = yvalue + height + ypad;
	
	i = 0;
	XtSetArg(arg[i], XtNx,xpad); i++;
	XtSetArg(arg[i],XtNy, yvalue); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Checkbox: "); i++;
	cb_caption = XtCreateManagedWidget("cb_caption",
		captionWidgetClass,bulletinboard,arg,i);

	i=0;
	checkbox = XtCreateManagedWidget("CHECKBOX_LABEL" ,
		checkBoxWidgetClass,cb_caption, arg,i);
	XtAddCallback(checkbox,XtNselect,checkboxCB,NULL);

/* 
 * TEXTEDIT WIDGET.
 */

	i=0;
	XtSetArg(arg[i],XtNheight, &height); i++;
	XtGetValues(checkbox,arg,1);

	yvalue = yvalue + height + ypad;
	
	i = 0;
	XtSetArg(arg[i],XtNx,(XtArgVal) xpad); i++;
	XtSetArg(arg[i],XtNy, (XtArgVal) yvalue); i++;
	XtSetArg(arg[i],XtNalignment, (XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Textedit:"); i++;
	te_caption = XtCreateManagedWidget("te_caption",
		captionWidgetClass,bulletinboard,arg,i);
				
	i = 0;
	XtSetArg(arg[i],XtNheight, (XtArgVal) N50_V_PIXELS); i++;
	XtSetArg(arg[i],XtNwidth, (XtArgVal) N150_H_PIXELS); i++;
	XtSetArg(arg[i],XtNsourceType, (XtArgVal) OL_STRING_SOURCE); i++;
	XtSetArg(arg[i],XtNsource, (XtArgVal) textedit_string); i++;
	XtCreateManagedWidget("textedit",textEditWidgetClass,te_caption,arg,i);
}

/*
 * SCROLLINGLIST
 */

{
	static Widget scrollinglist;
	static OlListToken  (*scrollinglistADDfn)();
	static OlListItem sl_item[10];

	int natoi;
/*
 * Use a caption to label the scrollinglist.
 */

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Scrollinglist"); i++;
	slist_caption = XtCreateManagedWidget("slist_caption",
		captionWidgetClass,form,arg,i);

	i = 0;
	XtSetArg(arg[i], XtNviewHeight, (XtArgVal) 7); i++;
	scrollinglist = XtCreateManagedWidget("scrollinglist",
		scrollingListWidgetClass,slist_caption,arg,i);

/*
 * Get the pointer to the scrollinglist function in order to add items.
 */

	i = 0;
	XtSetArg(arg[i], XtNapplAddItem, (XtArgVal) &scrollinglistADDfn); i++;
	XtGetValues(scrollinglist,arg,i);

	natoi = (int) 'A';

	for(i=0; i<10; i++) {
		sl_item[i].label_type = (OlDefine) OL_STRING;
		sl_item[i].label = strcpy(XtMalloc((unsigned)7),"ITEM   ");
		((String) sl_item[i].label)[5] = natoi;
		sl_item[i].mnemonic = tolower(natoi++);
/*
 * This field is for storing any type of data desired.
 */
		sl_item[i].user_data = (XtPointer) i ;
		(*scrollinglistADDfn) (scrollinglist,NULL,NULL,sl_item[i]); 
	}

/* 
 * Callback to be invoked when user selects an item. 
 */

	XtAddCallback(scrollinglist,XtNuserMakeCurrent,scrollinglistCB,NULL);
}


/*
 * SCROLLED WINDOW
 */

{
static Widget scrolledwindow;

static char	swstring[] = 

"This text is in a\n\
statictext widget\n\
in a scrolled window.\n\
You can move it up\n\
and down or left\n\
and right with the\n\
two scrollbars.\n";

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Scrolledwindow"); i++;
	sw_caption = XtCreateManagedWidget("sw_caption",
		captionWidgetClass,form,arg,i); 

	i = 0;
	XtSetArg(arg[i], XtNheight,N150_V_PIXELS); i++;
	XtSetArg(arg[i], XtNwidth, N100_H_PIXELS); i++;
	XtSetArg(arg[i], XtNvStepSize, 20); i++;
	XtSetArg(arg[i], XtNhStepSize, 20); i++;
	scrolledwindow = XtCreateManagedWidget("scrolledwindow",
		scrolledWindowWidgetClass,sw_caption,arg,i);
	XtAddCallback(scrolledwindow,XtNvSliderMoved,scrollbarCB,(XtPointer)1);
	XtAddCallback(scrolledwindow,XtNhSliderMoved,scrollbarCB,(XtPointer)2);

/* 
 * Make a statictext widget in the scrolled window
 * to scroll with the scrollbars.
 */

	i = 0;
	XtSetArg(arg[i], XtNheight,N150_V_PIXELS); i++;
	XtSetArg(arg[i], XtNstring, swstring); i++;
	XtCreateManagedWidget("statictext",
		staticTextWidgetClass,scrolledwindow,arg,i);
}

/*
 * SLIDER 
 */

/*
 * The slider will be used to change the background of the stub widget: 
 * calculate the number of colors for the screen and set the slider 
 * range from 0 to (N-1) with a granularity of 1; see the sliderCB 
 * function for the rest of the code/functionality.
 */

{
	Display *display = XtDisplay(toplevel);
	int screen = XDefaultScreen(display);
	int n, ncolors=2;
	Widget w;

	n= XDefaultDepth(display,screen);

	for(i=1; i<n; i++) {
		ncolors= ncolors*2;
	}

	ncolors = ncolors -1 ;

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Slider"); i++;
	slider_caption = XtCreateManagedWidget("slider_caption",
		captionWidgetClass,form,arg,i); 

	i = 0; 
	XtSetArg(arg[i], XtNwidth, (XtArgVal) N200_H_PIXELS); i++; 
	XtSetArg(arg[i],XtNorientation, (XtArgVal) OL_HORIZONTAL); i++;
	XtSetArg(arg[i],XtNsliderMax, (XtArgVal) ncolors); i++;
	XtSetArg(arg[i],XtNgranularity, (XtArgVal) 1); i++;
	XtSetArg(arg[i], XtNticks, (XtArgVal) 1); i++; 
	XtSetArg(arg[i], XtNtickUnit, (XtArgVal) OL_SLIDERVALUE); i++; 
	XtSetArg(arg[i], XtNdragCBType, (XtArgVal) OL_RELEASE); i++; 
	w = XtCreateManagedWidget("slider",
		sliderWidgetClass,slider_caption,arg,i);
	XtAddCallback(w,XtNsliderMoved,sliderCB,NULL);
}

/*
 * GAUGE
 */

	i = 0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Gauge"); i++;
	g_caption = XtCreateManagedWidget("g_caption",
		captionWidgetClass,form,arg,i); 

	i = 0; 
	XtSetArg(arg[i],XtNwidth,(XtArgVal) N200_H_PIXELS); i++; 
	XtSetArg(arg[i],XtNorientation, (XtArgVal) OL_HORIZONTAL); i++;
	XtSetArg(arg[i],XtNsliderMax, (XtArgVal) 100); i++;
	XtSetArg(arg[i],XtNgranularity, (XtArgVal) 10); i++;
	XtSetArg(arg[i], XtNticks, (XtArgVal) 10); i++; 
	XtSetArg(arg[i], XtNtickUnit, (XtArgVal) OL_SLIDERVALUE); i++; 
	gauge = XtCreateManagedWidget("gauge",gaugeWidgetClass,g_caption,arg,i);

/* 
 * GADGETS: the oblongbutton gadget and abbreviatedmenubutton
 * gadget are displayed in a bulletinboard, the latter in a 
 * caption entitled, "Gadgets."
 */

{
	Widget carea,button,menubutton,menupane;

	i=0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Gadgets"); i++;
	gd_caption = XtCreateManagedWidget("gd_caption",
		captionWidgetClass,form,arg,i);

	i=0;
	XtSetArg(arg[i],XtNborderWidth, (XtArgVal) 2); i++;
	XtSetArg(arg[i],XtNborderColor, (XtArgVal) orange_pixel); i++;
	carea = XtCreateManagedWidget("controlarea",
		controlAreaWidgetClass,gd_caption,arg,i);

	i=0;
	XtSetArg(arg[i],XtNlabel, (XtArgVal) "Oblongbutton" ); i++;
	XtSetArg(arg[i],XtNx,(XtArgVal) N10_H_PIXELS); i++;
	XtSetArg(arg[i], XtNy,(XtArgVal) N10_V_PIXELS); i++;
	button = XtCreateManagedWidget("buttongadget",
		oblongButtonGadgetClass,carea,arg,i);
 	XtAddCallback(button,XtNselect,genericCB,(XtPointer) 9);

	i = 0;
	XtSetArg(arg[i],XtNlabel, (XtArgVal) "Menubutton"); i++;
	XtSetArg(arg[i],XtNx,(XtArgVal) N100_H_PIXELS);
	XtSetArg(arg[i],XtNy,(XtArgVal) N10_V_PIXELS); i++;
	menubutton = XtCreateManagedWidget("menubutton",
		menuButtonGadgetClass,carea,arg,i);
	i = 0;
	XtSetArg(arg[i], XtNmenuPane,(XtArgVal) &menupane); i++;
	XtGetValues(menubutton,arg,i);

	i = 0;
	button = XtCreateManagedWidget("Button 1",
		oblongButtonGadgetClass,menupane,arg,i);
 	XtAddCallback(button,XtNselect,menuSelectCB,(XtPointer) 1);

	i = 0;
	button = XtCreateManagedWidget("Button 2",
		oblongButtonGadgetClass,menupane,arg,i);
 	XtAddCallback(button,XtNselect,menuSelectCB,(XtPointer) 2);
}

/* 
 * FLAT WIDGETS: the flatexclusives, flatnonexclusives, and
 * flatcheckbox are displayed in a bulletinboard, the latter
 * in a caption entitled, "Flat Widgets."
 */

{
	Widget widget,carea;
	WidgetClass class;
	int num_subobjects = 3;
	Dimension hspace = N10_H_PIXELS,
	          vspace = N10_V_PIXELS;

/*
 * For flatexclusives and flatnonexclusives: note "customized" fields.
 */

	static String fields1[] = {
		XtNbackground, XtNlabel, XtNselectProc, XtNclientData
	};

	typedef struct {
		XtArgVal	background;	/* item's background */
		XtArgVal	label;		/* item's label */
		XtArgVal	select;		/* item's select callback */
		XtArgVal	clientData;	/* clientData for callback */
	} FlatData1;

	static FlatData1 *items1;

/*
 * For flatcheckbox: note "customized" fields.
 */

	static String fields2[] = {
		XtNbackground, XtNlabel, XtNset, XtNselectProc, XtNclientData
	};

	typedef struct {
		XtArgVal	background;	/* item's background */
		XtArgVal	label;		/* item's label */
		XtArgVal	set;		/* item's set status */
		XtArgVal	select;		/* item's select callback */
		XtArgVal	clientData;	/* clientData for callback */
	} FlatData2;

	static FlatData2 *items2;

	items1 = (FlatData1 *)XtMalloc((Cardinal)
		(num_subobjects*sizeof(FlatData1)));

	items2 = (FlatData2 *)XtMalloc((Cardinal)
		(num_subobjects*sizeof(FlatData2)));

	i=0;
	XtSetArg(arg[i],XtNposition,(XtArgVal) OL_TOP); i++;
	XtSetArg(arg[i],XtNalignment,(XtArgVal) OL_CENTER); i++;
	XtSetArg(arg[i],XtNlabel,(XtArgVal) "Flat Widgets"); i++;
	f_caption = XtCreateManagedWidget("f_caption",
		captionWidgetClass,form,arg,i);

	i=0;
	XtSetArg(arg[i],XtNborderWidth,(XtArgVal) 2); i++;
	XtSetArg(arg[i],XtNborderColor,(XtArgVal) orange_pixel); i++;
	XtSetArg(arg[i],XtNhSpace,hspace); i++;
	XtSetArg(arg[i],XtNvSpace,vspace); i++;
	XtSetArg(arg[i],XtNhPad,hspace); i++;
	XtSetArg(arg[i],XtNvPad,vspace); i++;

	carea = XtCreateManagedWidget("controlarea",
		controlAreaWidgetClass,f_caption,arg,i);

	widget =XtCreateManagedWidget("Exclusives: ",
		captionWidgetClass,carea,NULL,0);

	items1[0].background = (XtArgVal) red_pixel;
	items1[1].background = (XtArgVal) green_pixel;
	items1[2].background = (XtArgVal) blue_pixel;
	items1[0].label = 
		items1[1].label = items1[2].label = (XtArgVal) "  ";
	items1[0].select = items1[1].select = items1[2].select
			= (XtArgVal) genericCB;
	items1[0].clientData = 
		items1[1].clientData = items1[2].clientData = (XtArgVal) 10;

	class = flatExclusivesWidgetClass;
	i=0;
	XtSetArg(arg[i],XtNitems,items1);i++;
	XtSetArg(arg[i],XtNnumItems,num_subobjects);i++;
	XtSetArg(arg[i],XtNitemFields,fields1); i++;
	XtSetArg(arg[i],XtNnumItemFields,XtNumber(fields1)); i++;
	XtCreateManagedWidget("flatexclusives",class,widget,arg,i);

	widget =XtCreateManagedWidget("Nonexclusives: ",
		captionWidgetClass,carea,NULL,0);

	class = flatNonexclusivesWidgetClass;
	XtCreateManagedWidget("flatnonexclusives",class,widget,arg,i);

	items2[0].background = (XtArgVal) red_pixel;
	items2[1].background = (XtArgVal) green_pixel;
	items2[2].background = (XtArgVal) blue_pixel;
	items2[0].label = (XtArgVal) "CheckBox1";
	items2[1].label = (XtArgVal) "CheckBox2";
	items2[2].label = (XtArgVal) "CheckBox3";
	items2[0].set = items2[1].set = items2[2].set = (XtArgVal) TRUE;
	items2[0].select = items2[1].select = items2[2].select
			= (XtArgVal) genericCB;
	items2[0].clientData = 
		items2[1].clientData = items2[2].clientData = (XtArgVal) 10;

	class = flatCheckBoxWidgetClass;
	XtSetArg(arg[i],XtNitems,items2);i++;
	XtSetArg(arg[i],XtNnumItems,num_subobjects);i++;
	XtSetArg(arg[i],XtNitemFields,fields2); i++;
	XtSetArg(arg[i],XtNnumItemFields,XtNumber(fields2)); i++;
	XtCreateManagedWidget("flatcheckbox",class,carea,arg,i);
}

/* 
 * SCROLLBAR: attach to the right & bottom of the form.
 */

{

static Arg scrollbarARGS[] = {

/* 
 * Widget resources. 
 */

	{ XtNproportionLength, 10 },
	{ XtNshowPage, OL_LEFT },

/* 
 * Form resources. 
 */

	{ XtNyResizable,(XtArgVal) TRUE },
	{ XtNyAttachBottom,(XtArgVal)TRUE },
	{ XtNxAttachRight,(XtArgVal) TRUE },
	{ XtNxVaryOffset,(XtArgVal) TRUE },
};

	scrollbar = XtCreateManagedWidget("scrollbar",
		scrollbarWidgetClass,form,scrollbarARGS,
		XtNumber(scrollbarARGS));
	XtAddCallback(scrollbar,XtNsliderMoved,scrollbarCB,(XtPointer) 0);
}

/*
 * Position all the widgets on the form.  Note that reference names are used.
 * Note that 10 horizontal and vertical pixels are used in most cases; in a  
 * few cases special values are used.
 */

{
	Arg arg;
	static Dimension width1, width2;
	static Position x1, x2;

	static Arg getArgs1[] = {
		{ XtNx, (XtArgVal) &x1},
		{ XtNwidth, (XtArgVal) &width1}
	};
		
	static Arg getArgs2[] = {
		{ XtNx, (XtArgVal) &x2},
		{ XtNwidth, (XtArgVal) &width2}
	};

	genericARGS[2].value = (XtArgVal) 0;
	genericARGS[3].value = (XtArgVal) 0;
	SetPosition(ca_caption,"form","form");
	genericARGS[2].value = (XtArgVal) N10_H_PIXELS;
	genericARGS[3].value = (XtArgVal) N10_V_PIXELS;

	SetPosition(caption,"form","ca_caption");
	SetPosition(stub,"form","caption");

/*
 * Position bulletinboard to right of caption or stub, whichever is wider.
 */

	XtSetArg(arg,XtNwidth,&width1);
	XtGetValues(caption,&arg,1);
	XtSetArg(arg,XtNwidth,&width2);
	XtGetValues(stub,&arg,1);
	if(width1>width2)
		SetPosition(bb_caption,"caption","ca_caption");
	else
		SetPosition(bb_caption,"stub","ca_caption");

	SetPosition(slider_caption,"form","stub");
	SetPosition(g_caption,"form","slider_caption");
	SetPosition(f_caption,"form","gd_caption");
	SetPosition(slist_caption,"bb_caption","ca_caption");
	SetPosition(sw_caption,"slist_caption","ca_caption");

	genericARGS[2].value = (XtArgVal) (7 * N10_H_PIXELS);
	SetPosition(gd_caption,"slider_caption","slist_caption");
	genericARGS[2].value = (XtArgVal) N10_H_PIXELS;

/*
 * Position scrollbar to right of flat widgets or scrolledwindow,
 * whichever extends further to the right of the form.
 */
	genericARGS[3].value = (XtArgVal) 0;

	XtGetValues(f_caption,getArgs1,XtNumber(getArgs1));
	XtGetValues(sw_caption,getArgs2,XtNumber(getArgs2));

	width1 = width1 + (Dimension) x1;
	width2 = width2 + (Dimension) x2;

	if(width1>width2)
		SetPosition(scrollbar,"f_caption","ca_caption");
	else
		SetPosition(scrollbar,"sw_caption","ca_caption");

	genericARGS[3].value = (XtArgVal) N10_V_PIXELS;
}

/*
 * Realize the widget tree.
 */

	XtRealizeWidget(toplevel); 

/* 
 * The special cursor for the stub widget can be set now
 * that it has been realized as part of the widget tree. 
 */

	SetStubCursor(stub);

/* 
 * Turn control over to the Xt intrinsics and OPEN LOOK.
 */

	XtMainLoop();

} /* MAIN */
