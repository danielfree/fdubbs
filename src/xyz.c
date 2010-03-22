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
 $Id: xyz.c 325 2006-10-27 15:38:46Z danielfree $
 */

#define EXTERN
#include "bbs.h"
int use_define = 0;
int child_pid = 0;
extern int iscolor;
extern int enabledbchar;

#ifdef ALLOWSWITCHCODE
extern int convcode;
#endif

extern struct UCACHE *uidshm;
#define TH_LOW	10
#define TH_HIGH	15


//�����û� ģʽ״̬��mode
int modify_user_mode(int mode) {
	uinfo.mode = mode;
	update_ulist(&uinfo, utmpent);
	return 0; //sdjfielsdfje
}

//      ����Ȩ�޶���ֵ,�ж����iλ�Ƿ�Ϊ��,������use_define��ֵ��
//      �������Ӧλ��Ȩ����ʾ�ַ���
//      �������iָʾ��λ�ô���ʾ,����
int showperminfo(int pbits, int i) {
	char buf[STRLEN];
	sprintf(buf, "%c. %-30s %2s", 'A' + i,
			(use_define) ? user_definestr[i] : permstrings[i], ((pbits
					>> i) & 1 ? "��" : "��"));
	move(i + 6 - ((i > 15) ? 16 : 0), 0 + ((i > 15) ? 40 : 0));
	prints(buf);
	refresh();
	return YEA;
}

//      �����û���Ȩ���趨
unsigned int setperms(unsigned int pbits, char *prompt, int numbers, int (*showfunc) ()) {
	int lastperm = numbers - 1;
	int i, done = NA;
	char choice[3], buf[80];
	move(4, 0);
	prints("�밴����Ҫ�Ĵ������趨%s���� Enter ����.\n", prompt);
	move(6, 0);
	clrtobot();
	for (i = 0; i <= lastperm; i++) {
		(*showfunc)(pbits, i, NA);
	}
	while (!done) {
		sprintf(buf, "ѡ��(ENTER ����%s): ",
				((strcmp(prompt, "Ȩ��") != 0)) ? "" : "��0 ͣȨ");
		getdata(t_lines - 1, 0, buf, choice, 2, DOECHO, YEA);
		*choice = toupper(*choice);
		/*		if (*choice == '0')
		 return (0);
		 else modified by kit,rem 0ͣȨ* remed all by Amigo 2002.04.03*/
		if (*choice == '\n' || *choice == '\0')
		done = YEA;
		else if (*choice < 'A' || *choice> 'A' + lastperm)
		bell ();
		else {
			i = *choice - 'A';
			pbits ^= (1 << i);
			if ((*showfunc) (pbits, i, YEA) == NA) {
				pbits ^= (1 << i);
			} //if
		} //else
	}
	//while !done
	return (pbits);
}

//      pager��msg�趨
//
int x_userdefine() {
	int id;
	unsigned int newlevel;
	modify_user_mode(USERDEF);
	if (!(id = getuser(currentuser.userid))) {
		move(3, 0);
		prints("�����ʹ���� ID...");
		clrtoeol();
		pressreturn();
		clear();
		return 0;
	}
	move(1, 0);
	clrtobot();
	move(2, 0);
	use_define = 1;
	newlevel = setperms(lookupuser.userdefine, "����", NUMDEFINES,
			showperminfo);
	move(2, 0);
	if (newlevel == lookupuser.userdefine)
		prints("����û���޸�...\n");
	else {
#ifdef ALLOWSWITCHCODE
		if ((!convcode && !(newlevel & DEF_USEGB))
				|| (convcode && (newlevel & DEF_USEGB)))
		switch_code ();
#endif
		lookupuser.userdefine = newlevel;
		currentuser.userdefine = newlevel;
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
		uinfo.pager |= FRIEND_PAGER;
		if (!(uinfo.pager & ALL_PAGER)) {
			if (!DEFINE(DEF_FRIENDCALL))
				uinfo.pager &= ~FRIEND_PAGER;
		}
		uinfo.pager &= ~ALLMSG_PAGER;
		uinfo.pager &= ~FRIENDMSG_PAGER;
		/* Following line added by Amigo 2002.04.03. For close logoff msg. */
		uinfo.pager &= ~LOGOFFMSG_PAGER;
		if (DEFINE(DEF_DELDBLCHAR))
			enabledbchar = 1;
		else
			enabledbchar = 0;
		uinfo.from[22] = DEFINE(DEF_NOTHIDEIP) ? 'S' : 'H';
		if (DEFINE(DEF_FRIENDMSG)) {
			uinfo.pager |= FRIENDMSG_PAGER;
		}
		if (DEFINE(DEF_ALLMSG)) {
			uinfo.pager |= ALLMSG_PAGER;
			uinfo.pager |= FRIENDMSG_PAGER;
		}
		/* Following 3 lines added by Amigo 2002.04.03. For close logoff msg. */
		if (DEFINE(DEF_LOGOFFMSG)) {
			uinfo.pager |= LOGOFFMSG_PAGER;
		}
		update_ulist(&uinfo, utmpent);
		prints("�µĲ����趨���...\n\n");
	}
	iscolor = (DEFINE(DEF_COLOR)) ? 1 : 0;
	pressreturn();
	clear();
	use_define = 0;
	return 0;
}

//��������������,get_status���ƺ�����,������ǰ����uinfo.invisible��
int x_cloak() {
	modify_user_mode(GMENU);
	report("toggle cloak", currentuser.userid);
	uinfo.invisible = (uinfo.invisible) ? NA : YEA;
	if (uinfo.invisible == YEA) {
		uidshm->passwd[uinfo.uid - 1].flags[0] |= CLOAK_FLAG;
	} else {
		uidshm->passwd[uinfo.uid - 1].flags[0] &= ~CLOAK_FLAG;
	}
	//end add
	update_ulist(&uinfo, utmpent);
	if (!uinfo.in_chat) {
		move(1, 0);
		clrtoeol();
		prints("������ (cloak) �Ѿ�%s��!", (uinfo.invisible) ? "����" : "ֹͣ");
		pressreturn();
	}
	return 0;
}

//�޸��û��ĵ���
void x_edits() {
	int aborted;
	char ans[7], buf[STRLEN];
	int ch, num, confirm;
	extern int WishNum;
	static char *e_file[] = { "plans", "signatures", "notes", "logout",
			"GoodWish", NULL };
	static char *explain_file[] = { "����˵����", "ǩ����", "�Լ��ı���¼", "��վ�Ļ���",
			"�ײ�������Ϣ", NULL };
	modify_user_mode(GMENU);
	clear();
	move(1, 0);
	prints("���޸��˵���\n\n");
	for (num = 0; e_file[num] != NULL && explain_file[num] != NULL; num++) {
		prints("[[1;32m%d[m] %s\n", num + 1, explain_file[num]);
	}
	prints("[[1;32m%d[m] �������\n", num + 1);

	getdata(num + 5, 0, "��Ҫ������һ����˵���: ", ans, 2, DOECHO, YEA);
	if (ans[0] - '0' <= 0 || ans[0] - '0' > num || ans[0] == '\n'
			|| ans[0] == '\0')
		return;

	ch = ans[0] - '0' - 1;
	setuserfile(genbuf, e_file[ch]);
	move(3, 0);
	clrtobot();
	sprintf(buf, "(E)�༭ (D)ɾ�� %s? [E]: ", explain_file[ch]);
	getdata(3, 0, buf, ans, 2, DOECHO, YEA);
	if (ans[0] == 'D' || ans[0] == 'd') {
		confirm = askyn("��ȷ��Ҫɾ���������", NA, NA);
		if (confirm != 1) {
			move(5, 0);
			prints("ȡ��ɾ���ж�\n");
			pressreturn();
			clear();
			return;
		}
		unlink(genbuf);
		move(5, 0);
		prints("%s ��ɾ��\n", explain_file[ch]);
		sprintf(buf, "delete %s", explain_file[ch]);
		report(buf, currentuser.userid);
		pressreturn();
		if (ch == 4) {
			WishNum = 9999;
		}
		clear();
		return;
	}
	modify_user_mode(EDITUFILE);
	aborted = vedit(genbuf, NA, YEA);
	clear();
	if (!aborted) {
		prints("%s ���¹�\n", explain_file[ch]);
		sprintf(buf, "edit %s", explain_file[ch]);
		if (!strcmp(e_file[ch], "signatures")) {
			set_numofsig();
			prints("ϵͳ�����趨�Լ���������ǩ����...");
		}
		report(buf, currentuser.userid);
	} else {
		prints("%s ȡ���޸�\n", explain_file[ch]);
	}
	pressreturn();
	if (ch == 4) {
		WishNum = 9999;
	}
}

//ȡ��genbuf�б�����û����ڵļ�¼λ�õ�*id��,Ϊ���ʾ������
int gettheuserid(int x, char *title, int *id) {
	move(x, 0);
	usercomplete(title, genbuf);
	if (*genbuf == '\0') {
		clear();
		return 0;
	}
	if (!(*id = getuser(genbuf))) {
		move(x + 3, 0);
		prints("�����ʹ���ߴ���");
		clrtoeol();
		pressreturn();
		clear();
		return 0;
	}
	return 1;
}

// �õ��û��������������,����������λ�ñ�����pos�з���,
//              ����ֵΪ1��ʾ�ɹ�,Ϊ0ʧ��
int gettheboardname(int x, char *title, int *pos, struct boardheader *fh,
		char *bname, int mode) {
	extern int cmpbnames();
	move(x, 0);
	make_blist(mode);
	namecomplete(title, bname);
	if (*bname == '\0') {
		return 0;
	}
	*pos = search_record(BOARDS, fh, sizeof(struct boardheader),
			cmpbnames, bname);
	if (!(*pos)) {
		move(x + 3, 0);
		prints("����ȷ������������");
		pressreturn();
		clear();
		return 0;
	}
	return 1;
}

//����
int x_lockscreen() {
	char buf[PASSLEN + 1];

	modify_user_mode(LOCKSCREEN);
	move(9, 0);
	clrtobot();
	buf[0] = '\0';
	move(9, 0);
	prints("[1;37m");
	prints("\n       _       _____   ___     _   _   ___     ___       __");
	prints("\n      ( )     (  _  ) (  _`\\  ( ) ( ) (  _`\\  (  _`\\    |  |");
	prints("\n      | |     | ( ) | | ( (_) | |/'/' | (_(_) | | ) |   |  |");
	prints("\n      | |  _  | | | | | |  _  | , <   |  _)_  | | | )   |  |");
	prints("\n      | |_( ) | (_) | | (_( ) | |\\`\\  | (_( ) | |_) |   |==|");
	prints("\n      (____/' (_____) (____/' (_) (_) (____/' (____/'   |__|\n");
	prints("\n\033[1;36m��Ļ����\033[33m %s\033[36m ʱ��%s��ʱ��ס��...\033[m",
			getdatestring(time(NULL), DATE_ZH), currentuser.userid);
	while (*buf == '\0' || !checkpasswd(currentuser.passwd, buf)) {
		move(18, 0);
		clrtobot();
		getdata(19, 0, "���������������Խ���: ", buf, PASSLEN, NOECHO, YEA);
	}
	return FULLUPDATE;
}

//#define MY_DEBUG
//  ִ������cmdfile,����Ϊparam1
void exec_cmd(int umode, int pager, char *cmdfile, char *param1) {
	char buf[160];
	char *my_argv[18], *ptr;
	int save_pager, i;

	signal(SIGALRM, SIG_IGN);
	modify_user_mode(umode);
	clear();
	move(2, 0);
	if (num_useshell() > MAX_USESHELL) {
		prints("̫����ʹ���ⲿ��ʽ�ˣ�����һ�����ð�...");
		pressanykey();
		return;
	}
	if (!dashf(cmdfile)) {
		prints("�ļ� [%s] �����ڣ�\n", cmdfile);
		pressreturn();
		return;
	}
	save_pager = uinfo.pager;
	if (pager == NA) {
		uinfo.pager = 0;
	}
	sprintf(buf, "%s %s %s %d", cmdfile, param1, currentuser.userid,
			getpid());
	report(buf, currentuser.userid);
	my_argv[0] = cmdfile;
	strcpy(buf, param1);
	if (buf[0] != '\0')
		ptr = strtok(buf, " \t");
	else
		ptr = NULL;
	for (i = 1; i < 18; i++) {
		if (ptr) {
			my_argv[i] = ptr;
			ptr = strtok(NULL, " \t");
		} else {
			my_argv[i] = NULL;
		}
	}
#ifdef MY_DEBUG
	for (i = 0; i < 18; i++) {
		if (my_argv[i] != NULL)
		prints ("my_argv[%d] = %s\n", i, my_argv[i]);
		else
		prints ("my_argv[%d] = (none)\n", i);
	}
	pressanykey ();
#else
	child_pid = fork();
	if (child_pid == -1) {
		prints("��Դ��ȱ��fork() ʧ�ܣ����Ժ���ʹ��");
		child_pid = 0;
		pressreturn();
		return;
	}
	if (child_pid == 0) {
		execv(cmdfile, my_argv);
		exit(0);
	} else {
		waitpid(child_pid, NULL, 0);
	}
#endif
	child_pid = 0;
	uinfo.pager = save_pager;
	clear();
}

//��ѯʹ��������
void x_showuser() {
	char buf[STRLEN];
	modify_user_mode(SYSINFO);
	clear();
	stand_title("��վʹ�������ϲ�ѯ");
	ansimore("etc/showuser.msg", NA);
	getdata(20, 0, "Parameter: ", buf, 30, DOECHO, YEA);
	if ((buf[0] == '\0') || dashf("tmp/showuser.result"))
		return;
	securityreport("��ѯʹ��������", 0, 0);
	exec_cmd(SYSINFO, YEA, "bin/showuser", buf);
	sprintf(buf, "tmp/showuser.result");
	if (dashf(buf)) {
		mail_file(buf, currentuser.userid, "ʹ�������ϲ�ѯ���");
		unlink(buf);
	}
}

//  ������Ϸ
int ent_winmine() {
	char buf[80];
	sprintf(buf, "%s %s", currentuser.userid, currentuser.lasthost);
	exec_cmd(WINMINE, NA, "so/winmine", buf);
}

/**
 * Load memorial day info.
 * @return 0 on success, -1 on error.
 */
int fill_date(void)
{
	if (resolve_boards() < 0)
		return -1;

	time_t now = time(NULL);
	if (now < brdshm->fresh_date && brdshm->date[0] != '\0')
		return 0;

	struct tm *mytm = localtime(&now);
	time_t next = now - mytm->tm_hour * 3600 - mytm->tm_min * 60
			- mytm->tm_sec + 86400;

	strlcpy(brdshm->date, DEF_VALUE, sizeof(brdshm->date));

	FILE *fp = fopen(DEF_FILE, "r");
	if (fp == NULL)
		return -1;

	char date[5], index[5], buf[80], msg[30];
	strftime(date, sizeof(date), "%m%d", mytm);
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (*buf == '#' || *buf == '\0')
			continue;
		if (strlcpy(index, buf, sizeof(index)) < sizeof(index))
			continue;
		strlcpy(msg, buf + sizeof(index), sizeof(msg));
		char *t = strchr(msg, '\n');
		if (t != NULL)
			*t = '\0';
		if (*index == '\0' || *msg == '\0')
			continue;
		if (strcmp(index, "0000") == 0 || strcmp(date, index) == 0) {
			// align center
			memset(brdshm->date, ' ', sizeof(msg));
			size_t len = strlen(msg);
			memcpy(brdshm->date + (sizeof(msg) - len) / 2, msg, len);
			brdshm->date[sizeof(msg)] = '\0';
		}
	}
	fclose(fp);
	brdshm->fresh_date = next;
	return 0;
}

//  ����������?
int is_birth(struct userec user) {
	struct tm *tm;
	time_t now;

	now = time(0);
	tm = localtime(&now);

	if (strcasecmp(user.userid, "guest") == 0)
		return NA;

	if (user.birthmonth == (tm->tm_mon + 1) && user.birthday
			== tm->tm_mday)
		return YEA;
	else
		return NA;
}

//      ��������
int sendgoodwish(char *uid) {
	return sendGoodWish(NULL);
}

int sendGoodWish(char *userid) {
	FILE *fp;
	int tuid, i, count;
	time_t now;
	char buf[5][STRLEN], tmpbuf[STRLEN];
	char uid[IDLEN + 1], *ptr, *timestr;

	modify_user_mode(GOODWISH);
	clear();
	move(1, 0);
	prints("[0;1;32m���Ա�[m\n�����������������������ȥ����ף����");
	prints("\nҲ����Ϊ������/������һ�����Ļ���");
	move(5, 0);
	if (userid == NULL) {
		usercomplete("���������� ID: ", uid);
		if (uid[0] == '\0') {
			clear();
			return 0;
		}
	} else {
		strcpy(uid, userid);
	}
	if (!(tuid = getuser(uid))) {
		move(7, 0);
		prints("[1m�������ʹ���ߴ���( ID )�����ڣ�[m\n");
		pressanykey();
		clear();
		return -1;
	}
	move(5, 0);
	clrtoeol();
	prints("[m���� [1m%s[m ���ԡ�", uid);
	move(6, 0);
	prints("��������[ֱ�Ӱ� ENTER �������ԣ���� 5 �䣬ÿ��� 50 �ַ�]:");
	for (count = 0; count < 5; count++) {
		getdata(7 + count, 0, ": ", tmpbuf, 51, DOECHO, YEA);
		if (tmpbuf[0] == '\0')
			break;;
		for (ptr = tmpbuf; *ptr == ' ' && *ptr != 0; ptr++)
			;
		if (*ptr == 0) {
			count--;
			continue;
		}
		for (i = strlen(ptr) - 1; i < 0; i--)
			if (ptr[i] != ' ')
				break;
		if (i < 0) {
			count--;
			continue;
		}
		ptr[i + 1] = 0;
		strcpy(buf[count], ptr);
	}
	if (count == 0)
		return 0;
	sprintf(genbuf, "��ȷ��Ҫ�����������Ը� [1m%s[m ��", uid);
	move(9 + count, 0);
	if (askyn(genbuf, YEA, NA) == NA) {
		clear();
		return 0;
	}
	sethomefile(genbuf, uid, "GoodWish");
	if ((fp = fopen(genbuf, "a")) == NULL) {
		prints("�޷��������û��ĵײ�������Ϣ�ļ�����֪ͨվ��...\n");
		pressanykey();
		return NA;
	}
	now = time(0);
	timestr = ctime(&now) + 11;
	*(timestr + 5) = '\0';
	for (i = 0; i < count; i++) {
		fprintf(fp, "%s(%s)[%d/%d]��%s\n", currentuser.userid, timestr, i
				+ 1, count, buf[i]);
	}
	fclose(fp);
	sethomefile(genbuf, uid, "HaveNewWish");
	if ((fp = fopen(genbuf, "w+")) != NULL) {
		fputs("Have New Wish", fp);
		fclose(fp);
	}
	move(11 + count, 0);
	prints("�Ѿ������ͳ����������ˡ�");
	sprintf(genbuf, "SendGoodWish to %s", uid);
	report(genbuf, currentuser.userid);
	pressanykey();
	clear();
	return 0;
}
