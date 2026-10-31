

/*
 * screen data (relevant to the dst wmgr)
 */
struct sdata {
	uint	sd_nwin;
};

/*
 * window data
 */
struct wdata {
	uint	wd_client,
		wd_xorg,
		wd_yorg,
		wd_xlen,
		wd_ylen,
		wd_nrect;
};

/*
 * rectangle data
 */
struct rdata {
	uint	rd_xorg,
		rd_yorg,
		rd_xlen,
		rd_ylen;
	bool	rd_hidden;
};


#define	SSTOP		0
#define	SSTOPPED	1
#define	SNEWR		2
#define	SGOTR		3
#define	SWANTT		4
#define	STOKEN		5
#define	SKEY		6
#define	STYPE		7
