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
 $Id: vote.c 317 2006-10-27 13:20:07Z danielfree $
 */

#include "bbs.h"
#include "vote.h"

extern cmpbnames();
extern int page, range;
extern struct boardheader *bcache;
extern struct BCACHE *brdshm;
static char *vote_type[] = { "�Ƿ�", "��ѡ", "��ѡ", "����", "�ʴ�" };
struct votebal currvote; //��ǰͶƱ
char controlfile[STRLEN];
unsigned int result[33]; //ͶƱ�������
int vnum;
int voted_flag;
FILE *sug; //ͶƱ������ļ�ָ��
int makevote(struct votebal *ball, char *bname); //����ͶƱ��

//Added by IAMFAT 2002.06.13
extern void ellipsis(char *str, int len); //��ʡ�Ժ�

//Added End
//commented by jacobson

//���ļ���Ҫ����ͶƱ����

//�Ƚ��ַ���userid��ͶƱ��uv 
//userid:�û��� uv:ͶƱ�� 
//����ֵ:0���ȣ� 1���
int cmpvuid(char *userid, struct ballot *uv) {
	return !strcmp(userid, uv->uid);
}

//���ð���ͶƱ�ı�־,           
//bname:������,flag�����־
//1:����ͶƱ,0:�ر�ͶƱ ����ֵ:��..
int setvoteflag(char *bname, int flag) {
	int pos;
	struct boardheader fh;

	pos = search_record(BOARDS, &fh, sizeof(fh), cmpbnames, bname);
	if (flag == 0)
		fh.flag = fh.flag & ~BOARD_VOTE_FLAG;
	else
		fh.flag = fh.flag | BOARD_VOTE_FLAG;
	if (substitute_record(BOARDS, &fh, sizeof(fh), pos) == -1)
		prints("Error updating BOARDS file...\n");
}

//��ʾbug����(Ŀǰ����û��ʵ��)
//str:������Ϣ�ַ���
void b_report(char *str) {
	char buf[STRLEN];

	sprintf(buf, "%s %s", currboard, str);
	report(buf);
}

//����Ŀ¼,Ŀ¼Ϊ vote/����,Ȩ��Ϊ755
//bname:��������
void makevdir(char *bname) {
	struct stat st;
	char buf[STRLEN];

	sprintf(buf, "vote/%s", bname);
	if (stat(buf, &st) != 0)
		mkdir(buf, 0755);
}

//�����ļ���
//bname��������
//filename:�ļ���
//buf:���ص��ļ���
void setvfile(char *buf, char *bname, char *filename) {
	sprintf(buf, "vote/%s/%s", bname, filename);
}

//���ÿ���controlfile�ļ���Ϊ vote\������\control
void setcontrolfile() {
	setvfile(controlfile, currboard, "control");
}

//�༭��ɾ�����汸��¼
//����ֵ:FULLUPDATE
#ifdef ENABLE_PREFIX
int b_notes_edit()
{
	char buf[STRLEN], buf2[STRLEN];
	char ans[4];
	int aborted;
	int notetype;

	if (!chk_currBM(currBM, 0)) { //����Ƿ����
		return 0;
	}
	clear();
	move(0, 0);
	prints("�趨��\n\n  (1)һ�㱸��¼\n  (2)���ܱ���¼\n");
	prints("  (3)����ǰ׺��\n  (4)�Ƿ�ǿ��ʹ��ǰ׺\n");
	while (1) {
		getdata(7, 0,"��ǰѡ��[1](0~4): ", ans, 2, DOECHO, YEA);
		if (ans[0] == '0')
		return FULLUPDATE;
		if (ans[0] == '\0')
		strcpy(ans, "1");
		if (ans[0] >= '1' && ans[0] <= '4' )
		break;
	}
	makevdir(currboard); //��������¼Ŀ¼
	notetype = ans[0] - '0';
	if (notetype == 2) {
		setvfile(buf, currboard, "secnotes");
	} else if (notetype == 3) {
		setvfile(buf, currboard, "prefix");
	} else if (notetype == 1) {
		setvfile(buf, currboard, "notes");
	} else if (notetype == 4 ) {
		int pos;
		struct boardheader fh;
		pos = search_record(BOARDS, &fh, sizeof(fh), cmpbnames, currboard);

		if (askyn("�Ƿ�ǿ��ʹ��ǰ׺��", (fh.flag & BOARD_PREFIX_FLAG)?YEA:NA,NA)) {
			fh.flag |= BOARD_PREFIX_FLAG;
		} else {
			fh.flag &= ~BOARD_PREFIX_FLAG;
		}
		substitute_record(BOARDS, &fh, sizeof(fh), pos);
		return FULLUPDATE;
	}
	sprintf(buf2, "(E)�༭ (D)ɾ�� %4s? [E]: ",
			(notetype == 3)?"����ǰ׺��":(notetype == 1) ? "һ�㱸��¼" : "���ܱ���¼");
	getdata(8, 0, buf2, ans, 2, DOECHO, YEA); //ѯ�ʱ༭����ɾ��
	if (ans[0] == 'D' || ans[0] == 'd') { //ɾ������¼
		move(9, 0);
		sprintf(buf2, "���Ҫɾ��ô��");
		if (askyn(buf2, NA, NA)) {
			move(10, 0);
			prints("�Ѿ�ɾ��...\n");
			pressanykey();
			unlink(buf);
			aborted = 1;
		} else
		aborted = -1;
	} else
	aborted = vedit(buf, NA, YEA); //�༭����¼
	if (aborted == -1) {
		pressreturn();
	} else {
		if (notetype == 1)
		setvfile(buf, currboard, "noterec");
		else
		setvfile(buf, currboard, "notespasswd");
		unlink(buf);
	}

	return FULLUPDATE;
}
#else
int b_notes_edit() {
	char buf[STRLEN], buf2[STRLEN];
	char ans[4];
	int aborted;
	int notetype;
	if (!chk_currBM(currBM, 0)) { //����Ƿ����
		return 0;
	}
	clear();
	move(1, 0);
	prints("�༭/ɾ������¼"); //ѯ�ʱ༭���ֱ���¼
	while (1) {
		getdata(3, 0, "�༭��ɾ������������ (0) �뿪  (1) һ�㱸��¼  (2) ���ܱ���¼? [1] ",
				ans, 2, DOECHO, YEA);
		if (ans[0] == '0')
			return FULLUPDATE;
		if (ans[0] == '\0')
			strcpy(ans, "1");
		if (ans[0] == '1' || ans[0] == '2')
			break;
	}
	makevdir(currboard); //��������¼Ŀ¼
	if (ans[0] == '2') {
		setvfile(buf, currboard, "secnotes");
		notetype = 2;
	} else {
		setvfile(buf, currboard, "notes");
		notetype = 1;
	}
	sprintf(buf2, "(E)�༭ (D)ɾ�� %4s����¼? [E]: ", (notetype == 1) ? "һ��"
			: "����");
	getdata(5, 0, buf2, ans, 2, DOECHO, YEA); //ѯ�ʱ༭����ɾ��
	if (ans[0] == 'D' || ans[0] == 'd') { //ɾ������¼
		move(6, 0);
		sprintf(buf2, "���Ҫɾ��%4s����¼", (notetype == 1) ? "һ��" : "����");
		if (askyn(buf2, NA, NA)) {
			move(7, 0);
			prints("����¼�Ѿ�ɾ��...\n");
			pressanykey();
			unlink(buf);
			aborted = 1;
		} else
			aborted = -1;
	} else
		aborted = vedit(buf, NA, YEA); //�༭����¼
	if (aborted == -1) {
		pressreturn();
	} else {
		if (notetype == 1)
			setvfile(buf, currboard, "noterec");
		else
			setvfile(buf, currboard, "notespasswd");
		unlink(buf);
	}

	return FULLUPDATE;
}
#endif 
//�������ܱ���¼����
int b_notes_passwd() {
	FILE *pass;
	char passbuf[20], prepass[20];
	char buf[STRLEN];

	if (!chk_currBM(currBM, 0)) { //����Ƿ����
		return 0;
	}
	clear();
	move(1, 0);
	prints("�趨/����/ȡ�������ܱ���¼������...");
	setvfile(buf, currboard, "secnotes");
	if (!dashf(buf)) {
		move(3, 0);
		prints("�����������ޡ����ܱ���¼����\n\n");
		prints("������ W ��á����ܱ���¼�������趨����...");
		pressanykey();
		return FULLUPDATE;
	}
	if (!check_notespasswd())
		return FULLUPDATE;
	getdata(3, 0, "�������µ����ܱ���¼����(Enter ȡ������): ", passbuf, 19, NOECHO, YEA);
	if (passbuf[0] == '\0') {
		setvfile(buf, currboard, "notespasswd");
		unlink(buf);
		prints("�Ѿ�ȡ������¼���롣");
		pressanykey();
		return FULLUPDATE;
	}
	getdata(4, 0, "ȷ���µ����ܱ���¼����: ", prepass, 19, NOECHO, YEA);
	if (strcmp(passbuf, prepass)) {
		prints("\n���벻���, �޷��趨�����....");
		pressanykey();
		return FULLUPDATE;
	}
	setvfile(buf, currboard, "notespasswd");
	if ((pass = fopen(buf, "w")) == NULL) {
		move(5, 0);
		prints("����¼�����޷��趨....");
		pressanykey();
		return FULLUPDATE;
	}
	fprintf(pass, "%s\n", genpasswd(passbuf));
	fclose(pass);
	move(5, 0);
	prints("���ܱ���¼�����趨���...");
	pressanykey();
	return FULLUPDATE;
}

//��һ���ļ�ȫ������д���Ѿ��򿪵���һ���ļ�
//fp: �Ѿ��򿪵��ļ�ָ��,����д���ļ���
//fname: ��Ҫд����ļ���·��
int b_suckinfile(FILE * fp, char *fname) {
	char inbuf[256];
	FILE *sfp;

	if ((sfp = fopen(fname, "r")) == NULL)
		return -1;
	while (fgets(inbuf, sizeof(inbuf), sfp) != NULL)
		fputs(inbuf, fp);
	fclose(sfp);
	return 0;
}

//��һ���ļ�ȫ������д���Ѿ��򿪵���һ���ļ�,(���ڶ����԰�)
//������ܴ�д��һ������
//fp: �Ѿ��򿪵��ļ�ָ��,����д���ļ���
//fname: ��Ҫд����ļ���·��
/*Add by SmallPig*/
int catnotepad(fp, fname)
FILE *fp;
char *fname;
{
	char inbuf[256];
	FILE *sfp;
	int count;

	count = 0;
	if ((sfp = fopen(fname, "r")) == NULL) {
		fprintf(fp,
				"[1;34m  ��[44m__________________________________________________________________________[m \n\n");
		return -1;
	}
	while (fgets(inbuf, sizeof(inbuf), sfp) != NULL) {
		if (count != 0)
		fputs(inbuf, fp);
		else
		count++;
	}
	fclose(sfp);
	return 0;
}

//�ر�ͶƱ
//����ֵ:�̶�Ϊ0
int b_closepolls() {
	char buf[80];
	time_t now, nextpoll;
	int i, end;

	now = time(0);
	resolve_boards();

	if (now < brdshm->pollvote) { //����ʱ��С���´ο�ͶƱʱ���򷵻أ�
		return;
	}
	//�ر���ʾ ���������Ƶ�miscd
	/*
	 move(t_lines - 1, 0);
	 prints("�Բ���ϵͳ�ر�ͶƱ�У����Ժ�...");
	 refresh();
	 */

	nextpoll = now + 7 * 3600;

	strcpy(buf, currboard);
	for (i = 0; i < brdshm->number; i++) {
		strcpy(currboard, (&bcache[i])->filename);
		setcontrolfile();
		end = get_num_records(controlfile, sizeof(currvote));
		for (vnum = end; vnum >= 1; vnum--) {
			time_t closetime;

			get_record(controlfile, &currvote, sizeof(currvote), vnum);
			closetime = currvote.opendate + currvote.maxdays * 86400;
			if (now > closetime)
				mk_result(vnum); //��ͶƱ�����ѹ�д��ͶƱ���
			else if (nextpoll > closetime)
				nextpoll = closetime + 300;
		}
	}
	strcpy(currboard, buf);

	brdshm->pollvote = nextpoll; //�´ο�ͶƱʱ�䣿
	return 0;
}

//����һ�ε�ͶƱ���,������result������,����mk_result�е�apply_record�����еĻص����� -.-!
//result[32]��¼���ô���
//����ptr:һ�ε�ͶƱ���
//����ֵ:�̶�Ϊ0
int count_result(struct ballot *ptr) {
	int i;

	/*	if (ptr == NULL) {
	 if (sug != NULL) {
	 fclose(sug);
	 sug == NULL;
	 }
	 return 0;
	 }
	 */if (ptr->msg[0][0] != '\0') {
		if (currvote.type == VOTE_ASKING) {
			fprintf(sug, "[1m%s [m���������£�\n", ptr->uid);
		} else
			fprintf(sug, "[1m%s [m�Ľ������£�\n", ptr->uid);
		for (i = 0; i < 3; i++)
			fprintf(sug, "%s\n", ptr->msg[i]);
	}
	result[32]++;
	if (currvote.type == VOTE_ASKING) {
		return 0;
	}
	if (currvote.type != VOTE_VALUE) {
		for (i = 0; i < 32; i++) {
			if ((ptr->voted >> i) & 1)
				(result[i])++;
		}

	} else {
		result[31] += ptr->voted;
		result[(ptr->voted * 10) / (currvote.maxtkt + 1)]++;
	}
	return 0;
}

//��ͶƱ��̧ͷд��sugͶƱ����ļ�
get_result_title() {
	char buf[STRLEN];

	getdatestring(currvote.opendate, NA);
	fprintf(sug, "�� ͶƱ����춣�[1m%s[m  ���[1m%s[m\n", datestring,
			vote_type[currvote.type - 1]);
	fprintf(sug, "�� ���⣺[1m%s[m\n", currvote.title);
	if (currvote.type == VOTE_VALUE)
		fprintf(sug, "�� �˴�ͶƱ��ֵ���ɳ�����[1m%d[m\n\n", currvote.maxtkt);
	fprintf(sug, "�� Ʊѡ��Ŀ������\n\n");
	sprintf(buf, "vote/%s/desc.%d", currboard, currvote.opendate);
	b_suckinfile(sug, buf);
}

//����ͶƱ,����ͶƱ���
//num:ͶƱcontrol�ļ��еڼ�����¼
int mk_result(int num) {
	char fname[STRLEN], nname[STRLEN];
	char sugname[STRLEN];
	char title[STRLEN];
	int i;
	unsigned int total = 0;

	setcontrolfile();
	sprintf(fname, "vote/%s/flag.%d", currboard, currvote.opendate); //ͶƱ��¼�ļ�·��Ϊ vote/����/flag.����ͶƱ��
	/*	count_result(NULL); */
	sug = NULL;
	sprintf(sugname, "vote/%s/tmp.%d", currboard, uinfo.pid); //ͶƱ��ʱ�ļ�·��Ϊ vote/����/tmp.�û�id
	if ((sug = fopen(sugname, "w")) == NULL) {
		report("open vote tmp file error");
		//prints("Error: ����ͶƱ����...\n");
		pressanykey();
	}
	(void) memset(result, 0, sizeof(result));
	if (apply_record(fname, count_result, sizeof(struct ballot), 0, 0, 0)
			== -1) {
		report("Vote apply flag error");
	}
	fprintf(sug, "[1;44;36m������������������������������ʹ����%s������������������������������[m\n\n\n",
			(currvote.type != VOTE_ASKING) ? "��������" : "�˴ε�����");
	fclose(sug);
	sprintf(nname, "vote/%s/results", currboard); //ͶƱ����ļ�·��Ϊ vote/����/results
	if ((sug = fopen(nname, "w")) == NULL) {
		report("open vote newresult file error");
		//prints("Error: ����ͶƱ����...\n");
	}
	get_result_title(sug);
	//����ͶƱ���
	fprintf(sug, "** ͶƱ���:\n\n");
	if (currvote.type == VOTE_VALUE) {
		total = result[32];
		for (i = 0; i < 10; i++) {
			fprintf(
					sug,
					"[1m  %4d[m �� [1m%4d[m ֮���� [1m%4d[m Ʊ  Լռ [1m%d%%[m\n",
					(i * currvote.maxtkt) / 10 + ((i == 0) ? 0 : 1), ((i
							+ 1) * currvote.maxtkt) / 10, result[i],
					(result[i] * 100) / ((total <= 0) ? 1 : total));
		}
		fprintf(sug, "�˴�ͶƱ���ƽ��ֵ��: [1m%d[m\n", result[31]
				/ ((total <= 0) ? 1 : total));
	} else if (currvote.type == VOTE_ASKING) {
		total = result[32];
	} else {
		for (i = 0; i < currvote.totalitems; i++) {
			total += result[i];
		}
		for (i = 0; i < currvote.totalitems; i++) {
			fprintf(sug, "(%c) %-40s  %4d Ʊ  Լռ [1m%d%%[m\n", 'A' + i,
					currvote.items[i], result[i], (result[i] * 100)
							/ ((total <= 0) ? 1 : total));
		}
	}
	fprintf(sug, "\nͶƱ������ = [1m%d[m ��\n", result[32]);
	fprintf(sug, "ͶƱ��Ʊ�� =[1m %d[m Ʊ\n\n", total);
	fprintf(sug, "[1;44;36m������������������������������ʹ����%s������������������������������[m\n\n\n",
			(currvote.type != VOTE_ASKING) ? "��������" : "�˴ε�����");
	b_suckinfile(sug, sugname);
	unlink(sugname); //ɾ��ͶƱ��ʱ�ļ�,����ͶƱ�ļ�д��sugͶƱ����ļ�
	fclose(sug);

	sprintf(title, "[����] %s ���ͶƱ���", currboard);
	Postfile(nname, "vote", title, 1); //ͶƱ�������vote��
	if (currboard != "vote")
		Postfile(nname, currboard, title, 1); //ͶƱ������뵱ǰ��
	dele_vote(num); //�ر�ͶƱ,ɾ����ʱ�ļ�
	return;
}

//ȡ��ѡ�����ѡ��Ŀ,����bal��
//����ֵ num����ѡ��Ŀ��
int get_vitems(struct votebal *bal) {
	int num;
	char buf[STRLEN];

	move(3, 0);
	prints("�����������ѡ����, �� ENTER ����趨.\n");
	num = 0;
	for (num = 0; num < 32; num++) {
		sprintf(buf, "%c) ", num + 'A');
		getdata((num % 16) + 4, (num / 16) * 40, buf, bal->items[num], 36,
				DOECHO, YEA);
		if (strlen(bal->items[num]) == 0) {
			if (num != 0)
				break;
			num = -1;
		}
	}
	bal->totalitems = num;
	return num;
}

//����ͶƱ�䲢����ͶƱ��
//bname:����
//����ֵ:�̶�Ϊ FULLUPDATE
int vote_maintain(char *bname) {
	char buf[STRLEN];
	struct votebal *ball = &currvote;

	setcontrolfile();
	if (!chk_currBM(currBM, 0)) {
		return 0;
	}
	stand_title("����ͶƱ��");
	makevdir(bname);
	for (;;) {
		getdata(2, 0, "(1)�Ƿ�, (2)��ѡ, (3)��ѡ, (4)��ֵ (5)�ʴ� (6)ȡ�� ? : ",
				genbuf, 2, DOECHO, YEA);
		genbuf[0] -= '0';
		if (genbuf[0] < 1 || genbuf[0] > 5) {
			prints("ȡ���˴�ͶƱ\n");
			return FULLUPDATE;
		}
		ball->type = (int) genbuf[0];
		break;
	}
	ball->opendate = time(NULL);
	if (makevote(ball, bname))
		return FULLUPDATE; //����ͶƱ��
	setvoteflag(currboard, 1);
	clear();
	strcpy(ball->userid, currentuser.userid);
	if (append_record(controlfile, ball, sizeof(*ball)) == -1) {
		prints("�������صĴ����޷�����ͶƱ����ͨ��վ��");
		b_report("Append Control file Error!!");
	} else {
		char votename[STRLEN];
		int i;

		b_report("OPEN");
		prints("ͶƱ�俪���ˣ�\n");
		range++;;
		sprintf(votename, "tmp/votetmp.%s.%05d", currentuser.userid,
				uinfo.pid);
		if ((sug = fopen(votename, "w")) != NULL) {
			//Modified by IAMFAT 2002.06.13
			//sprintf(buf, "[֪ͨ] %s �ٰ�ͶƱ��%s", currboard, ball->title);
			strcpy(genbuf, ball->title);
			ellipsis(genbuf, 31 - strlen(currboard));
			sprintf(buf, "[֪ͨ] %s �ٰ�ͶƱ: %s", currboard, ball->title);
			get_result_title(sug);
			if (ball->type != VOTE_ASKING && ball->type != VOTE_VALUE) {
				fprintf(sug, "\n��[1mѡ������[m��\n");
				for (i = 0; i < ball->totalitems; i++) {
					fprintf(sug, "([1m%c[m) %-40s\n", 'A' + i,
							ball->items[i]);
				}
			}
			fclose(sug);
			Postfile(votename, "vote", buf, 1);
			unlink(votename);
		}
	}
	pressreturn();
	return FULLUPDATE;
}

//����ͶƱ��
//ball: ͶƱ��
//bname������
//����ֵ0�� �����˳� 1���û�ȡ��
int makevote(struct votebal *ball, char *bname) {
	char buf[STRLEN];
	int aborted;

	prints("�밴�κμ���ʼ�༭�˴� [ͶƱ������]: \n");
	igetkey();
	setvfile(genbuf, bname, "desc");
	sprintf(buf, "%s.%d", genbuf, ball->opendate);
	aborted = vedit(buf, NA, YEA);
	if (aborted) {
		clear();
		prints("ȡ���˴�ͶƱ�趨\n");
		pressreturn();
		return 1;
	}
	clear();
	getdata(0, 0, "�˴�ͶƱ�������� (���ɣ���): ", buf, 3, DOECHO, YEA);

	if (*buf == '\n' || atoi(buf) == 0 || *buf == '\0')
		strcpy(buf, "1");

	ball->maxdays = atoi(buf);
	for (;;) {
		//Modified by IAMFAT 2002.06.13
		//getdata(1, 0, "ͶƱ��ı���: ", ball->title, 61, DOECHO, YEA);
		getdata(1, 0, "ͶƱ��ı���: ", ball->title, 50, DOECHO, YEA);
		if (strlen(ball->title) > 0)
			break;
		bell();
	}
	switch (ball->type) {
		case VOTE_YN:
			ball->maxtkt = 0;
			strcpy(ball->items[0], "�޳�  ���ǵģ�");
			strcpy(ball->items[1], "���޳ɣ����ǣ�");
			strcpy(ball->items[2], "û������������");
			ball->maxtkt = 1;
			ball->totalitems = 3;
			break;
		case VOTE_SINGLE:
			get_vitems(ball);
			ball->maxtkt = 1;
			break;
		case VOTE_MULTI:
			get_vitems(ball);
			for (;;) {
				getdata(21, 0, "һ������༸Ʊ? [1]: ", buf, 5, DOECHO, YEA);
				ball->maxtkt = atoi(buf);
				if (ball->maxtkt <= 0)
					ball->maxtkt = 1;
				if (ball->maxtkt > ball->totalitems)
					continue;
				break;
			}
			break;
		case VOTE_VALUE:
			for (;;) {
				getdata(3, 0, "������ֵ��󲻵ó��� [100] : ", buf, 4, DOECHO, YEA);
				ball->maxtkt = atoi(buf);
				if (ball->maxtkt <= 0)
					ball->maxtkt = 100;
				break;
			}
			break;
		case VOTE_ASKING:
			/*                    getdata(3,0,"���ʴ�����������֮���� :",buf,3,DOECHO,YEA) ;
			 ball->maxtkt = atof(buf) ;
			 if(ball->maxtkt <= 0) ball->maxtkt = 10;*/
			ball->maxtkt = 0;
			currvote.totalitems = 0;
			break;
		default:
			ball->maxtkt = 1;
			break;
	}
	return 0;
}

// ����Ƿ�����µı���¼���߽�վwelcome ����д��
// bname:����, mode =2ʱ��ΪNULL
// val:  0�����ģʽ    ������0:д��ģʽ
// mode: 1:��鱸��¼   2:����վWelcome
// ����ֵ 0:δ�� 1:�Ѷ�
int vote_flag(char *bname, char val, int mode) {
	char buf[STRLEN], flag;
	int fd, num, size;

	num = usernum - 1;

	switch (mode) {
		case 2:
			sprintf(buf, "Welcome.rec"); /* ��վ�� Welcome ���� */
			break;
		case 1:
			setvfile(buf, bname, "noterec"); /* ����������¼����� */
			break;
		default:
			return -1;
	}

	if (num >= MAXUSERS) {
		report("Vote Flag, Out of User Numbers");
		return -1;
	}

	if ((fd = open(buf, O_RDWR | O_CREAT, 0600)) == -1) {
		return -1;
	}

	FLOCK(fd, LOCK_EX);
	size = (int) lseek(fd, 0, SEEK_END);
	memset(buf, 0, sizeof(buf));
	while (size <= num) {
		write(fd, buf, sizeof(buf));
		size += sizeof(buf);
	}
	lseek(fd, (off_t) num, SEEK_SET);
	read(fd, &flag, 1); //���Ƿ��Ѿ������ı�־flag
	if ((flag == 0 && val != 0)) {
		lseek(fd, (off_t) num, SEEK_SET);
		write(fd, &val, 1);
	}
	FLOCK(fd, LOCK_UN);
	close(fd);

	return flag;
}

//���Ͷ�˼�Ʊ
//bits: 32λ��ֵ
//����ֵ ������32λbits�� ����1��λ��������
int vote_check(int bits) {
	int i, count;

	for (i = count = 0; i < 32; i++) {
		if ((bits >> i) & 1)
			count++;
	}
	return count;
}

//��ʾ�û�Ͷ����Ʊ���Լ���ѡ��
//pbits:Ʊ���ֶ� i:��ʾλ�� flag:�Ƿ���ʾ���Ѿ�Ͷ�˼�Ʊ YEA:��ʾ NO:����ʾ
//����ֵ:�̶�ΪYEA
int showvoteitems(unsigned int pbits, int i, int flag) {
	char buf[STRLEN];
	int count;

	if (flag == YEA) {
		count = vote_check(pbits);
		if (count > currvote.maxtkt)
			return NA;
		move(2, 0);
		clrtoeol();
		prints("���Ѿ�Ͷ�� [1m%d[m Ʊ", count);
	}
	sprintf(buf, "%c.%2.2s%-36.36s", 'A' + i, ((pbits >> i) & 1 ? "��"
			: "  "), currvote.items[i]);
	move(i + 6 - ((i > 15) ? 16 : 0), 0 + ((i > 15) ? 40 : 0));
	prints(buf);
	refresh();
	return YEA;
}

//��ʾͶƱ����
void show_voteing_title() {
	time_t closedate;
	char buf[STRLEN];

	if (currvote.type != VOTE_VALUE && currvote.type != VOTE_ASKING)
		sprintf(buf, "��ͶƱ��: [1m%d[m Ʊ", currvote.maxtkt);
	else
		buf[0] = '\0';
	closedate = currvote.opendate + currvote.maxdays * 86400;
	getdatestring(closedate, NA);
	prints("ͶƱ�������: [1m%s[m  %s  %s\n", datestring, buf,
			(voted_flag) ? "([5;1m�޸�ǰ��ͶƱ[m)" : "");
	prints("ͶƱ������: [1m%-50s[m����: [1m%s[m \n", currvote.title,
			vote_type[currvote.type - 1]);
}

//ȡ��������ͶƱ��
//uv:�û�ͶƱ������,���غ��û�����Ĵ𰸷��� uv->msg��,���3��
//����ֵ: �û�����Ĵ�����
int getsug(struct ballot *uv) {
	int i, line;

	move(0, 0);
	clrtobot();
	if (currvote.type == VOTE_ASKING) {
		show_voteing_title();
		line = 3;
		prints("��������������(����):\n");
	} else {
		line = 1;
		prints("����������������(����):\n");
	}
	move(line, 0);
	for (i = 0; i < 3; i++) {
		prints(": %s\n", uv->msg[i]);
	}
	for (i = 0; i < 3; i++) {
		getdata(line + i, 0, ": ", uv->msg[i], STRLEN - 2, DOECHO, NA);
		if (uv->msg[i][0] == '\0')
			break;
	}
	return i;
}

//�����ѡ/��ѡ/�ǷǵĴ�
//uv:�û�ͶƱ������,���غ��û�����Ĵ𰸷��� uv->msg��
//����ֵ: �ɹ�1 �û�ȡ��-1
int multivote(struct ballot *uv) {
	unsigned int i;

	i = uv->voted;
	move(0, 0);
	show_voteing_title();
	uv->voted = setperms(uv->voted, "ѡƱ", currvote.totalitems,
			showvoteitems);
	if (uv->voted == i)
		return -1;
	return 1;
}

//����ֵ��ѡ��Ĵ�
//uv:�û�ͶƱ������,���غ��û�����Ĵ𰸷��� uv->msg��
//����ֵ: �ɹ�1 �û�ȡ��-1
int valuevote(struct ballot *uv) {
	unsigned int chs;
	char buf[10];

	chs = uv->voted;
	move(0, 0);
	show_voteing_title();
	prints("�˴������ֵ���ܳ��� [1m%d[m", currvote.maxtkt);
	if (uv->voted != 0)
		sprintf(buf, "%d", uv->voted);
	else
		memset(buf, 0, sizeof(buf));
	do {
		getdata(3, 0, "������һ��ֵ? [0]: ", buf, 5, DOECHO, NA);
		uv->voted = abs(atoi(buf));
	} while (uv->voted > currvote.maxtkt && buf[0] != '\n' && buf[0]
			!= '\0');
	if (buf[0] == '\n' || buf[0] == '\0' || uv->voted == chs)
		return -1;
	return 1;
}

//�û�����ͶƱ,��vote_key,b_vote��������
//num:ͶƱcontrolfile�еڼ�����¼
//����ֵ:��
int user_vote(int num) {
	char fname[STRLEN], bname[STRLEN];
	char buf[STRLEN];
	struct ballot uservote, tmpbal;
	int votevalue;
	int aborted = NA, pos;

	move(t_lines - 2, 0);
	get_record(controlfile, &currvote, sizeof(struct votebal), num);
	if (currentuser.firstlogin > currvote.opendate) { //ע������ͶƱ������ǰ����ͶƱ
		prints("�Բ���, ͶƱ�������Ҳ������Ĵ���\n");
		pressanykey();
		return;
	}
	sprintf(fname, "vote/%s/flag.%d", currboard, currvote.opendate);
	if ((pos = search_record(fname, &uservote, sizeof(uservote), cmpvuid,
			currentuser.userid)) <= 0) {
		(void) memset(&uservote, 0, sizeof(uservote));
		voted_flag = NA;
	} else {
		voted_flag = YEA;
	}
	strcpy(uservote.uid, currentuser.userid);
	sprintf(bname, "desc.%d", currvote.opendate);
	setvfile(buf, currboard, bname);
	ansimore(buf, YEA);
	move(0, 0);
	clrtobot();
	switch (currvote.type) {
		case VOTE_SINGLE:
		case VOTE_MULTI:
		case VOTE_YN:
			votevalue = multivote(&uservote);
			if (votevalue == -1)
				aborted = YEA;
			break;
		case VOTE_VALUE:
			votevalue = valuevote(&uservote);
			if (votevalue == -1)
				aborted = YEA;
			break;
		case VOTE_ASKING:
			uservote.voted = 0;
			aborted = !getsug(&uservote);
			break;
	}
	clear();
	if (aborted == YEA) {
		prints("���� ��[1m%s[m��ԭ���ĵ�ͶƱ��\n", currvote.title);
	} else {
		if (currvote.type != VOTE_ASKING)
			getsug(&uservote);
		pos = search_record(fname, &tmpbal, sizeof(tmpbal), cmpvuid,
				currentuser.userid);
		if (pos) {
			substitute_record(fname, &uservote, sizeof(uservote), pos);
		} else if (append_record(fname, &uservote, sizeof(uservote)) == -1) {
			move(2, 0);
			clrtoeol();
			prints("ͶƱʧ��! ��֪ͨվ���μ���һ��ѡ��ͶƱ\n");
			pressreturn();
		}
		prints("\n�Ѿ�����Ͷ��Ʊ����...\n");
	}
	pressanykey();
	return;
}

//��ʾͶƱ����Ϣ��ͷ��
void voteexp() {
	clrtoeol();
	prints("[1;44m��� ����ͶƱ���� ������ %-39s ��� ���� ����[m\n", "ͶƱ����");
}

//��ʾͶƱ����Ϣ
//ent ͶƱ��Ϣ
int printvote(struct votebal *ent) {
	static int i;
	struct ballot uservote;
	char buf[STRLEN + 10];
	char flagname[STRLEN];
	int num_voted;

	//Added by IAMFAT 2002.06.13
	char title[STRLEN];

	//Added End

	if (ent == NULL) {
		move(2, 0);
		voteexp();
		i = 0;
		return 0;
	}
	i++;
	if (i > page + 19 || i > range)
		return QUIT;
	else if (i <= page)
		return 0;
	sprintf(buf, "flag.%d", ent->opendate);
	setvfile(flagname, currboard, buf);
	if (search_record(flagname, &uservote, sizeof(uservote), cmpvuid,
			currentuser.userid) <= 0) {
		voted_flag = NA;
	} else
		voted_flag = YEA;
	num_voted = get_num_records(flagname, sizeof(struct ballot));
	getdatestring(ent->opendate, NA);
	//Modified by IAMFAT 2002.06.13
	/*
	 sprintf(buf, " %s%3d %-12.12s %6.6s %-40.40s%-4.4s %3d  %4d[m\n", (voted_flag == NA) ? "[1m" : "", i, ent->userid,
	 datestring+6, ent->title, vote_type[ent->type - 1], ent->maxdays, num_voted);
	 */
	strcpy(title, ent->title);
	ellipsis(title, 39);
	sprintf(buf, " %s%3d %-12.12s %6.6s %-39.39s %-4.4s %3d  %4d[m\n",
			(voted_flag == NA) ? "[1m" : "", i, ent->userid, datestring
					+ 6, title, vote_type[ent->type - 1], ent->maxdays,
			num_voted);
	//Ended IAMFAT
	prints("%s", buf);
}

//ɾ��ͶƱ�ļ�
//num ͶƱcontrolfile�еڼ�����¼
//����ֵ ��
int dele_vote(num)
int num;
{
	char buf[STRLEN];

	sprintf(buf, "vote/%s/flag.%d", currboard, currvote.opendate);
	unlink(buf);
	sprintf(buf, "vote/%s/desc.%d", currboard, currvote.opendate);
	unlink(buf);
	if (delete_record(controlfile, sizeof(currvote), num, NULL, NULL) == -1) {
		prints("����������֪ͨվ��....");
		pressanykey();
	}
	range--;
	if (get_num_records(controlfile, sizeof(currvote)) == 0) {
		setvoteflag(currboard, 0);
	}
}

//��ʾͶƱ���
//bname:����
//����ֵ:�̶�ΪFULLUPDATE
int vote_results(char *bname) {
	char buf[STRLEN];

	setvfile(buf, bname, "results");
	if (ansimore(buf, YEA) == -1) {
		move(3, 0);
		prints("Ŀǰû���κ�ͶƱ�Ľ����\n");
		clrtobot();
		pressreturn();
	} else
		clear();
	return FULLUPDATE;
}

//����ͶƱ�䲢����ͶƱ��
int b_vote_maintain() {
	return vote_maintain(currboard);
}

//��ʾͶƱ��ѡ��
void vote_title() {

	docmdtitle(
			"[ͶƱ���б�]",
			"[[1;32m��[m,[1;32me[m] �뿪 [[1;32mh[m] ���� [[1;32m��[m,[1;32mr <cr>[m] ����ͶƱ [[1;32m��[m,[1;32m��[m] ��,��ѡ�� [1m������[m��ʾ��δͶƱ");
	update_endline();
}

//�����û��İ�����ͶƱ����в���,���Խ���/�޸�/ǿ�ƹر�/��ʾͶƱ���
//ch: �û��İ���
//allnum:ͶƱcontrolfile�ĵڼ�����¼
//pagenum:δʹ��
//����ֵ 0:ʧ�� 1:�ɹ�
int vote_key(int ch, int allnum, int pagenum) {
	int deal = 0, ans;
	char buf[STRLEN];

	switch (ch) {
		case 'v':
		case 'V':
		case '\n':
		case '\r':
		case 'r':
		case KEY_RIGHT:
			user_vote(allnum + 1);
			deal = 1;
			break;
		case 'R':
			vote_results(currboard);
			deal = 1;
			break;
		case 'H':
		case 'h':
			show_help("help/votehelp");
			deal = 1;
			break;
		case 'A':
		case 'a':
			if (!chk_currBM(currBM, 0))
				return YEA;
			vote_maintain(currboard);
			deal = 1;
			break;
		case 'O':
		case 'o':
			if (!chk_currBM(currBM, 0))
				return YEA;
			clear();
			deal = 1;
			get_record(controlfile, &currvote, sizeof(struct votebal),
					allnum + 1);
			prints("[5;1;31m����!![m\n");
			prints("ͶƱ����⣺[1m%s[m\n", currvote.title);
			ans = askyn("��ȷ��Ҫ����������ͶƱ��", NA, NA);

			if (ans != 1) {
				move(2, 0);
				prints("ȡ��ɾ���ж�\n");
				pressreturn();
				clear();
				break;
			}
			mk_result(allnum + 1);
			sprintf(buf, "[����] �������ͶƱ %s", currvote.title);
			securityreport(buf, 0, 4);
			break;
		case 'M':
		case 'm':
			if (!chk_currBM(currBM, 0))
				return YEA;
			clear();
			deal = 1;
			get_record(controlfile, &currvote, sizeof(struct votebal),
					allnum + 1);
			prints("[5;1;31m����!![m\n");
			prints("ͶƱ����⣺[1m%s[m\n", currvote.title);
			ans = askyn("��ȷ��Ҫ�޸����ͶƱ���趨��", NA, NA);

			if (ans != 1) {
				move(2, 0);
				prints("ȡ���޸��ж�\n");
				pressreturn();
				clear();
				break;
			}
			makevote(&currvote, currboard);
			substitute_record(controlfile, &currvote,
					sizeof(struct votebal), allnum + 1);
			sprintf(buf, "[�޸�] �޸�ͶƱ�趨 %s", currvote.title);
			securityreport(buf, 0, 4);
			break;
		case 'D':
		case 'd':
			if (!chk_currBM(currBM, 0)) {
				return 1;
			}
			deal = 1;
			get_record(controlfile, &currvote, sizeof(struct votebal),
					allnum + 1);
			clear();
			prints("[5;1;31m����!![m\n");
			prints("ͶƱ����⣺[1m%s[m\n", currvote.title);
			ans = askyn("��ȷ��Ҫǿ�ƹر����ͶƱ��", NA, NA);

			if (ans != 1) {
				move(2, 0);
				prints("ȡ��ɾ���ж�\n");
				pressreturn();
				clear();
				break;
			}
			sprintf(buf, "[�ر�] ǿ�ƹر�ͶƱ %s", currvote.title);
			securityreport(buf, 0, 4);
			dele_vote(allnum + 1);
			break;
		default:
			return 0;
	}
	if (deal) {
		Show_Votes();
		vote_title();
	}
	return 1;
}

//��ʾͶƱ����Ϣ
int Show_Votes() {

	move(3, 0);
	clrtobot();
	printvote(NULL);
	setcontrolfile();
	if (apply_record(controlfile, printvote, sizeof(struct votebal), 0, 0,
			0) == -1) {
		prints("����û��ͶƱ�俪��....");
		pressreturn();
		return 0;
	}
	clrtobot();
	return 0;
}

//�û��Ա������ͶƱ��bbs.c����
//����ֵ:�̶�ΪFULLUPDATE
int b_vote() {
	int num_of_vote;
	int voting;

	if (!HAS_PERM(PERM_VOTE) || (currentuser.stay < 1800)) {
		return;
	}
	setcontrolfile();
	num_of_vote = get_num_records(controlfile, sizeof(struct votebal));
	if (num_of_vote == 0) {
		move(2, 0);
		clrtobot();
		prints("\n��Ǹ, Ŀǰ��û���κ�ͶƱ���С�\n");
		pressreturn();
		setvoteflag(currboard, 0);
		return FULLUPDATE;
	}
	setlistrange(num_of_vote);
	clear();
	voting = choose(NA, 0, vote_title, vote_key, Show_Votes, user_vote); //?
	clear();
	return /* user_vote( currboard ) */FULLUPDATE;
}

//��ʾͶƱ���  bbs.c����
int b_results() {
	return vote_results(currboard);
}

//SYSOP�濪��ͶƱ��
int m_vote() {
	char buf[STRLEN];

	strcpy(buf, currboard);
	strcpy(currboard, DEFAULTBOARD);
	modify_user_mode(ADMIN);
	vote_maintain(DEFAULTBOARD);
	strcpy(currboard, buf);
	return;
}

//��SYSOP�����ͶƱ
int x_vote() {
	char buf[STRLEN];

	modify_user_mode(XMENU);
	strcpy(buf, currboard);
	strcpy(currboard, DEFAULTBOARD);
	b_vote();
	strcpy(currboard, buf);
	return;
}

//��ʾsysop��ͶƱ���
int x_results() {
	modify_user_mode(XMENU); //�����û� ģʽ״̬��??
	return vote_results(DEFAULTBOARD); //��ʾsysop��ͶƱ���
}
