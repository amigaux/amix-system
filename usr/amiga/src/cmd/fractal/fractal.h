#include <sys/types.h>
#include <amiga/screen.h>
#include "pointer.h"

#ifdef TESTING
#define COLOR_PROGRAM	"./scrcols"
#else
#define COLOR_PROGRAM	"/usr/amiga/bin/scrcols"
#endif

/* for fractal_type */
#define MANDELBROT	(0)
#define JULIA		(1)

#define DEFAULT_MANDELBROT_SET { -0.7, 0.0, 0.006519, 0.006519, 0.0, 0.0, 2, }
#define DEFAULT_JULIA_SET { 0.0, 0.0, 0.013, 0.013, -0.12, 0.74, 2, }

struct zoombox
{
    boolean_t			displayed; /* Is box drawn? */
    boolean_t			zooming; /* oh wow man */
    int				x;	/* start x */
    int				y;	/* end x */
    int				width;	/* length of x side */
    int				height;	/* length of y side */
};

struct screen_attributes
{
    int				sd;	/* Screen descriptor */
    struct Bitmap		bp;	/* Bitmap pointer to scrdev */
    struct scolor		*scolor; /* color table */
    int				size;	/* x*y/8 */
    int				base;	/* what we've displayed */
    int				x, y;	/* position in the computed fractal */
    struct pointer		pointer; /* the cursor (if drawn) */
    struct zoombox		zoombox;	/* A zoom box (if drawn) */
    boolean_t			color_cycle; /* Are we to do color cycling */
    boolean_t			slide_window; /* are we moving the window */
    boolean_t			nodisplay; /* disply fracter while computing */
    boolean_t			batch;	/* save and quit when done */
};

struct fractal_coordinates
{
    double			x;
    double			y;
    double			x_increment;
    double			y_increment;
    double			p;
    double			q;
    unsigned int		power;
};

struct fractal_set
{
    int				type;	/* 0=Mandelbrot, 1=Julia */
    struct fractal_coordinates	coordinates;
    int				attractor_point;
    int				max_interrations;
    struct Bitmap		bp;	/* pointer to to computed fractal */
    int				size;	/* x*y/8 -- yeah yeah... your mother */
    int				owned;	/* 0=parrent, 1=computation */
    int				base;	/* where the compute engine is */
};

#define STOPPED			(0)	/* owner when set is displayer */
#define COMPUTING		(1)	/* owner when set is displayer */
#define START_COMPUTATION	(2)	/* owner when set is compute engine */
#define STOP_COMPUTATION	(3)	/* owner when set is compute engine */

#define HACK_MOUSE_DELAY_MOTION	(0)
#define HACK_MOUSE_CLEANUP	(1)

#define ZOOM_BUTTON		(0)
#define SLIDE_BUTTON		(1)

#define EVENT_LIST_SIZE	100

#define META(x)		((x) | 0x80)
#define CTRL(x)		((x) & 0x1F)
#define ESC		0x1B
#define CSI		META(ESC)
#define CR		(CTRL('m'))
#define TAB		(0x09)

#define min(a, b) 	((a) < (b) ? (a) : (b))
#define max(a, b) 	((a) > (b) ? (a) : (b))

extern char *progname;
extern char *gifname;			/* Here until I can get an input */
					/* routine working. */
extern void process_input_events(struct fractal_set *,
				 struct screen_attributes *,
				 int);

extern boolean_t update_screen(struct fractal_set *,
			       struct screen_attributes *);
extern pid_t fork_computation_process(struct fractal_set *,
				      struct screen_attributes *);
extern void clean_exit(int);

extern void display_zoombox(struct screen_attributes *);
extern void resize_zoombox(struct screen_attributes *, int, int);
extern void move_zoombox(struct screen_attributes *, int, int);
extern void remove_zoombox(struct screen_attributes *);

extern void start_fractal_computation(struct fractal_set *,
				      struct screen_attributes *);
extern void stop_fractal_computation(struct fractal_set *);
extern void slide_window(struct fractal_set *,
			 struct screen_attributes *,
			 int,
			 int);

extern int set_colors(struct screen_attributes *);
extern void cycle_colors(struct screen_attributes *);
extern void select_colors(struct screen_attributes *);

extern void save_fractal(struct screen_attributes *,
			 struct fractal_set *,
			 char *);
