#!/usr/bin/sh
#
# Patchlevel: .xsession_1.2
#
# Default .xsession
# Edit to start your favorite window manager and
# X applications each time you log into xdm.
#
#

PATH=$PATH:/usr/X/bin export PATH
resources=$HOME/.Xresources
if [ -f $resources ]; then
	xrdb -load $resources
fi

# CHOOSE ONE WINDOW MANAGER:

#################################################################
#
#
# Open Look is more sophisticated
#
#
#   F=/usr/X/lib/fonts
#   XWINFONTPATH=$F/Xol,$F/misc,$F/75dpi,$F/100dpi
#   export XWINFONTPATH
#   olwsm &
#   olwm  &  #Do not use this line if it is in your .olinitrc
#   olfm  &  #Do not use this line if it is in your .olinitrc
#
#
# Twm is fast and simple:
    twm &
#
#
#
#
# Motif is available from Digital Micronics, Inc.
#   mwm &
#
#
#
#################################################################



# Start other applications in the background:
#
# xclock &
# xboard &
# xemacs &



# Exec an xterm -ls or olterm last - REQUIRED.
#
exec xterm -geometry 80x24+10+30 -ls


