struct pointer
{
    boolean_t			displayed;
    int				x, y;
    int				width, height;
    int				minx, miny, maxx, maxy;
};

extern void display_pointer(struct Bitmap *, struct pointer *);
extern void move_pointer(struct Bitmap *, struct pointer *, int, int);
extern void remove_pointer(struct Bitmap *, struct pointer *);
