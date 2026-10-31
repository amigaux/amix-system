#ifndef _AMIGA_SCREEN_H
#define _AMIGA_SCREEN_H

#ifndef MAXBPL
#include <sys/amiga/screen.h>
#endif

struct Bitmap
{
    unsigned short flags;
    unsigned short width, height, depth, offset;
    unsigned short pad0;
    unsigned char *bpl[MAXBPL];
};

extern struct scrtype DefaultScrtype;
#define DEFAULT_SCRTYPE &DefaultScrtype
extern int OpenScreen(const char *title, const struct scrtype *stp, int flags);
extern int ScreenColors(int sfd, const struct scolor colors[]);
extern int NewBitmap(int sfd, struct Bitmap *bp);
extern int DisplayScreen(int sfd);
extern char *ScreenDeviceName(int sfd);
extern int CloseScreen(int sfd);
extern char *ScreenError(void);

#endif /* _AMIGA_SCREEN_H */
