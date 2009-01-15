#ifndef WITHOUT_ADMIN_TOOLS
#ifndef DLM
#include <stdio.h>
#include "bbs.h"

extern int cmpbnames();
extern int cleanmail();
extern char *chgrp();
extern int dowall();
extern int t_cmpuids();
extern void rebuild_brdshm();
int showperminfo(int, int);
char cexplain[STRLEN];
char buf2[STRLEN];
char lookgrp[30];
char bnames[3][STRLEN]; //����û����ΰ����İ���,���Ϊ��
FILE *cleanlog;

//��userid����Ŀ¼�� ��.bmfile�ļ�,���������������bname��Ƚ�
//              find��Ŵ�1��ʼ�������ΰ�������,Ϊ0��ʾû�ҵ�
//�����ķ���ֵΪuserid���ΰ����İ�����
int getbnames(char *userid, char *bname, int *find) {
	int oldbm = 0;
	FILE *bmfp;
	char bmfilename[STRLEN], tmp[20];
	*find = 0;
	sethomefile(bmfilename, userid, ".bmfile");
	bmfp = fopen(bmfilename, "r");
	if (!bmfp) {
		return 0;
	}
	for (oldbm = 0; oldbm < 3;) {
		fscanf(bmfp, "%s\n", tmp);
		if (!strcmp(bname, tmp)) {
			*find = oldbm + 1;
		}
		strcpy(bnames[oldbm++], tmp);
		if (feof(bmfp)) {
			break;
		}
	}
	fclose(bmfp);
	return oldbm;
}
//      �޸�ʹ��������
int m_info() {
	struct userec uinfo;
	char reportbuf[30];
	int id;

	if (!(HAS_PERM(PERM_USER)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	stand_title("�޸�ʹ��������");
	if (!gettheuserid(1, "������ʹ���ߴ���: ", &id))
		return -1;
	memcpy(&uinfo, &lookupuser, sizeof (uinfo));
	sprintf(reportbuf, "check info: %s", uinfo.userid);
	report(reportbuf, currentuser.userid);

	move(1, 0);
	clrtobot();
	disply_userinfo(&uinfo);
	uinfo_query(&uinfo, 1, id);
	return 0;
}

//��������
int m_ordainBM() {

	int id, pos, oldbm = 0, i, find, bm = 1;
	struct boardheader fh;
	FILE *bmfp;
	char bmfilename[STRLEN], bname[STRLEN];
	char buf[5][STRLEN];

	if (!(HAS_PERM(PERM_USER)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd())
		return;

	clear();
	stand_title("��������\n");
	clrtoeol();
	if (!gettheuserid(2, "������������ʹ�����ʺ�: ", &id))
		return 0;
	if (!gettheboardname(3, "�����ʹ���߽����������������: ", &pos, &fh, bname, 0))
		return -1;
	if (fh.BM[0] != '\0') {
		if (!strncmp(fh.BM, "SYSOPs", 6)) {
			move(5, 0);
			prints("%s �������İ����� SYSOPs �㲻������������", bname);
			pressreturn();
			clear();
			return -1;
		}
		for (i = 0, oldbm = 1; fh.BM[i] != '\0'; i++) {
			if (fh.BM[i] == ' ')
				oldbm++;
		}
		//added by infotech,��ֹ�����б����
		if (i + strlen(lookupuser.userid) > BMNAMEMAXLEN) {
			move(5, 0);
			prints("%s �����������б�̫��,�޷�����!", bname);
			pressreturn();
			clear();
			return -1;
		}
		//add end
		if (oldbm >= 3) {
			move(5, 0);
			prints("%s ���������� %d ������", bname, oldbm);
			pressreturn();
			if (oldbm >= BMMAXNUM) {
				clear();
				return -1;
			}
		}

		bm = 0;
	}
	if (!strcmp(lookupuser.userid, "guest")) {
		move(5, 0);
		prints("�㲻������ guest ������");
		pressanykey();
		clear();
		return -1;
	}
	oldbm = getbnames(lookupuser.userid, bname, &find);
	if (find || oldbm == 3) { //ͬһID���ܼ��γ���������İ���
		move(5, 0);
		prints(" %s �Ѿ���%s��İ�����", lookupuser.userid, find ? "��" : "����");
		pressanykey();
		clear();
		return -1;
	}
	prints("\n�㽫���� %s Ϊ %s ���%s.\n", lookupuser.userid, bname, bm ? "��"
			: "��");
	if (askyn("��ȷ��Ҫ������?", NA, NA) == NA) {
		prints("ȡ����������");
		pressanykey();
		clear();
		return -1;
	}
	strcpy(bnames[oldbm], bname);
	if (!oldbm) { //��һ��������
		char secu[STRLEN];

		lookupuser.userlevel |= PERM_BOARDS;
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
		sprintf(secu, "��������, ���� %s �İ���Ȩ��", lookupuser.userid);
		securityreport(secu, 0, 1);
		move(15, 0);
		prints(secu);
		pressanykey();
		clear();
	}
	if (fh.BM[0] == '\0')
		strcpy(genbuf, lookupuser.userid);
	else
		sprintf(genbuf, "%s %s", fh.BM, lookupuser.userid);
	strlcpy(fh.BM, genbuf, sizeof (fh.BM));
	//added by infotech
	strcpy(buf[0], fh.BM);
#ifdef  BMNAMELISTLIMIT
	for (i = 0; i < BMNAMELISTLEN && buf[0][i]; i++);
	if (i == BMNAMELISTLEN) {
		buf[0][i++] = '.';
		buf[0][i++] = '.';
		buf[0][i++] = '.';
		buf[0][i] = '\0';
	}
#endif
	//endadd
	//sprintf(genbuf, "%-38.38s(BM: %s)", fh.title +8, fh.BM);
	//����������ʾ: ��̬����        ��ʾ10���ո� printf("%*c",10,' ');
	{
		int blanklen; //ǰ�����ռ��С
		static const char BLANK = ' ';
		blanklen = STRLEN - strlen(fh.title + 8) - strlen(buf[0]) - 7;
		blanklen /= 2;
		blanklen = (blanklen > 0) ? blanklen : 1;
		sprintf(genbuf, "%s%*c(BM: %s)", fh.title + 8, blanklen, BLANK,
				buf[0]);
	}
	buf[0][0] = '\0';
	get_grp(fh.filename);
	edit_grp(fh.filename, lookgrp, fh.title + 8, genbuf);
	substitute_record(BOARDS, &fh, sizeof (fh), pos);
	sethomefile(bmfilename, lookupuser.userid, ".bmfile");
	bmfp = fopen(bmfilename, "w+");
	for (i = 0; i < oldbm + 1; i++) {
		fprintf(bmfp, "%s\n", bnames[i]);
	}
	fclose(bmfp);
	/* Modified by Amigo 2002.07.01. Add reference to BM-Guide. */
	//sprintf (genbuf, "\n\t\t\t�� ͨ�� ��\n\n"
	//	   "\t���� %s Ϊ %s ��%s��\n"
	//	   "\t��ӭ %s ǰ�� BM_Home ��ͱ��� Zone �������ʺá�\n"
	//	   "\t��ʼ����ǰ������ͨ��BM_Home�澫�����İ���ָ��Ŀ¼��\n",
	//	   lookupuser.userid, bname, bm ? "����" : "�渱", lookupuser.userid);

	//the new version add by Danielfree 06.11.12
	sprintf(
			genbuf,
			"\n"
				" 		[1;31m   �X�T�[�X�T�[�X�T�[�X�T�[										 [m\n"
				" 	 [31m�賓��[1m�U[33m��[31m�U�U[33m��[31m�U�U[33m��[31m�U�U[33m��[31m�U[0;33m����[1;36m�����վ�澫����Ϥ����������[0;33m�����  [m\n"
				" 	 [31m��    [1m�^�T�a�^�T�a�^�T�a�^�T�a										  [m\n"
				" 	 [31m��																	  [m\n"
				" 		 [1;33m��	[37m����  %s  Ϊ  %s  ��%s��							   [m\n"
				" 		 [1;33mͨ																  [m\n"
				" 		[1m	��ӭ  %s  ǰ�� BM_Home ��ͱ��� Zone �������ʺá�			 [m\n"
				" 		 [1;33m��																  [m\n"
				" 		 [1;33m��	[37m��ʼ����ǰ������ͨ��BM_Home�澫�����İ���ָ��Ŀ¼��		   [m\n"
				" 																		 [33m��  [m\n"
				" 											 [1;33m�X�T�[�X�T�[�X�T�[�X�T�[   [0;33m ��  [m\n"
				" 	 [31m�����[1;35m��ά���������򡤽����г�⻪��[0;31m����[1;33m�U[31m��[33m�U�U[31m��[33m�U�U[31mί[33m�U�U[31m��[33m�U[0;33m������	[m\n"
				" 											 [1;33m�^�T�a�^�T�a�^�T�a�^�T�a		  [m\n"
				" 																			 [m\n", lookupuser.userid, bname,
			bm ? "����" : "�渱", lookupuser.userid);
	//add end
	for (i = 0; i < 5; i++)
		buf[i][0] = '\0';
	move(8, 0);
	prints("��������������(������У��� Enter ����)");
	for (i = 0; i < 5; i++) {
		getdata(i + 9, 0, ": ", buf[i], STRLEN - 5, DOECHO, YEA);
		if (buf[i][0] == '\0')
			break;
	}
	for (i = 0; i < 5; i++) {
		if (buf[i][0] == '\0')
			break;
		if (i == 0)
			strcat(genbuf, "\n\n");
		strcat(genbuf, "\t");
		strcat(genbuf, buf[i]);
		strcat(genbuf, "\n");
	}
	strcpy(currboard, bname);
	sprintf(bmfilename, "���� %s Ϊ %s ��%s", lookupuser.userid, fh.filename,
			bm ? "����" : "�渱");
	//autoreport(bmfilename,genbuf,YEA,NULL);
	autoreport(bmfilename, genbuf, YEA, lookupuser.userid, 1); //3x rubing and erebus:)   
#ifdef ORDAINBM_POST_BOARDNAME
	strcpy (currboard, ORDAINBM_POST_BOARDNAME);
	//autoreport(bmfilename,genbuf,YEA,NULL);
	autoreport (bmfilename, genbuf, YEA, lookupuser.userid, 1);
#endif
	/* Added by Amigo 2002.07.01. Send BM assign mail to user's mail box. */
	//{
	//  FILE *se;
	//  char fname[STRLEN];

	//  sprintf( fname, "tmp/AutoPoster.%s.%05d", currentuser.userid, uinfo.pid );
	//  if( ( se = fopen(fname,"w") ) != NULL ){
	//          fprintf( se, "%s", genbuf );
	//          fclose( se );
	//          if( lookupuser.userid != NULL )
	//          mail_file( fname, lookupuser.userid, bmfilename );
	//  }
	//}
	/* Add end. */
	securityreport(bmfilename, 0, 1);
	move(16, 0);
	prints(bmfilename);
	pressanykey();
	return 0;
}

//������ְ
int m_retireBM() {
	int id, pos, right = 0, oldbm = 0, i, j, bmnum;
	int find, bm = 1;
	struct boardheader fh;
	FILE *bmfp;
	char bmfilename[STRLEN], buf[5][STRLEN];
	char bname[STRLEN], usernames[BMMAXNUM][STRLEN];

	if (!(HAS_PERM(PERM_USER)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd())
		return;

	clear();
	stand_title("������ְ\n");
	clrtoeol();
	if (!gettheuserid(2, "��������ְ�İ����ʺ�: ", &id))
		return -1;
	if (!gettheboardname(3, "������ð���Ҫ��ȥ�İ���: ", &pos, &fh, bname, 0))
		return -1;
	oldbm = getbnames(lookupuser.userid, bname, &find);
	if (!oldbm || !find) {
		move(5, 0);
		prints(" %s %s����������д�����֪ͨ����վ����", lookupuser.userid,
				(oldbm) ? "���Ǹ�" : "û�е����κ�");
		pressanykey();
		clear();
		return -1;
	}
	for (i = find - 1; i < oldbm; i++) {
		if (i != oldbm - 1)
			strcpy(bnames[i], bnames[i + 1]);
	}
	bmnum = 0;
	for (i = 0, j = 0; fh.BM[i] != '\0'; i++) {
		if (fh.BM[i] == ' ') {
			usernames[bmnum][j] = '\0';
			bmnum++;
			j = 0;
		} else {
			usernames[bmnum][j++] = fh.BM[i];
		}
	}
	usernames[bmnum++][j] = '\0';
	for (i = 0, right = 0; i < bmnum; i++) {
		if (!strcmp(usernames[i], lookupuser.userid)) {
			right = 1;
			if (i)
				bm = 0;
		}
		if (right && i != bmnum - 1) //while(right&&i<bmnum)�ƺ�����һЩ;infotech
			strcpy(usernames[i], usernames[i + 1]);
	}
	if (!right) {
		move(5, 0);
		prints("�Բ��� %s �����������û�� %s �����д�����֪ͨ����վ����", bname,
				lookupuser.userid);
		pressanykey();
		clear();
		return -1;
	}
	prints("\n�㽫ȡ�� %s �� %s ���%sְ��.\n", lookupuser.userid, bname, bm ? "��"
			: "��");
	if (askyn("��ȷ��Ҫȡ�����ĸð����ְ����?", NA, NA) == NA) {
		prints("\n�Ǻǣ���ı������ˣ� %s �������� %s �����ְ��", lookupuser.userid, bname);
		pressanykey();
		clear();
		return -1;
	}
	if (bmnum - 1) { //���а���,Ϊʲô����strcat ?
		sprintf(genbuf, "%s", usernames[0]);
		for (i = 1; i < bmnum - 1; i++)
			sprintf(genbuf, "%s %s", genbuf, usernames[i]);
	} else {
		genbuf[0] = '\0';
	}
	strlcpy(fh.BM, genbuf, sizeof (fh.BM));
	if (fh.BM[0] != '\0') {
		//added by infotech
		strcpy(buf[0], fh.BM);
#ifdef BMNAMELISTLIMIT
		for (i = 0; i < BMNAMELISTLEN && buf[0][i]; i++);
		if (i == BMNAMELISTLEN) {
			buf[0][i++] = '.';
			buf[0][i++] = '.';
			buf[0][i++] = '.';
			buf[0][i] = '\0';
		}
#endif
		//endadd
		//sprintf(genbuf, "%-38.38s(BM: %s)", fh.title +8, fh.BM);
		//����������ʾ: ��̬����        ��ʾ10���ո� printf("%*c",10,' ');
		{
			int blanklen; //ǰ�����ռ��С
			static const char BLANK = ' ';
			blanklen = STRLEN - strlen(fh.title + 8) - strlen(buf[0]) - 7;
			blanklen /= 2;
			blanklen = (blanklen > 0) ? blanklen : 1;
			sprintf(genbuf, "%s%*c(BM: %s)", fh.title + 8, blanklen,
					BLANK, buf[0]);
		}
	} else {
		sprintf(genbuf, "%-38.38s", fh.title + 8);
	}
	get_grp(fh.filename);
	edit_grp(fh.filename, lookgrp, fh.title + 8, genbuf);
	substitute_record(BOARDS, &fh, sizeof (fh), pos);
	sprintf(genbuf, "ȡ�� %s �� %s �����ְ��", lookupuser.userid, fh.filename);
	securityreport(genbuf, 0, 1);
	move(8, 0);
	prints(genbuf);
	sethomefile(bmfilename, lookupuser.userid, ".bmfile");
	if (oldbm - 1) {
		bmfp = fopen(bmfilename, "w+");
		for (i = 0; i < oldbm - 1; i++)
			fprintf(bmfp, "%s\n", bnames[i]);
		fclose(bmfp);
	} else {
		char secu[STRLEN];

		unlink(bmfilename);
		if (!(lookupuser.userlevel & PERM_OBOARDS) //û������������Ȩ��
				&& !(lookupuser.userlevel & PERM_SYSOPS) //û��վ��Ȩ��
		) {
			lookupuser.userlevel &= ~PERM_BOARDS;
			substitut_record(PASSFILE, &lookupuser, sizeof(struct userec),
					id);
			sprintf(secu, "����жְ, ȡ�� %s �İ���Ȩ��", lookupuser.userid);
			securityreport(secu, 0, 1);
			move(9, 0);
			prints(secu);
		}
	}
	prints("\n\n");
	if (askyn("��Ҫ����ذ��淢��ͨ����?", YEA, NA) == NA) {
		pressanykey();
		return 0;
	}
	prints("\n");
	if (askyn("���������밴 Enter ��ȷ�ϣ���ְ�ͷ��� N ��", YEA, NA) == YEA)
		right = 1;
	else
		right = 0;
	if (right)
		sprintf(bmfilename, "%s ��%s %s ����ͨ��", bname, bm ? "����" : "�渱",
				lookupuser.userid);
	else
		sprintf(bmfilename, "[ͨ��]���� %s ��%s %s", bname, bm ? "����" : "�渱",
				lookupuser.userid);
	strcpy(currboard, bname);
	if (right) {
		sprintf(genbuf, "\n\t\t\t�� ͨ�� ��\n\n"
			"\t��վ��ίԱ�����ۣ�\n"
			"\tͬ�� %s ��ȥ %s ���%sְ��\n"
			"\t�ڴˣ����������� %s �������������ʾ��л��\n\n"
			"\tϣ�����Ҳ��֧�ֱ���Ĺ���.\n", lookupuser.userid, bname, bm ? "����"
				: "�渱", bname);
	} else {
		sprintf(genbuf, "\n\t\t\t����ְͨ�桿\n\n"
			"\t��վ��ίԱ�����۾�����\n"
			"\t���� %s ��%s %s ��%sְ��\n", bname, bm ? "����" : "�渱",
				lookupuser.userid, bm ? "����" : "�渱");
	}
	for (i = 0; i < 5; i++)
		buf[i][0] = '\0';
	move(14, 0);
	prints("������%s����(������У��� Enter ����)", right ? "��������" : "������ְ");
	for (i = 0; i < 5; i++) {
		getdata(i + 15, 0, ": ", buf[i], STRLEN - 5, DOECHO, YEA);
		if (buf[i][0] == '\0')
			break;
		//      if(i == 0) strcat(genbuf,right?"\n\n���θ��ԣ�\n":"\n\n��ְ˵����\n");
		if (i == 0)
			strcat(genbuf, "\n\n");
		strcat(genbuf, "\t");
		strcat(genbuf, buf[i]);
		strcat(genbuf, "\n");
	}
	//autoreport(bmfilename,genbuf,YEA,NULL);
	autoreport(bmfilename, genbuf, YEA, lookupuser.userid, 1);

	/* Added by Amigo 2002.07.01. Send BM assign mail to mail box. */
	/*	{
	 FILE *	se;
	 char 	fname[STRLEN];

	 sprintf( fname, "tmp/AutoPoster.%s.%05d", currentuser.userid, uinfo.pid );
	 if( ( se = fopen(fname,"w") ) != NULL ){
	 fprintf( se, "%s", genbuf );
	 fclose( se );
	 if( lookupuser.userid != NULL )
	 mail_file( fname, lookupuser.userid, bmfilename );
	 }
	 }*/
	/* Add end. */
	prints("\nִ����ϣ�");
	pressanykey();
	return 0;
}

//  �����°�
int m_newbrd() {
	struct boardheader newboard, fh;
	char ans[20];
	char vbuf[100];
	char *group;
	int bid, pos;

	if (!(HAS_PERM(PERM_BLEVELS)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	stand_title("������������");
	memset(&newboard, 0, sizeof (newboard));
	move(2, 0);
	ansimore2("etc/boardref", NA, 3, 7);
	while (1) {
		getdata(10, 0, "����������:   ", newboard.filename, 18, DOECHO, YEA);
		if (newboard.filename[0] != 0) {
			struct boardheader dh;
			if (search_record(BOARDS, &dh, sizeof (dh), cmpbnames,
					newboard.filename)) {
				prints("\n����! ���������Ѿ�����!!");
				pressanykey();
				return -1;
			}
		} else {
			return -1;
		}
		if (valid_brdname(newboard.filename))
			break;
		prints("\n���Ϸ�����!!");
	}
	newboard.flag = 0;
	while (1) {
		getdata(11, 0, "������˵��:   ", newboard.title, 60, DOECHO, YEA);
		if (newboard.title[0] == '\0')
			return -1;
		if (strstr(newboard.title, "��") || strstr(newboard.title, "��")) {
			newboard.flag |= BOARD_OUT_FLAG;
			break;
		} else if (strstr(newboard.title, "��")) {
			newboard.flag &= ~BOARD_OUT_FLAG;
			break;
		} else {
			prints("����ĸ�ʽ, �޷��ж��Ƿ�ת��!!");
		}
	}
	strcpy(vbuf, "vote/");
	strcat(vbuf, newboard.filename);
	setbpath(genbuf, newboard.filename);
	if (getbnum(newboard.filename, &currentuser) > 0 || mkdir(genbuf, 0755) == -1
			|| mkdir(vbuf, 0755) == -1) {
		prints("\n���������������!!\n");
		pressreturn();
		clear();
		return -1;
	}
	//sprintf(vbuf, "/dev/shm/bbs/boards/%s", newboard.filename);
	//mkdir(vbuf, 0755);

	move(12, 0);
	if (gettheboardname(12, "����������������: ", &pos, &fh, ans, 2)) {
		newboard.group = pos;
	} else {
		newboard.group = 0;
		newboard.flag |= BOARD_NOZAP_FLAG; //root dir can't zap.Danielfree 06.2.22
	}
	if (askyn("������Ŀ¼��?", NA, NA) == YEA) {
		newboard.flag |= BOARD_DIR_FLAG;
		//suggest by monoply.06.2.22
		newboard.flag |= BOARD_JUNK_FLAG;
		newboard.flag |= BOARD_NOREPLY_FLAG;
		newboard.flag |= BOARD_POST_FLAG;
		if (askyn("�Ƿ����ƴ�ȡȨ��", NA, NA) == YEA) {
			getdata(14, 0, "���� Read? [R]: ", ans, 2, DOECHO, YEA);
			move(1, 0);
			clrtobot();
			move(2, 0);
			prints("�趨 %s Ȩ��. ������: '%s'\n", "READ", newboard.filename);
			newboard.level = setperms(newboard.level, "Ȩ��", NUMPERMS,
					showperminfo);
			clear();
		} else {
			newboard.level = 0;
		}
		//add  end
	} else {
		newboard.flag &= ~BOARD_DIR_FLAG;

		if (askyn("�������������(������SYSOPs����)?", YEA, NA) == NA) {
			strcpy(newboard.BM, "SYSOPs");
		} else {
			newboard.BM[0] = '\0';
		}

		if (askyn("�ð��ȫ�����¾������Իظ�", NA, NA) == YEA) {
			newboard.flag |= BOARD_NOREPLY_FLAG;
		} else {
			newboard.flag &= ~BOARD_NOREPLY_FLAG;
		}

		if (askyn("�Ƿ��Ǿ��ֲ�����", NA, NA) == YEA) {
			newboard.flag |= BOARD_CLUB_FLAG;
			if (askyn("�Ƿ�����ƾ��ֲ�����", NA, NA) == YEA) {
				newboard.flag |= BOARD_READ_FLAG;
			} else {
				newboard.flag &= ~BOARD_READ_FLAG;
			}
		} else {
			newboard.flag &= ~BOARD_CLUB_FLAG;
		}

		if (askyn("�Ƿ񲻼���������", NA, NA) == YEA) {
			newboard.flag |= BOARD_JUNK_FLAG;
		} else {
			newboard.flag &= ~BOARD_JUNK_FLAG;
		}

		if (askyn("�Ƿ����������", NA, NA) == YEA) {
			newboard.flag |= BOARD_ANONY_FLAG;
		} else {
			newboard.flag &= ~BOARD_ANONY_FLAG;
		}
#ifdef ENABLE_PREFIX
		if (askyn ("�Ƿ�ǿ��ʹ��ǰ׺", NA, NA) == YEA) {
			newboard.flag |= BOARD_PREFIX_FLAG;
		} else {
			newboard.flag &= ~BOARD_PREFIX_FLAG;
		}
#endif
		if (askyn("�Ƿ����ƴ�ȡȨ��", NA, NA) == YEA) {
			getdata(14, 0, "���� Read/Post? [R]: ", ans, 2, DOECHO, YEA);
			if (*ans == 'P' || *ans == 'p') {
				newboard.flag |= BOARD_POST_FLAG;
			} else {
				newboard.flag &= ~BOARD_POST_FLAG;
			}
			move(1, 0);
			clrtobot();
			move(2, 0);
			prints("�趨 %s Ȩ��. ������: '%s'\n", (newboard.flag
					& BOARD_POST_FLAG ? "POST" : "READ"),
					newboard.filename);
			newboard.level = setperms(newboard.level, "Ȩ��", NUMPERMS,
					showperminfo);
			clear();
		} else {
			newboard.level = 0;
		}
	}
	if (askyn("�Ƿ� ���� ZAP��������", (newboard.flag & BOARD_NOZAP_FLAG) ? NA
			: YEA, YEA) == NA) {
		newboard.flag |= BOARD_NOZAP_FLAG;
	} else {
		newboard.flag &= ~BOARD_NOZAP_FLAG;
	}
	if ((bid = getblankbnum()) > 0) {
		substitute_record(BOARDS, &newboard, sizeof (newboard), bid);
		flush_bcache();
	} else if (append_record(BOARDS, &newboard, sizeof (newboard)) == -1) {
		pressreturn();
		clear();
		return -1;
	}

	if (!(newboard.flag & BOARD_DIR_FLAG)) {
		group = chgrp();
		if (group != NULL) {
			if (newboard.BM[0] != '\0') {
				sprintf(vbuf, "%-38.38s(BM: %s)", newboard.title + 8,
						newboard.BM);
			} else {
				sprintf(vbuf, "%-38.38s", newboard.title + 8);
			}
			if (add_grp(group, cexplain, newboard.filename, vbuf) == -1) {
				prints("\n����������ʧ��....\n");
			} else {
				prints("�Ѿ����뾫����...\n");
			}
		}
	}

	flush_bcache();
	rebuild_brdshm(); //add by cometcaptor 2006-10-13
	prints("\n������������\n");

	char secu[STRLEN];
	sprintf(secu, "�����°棺%s", newboard.filename);
	securityreport(secu, 0, 1);

	pressreturn();
	clear();
	return 0;
}

//      �޸��������趨
int m_editbrd() {
	char bname[STRLEN], buf[STRLEN], oldtitle[STRLEN], vbuf[256], *group;
	char type[10];
	char oldpath[STRLEN], newpath[STRLEN], tmp_grp[30];
	int pos, tmppos, a_mv;
	struct boardheader fh, newfh, tmpfh;

	a_mv = 0; // added by Danielfree 05.12.4

	//added by roly 02.03.07
	if (!(HAS_PERM(PERM_BLEVELS)))
		return;
	//add end

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	stand_title("�޸���������Ѷ");
	if (!gettheboardname(2, "��������������: ", &pos, &fh, bname, 0))
		return -1;
	if (fh.flag & BOARD_DIR_FLAG)
		sprintf(type, "Ŀ¼");
	else
		sprintf(type, "����");
	move(2, 0);
	memcpy(&newfh, &fh, sizeof (newfh));
	while (1) {
		clear();
		stand_title("�޸���������Ѷ");
		move(2, 0);
		prints("1)�޸�%s����:            %s\n", type, newfh.filename);
		prints("2)�޸�%s˵��:            %s\n", type, newfh.title);
		prints("3)�޸�%s����Ա:          %s\n", type, newfh.BM);
		prints("4)�޸�%s����Ŀ¼:        %s(%d)\n", type,
				bcache[fh.group - 1].filename, newfh.group);
		if (fh.flag & BOARD_DIR_FLAG) {
			prints("5)�޸�%s��д����:        %s\n", type,
					(newfh.level == 0) ? "û������" : "r(�����Ķ�)");
		} else {
			prints("5)�޸�%s��д����:        %s\n", type, (newfh.flag
					& BOARD_POST_FLAG) ? "p(���Ʒ���)"
					: (newfh.level == 0) ? "û������" : "r(�����Ķ�)");
		}
		//zap dir and board. Danielfree 06.2.22
		prints("6)����ZAP%s:             %s\n", type, (newfh.flag
				& BOARD_NOZAP_FLAG) ? "��" : "��");
		if (!(newfh.flag & BOARD_DIR_FLAG)) {
			prints("7)�ƶ�������λ��\n");
			//prints ("7)����ZAP����:             %s\n",
			//    (newfh.flag & BOARD_POST_FLAG) ? "��" : "��");
			prints("8)��������:                %s\n", (newfh.flag
					& BOARD_ANONY_FLAG) ? "����" : "������");
			prints("9)���Իظ�:                %s\n", (newfh.flag
					& BOARD_NOREPLY_FLAG) ? "���ɻظ�" : "���Իظ�");
			prints("A)�Ƿ����������:          %s\n", (newfh.flag
					& BOARD_JUNK_FLAG) ? "������" : "����");
			prints(
					"B)���ֲ�����:              %s\n",
					(newfh.flag & BOARD_CLUB_FLAG) ? (newfh.flag
							& BOARD_READ_FLAG) ? "\033[1;31mc\033[0m(�����ƾ��ֲ�)"
							: "\033[1;33mc\033[0m(��ͨ���ֲ�)"
							: "���Ǿ��ֲ�");
#ifdef ENABLE_PREFIX
			prints ("C)�Ƿ�ǿ��ʹ��ǰ׺:        %s\n",
					(newfh.flag & BOARD_PREFIX_FLAG) ? "����" : "����");
#endif
			getdata(14, 0, "����������Ѷ��[1-9,A,B][0]", genbuf, 2, DOECHO, YEA);
		} else {
			getdata(14, 0, "����������Ѷ��[1-6][0]", genbuf, 2, DOECHO, YEA);
		}
		if (genbuf[0] == '0' || genbuf[0] == 0)
			break;
		move(15, 0);
		strcpy(oldtitle, fh.title);
		switch (genbuf[0]) {
			case '1':
				while (1) {
					sprintf(buf, "������������[%s]: ", fh.filename);
					getdata(15, 0, buf, genbuf, 18, DOECHO, YEA);
					if (*genbuf != 0) {
						struct boardheader dh;
						if (search_record(BOARDS, &dh, sizeof (dh),
								cmpbnames, genbuf)) {
							move(16, 0);
							prints("����! ���������Ѿ�����!!");
							move(0, 0);
							clrtoeol();
							continue;
						}
						if (valid_brdname(genbuf)) {
							strlcpy(newfh.filename, genbuf,
									sizeof (newfh.filename));
							strcpy(bname, genbuf);
							break;
						} else {
							move(16, 0);
							prints("���Ϸ�������������!");
							move(0, 0);
							clrtoeol();
							continue;
						}
					} else {
						break;
					}
				}
				break;
			case '2':
				ansimore2("etc/boardref", NA, 11, 7);
				snprintf(genbuf, sizeof(newfh.title), "%s", newfh.title);
				while (1) {
					getdata(22, 0, "��������˵��: ", genbuf, 60, DOECHO, YEA);
					if (*genbuf != 0) {
						strlcpy(newfh.title, genbuf, sizeof (newfh.title));
					} else {
						break;
					}
					if (strstr(newfh.title, "��") || strstr(newfh.title,
							"��")) {
						newfh.flag |= BOARD_OUT_FLAG;
						break;
					} else if (strstr(newfh.title, "��")) {
						newfh.flag &= ~BOARD_OUT_FLAG;
						break;
					} else {
						prints("\n����ĸ�ʽ, �޷��ж��Ƿ�ת��!!");
					}
				}
				break;
			case '3':
				if (fh.BM[0] != '\0' && strcmp(fh.BM, "SYSOPs")) {
					if (askyn("�޸�����������Ա��ע�⣺������������ʹ�ã�������������Ķ��˴���", NA, NA)
							== YEA) {
						getdata(16, 0, "����������Ա: ", newfh.BM,
								sizeof (newfh.BM), DOECHO, YEA);
						if (newfh.BM[0] == '\0') {
							strcpy(newfh.BM, fh.BM);
						} else if (newfh.BM[0] == ' ') {
							newfh.BM[0] = '\0';
						}
					}
				} else {
					if (askyn("�������������(������SYSOPs����)?", YEA, NA) == NA) {
						strlcpy(newfh.BM, "SYSOPs", sizeof (newfh.BM));
					} else {
						strlcpy(newfh.BM, "\0", sizeof (newfh.BM));
					}
				}
				break;
			case '4':
				if (gettheboardname(15, "����������������: ", &tmppos, &tmpfh,
						genbuf, 2))
					newfh.group = tmppos;
				else if (askyn("����������Ϊ��Ŀ¼ô��", NA, NA) == YEA)
					newfh.group = 0;
				break;
			case '5':
				if (fh.flag & BOARD_DIR_FLAG) { //modiy for dir. Danielfree 06.2.23
					sprintf(buf, "(N)������ (R)�����Ķ� [%c]: ",
							(newfh.level) ? 'R' : 'N');
					getdata(15, 0, buf, genbuf, 2, DOECHO, YEA);
					if (genbuf[0] == 'N' || genbuf[0] == 'n') {
						newfh.flag &= ~BOARD_POST_FLAG;
						newfh.level = 0;
					} else {
						if (genbuf[0] == 'R' || genbuf[0] == 'r')
							newfh.flag &= ~BOARD_POST_FLAG;
						clear();
						move(2, 0);
						prints("�趨 %s '%s' ��������Ȩ��\n", "�Ķ�", newfh.filename);
						newfh.level = setperms(newfh.level, "Ȩ��",
								NUMPERMS, showperminfo);
						clear();
					}
				} // if dir
				else { //if board
					sprintf(buf, "(N)������ (R)�����Ķ� (P)�������� ���� [%c]: ",
							(newfh.flag & BOARD_POST_FLAG) ? 'P' : (newfh.
							level) ? 'R' : 'N');
					getdata(15, 0, buf, genbuf, 2, DOECHO, YEA);
					if (genbuf[0] == 'N' || genbuf[0] == 'n') {
						newfh.flag &= ~BOARD_POST_FLAG;
						newfh.level = 0;
					} else {
						if (genbuf[0] == 'R' || genbuf[0] == 'r')
							newfh.flag &= ~BOARD_POST_FLAG;
						else if (genbuf[0] == 'P' || genbuf[0] == 'p')
							newfh.flag |= BOARD_POST_FLAG;
						clear();
						move(2, 0);
						prints("�趨 %s '%s' ��������Ȩ��\n", newfh.flag
								& BOARD_POST_FLAG ? "����" : "�Ķ�",
								newfh.filename);
						newfh.level = setperms(newfh.level, "Ȩ��",
								NUMPERMS, showperminfo);
						clear();
					}
				}
				break;
				//both dir and board can zap. Danielfree 06.2.22
			case '6':
				if (askyn("�Ƿ� ���� ZAP��������",
						(fh.flag & BOARD_NOZAP_FLAG) ? NA : YEA, YEA)
						== NA) {
					newfh.flag |= BOARD_NOZAP_FLAG;
				} else {
					newfh.flag &= ~BOARD_NOZAP_FLAG;
				}
				break;
				//modify end
			default:
				if (!(fh.flag & BOARD_DIR_FLAG)) {
					switch (genbuf[0]) {
						case '7':
							a_mv = 2;
							break; // move from out of default into default -.- Danielfree 05.12.4
						case '8':
							if (askyn("�Ƿ������棿", (fh.flag
									& BOARD_ANONY_FLAG) ? YEA : NA, NA)
									== YEA) {
								newfh.flag |= BOARD_ANONY_FLAG;
							} else {
								newfh.flag &= ~BOARD_ANONY_FLAG;
							}

							break;
						case '9':
							if (askyn("�����Ƿ� ���� �ظ���", (fh.flag
									& BOARD_NOREPLY_FLAG) ? NA : YEA, YEA)
									== NA) {
								newfh.flag |= BOARD_NOREPLY_FLAG;
							} else {
								newfh.flag &= ~BOARD_NOREPLY_FLAG;
							}
							break;
						case 'a':
						case 'A':
							if (askyn("�Ƿ� ������ ��������", (fh.flag
									& BOARD_JUNK_FLAG) ? YEA : NA, NA)
									== YEA) {
								newfh.flag |= BOARD_JUNK_FLAG;
							} else {
								newfh.flag &= ~BOARD_JUNK_FLAG;
							}
							break;
						case 'b':
						case 'B':
							if (askyn("�Ƿ���ֲ����棿", (fh.flag
									& BOARD_CLUB_FLAG) ? YEA : NA, NA)
									== YEA) {
								newfh.flag |= BOARD_CLUB_FLAG;
								if (askyn("�Ƿ�����ƾ��ֲ���", (fh.flag
										& BOARD_READ_FLAG) ? YEA : NA, NA)
										== YEA) {
									newfh.flag |= BOARD_READ_FLAG;
								} else {
									newfh.flag &= ~BOARD_READ_FLAG;
								}
							} else {
								newfh.flag &= ~BOARD_CLUB_FLAG;
								newfh.flag &= ~BOARD_READ_FLAG;
							}
							break;
#ifdef ENABLE_PREFIX
							case 'c':
							case 'C':
							if (askyn("�Ƿ�ǿ��ʹ��ǰ׺��", (fh.flag & BOARD_PREFIX_FLAG) ? YEA : NA, NA)
									== YEA) {
								newfh.flag |= BOARD_PREFIX_FLAG;
							} else {
								newfh.flag &= ~BOARD_PREFIX_FLAG;
							}
#endif
					}//wswitch
				}//if dir
		}//switch
	}//while
	getdata(23, 0, "ȷ��Ҫ������? (Y/N) [N]: ", genbuf, 4, DOECHO, YEA);
	if (*genbuf == 'Y' || *genbuf == 'y') {
		char secu[STRLEN];
		sprintf(secu, "�޸���������%s(%s)", fh.filename, newfh.filename);
		securityreport(secu, 0, 1);
		if (strcmp(fh.filename, newfh.filename)) {
			char old[256], tar[256];
			a_mv = 1;
			setbpath(old, fh.filename);
			setbpath(tar, newfh.filename);
			rename(old, tar);
			sprintf(old, "vote/%s", fh.filename);
			sprintf(tar, "vote/%s", newfh.filename);
			rename(old, tar);
		}
		if (newfh.BM[0] != '\0')
			sprintf(vbuf, "%-38.38s(BM: %s)", newfh.title + 8, newfh.BM);
		else
			sprintf(vbuf, "%-38.38s", newfh.title + 8);
		get_grp(fh.filename);
		edit_grp(fh.filename, lookgrp, oldtitle + 8, vbuf);
		if (a_mv >= 1) {
			group = chgrp();
			get_grp(fh.filename);
			strcpy(tmp_grp, lookgrp);
			if (strcmp(tmp_grp, group) || a_mv == 1) {
				char tmpbuf[160];
				sprintf(tmpbuf, "%s:", fh.filename);
				del_from_file("0Announce/.Search", tmpbuf);
				if (group != NULL) {
					if (newfh.BM[0] != '\0')
						sprintf(vbuf, "%-38.38s(BM: %s)", newfh.title + 8,
								newfh.BM);
					else
						sprintf(vbuf, "%-38.38s", newfh.title + 8);
					if (add_grp(group, cexplain, newfh.filename, vbuf)
							== -1)
						prints("\n����������ʧ��....\n");
					else
						prints("�Ѿ����뾫����...\n");
					sprintf(newpath, "0Announce/groups/%s/%s", group,
							newfh.filename);
					sprintf(oldpath, "0Announce/groups/%s/%s", tmp_grp,
							fh.filename);
					if (strcmp(oldpath, newpath) != 0 && dashd(oldpath)) {
						deltree(newpath);
						rename(oldpath, newpath);
						del_grp(tmp_grp, fh.filename, fh.title + 8);
					} // add by Danielfree,suggest by fancitron 05.12.4
				} // if group !=NULL
			} // if strcmp
		} // if a_mv >= 1
		substitute_record(BOARDS, &newfh, sizeof (newfh), pos);
		sprintf(genbuf, "���������� %s ������ --> %s", fh.filename, newfh.filename);
		report(genbuf, currentuser.userid);
		// numboards = -1;/* force re-caching */
		flush_bcache();
	} // if askyn
	clear();
	return 0;

}

// ��ע�ᵥʱ��ʾ�ı���
void regtitle() {
	prints("[1;33;44m��ע�ᵥ NEW VERSION wahahaha                                                   [m\n");
	prints(" �뿪[[1;32m��[m,[1;32me[m] ѡ��[[1;32m��[m,[1;32m��[m] �Ķ�[[1;32m��[m,[1;32mRtn[m] ��׼[[1;32my[m] ɾ��[[1;32md[m]\n");

	prints("[1;37;44m  ��� �û�ID       ��  ��       ϵ��             סַ             ע��ʱ��     [m\n");
}

//      ����ע�ᵥʱ��ʾ��ע��ID�б�
char *regdoent(int num, REGINFO * ent) {
	static char buf[128];
	char rname[17];
	char dept[17];
	char addr[17];
	//struct tm* tm;
	//tm=gmtime(&ent->regdate);
	strlcpy(rname, ent->realname, 12);
	strlcpy(dept, ent->dept, 16);
	strlcpy(addr, ent->addr, 16);
	ellipsis(rname, 12);
	ellipsis(dept, 16);
	ellipsis(addr, 16);
	getdatestring(ent->regdate, 2);
	sprintf(buf, "  %4d %-12s %-12s %-16s %-16s %s", num, ent->userid,
			rname, dept, addr, datestring);
	return buf;
}

//      ����userid ��ent->userid�Ƿ����
int filecheck(REGINFO * ent, char *userid) {
	return !strcmp(ent->userid, userid);
}

// ɾ��ע�ᵥ�ļ����һ����¼
int delete_register(int index, REGINFO * ent, char *direct) {
	delete_record(direct, sizeof(REGINFO), index, filecheck, ent->userid);
	return DIRCHANGED;
}

//      ͨ��ע�ᵥ
int pass_register(int index, REGINFO * ent, char *direct) {
	int unum;
	struct userec uinfo;
	char buf[80];
	FILE *fout;

	strlcpy(uinfo.realname, ent->realname, NAMELEN);
	unum = getuser(ent->userid);
	if (!unum) {
		clear();
		prints("ϵͳ����! ���޴��˺�!\n"); //      �ڻص�����ĳЩ�����,�Ҳ�����ע�ᵥ�ļ�
		pressanykey(); // unregister�еĴ˼�¼,��ɾ��
		delete_record(direct, sizeof(REGINFO), index, filecheck,
				ent->userid);
		return DIRCHANGED;
	}

	delete_record(direct, sizeof(REGINFO), index, filecheck, ent->userid);

	memcpy(&uinfo, &lookupuser, sizeof (uinfo));
	strlcpy(uinfo.address, ent->addr, NAMELEN);
	sprintf(genbuf, "%s$%s@%s", ent->dept, ent->phone, currentuser.userid);
	genbuf[STRLEN - 16] = '\0';
	strlcpy(uinfo.reginfo, genbuf, STRLEN - 17);
#ifdef ALLOWGAME
	uinfo.money = 1000;
#endif
	uinfo.lastjustify = time(0);
	substitut_record(PASSFILE, &uinfo, sizeof (uinfo), unum);
	sethomefile(buf, uinfo.userid, "register");
	if ((fout = fopen(buf, "a")) != NULL) {
		getdatestring(ent->regdate, YEA);
		fprintf(fout, "ע��ʱ��     : %s\n", datestring);
		fprintf(fout, "�����ʺ�     : %s\n", ent->userid);
		fprintf(fout, "��ʵ����     : %s\n", ent->realname);
		fprintf(fout, "ѧУϵ��     : %s\n", ent->dept);
		fprintf(fout, "Ŀǰסַ     : %s\n", ent->addr);
		fprintf(fout, "����绰     : %s\n", ent->phone);
#ifndef FDQUAN
		fprintf(fout, "�����ʼ�     : %s\n", ent->email);
#endif
		fprintf(fout, "У �� ��     : %s\n", ent->assoc);
		getdatestring(time(0), YEA);
		fprintf(fout, "�ɹ�����     : %s\n", datestring);
		fprintf(fout, "��׼��       : %s\n", currentuser.userid);
		fclose(fout);
	}
	mail_file("etc/s_fill", uinfo.userid, "�����������Ѿ����ע�ᡣ");
	sethomefile(buf, uinfo.userid, "mailcheck");
	unlink(buf);
	sprintf(genbuf, "�� %s ͨ�����ȷ��.", uinfo.userid);
	securityreport(genbuf, 0, 0);

	return DIRCHANGED;
}

//      ����ע�ᵥ
int do_register(int index, REGINFO * ent, char *direct) {
	int unum;
	struct userec uinfo;
	//char ps[80];
	register int ch;
	static char *reason[] = { "��ȷʵ��д��ʵ����.", "������ѧУ��ϵ���꼶.", "����д������סַ����.",
			"����������绰.", "��ȷʵ��дע�������.", "����������д���뵥.", "����" };
	unsigned char rejectindex = 4;

	if (!ent)
		return DONOTHING;

	unum = getuser(ent->userid);
	if (!unum) {
		prints("ϵͳ����! ���޴��˺�!\n"); //ɾ�������ڵļ�¼,����еĻ�
		delete_record(direct, sizeof(REGINFO), index, filecheck,
				ent->userid);
		return DIRCHANGED;
	}

	memcpy(&uinfo, &lookupuser, sizeof (uinfo));
	clear();
	move(0, 0);
	prints("[1;33;44m ��ϸ����                                                                      [m\n");
	prints("[1;37;42m [.]���� [+]�ܾ� [d]ɾ�� [0-6]������ԭ��                                       [m");

	//strcpy(ps, "(��)");
	for (;;) {
		disply_userinfo(&uinfo);
		move(14, 0);
		printdash(NULL);
		getdatestring(ent->regdate, YEA);
		prints("   ע��ʱ��   : %s\n", datestring);
		prints("   �����ʺ�   : %s\n", ent->userid);
		prints("   ��ʵ����   : %s\n", ent->realname);
		prints("   ѧУϵ��   : %s\n", ent->dept);
		prints("   Ŀǰסַ   : %s\n", ent->addr);
		prints("   ����绰   : %s\n", ent->phone);
#ifndef FDQUAN
		prints("   �����ʼ�   : %s\n", ent->email);
#endif
		prints("   У �� ��   : %s\n", ent->assoc);
		ch = egetch();
		switch (ch) {
			case '.':
				pass_register(index, ent, direct);
				return READ_AGAIN;
			case '+':
				uinfo.userlevel &= ~PERM_SPECIAL4;
				substitut_record(PASSFILE, &uinfo, sizeof (uinfo), unum);
				//mail_file("etc/f_fill", uinfo.userid, "��������д����ע������");
				mail_file("etc/f_fill", uinfo.userid, reason[rejectindex]);
			case 'd':
				uinfo.userlevel &= ~PERM_SPECIAL4;
				substitut_record(PASSFILE, &uinfo, sizeof (uinfo), unum);
				delete_register(index, ent, direct);
				return READ_AGAIN;
			case KEY_DOWN:
			case '\r':
				return READ_NEXT;
			case KEY_LEFT:
				return DIRCHANGED;
			default:
				if (ch >= '0' && ch <= '6') {
					rejectindex = ch - '0';
					//strcpy(uinfo.address, reason[ch-'0']);
				}
				break;
		}
	}
	return 0;
}

struct one_key reg_comms[] = { 'r', do_register, 'y', pass_register, 'd',
		delete_register, '\0', NULL };
void show_register() {
	FILE *fn;
	int x; //, y, wid, len;
	char uident[STRLEN];
	if (!(HAS_PERM(PERM_USER)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	stand_title("��ѯʹ����ע������");
	move(1, 0);
	usercomplete("������Ҫ��ѯ�Ĵ���: ", uident);
	if (uident[0] != '\0') {
		if (!getuser(uident)) {
			move(2, 0);
			prints("�����ʹ���ߴ���...");
		} else {
			sprintf(genbuf, "home/%c/%s/register",
					toupper(lookupuser.userid[0]), lookupuser.userid);
			if ((fn = fopen(genbuf, "r")) != NULL) {
				prints("\nע����������:\n\n");
				for (x = 1; x <= 15; x++) {
					if (fgets(genbuf, STRLEN, fn))
						prints("%s", genbuf);
					else
						break;
				}
			} else {
				prints("\n\n�Ҳ�����/����ע������!!\n");
			}
		}
	}
	pressanykey();
}
//  ���� ע�ᵥ�쿴��,��ʹ���ߵ�ע�����ϻ��ע�ᵥ�������
int m_register() {
	FILE *fn;
	char ans[3]; //, *fname;
	int x; //, y, wid, len;
	char uident[STRLEN];

	if (!(HAS_PERM(PERM_USER)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();

	stand_title("�趨ʹ����ע������");
	for (;;) {
		getdata(1, 0, "(0)�뿪  (1)�����ע�� (2)��ѯʹ����ע������ ? : ", ans, 2, DOECHO,
				YEA);
		if (ans[0] == '1' || ans[0] == '2') { // || ans[0]=='3') ����ֻ��0,1,2
			break;
		} else {
			return 0;
		}
	}
	switch (ans[0]) {
		/*
		 case '1':
		 fname = "new_register";
		 if ((fn = fopen(fname, "r")) == NULL) {
		 prints("\n\nĿǰ������ע������.");
		 pressreturn();
		 } else {
		 y = 3, x = wid = 0;
		 while (fgets(genbuf, STRLEN, fn) != NULL && x < 65) {
		 if (strncmp(genbuf, "userid: ", 8) == 0) {
		 move(y++, x);
		 prints("%s",genbuf + 8);
		 len = strlen(genbuf + 8);
		 if (len > wid)
		 wid = len;
		 if (y >= t_lines - 2) {
		 y = 3;
		 x += wid + 2;
		 }
		 }
		 }
		 fclose(fn);
		 if (askyn("�趨������", NA, YEA) == YEA) {
		 securityreport("�趨ʹ����ע������");
		 scan_register_form(fname);
		 }
		 }
		 break; */
		case '2':
			move(1, 0);
			usercomplete("������Ҫ��ѯ�Ĵ���: ", uident);
			if (uident[0] != '\0') {
				if (!getuser(uident)) {
					move(2, 0);
					prints("�����ʹ���ߴ���...");
				} else {
					sprintf(genbuf, "home/%c/%s/register",
							toupper(lookupuser.userid[0]),
							lookupuser.userid);
					if ((fn = fopen(genbuf, "r")) != NULL) {
						prints("\nע����������:\n\n");
						for (x = 1; x <= 15; x++) {
							if (fgets(genbuf, STRLEN, fn))
								prints("%s", genbuf);
							else
								break;
						}
					} else {
						prints("\n\n�Ҳ�����/����ע������!!\n");
					}
				}
			}
			pressanykey();
			break;
		case '1':
			i_read(ADMIN, "unregistered", regtitle, regdoent,
					&reg_comms[0], sizeof(REGINFO));
			break;
	}
	clear();
	return 0;
}

//      ɾ��������
int d_board() {
	struct boardheader binfo;
	int bid, ans;
	char bname[STRLEN];
	extern char lookgrp[];
	char genbuf_rm[STRLEN]; //added by roly 02.03.24

	if (!HAS_PERM(PERM_BLEVELS)) {
		return 0;
	}
	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	stand_title("ɾ��������");
	make_blist(0); //�����������б�
	move(1, 0);
	namecomplete("������������: ", bname);
	if (bname[0] == '\0')
		return 0;
	bid = getbnum(bname, &currentuser);
	if (get_record(BOARDS, &binfo, sizeof (binfo), bid) == -1) { //ȡ���������ļ�¼
		move(2, 0);
		prints("����ȷ��������\n");
		pressreturn();
		clear();
		return 0;
	}
	if (binfo.BM[0] != '\0' && strcmp(binfo.BM, "SYSOPs")) { //���в��ǽ�SYSOPs�İ���
		move(5, 0);
		prints("�ð滹�а�������ɾ������ǰ������ȡ��������������\n");
		pressanykey();
		clear();
		return 0;
	}
	ans = askyn("��ȷ��Ҫɾ�����������", NA, NA);
	if (ans != 1) {
		move(2, 0);
		prints("ȡ��ɾ���ж�\n");
		pressreturn();
		clear();
		return 0;
	}
	{
		char secu[STRLEN];
		sprintf(secu, "ɾ����������%s", binfo.filename);
		securityreport(secu, 0, 1);
	}
	if (seek_in_file("0Announce/.Search", bname)) {
		move(4, 0);
		if (askyn("�Ƴ�������", NA, NA) == YEA) {
			get_grp(binfo.filename);
			del_grp(lookgrp, binfo.filename, binfo.title + 8);
		}
	}
	if (seek_in_file("etc/junkboards", bname))
		del_from_file("etc/junkboards", bname);
	if (seek_in_file("0Announce/.Search", bname)) {
		char tmpbuf[160];
		sprintf(tmpbuf, "%s:", bname);
		del_from_file("0Announce/.Search", tmpbuf);
	}
	if (binfo.filename[0] == '\0') {
		return -1; /* rrr - precaution */
	}
	sprintf(genbuf, "boards/%s", binfo.filename);
	//f_rm(genbuf);
	/* added by roly 02.03.24 */
	sprintf(genbuf_rm, "/bin/rm -fr %s", genbuf); //added by roly 02.03.24
	system(genbuf_rm); //��f_rm(genbuf)�ǲ����ظ���?
	/* add end */
	sprintf(genbuf, "vote/%s", binfo.filename);
	//f_rm(genbuf);
	/* added by roly 02.03.24 */
	sprintf(genbuf_rm, "/bin/rm -fr %s", genbuf); //added by roly 02.03.24
	system(genbuf_rm);
	/* add end */
	sprintf(genbuf, " << '%s' �� %s ɾ�� >>", binfo.filename,
			currentuser.userid);
	memset(&binfo, 0, sizeof (binfo));
	strlcpy(binfo.title, genbuf, STRLEN);
	binfo.level = PERM_SYSOPS;
	substitute_record(BOARDS, &binfo, sizeof (binfo), bid);

	move(4, 0);
	prints("\n���������Ѿ�ɾ��...\n");
	pressreturn();
	flush_bcache();
	clear();
	return 0;
}

//      ɾ��һ���ʺ�
int d_user(char *cid) {
	int id, num, i;
	char secu[STRLEN];
	char genbuf_rm[STRLEN]; //added by roly 02.03.24
	char passbuf[PASSLEN];

	if (!(HAS_PERM(PERM_USER)))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	stand_title("ɾ��ʹ�����ʺ�");
	// Added by Ashinmarch in 2008.10.20 
	// �����˺�ʱ����������֤
	getdata(1, 0, "[1;37m����������: [m", passbuf, PASSLEN, NOECHO, YEA);
	passbuf[8] = '\0';
	if (!checkpasswd(currentuser.passwd, passbuf)) {
		prints("[1;31m�����������...[m\n");
		return 0;
	}
	// Add end.
	if (!gettheuserid(1, "��������ɾ����ʹ���ߴ���: ", &id))
		return 0;
	if (!strcmp(lookupuser.userid, "SYSOP")) {
		prints("\n�Բ����㲻����ɾ�� SYSOP �ʺ�!!\n");
		pressreturn();
		clear();
		return 0;
	}
	if (!strcmp(lookupuser.userid, currentuser.userid)) {
		prints("\n�Բ����㲻����ɾ���Լ�������ʺ�!!\n");
		pressreturn();
		clear();
		return 0;
	}
	prints("\n\n������ [%s] �Ĳ�������:\n", lookupuser.userid);
	prints("    User ID:  [%s]\n", lookupuser.userid);
	prints("    ��   ��:  [%s]\n", lookupuser.username);
	prints("    ��   ��:  [%s]\n", lookupuser.realname);
	strcpy(secu, "ltmprbBOCAMURS#@XLEast0123456789\0");
	for (num = 0; num < strlen(secu) - 1; num++) {
		if (!(lookupuser.userlevel & (1 << num)))
			secu[num] = '-';
	}
	prints("    Ȩ   ��: %s\n\n", secu);

	num = getbnames(lookupuser.userid, secu, &num);
	if (num) {
		prints("[%s] Ŀǰ�е����� %d ����İ���: ", lookupuser.userid, num);
		for (i = 0; i < num; i++)
			prints("%s ", bnames[i]);
		prints("\n����ʹ�ð���жְ����ȡ�������ְ�������ò���.");
		pressanykey();
		clear();
		return 0;
	}

	sprintf(genbuf, "��ȷ��Ҫɾ�� [%s] ��� ID ��", lookupuser.userid);
	if (askyn(genbuf, NA, NA) == NA) {
		prints("\nȡ��ɾ��ʹ����...\n");
		pressreturn();
		clear();
		return 0;
	}
	sprintf(secu, "ɾ��ʹ���ߣ�%s", lookupuser.userid);
	securityreport(secu, 0, 0);
	sprintf(genbuf, "mail/%c/%s", toupper(lookupuser.userid[0]),
			lookupuser.userid);
	//f_rm(genbuf);
	/* added by roly 02.03.24 */
	sprintf(genbuf_rm, "/bin/rm -fr %s", genbuf); //added by roly 02.03.24
	system(genbuf_rm);
	/* add end */
	sprintf(genbuf, "home/%c/%s", toupper(lookupuser.userid[0]),
			lookupuser.userid);
	//f_rm(genbuf);
	/* added by roly 02.03.24 */
	sprintf(genbuf_rm, "/bin/rm -fr %s", genbuf); //added by roly 02.03.24
	system(genbuf_rm);
	/* add end */
	lookupuser.userlevel = 0;
	strcpy(lookupuser.address, "");
#ifdef ALLOWGAME
	lookupuser.money = 0;
	lookupuser.nummedals = 0;
	lookupuser.bet = 0;
#endif
	strcpy(lookupuser.username, "");
	strcpy(lookupuser.realname, "");
	strcpy(lookupuser.termtype, "");
	prints("\n%s �Ѿ��������...\n", lookupuser.userid);
	lookupuser.userid[0] = '\0';
	substitut_record(PASSFILE, &lookupuser, sizeof(lookupuser), id);
	setuserid(id, lookupuser.userid);
	pressreturn();
	clear();
	return 1;
}

//      ����ʹ���ߵ�Ȩ��
int x_level() {
	int id;
	char reportbuf[60];
	unsigned int newlevel;

	if (!HAS_PERM(PERM_SYSOPS))
		return;

	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	move(0, 0);
	prints("����ʹ����Ȩ��\n");
	clrtoeol();
	move(1, 0);
	usercomplete("���������ĵ�ʹ�����ʺ�: ", genbuf);
	if (genbuf[0] == '\0') {
		clear();
		return 0;
	}
	if (!(id = getuser(genbuf))) {
		move(3, 0);
		prints("Invalid User Id");
		clrtoeol();
		pressreturn();
		clear();
		return 0;
	}
	move(1, 0);
	clrtobot();
	move(2, 0);
	prints("�趨ʹ���� '%s' ��Ȩ�� \n", genbuf);
	newlevel
			= setperms(lookupuser.userlevel, "Ȩ��", NUMPERMS, showperminfo);
	move(2, 0);
	if (newlevel == lookupuser.userlevel)
		prints("ʹ���� '%s' Ȩ��û�б��\n", lookupuser.userid);
	else {
		sprintf(reportbuf, "change level: %s %.8x -> %.8x",
				lookupuser.userid, lookupuser.userlevel, newlevel);
		report(reportbuf, currentuser.userid);
		lookupuser.userlevel = newlevel;
		{
			char secu[STRLEN];
			sprintf(secu, "�޸� %s ��Ȩ��", lookupuser.userid);
			securityreport(secu, 0, 0);
		}

		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
		if (!(lookupuser.userlevel & PERM_REGISTER)) {
			char src[STRLEN], dst[STRLEN];
			sethomefile(dst, lookupuser.userid, "register.old");
			if (dashf(dst))
				unlink(dst);
			sethomefile(src, lookupuser.userid, "register");
			if (dashf(src))
				rename(src, dst);
		}
		prints("ʹ���� '%s' Ȩ���Ѿ��������.\n", lookupuser.userid);
	}
	pressreturn();
	clear();
	return 0;
}

//added by iamfat 2002.07.22
//rewrite by iamfat 2002.08.19
extern void list_text(char *fname, void (*title_show) (), int (*key_deal) (), int (*ifcheck) ());
extern void changereason(char *fname);
extern void setreasondefault();
extern void setreason(char *rsn, int i);
extern char *getreason();
extern char *getdetailreason();
char *denylevellist = ".DenyLevel";
int add_denylevel(char *line);

char date_name[STRLEN];

//�ָ����е���ȫվ����
extern int text_find(char *fname, int no, char *str, char *line);

//��ʾ�������ڵ�ID�б����.
void denylist_title_show() {
	move(0, 0);
	prints("[1;44;36m �������ڵ�ID�б�                                                              [m\n");
	prints(" �뿪[[1;32m��[m] ѡ��[[1;32m��[m,[1;32m��[m] ���[[1;32ma[m]  �޸�[[1;32mc[m] �ָ�[[1;32md[m] ����[[1;32mx[m] ����[[1;32m/[m]\n");
	prints("[1;44m �û�����     ����˵��(A-Z;'.[])                 Ȩ�� ��������   վ��          [m\n");
}

// ������ʾ����б�ʱ�İ�������
int denylist_key_deal(char *fname, int ch, char *line) {
	switch (ch) {
		case 'a': //���
			add_denylevel(0);
			break;
		case 'c': //�޸�
			if (!line)
				break;
			add_denylevel(line);
			break;
		case 'd': //�ָ�
			if (!line)
				break;
			move(1, 0);
			if (askyn("��ȷ����?", NA, NA) == NA) {
				denylist_title_show();
				return 0;
			}
			del_from_denylist(fname, line);
			break;
		case 'x': //�ͷ����е��ڵ�
			move(1, 0);
			if (askyn("��ȷ����?", NA, NA) == NA) {
				denylist_title_show();
				return 0;
			}
			del_from_denylist(fname, NULL);
			break;
		case Ctrl('A'):
		case KEY_RIGHT: //�û���Ϣ
			if (!line)
				return 0;
			t_query(line);
			break;
	}
	return 1;
}

//  �����޸ķ��Ȩ���б�
int x_new_denylevel() {
	if (!HAS_PERM(PERM_OBOARDS) && !HAS_PERM(PERM_SPECIAL0))
		return;
	modify_user_mode(ADMIN);
	list_text(denylevellist, denylist_title_show, denylist_key_deal, NULL);
}

// ����Ȩ��ch���Ӧ����Ϣ
char *clevel(char ch) {
	switch (ch) {
		case '1':
			return "����";
		case '2':
			return "��½";
		case '3':
			return "����";
		case '4':
			return "�ż�";
		case '5':
			return "��Ϣ";
	}
	return "δ֪";
}

// ����st���Ӧ������ʱ��ֵ.
time_t get_denydate(char *st) {
	time_t tt;
	struct tm tms;
	int ch1, ch2, ch3;
	if (!strncmp(st, "����", 4))
		return 0;
	if (!strncmp(st, "����", 4))
		return 0x7fffffff;
	if (!strncmp(st, "����", 4))
		return 0x7fffffff;
	ch1 = st[4];
	st[4] = '\0'; //��
	ch2 = st[7];
	st[7] = '\0'; //��
	ch3 = st[10];
	st[10] = '\0'; //��
	tms.tm_year = atoi(st) - 1900; //      ��
	tms.tm_mon = atoi(st + 5) - 1; //      ��
	tms.tm_mday = atoi(st + 8); //      ��
	st[4] = ch1;
	st[7] = ch2;
	st[10] = ch3;
	tms.tm_hour = tms.tm_min = tms.tm_sec = 0;
	tt = mktime(&tms); //����tms���Ӧ��ʱ��
	return tt;
}

//      ��ӵ�����б�
int

add_to_denylist(char *uident, char ch, int day, char *msg) {
	char strtosave[STRLEN];
	char line[256];
	char luid[IDLEN + 1];
	char fname[STRLEN];
	char ps[40];
	FILE *fpr, *fpw;
	int added = 0;
	int change = YEA;
	struct tm *tmtime;
	time_t tt;
	time_t tt2;

	getdata(12, 0, "����˵��: ", ps, 40, DOECHO, YEA);
	move(13, 0);
	if (askyn("��ȷ����?", NA, NA) == NA)
		return NA;
	sprintf(fname, "%s.%d", denylevellist, uinfo.pid);
	if (!(fpr = fopen(denylevellist, "r")))
		return NA;
	if (!(fpw = fopen(fname, "w")))
		return NA;
	if (day != 999) {
		day = (day > 999) ? 999 : day;
	}
	if (day == 999) {
		tt = 0x7fffffff;
	} else if (day > 0) {
		tt = time(0) + (day) * 24 * 60 * 60;
	} else {
		tt = 0;
	}
	//commented by iamfat 2002.10.08
	/*
	 while(fgets(line,256,fpr))
	 {
	 strncpy(luid,line,IDLEN);
	 luid[IDLEN]='\0';
	 strtok(luid," \r\n\t");
	 if(!strcmp(luid, uident) && !strncmp(line+48, clevel(ch),4))
	 {
	 if(tt<=get_denydate(line+53))
	 {
	 change=NA;
	 break;
	 }
	 }
	 } */
	if (change == YEA) {
		if (day == 999) {
			sprintf(strtosave, "%-12s %-34s %-4s ����       %-12s\n",
					uident, getreason(), clevel(ch), currentuser.userid);
			sprintf(msg, "%s��:\n%s\nӦ����� %s Ȩ������\n\n�粻��������,"
				" ������ϵ���������˻���7���ڵ�Appeal���븴�顣\n"
				"P.S.: %s\n\nִ����: %s\n", uident, getdetailreason(),
					clevel(ch), ps, currentuser.userid);
		} else if (day > 0) {
			tmtime = localtime(&tt);
			sprintf(strtosave, "%-12s %-34s %-4s %04d.%02d.%02d %-12s\n",
					uident, getreason(), clevel(ch), 1900
							+ tmtime->tm_year, tmtime->tm_mon + 1,
					tmtime->tm_mday, currentuser.userid);
			sprintf(msg,
					"%s��:\n%s\nӦ����� %s Ȩ��%d��\n\n���ڴ���������(%04d.%02d.%02d),"
						" ��%sд��Ҫ��������.\n�粻��������,"
						" ������ϵ���������˻���7���ڵ�Appeal���븴�顣\n"
						"P.S.: %s\n\nִ����: %s\n", uident,
					getdetailreason(), clevel(ch), day, 1900
							+ tmtime->tm_year, tmtime->tm_mon + 1,
					tmtime->tm_mday, currentuser.userid, ps,
					currentuser.userid);
		} else { //����
			sprintf(strtosave, "%-12s %-34s %-4s ����       %-12s\n",
					uident, getreason(), clevel(ch), currentuser.userid);
			sprintf(msg, "%s��:\n%s\n��ʱ����� %s Ȩ��\n\n���������Ժ�����\n"
				"�粻��������, ������ϵ���������˻���7���ڵ�Appeal���븴�顣\n"
				"P.S.: %s\n\nִ����: %s\n", uident, getdetailreason(),
					clevel(ch), ps, currentuser.userid);
		}
		fseek(fpr, 0, SEEK_SET);
		while (fgets(line, 256, fpr)) {
			tt2 = get_denydate(line + 53);
			strlcpy(luid, line, IDLEN);
			luid[IDLEN] = '\0';
			strtok(luid, " \r\n\t");
			if (!strcmp(luid, uident)
					&& !strncmp(line + 48, clevel(ch), 4))
				continue;
			if (!added && tt < tt2) {
				fputs(strtosave, fpw);
				added = 1;
			}
			fputs(line, fpw);
		} // while(fgets(line,256,fpr)
		if (!added)
			fputs(strtosave, fpw);
	} // if change == YEA
	fclose(fpw);
	fclose(fpr);

	if (change == YEA)
		rename(fname, denylevellist);
	else
		unlink(fname);

	return change;
}

// �ָ��û���Ȩ��
int release_user(char *line) {
	int id;
	FILE *se;
	char uident[IDLEN + 1];
	char fname[STRLEN];
	char secu[STRLEN];
	char rep[STRLEN];
	char msgbuf[256];
	strlcpy(uident, line, IDLEN);
	uident[IDLEN] = '\0';
	strtok(uident, " \r\n\t");
	if (!(id = getuser(uident)))
		return -1;
	if (!strncmp(&line[48], clevel('1'), 4)) {
		lookupuser.userlevel |= PERM_POST;
		sprintf(secu, "[վ�ڹ���]�ָ�%s�������µ�Ȩ��", lookupuser.userid);
		sprintf(rep, "��%s�ָ��������µ�Ȩ��", currentuser.userid);
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	} else if (!strncmp(&line[48], clevel('2'), 4)) {
		lookupuser.userlevel |= PERM_LOGIN;
		sprintf(secu, "[վ�ڹ���]�ָ�%s������վ��Ȩ��", lookupuser.userid);
		sprintf(rep, "��%s�ָ�������վ��Ȩ��", currentuser.userid);
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	} else if (!strncmp(&line[48], clevel('3'), 4)) {
		lookupuser.userlevel |= PERM_TALK;
		sprintf(secu, "[վ�ڹ���]�ָ�%s�����������Ȩ��", lookupuser.userid);
		sprintf(rep, "��%s�ָ������������Ȩ��", currentuser.userid);
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	} else if (!strncmp(&line[48], clevel('4'), 4)) {
		lookupuser.userlevel |= PERM_MAIL;
		sprintf(secu, "[վ�ڹ���]�ָ�%s�����ż���Ȩ��", lookupuser.userid);
		sprintf(rep, "��%s�ָ������ż���Ȩ��", currentuser.userid);
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	} else {
		lookupuser.userlevel |= PERM_POST;
		sprintf(secu, "[վ�ڹ���]�ָ�%sδ֪Ȩ��", lookupuser.userid);
		sprintf(rep, "��%s�ָ�δ֪Ȩ��", currentuser.userid);
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	}
	//���Ÿ�����
	sprintf(fname, "tmp/AutoPoster.%s.%05d", currentuser.userid, uinfo.pid);
	sprintf(msgbuf, "ִ����:%s\n", currentuser.userid);
	if ((se = fopen(fname, "w")) != NULL) {
		fprintf(se, "%s", msgbuf);
		fclose(se);
		if (lookupuser.userid != NULL)
			mail_file(fname, lookupuser.userid, secu);
	}
	Poststring(msgbuf, "Notice", secu, 1);
	//��ȫ��¼
	securityreport(secu, 0, 0);
	log_DOTFILE(lookupuser.userid, rep);
	substitute_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	sprintf(msgbuf, "�ͷ� %s", line);
	do_report(".Released", strtok(msgbuf, "\r\n"));
}

//      �ӷ���б����ͷ�
int del_from_denylist(char *fname, char *line) {
	FILE *fpr, *fpw;
	int deleted = NA;
	char tmpbuf[256], fnnew[STRLEN];
	char tmpdate[11];

	if (!(fpr = fopen(fname, "r")))
		return -1;
	sprintf(fnnew, "%s.%d", fname, uinfo.pid);
	if (!(fpw = fopen(fnnew, "w")))
		return -1;
	if (line) {
		while (fgets(tmpbuf, 256, fpr)) {
			if (deleted == NA && !strcmp(tmpbuf, line))
				deleted = YEA;
			else
				fputs(tmpbuf, fpw);
		}
		if (deleted == YEA)
			release_user(line);
	} else {
		time_t tt;
		time_t now = time(0);
		while (fgets(tmpbuf, 256, fpr)) {
			//pighead      A-----------------------------     ���� 2002.07.24 iamfat
			//123456789012345678901234567890123456789012345678901234
			strlcpy(tmpdate, tmpbuf + 53, 10);
			tt = get_denydate(tmpdate);
			if (tt != 0 && tt <= now) {
				deleted = YEA;
				release_user(tmpbuf);
			} else {
				fputs(tmpbuf, fpw);
			}
		}
	}
	fclose(fpw);
	fclose(fpr);
	if (deleted == YEA)
		return rename(fnnew, fname);
	return 0;
}

//  �޸ķ��Ȩ��
int add_denylevel(char *line) {
	int id;
	char ans[7];
	char buf2[5];
	char msgbuf[4096];
	char genbuf[STRLEN];
	char secu[STRLEN];
	char rep[STRLEN];
	char deny_uid[IDLEN + 1];

	int bDeny = NA;
	modify_user_mode(ADMIN);
	if (!check_systempasswd())
		return 0;
	move(1, 0);
	if (line) {
		strlcpy(deny_uid, line, IDLEN);
		deny_uid[IDLEN] = '\0';
		strtok(deny_uid, " \n\r\t");
		setreason(line + IDLEN + 1, strlen(getreason()));
		prints("�޸�%s�ķ��ʱ��\n", deny_uid);
	} else {
		usercomplete("���ʹ����: ", deny_uid);
		if (*deny_uid == '\0')
			return 0;
		setreasondefault();
		prints("\n");
	}
	if (!(id = getuser(deny_uid))) {
		prints("�Ƿ���ʹ�����ʺ�!\n");
		clrtoeol();
		egetch();
		return 0;
	}
	if (line) {
		if (!strncmp(&line[48], clevel('1'), 4))
			ans[0] = '1';
		else if (!strncmp(&line[48], clevel('2'), 4))
			ans[0] = '2';
		else if (!strncmp(&line[48], clevel('3'), 4))
			ans[0] = '3';
		else if (!strncmp(&line[48], clevel('4'), 4))
			ans[0] = '4';
		else
			ans[0] = '0';
		sprintf(secu, "[վ�ڹ���]�޸�%s�ķ��ʱ��", lookupuser.userid);
	} else {
		changereason("etc/denylevel");
		clear();
		move(2, 0);
		prints("�趨ʹ����%s�Ļ���Ȩ�޻����� \n\n", deny_uid);
		prints("(1) �����������Ȩ�� %s\n",
				(lookupuser.userlevel & PERM_POST) ? "ON" : "OFF");
		prints("(2) ȡ��������վȨ�� %s\n",
				(lookupuser.userlevel & PERM_LOGIN) ? "ON" : "OFF");
		prints("(3) ��ֹ����������   %s\n",
				(lookupuser.userlevel & PERM_TALK) ? "ON" : "OFF");
		prints("(4) ��ֹ�����ż�     %s\n",
				(lookupuser.userlevel & PERM_MAIL) ? "ON" : "OFF");
		while (1) {
			move(12, 0);
			clrtobot();
			getdata(10, 0, "��������Ĵ���: ", ans, 3, DOECHO, YEA);
			switch (ans[0]) {
				case '1':
					if (!(lookupuser.userlevel & PERM_POST)) {
						prints("\n%s�������µ�Ȩ���ѱ����!\n", lookupuser.userid);
						egetch();
					}
					sprintf(secu, "[վ�ڹ���]���%s�������µ�Ȩ��", lookupuser.userid);
					sprintf(rep, "��%s���[վ��]�������µ�Ȩ��", currentuser.userid);
					bDeny = YEA;
					lookupuser.userlevel &= ~PERM_POST;
					break;
				case '2':
					if (!(lookupuser.userlevel & PERM_LOGIN)) {
						prints("\n%s������վ��Ȩ���ѱ����!\n", lookupuser.userid);
						egetch();
					}
					sprintf(secu, "[վ�ڹ���]���%s������վ��Ȩ��", lookupuser.userid);
					sprintf(rep, "��%s���[վ��]������վ��Ȩ��", currentuser.userid);
					bDeny = YEA;
					lookupuser.userlevel &= ~PERM_LOGIN;
					break;
				case '3':
					if (!(lookupuser.userlevel & PERM_TALK)) {
						prints("\n%s�����������Ȩ���ѱ����!\n", lookupuser.userid);
						egetch();
					}
					sprintf(secu, "[վ�ڹ���]���%s�����������Ȩ��", lookupuser.userid);
					sprintf(rep, "��%s���[վ��]�����������Ȩ��", currentuser.userid);
					bDeny = YEA;
					lookupuser.userlevel &= ~PERM_TALK;
					break;
				case '4':
					if (!(lookupuser.userlevel & PERM_MAIL)) {
						prints("\n%s�����ż���Ȩ���ѱ����!\n", lookupuser.userid);
						egetch();
					}
					sprintf(secu, "[վ�ڹ���]���%s�����ż���Ȩ��", lookupuser.userid);
					sprintf(rep, "��%s���[վ��]�����ż���Ȩ��", currentuser.userid);
					bDeny = YEA;
					lookupuser.userlevel &= ~PERM_MAIL;
					break;
			} //switch ans[]
			if (bDeny == YEA)
				break;
		} // while (1)
		// switch (ans[0]) {
		// case '1':
		//lookupuser.userlevel &= ~PERM_POST;
		//break;
		// case '2':
		//lookupuser.userlevel &= ~PERM_LOGIN;
		//break;
		//  case '3':
		//lookupuser.userlevel &= ~PERM_TALK;
		//break;
		//  case '4':
		//lookupuser.userlevel &= ~PERM_MAIL;
		//break;
		//  default:
		//break;
		//  }
	}
	getdata(11, 0, "�������(999-����, 0-����): ", buf2, 5, DOECHO, YEA);
	if (add_to_denylist(lookupuser.userid, ans[0], atoi(buf2), msgbuf)
			== NA) {
		return 0;
	}
	substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
	sethomefile(genbuf, lookupuser.userid, "giveupBBS");
	if (dashf(genbuf))
		unlink(genbuf);
	{
		//���Ÿ�����
		FILE *se;
		char fname[STRLEN];
		sprintf(fname, "tmp/AutoPoster.%s.%05d", currentuser.userid,
				uinfo.pid);
		if ((se = fopen(fname, "w")) != NULL) {
			fprintf(se, "%s", msgbuf);
			fclose(se);
			if (lookupuser.userid != NULL)
				mail_file(fname, lookupuser.userid, secu);
		}
	}
	Poststring(msgbuf, "Notice", secu, 1);
	//��ȫ��¼
	securityreport(secu, 0, 0);
	log_DOTFILE(lookupuser.userid, rep);
	//if (!line)
	//  substitut_record (PASSFILE, &lookupuser, sizeof (struct userec), id);
	return 1;
}

void a_edits() {
	int aborted;
	char ans[7], buf[STRLEN], buf2[STRLEN];
	int ch, num, confirm;
	static char *e_file[] = { "../Welcome", "../Welcome2", "issue",
			"logout", "../vote/notes", "hotspot", "menu.ini",
			"../.badname", "../.bad_email", "../.bad_host", "autopost",
			"junkboards", "sysops", "whatdate", "../NOLOGIN",
			"../NOREGISTER", "special.ini", "hosts", "restrictip",
			"freeip", "s_fill", "f_fill", "register", "firstlogin",
			"chatstation", "notbackupboards", "bbsnet.ini", "bbsnetip",
			"bbsnet2.ini", "bbsnetip2", NULL };
	static char *explain_file[] = { "�����վ������", "��վ����", "��վ��ӭ��", "��վ����",
			"���ñ���¼", "ϵͳ�ȵ�", "menu.ini", "����ע��� ID", "����ȷ��֮E-Mail",
			"������վ֮λַ", "ÿ���Զ����ŵ�", "����POST���İ�", "����������", "�������嵥",
			"��ͣ��½(NOLOGIN)", "��ͣע��(NOREGISTER)", "����ip��Դ�趨��", "����ip��Դ�趨��",
			"ֻ�ܵ�½5id��ip�趨��", "����5 id���Ƶ�ip�趨��", "ע��ɹ��ż�", "ע��ʧ���ż�",
			"���û�ע�᷶��", "�û���һ�ε�½����", "���ʻ������嵥", "����ɾ�����豸��֮�嵥",
			"BBSNET תվ�嵥", "��������ip", "BBSNET2 תվ�嵥", "����2����IP", NULL };
	modify_user_mode(ADMIN);
	if (!check_systempasswd()) {
		return;
	}
	clear();
	move(1, 0);
	prints("����ϵͳ����\n\n");
	for (num = 0; (HAS_PERM(PERM_ESYSFILE)) ? e_file[num] != NULL
			&& explain_file[num] != NULL : explain_file[num] != "menu.ini"; num++) {
		prints("[[1;32m%2d[m] %s", num + 1, explain_file[num]);
		if (num < 17)
			move(4 + num, 0);
		else
			move(num - 14, 50);
	}
	prints("[[1;32m%2d[m] �������\n", num + 1);

	getdata(23, 0, "��Ҫ������һ��ϵͳ����: ", ans, 3, DOECHO, YEA);
	ch = atoi(ans);
	if (!isdigit(ans[0]) || ch <= 0 || ch > num || ans[0] == '\n'
			|| ans[0] == '\0')
		return;
	ch -= 1;
	sprintf(buf2, "etc/%s", e_file[ch]);
	move(3, 0);
	clrtobot();
	sprintf(buf, "(E)�༭ (D)ɾ�� %s? [E]: ", explain_file[ch]);
	getdata(3, 0, buf, ans, 2, DOECHO, YEA);
	if (ans[0] == 'D' || ans[0] == 'd') {
		sprintf(buf, "��ȷ��Ҫɾ�� %s ���ϵͳ��", explain_file[ch]);
		confirm = askyn(buf, NA, NA);
		if (confirm != 1) {
			move(5, 0);
			prints("ȡ��ɾ���ж�\n");
			pressreturn();
			clear();
			return;
		}
		{
			char secu[STRLEN];
			sprintf(secu, "ɾ��ϵͳ������%s", explain_file[ch]);
			securityreport(secu, 0, 0);
		}
		unlink(buf2);
		move(5, 0);
		prints("%s ��ɾ��\n", explain_file[ch]);
		pressreturn();
		clear();
		return;
	}
	modify_user_mode(EDITSFILE);
	aborted = vedit(buf2, NA, YEA); /* ������ļ�ͷ, �����޸�ͷ����Ϣ */
	clear();
	if (aborted != -1) {
		prints("%s ���¹�", explain_file[ch]);
		{
			char secu[STRLEN];
			sprintf(secu, "�޸�ϵͳ������%s", explain_file[ch]);
			securityreport(secu, 0, 0);
		}

		if (!strcmp(e_file[ch], "../Welcome")) {
			unlink("Welcome.rec");
			prints("\nWelcome ��¼������");
		} else if (!strcmp(e_file[ch], "whatdate")) {
			brdshm->fresh_date = time(0);
			prints("\n�������嵥 ����");
		}
	}
	pressreturn();
}

// ȫվ�㲥...
int wall() {
	char passbuf[PASSLEN];

	if (!HAS_PERM(PERM_SYSOPS))
		return 0;
	// Added by Ashinmarch on 2008.10.20
	// ȫվ�㲥ǰ����������֤
	clear();
	stand_title("ȫվ�㲥!");
	getdata(1, 0, "[1;37m����������: [m", passbuf, PASSLEN, NOECHO, YEA);
	passbuf[8] = '\0';
	if (!checkpasswd(currentuser.passwd, passbuf)) {
		prints("[1;31m�����������...[m\n");
		return 0;
	}
	// Add end.

	modify_user_mode(MSG);
	move(2, 0);
	clrtobot();
	if (!get_msg("����ʹ����", buf2, 1)) {
		return 0;
	}
	if (apply_ulist(dowall) == -1) {
		move(2, 0);
		prints("���Ͽ���һ��\n");
		pressanykey();
	}
	prints("\n�Ѿ��㲥���...\n");
	pressanykey();
	return 1;
}

// �趨ϵͳ����
int setsystempasswd() {
	FILE *pass;
	char passbuf[20], prepass[20];
	modify_user_mode(ADMIN);
	if (!check_systempasswd())
		return;
	if (strcmp(currentuser.userid, "SYSOP")) {
		clear();
		move(10, 20);
		prints("�Բ���ϵͳ����ֻ���� SYSOP �޸ģ�");
		pressanykey();
		return;
	}
	getdata(2, 0, "�������µ�ϵͳ����(ֱ�ӻس���ȡ��ϵͳ����): ", passbuf, 19, NOECHO, YEA);
	if (passbuf[0] == '\0') {
		if (askyn("��ȷ��Ҫȡ��ϵͳ������?", NA, NA) == YEA) {
			unlink("etc/.syspasswd");
			securityreport("ȡ��ϵͳ����", 0, 0);
		}
		return;
	}
	getdata(3, 0, "ȷ���µ�ϵͳ����: ", prepass, 19, NOECHO, YEA);
	if (strcmp(passbuf, prepass)) {
		move(4, 0);
		prints("�������벻��ͬ, ȡ���˴��趨.");
		pressanykey();
		return;
	}
	if ((pass = fopen("etc/.syspasswd", "w")) == NULL) {
		move(4, 0);
		prints("ϵͳ�����޷��趨....");
		pressanykey();
		return;
	}
	fprintf(pass, "%s\n", genpasswd(passbuf));
	fclose(pass);
	move(4, 0);
	prints("ϵͳ�����趨���....");
	pressanykey();
	return;
}

//      ����c shell����
int x_csh() {
	char buf[PASSLEN];
	int save_pager;
	int magic;

	if (!HAS_PERM(PERM_SYSOPS)) {
		return 0;
	}
	if (!check_systempasswd()) {
		return;
	}
	modify_user_mode(SYSINFO);
	clear();
	getdata(1, 0, "������ͨ�а���: ", buf, PASSLEN, NOECHO, YEA);
	if (*buf == '\0' || !checkpasswd(currentuser.passwd, buf)) {
		prints("\n\n���Ų���ȷ, ����ִ�С�\n");
		pressreturn();
		clear();
		return;
	}
	randomize();
	magic = rand() % 1000;
	prints("\nMagic Key: %d", magic * 5 - 2);
	getdata(4, 0, "Your Key : ", buf, PASSLEN, NOECHO, YEA);
	if (*buf == '\0' || !(atoi(buf) == magic)) {
		securityreport("Fail to shell out", 0, 0);
		prints("\n\nKey ����ȷ, ����ִ�С�\n");
		pressreturn();
		clear();
		return;
	}
	securityreport("Shell out", 0, 0);
	modify_user_mode(SYSINFO);
	clear();
	refresh();
	reset_tty();
	save_pager = uinfo.pager;
	uinfo.pager = 0;
	update_utmp();
	do_exec("csh", NULL);
	restore_tty();
	uinfo.pager = save_pager;
	update_utmp();
	clear();
	return 0;
}
#endif
#endif
