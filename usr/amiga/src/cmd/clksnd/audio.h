#ifndef _AUDIO_H
#define _AUDIO_H

#define MAXCHANNELS	(4)

#ifdef _KERNEL
#define AUD_COPEN	(0x01)		/* client open */
#define AUD_CCLOSING	(0x02)		/* client in close */
#define AUD_CATTACHED	(0x04)		/* q attached to audio port */

struct audio_client
{
    int			flags;
    int			ports[2];	/* ports used */
    unsigned short	volume;
    unsigned short	period;
    queue_t		*q;
    struct audio_client *next;
};

struct audio_channel
{
    int				dmadone; /* AMIGA->intreqr; AUD block done */
    int				enable;	/* AMIGA->dmacon; AUD chan enable */
    queue_t			*wrq;	/* write queue used for this chan */
    struct audio_client		*client;
    int				pending; /* pending sounds */
    int				used;	/* buffer used */
    int				size;	/* size of buffer */
    signed char			*buffer[2]; /* data for audio */
};
#endif /* _KERNEL */

#define AUDIO_BUFFER_SIZE	8192

#define	AUIOC			('U'<<8)
#define AUDIO_VOLUME		(AUIOC|1)
#define AUDIO_PERIOD		(AUIOC|2)

#define BLOUD 63
#define BPER  400

#endif /* _AUDIO_H */
