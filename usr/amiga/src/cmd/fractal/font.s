








#
# FONT INFORMATION
#
# WARNING: the layouts of (struct charent) and (struct font) are also
# defined in screen.h, which must always be kept in sync with these
# definitions.
#

set structoff,0
	set ce_height,structoff;set structoff,structoff+1
	set ce_width,structoff;set structoff,structoff+1
	set ce_basel,structoff;set structoff,structoff+1
	set ce_move,structoff;set structoff,structoff+1
	set ce_bpl,structoff;set structoff,structoff+1
	set ce_pad,structoff;set structoff,structoff+1
	set ce_data,structoff;set structoff,structoff+2
set sizeof_charent,structoff

set structoff,0
	set f_magic,structoff;set structoff,structoff+2
	set f_flags,structoff;set structoff,structoff+2
	set f_count,structoff;set structoff,structoff+2
	set f_pad0,structoff;set structoff,structoff+2
	set f_length,structoff;set structoff,structoff+4
	set f_height,structoff;set structoff,structoff+1
	set f_width,structoff;set structoff,structoff+1
	set f_baseline,structoff;set structoff,structoff+1
	set f_pad1,structoff;set structoff,structoff+1
	set f_ctable,structoff;set structoff,structoff+(sizeof_charent*256)
set sizeof_font,structoff

# font.f_magic



#
# BITMAP INFORMATION
#
# WARNING: the layout of (struct bitmap) is also defined in screen.h,
# which must always be kept in sync with these definitions.
#

			# Max bitplanes in a bitmap




set structoff,0
	set bm_flags,structoff;set structoff,structoff+2
	set bm_width,structoff;set structoff,structoff+2
	set bm_height,structoff;set structoff,structoff+2
	set bm_depth,structoff;set structoff,structoff+2
	set bm_offset,structoff;set structoff,structoff+2
	set bm_pad0,structoff;set structoff,structoff+2
	set bm_bpl,structoff;set structoff,structoff+(4*8)
set sizeof_bitmap,structoff


#
# ANSI ATTRIBUTE INFORMATION
#









# void scrcon_putch(struct bitmap *bp, short x, short y,
#	     struct font *fp, unsigned char c, unsigned char attr);

#	08(fp) = struct bitmap *bp
#	0c(fp) = short x
#	10(fp) = short y
#	14(fp) = struct font *fp
#	18(fp) = unsigned char c
#	1c(fp) = unsigned char c

#	a0 = fp
#	a1 = charent
#	d0 = c
#	a0 = bp
#	d1 = x
#	d2 = charwidth
#	d3 = charheight
#	a0 = bitmap line pointer
#	a1 = bitmap width
#	a2 = chardata
#	a3 = bitmap

	global	scrcon_putch
scrcon_putch:
	link.w	%fp,&-16
	movm.l	&0x0c0c,(%sp)
	mov.l	0x14(%fp),%a0				# font
	mov.w	0x1a(%fp),%d0				# c
	lea.l	f_ctable(%d0.w*8,%a0),%a1		# f_charent
	mov.w	ce_data(%a1),%a2			# fontdata offset
	add.l	%a0,%a2					# fontdata address
	clr.l	%d2
	mov.b	ce_width(%a1),%d2			# charwidth
	swap	%d2					# save for later
	mov.b	ce_bpl(%a1),%d2				# bytes/line
	clr.l	%d3
	mov.b	ce_height(%a1),%d3			# charheight
	mov.b	f_baseline(%a0),%d0
	sub.b	ce_basel(%a1),%d0
	ext.w	%d0
	add.w	0x12(%fp),%d0				# ypos-=f_base+ce_base
	mov.l	0x08(%fp),%a3				# bitmap
	mov.w	bm_height(%a3),%d1
	sub.w	%d0,%d1					# bm_height-ypos
	bls	done					# off end of logical bitmap?
	cmp.w	%d1,%d3					# partially off end?
	bcc	L%0
	mov.w	%d1,%d3
L%0:	add.w	bm_offset(%a3),%d0			# add offset
	mov.w	bm_height(%a3),%d1
	sub.w	%d0,%d1					# wrapped around physical bitmap?
	bhi	L%1
	add.w	bm_height(%a3),%d1
	sub.w	bm_height(%a3),%d0
L%1:	sub.w	%d3,%d1
	bcc	L%2
	add.w	%d1,%d3
	swap	%d3
	sub.w	%d1,%d3
	swap	%d3
L%2:	mov.w	bm_width(%a3),%d1			# bm_width
	lsr.w	&3,%d1					# pixels->bytes
	mov.w	%d1,%a1					# Save in a1
	mulu.w	%d1,%d0					# bm_width*line# = pos
	add.l	bm_bpl(%a3),%d0				# bitmap.bpl[0]
	mov.l	%d0,%a0					# bitplane line pointer
	mov.l	0x0c(%fp),%d1				# xpos

doit:	subq.w	&1,%d3
	bcs.b	done

	btst	&7,0x1f(%fp)
	bne	L%4
	mov.l	pointers(%d2.w*4),%d0
	swap	%d2
	jmp	(%d0.l)
L%4:	mov.l	ipointers(%d2.w*4),%d0
	swap	%d2
	jmp	(%d0.l)

byteloop:
	mov.b	(%a2)+,%d0		# fetch next line from font
	bfins	%d0,(%a0){%d1:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,byteloop		# count lines
	bra	maybewrap

wordloop:
	mov.w	(%a2)+,%d0		# fetch next line from font
	bfins	%d0,(%a0){%d1:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,wordloop		# count lines
	bra	maybewrap

longloop:
	mov.l	(%a2)+,%d0		# fetch next line from font
	bfins	%d0,(%a0){%d1:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,longloop		# count lines
maybewrap:
	mov.l	bm_bpl(%a3),%a0
	clr.w	%d3
	swap	%d2
	swap	%d3
	bne	doit
done:	unlk	%fp
	movm.l	-20(%sp),&0x0c0c
	rts

ibyteloop:
	mov.b	(%a2)+,%d0		# fetch next line from font
	not.b	%d0
	bfins	%d0,(%a0){%d1:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,ibyteloop		# count lines
	bra	maybewrap

iwordloop:
	mov.w	(%a2)+,%d0		# fetch next line from font
	not.w	%d0
	bfins	%d0,(%a0){%d1:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,iwordloop		# count lines
	bra	maybewrap

ilongloop:
	mov.l	(%a2)+,%d0		# fetch next line from font
	not.l	%d0
	bfins	%d0,(%a0){%d1:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,ilongloop		# count lines
	bra	maybewrap

pointers:
	long	done,byteloop,wordloop,done,longloop,done,done,done
ipointers:
	long	done,ibyteloop,iwordloop,done,ilongloop,done,done,done


# void scrcon_cursor(struct bitmap *bp, short x, short y,
#		     struct font *fp);

#	08(fp) = struct bitmap *bp
#	0c(fp) = short x
#	10(fp) = short y
#	14(fp) = struct font *fp

	global	scrcon_cursor
scrcon_cursor:
	link.w	%fp,&-16
	movm.l	&0x0c0c,(%sp)
	mov.l	0x14(%fp),%a0				# font
	clr.l	%d2
	mov.b	f_width(%a0),%d2			# fontwidth
	clr.l	%d3
	mov.b	f_height(%a0),%d3			# fontheight
	mov.w	0x12(%fp),%d0				# ypos
	mov.l	0x08(%fp),%a3				# bitmap
	mov.w	bm_height(%a3),%d1
	sub.w	%d0,%d1					# bm_height-ypos
	bls	done					# off end of logical bitmap?
	cmp.w	%d1,%d3					# partially off end?
	bcc	L%10
	mov.w	%d1,%d3
L%10:	add.w	bm_offset(%a3),%d0			# add offset
	mov.w	bm_height(%a3),%d1
	sub.w	%d0,%d1					# wrapped around physical bitmap?
	bhi	L%11
	add.w	bm_height(%a3),%d1
	sub.w	bm_height(%a3),%d0
L%11:	sub.w	%d3,%d1
	bcc	L%12
	add.w	%d1,%d3
	swap	%d3
	sub.w	%d1,%d3
	swap	%d3
L%12:	mov.w	bm_width(%a3),%d1			# bm_width
	lsr.w	&3,%d1					# pixels->bytes
	mov.w	%d1,%a1					# Save in a1
	mulu.w	%d1,%d0					# bm_width*line# = pos
	add.l	bm_bpl(%a3),%d0				# bitmap.bpl[0]
	mov.l	%d0,%a0					# bitplane line pointer
	mov.l	0x0c(%fp),%d0				# xpos

dorev:	subq.w	&1,%d3
	bcs.b	rdone

revloop:
	bfchg	(%a0){%d0:%d2}
	add.l	%a1,%a0			# bump to next line
	dbf.w	%d3,revloop		# count lines
	mov.l	bm_bpl(%a3),%a0
	clr.w	%d3
	swap	%d3
	bne	dorev
rdone:	unlk	%fp
	movm.l	-20(%sp),&0x0c0c
	rts
