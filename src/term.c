/*
 Pirate Bulletin Board System
 Copyright (C) 1990, Edward Luke, lush@Athena.EE.MsState.EDU
 Eagles Bulletin Board System
 Copyright (C) 1992, Raymond Rocker, rocker@rock.b11.ingr.com
 Guy Vega, gtvega@seabass.st.usm.edu
 Dominic Tynes, dbtynes@seabass.st.usm.edu
 Firebird Bulletin Board System
 Copyright (C) 1996, Hsien-Tsung Chang, Smallpig.bbs@bbs.cs.ccu.edu.tw
 Peng Piaw Foong, ppfoong@csie.ncu.edu.tw

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 1, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 */
/*
 $Id: term.c 2 2005-07-14 15:06:08Z root $
 */
#include "bbs.h"
#include <sys/ioctl.h>

#ifdef HP_UX
#define O_HUPCL 01
#define O_XTABS 02
#endif

#ifdef TERMIOS
#include <termios.h>
#define stty(fd, data) tcsetattr( fd, TCSANOW, data )
//����fd���������ն�����Ϊtermios�ṹ��ָ��data,��������Ч(TCSANOW)
#define gtty(fd, data) tcgetattr( fd, data )
//ȡ��fd���������ն˵�����,�������data��ָ��Ľṹ��
struct termios tty_state, tty_new;
#else
struct sgttyb tty_state, tty_new;
#endif

#ifndef TANDEM
#define TANDEM	0x00000001
#endif

#ifndef CBREAK
#define CBREAK  0x00000002
#endif

#ifdef TERMIOS
//	��ʼ��tty����
void init_tty()
{
	long vdisable;
	memcpy(&tty_new, &tty_state, sizeof(tty_new));
	tty_new.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ISIG);
	tty_new.c_cflag &= ~CSIZE;
	tty_new.c_cflag |= CS8;
	tty_new.c_cc[VMIN] = 1;
	tty_new.c_cc[VTIME] = 0;
	if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) >= 0) {
		//fpathconf(fd,name) ȡ��������ļ�������fd������ѡ��name��ֵ
		//_PC_VDISABLE returns nonzero if special character processing can be disabled
		tty_new.c_cc[VSTART] = vdisable;
		tty_new.c_cc[VSTOP] = vdisable;
		tty_new.c_cc[VLNEXT] = vdisable;
	}
	tcsetattr(1, TCSANOW, &tty_new);
	//�൱��stty(1,&tty_new);
}
#else
//��ʼ���ն�,�趨�ն˵�����
void init_tty() {
	memcpy(&tty_new, &tty_state, sizeof(tty_new));
	tty_new.sg_flags |= RAW;

#ifdef HP_UX
	tty_new.sg_flags &= ~(O_HUPCL | O_XTABS | LCASE | ECHO | CRMOD);
#else
	tty_new.sg_flags &= ~(TANDEM | CBREAK | LCASE | ECHO | CRMOD);
#endif

	stty(1, &tty_new);
}
#endif

#define TERMCOMSIZE (1024)

int dumb_term = YEA; //���ն����ó���

char clearbuf[TERMCOMSIZE];
int clearbuflen;

char cleolbuf[TERMCOMSIZE];
int cleolbuflen;

char cursorm[TERMCOMSIZE];
char *cm;

char scrollrev[TERMCOMSIZE];
int scrollrevlen;

char strtstandout[TERMCOMSIZE];
int strtstandoutlen;

char endstandout[TERMCOMSIZE];
int endstandoutlen;

int t_lines = 24; //�ն˵�����
int t_columns = 255; //�ն˵�����

int automargins; //�������߽�,�Ƿ��Զ�ת����һ��

char *outp;
int *outlp; //�����ָ��

//��ch����outlp��ָ����ַ�������
static outcf(char ch) {
	if (*outlp < TERMCOMSIZE) {
		(*outlp)++;
		*outp++ = ch;
	}
}

//�ն˳�ʼ������,�ɹ����term�����ʱ,�����ƺ�������YEA,ʧ�ܷ���NA
int term_init(char *term) {
	extern char PC, *UP, *BC;
	extern short ospeed;
	static char UPbuf[TERMCOMSIZE];
	static char BCbuf[TERMCOMSIZE];
	static char buf[5120];
	char sbuf[5120];
	char *sbp, *s;
	char *tgetstr();
#ifdef TERMIOS
	ospeed = cfgetospeed(&tty_state); //����tty_state���Ե��ն˴�������
#else
	ospeed = tty_state.sg_ospeed;
#endif

	if (tgetent(buf, term) != 1) //���term�����,�ɹ�����1,ʧ����0,�������ݿ�
		//	����-1;	buf�ƺ���������
		return NA;

	sbp = sbuf;
	s = tgetstr("pc", &sbp);/* get pad character */
	//����"pc"���ַ������,������0��ʾ������
	//�ַ������Ҳ������sbp��,��null����
	if (s)
		PC = *s; //PC is set by tgetent to the terminfo entry's data for pad_char
	t_lines = tgetnum("li"); //tgetnum����"li"����ֵ���,����ֵΪ-1��ʾ������
	t_columns = tgetnum("co");
	automargins = tgetflag("am");//tgetflag����"am"�Ĳ���ֵ���,0��ʾ������
	outp = clearbuf; /* fill clearbuf with clear screen command */
	outlp = &clearbuflen;
	clearbuflen = 0;
	sbp = sbuf;
	s = tgetstr("cl", &sbp); //returns the string entry for "cl",Use tputs() to output
	//the rerurned string.The return value will also be copied
	//to the buffer pointed to by "sbp".
	if (s)
		tputs(s, t_lines, outcf); //������ص��ַ�ֵ
	outp = cleolbuf; /* fill cleolbuf with clear to eol command */
	outlp = &cleolbuflen;
	cleolbuflen = 0;
	sbp = sbuf;
	s = tgetstr("ce", &sbp);
	if (s)
		tputs(s, 1, outcf);
	outp = scrollrev;
	outlp = &scrollrevlen;
	scrollrevlen = 0;
	sbp = sbuf;
	s = tgetstr("sr", &sbp);
	if (s)
		tputs(s, 1, outcf);
	outp = strtstandout;
	outlp = &strtstandoutlen;
	strtstandoutlen = 0;
	sbp = sbuf;
	s = tgetstr("so", &sbp);
	if (s)
		tputs(s, 1, outcf);
	outp = endstandout;
	outlp = &endstandoutlen;
	endstandoutlen = 0;
	sbp = sbuf;
	s = tgetstr("se", &sbp);
	if (s)
		tputs(s, 1, outcf);
	sbp = cursorm;
	cm = tgetstr("cm", &sbp);
	if (cm)
		dumb_term = NA;
	else
		dumb_term = YEA;
	sbp = UPbuf;
	UP = tgetstr("up", &sbp);
	sbp = BCbuf;
	BC = tgetstr("bc", &sbp);
	if (dumb_term) {
		t_lines = 24;
		t_columns = 255;
	}

	return YEA;
}

//�ƶ���λ��(destline,destcol),ͬʱ��outc���
void do_move(int destcol, int destline, int (*outc)()) {
	tputs(tgoto(cm, destcol, destline), 0, outc);
	//tgoto�Դ��ݽ����Ĳ���ʵ�ֶ�Ӧ������,���ѽ����tputs����
	//tputs����outc�������tgoto�������Ĳ���
}

