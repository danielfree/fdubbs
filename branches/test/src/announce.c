#include "bbs.h"

#define MAXITEMS        1024
#define PATHLEN         256
#define A_PAGESIZE      (t_lines - 4)
#define MAXANNPATHS	40

#define ADDITEM         0
#define ADDGROUP        1
#define ADDMAIL         2

void a_menu();

extern char BoardName[];
struct boardheader *getbcache();
void a_prompt(); /* added by netty */
extern struct boardheader *getbcache();
extern char ANN_LOG_PATH[];
int readonly=NA;

typedef struct {
	char title[84];//modified by money 2002-6-7
	char fname[80];
} ITEM;

int a_fmode = 0;

typedef struct {
	ITEM *item[MAXITEMS];
	char mtitle[STRLEN];
	char *path;
	int num, page, now;
	int level;
} MENU;

//����ʱ��t����Ӧ�Ķ����ڸ�ʽ
char *get_short_date(time_t t) {
	static char short_date[15];
	struct tm *tm;

	tm=localtime(&t);
	sprintf(short_date, "%04d.%02d.%02d", tm->tm_year+1900, tm->tm_mon+1,
			tm->tm_mday);
	return short_date;
}
char nowpath[STRLEN]="\0"; //add by Danielfree 06.12.6
//���뾫����ʱ��ʾ���������н��
//	����	ǰһ��,����ʾ����,������ʾ�ż���Ϣ
//			�ڶ�,����,����ʾ��ʹ�ö�������
//			�м���ʾ���Ǿ������б�
//			���һ���ǹ��ܼ�,������ǰ�����һ��
void a_showmenu(MENU *pm) {
	struct stat st;
	struct tm *pt;
	char title[STRLEN * 2], kind[20];
	char fname[STRLEN];
	char ch;
	char buf[STRLEN], genbuf[STRLEN * 2];
	time_t mtime;
	int n, len;

	clear();
	if (chkmail()) {
		prints("\033[5m");
		sprintf(genbuf, "[�����ż����� M ������]");
	} else {
		strcpy(genbuf, pm->mtitle);
	}
	memset(buf, ' ', sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\0';
	len = strlen(genbuf);
	if (len >= sizeof(buf)) {
		genbuf[sizeof(buf) - 1] = '\0';
		len = sizeof(buf) - 1;
	}
	memcpy(buf + (sizeof(buf) - len) / 2, genbuf, len);
	prints("\033[1;44m%s\033[m\n", buf);
	prints("��ǰ������Ŀ¼ x%s\n", nowpath); //by Danielfree 06.12.6
	prints("\033[1;44;37m  ��� %-45s ������           %8s \033[m",
			"[���]                ��    ��", a_fmode ? "��������" : "�༭����");
	prints("\n");
	if (pm->num == 0) {
		prints("      << Ŀǰû������ >>\n");
	}
	for (n = pm->page; n < pm->page + A_PAGESIZE && n < pm->num; n++) {
		strcpy(title, pm->item[n]->title);
		sprintf(fname, "%s", pm->item[n]->fname);
		sprintf(genbuf, "%s/%s", pm->path, fname);
		if (a_fmode &&(pm->level&PERM_BOARDS)!=0) {
			ch = (dashd(genbuf) ? '/' : ' ');
			fname[10] = '\0';
		} else {
			//if (dashf(genbuf) || ch == '/') { //modified by roly 02.03.18
			if (dashf(genbuf) || dashd(genbuf) || ch == '/') {
				stat(genbuf, &st);
				mtime = st.st_mtime;
			} else {
				mtime = time(0);
			}
			pt = localtime(&mtime);
			sprintf(fname, "[%02d.%02d.%02d]",
					pt->tm_year%100, pt->tm_mon + 1, pt->tm_mday);
			ch = ' ';
		}
		if (dashf(genbuf)) {
			strcpy(kind, "[[1;36m�ļ�[m]");
		} else if (dashd(genbuf)) {
			strcpy(kind, "[[1;37mĿ¼[m]");
		} else {
			strcpy(kind, "[[1;32m����[m]");
		}
		if ( !strncmp(title, "[Ŀ¼] ", 7) || !strncmp(title, "[�ļ�] ", 7)
				|| !strncmp(title, "[��Ŀ] ", 7) || !strncmp(title, "[����] ",
				7))
			sprintf(genbuf, "%-s %-55.55s%-s%c", kind, title + 7, fname,
					ch);
		else
			sprintf(genbuf, "%-s %-55.55s%-s%c", kind, title, fname, ch);
		strlcpy(title, genbuf, STRLEN * 2);
		title[STRLEN * 2 - 1] = '\0';
		//Modified by IAMFAT 2002-05-30 3->4
		prints("  %4d %s\n", n + 1, title);
	}
	clrtobot();
	move(t_lines - 1, 0);
	//Modified by IAMFAT 2002-05-26
	//Roll Back by IAMFAT 2002-05-29
	sprintf(
			genbuf,
			"[1;44;31m[��  ��]  [33m˵�� h���뿪 q,������������ a������Ŀ¼ g���༭���� E   [%s˿·ģʽ] [m",
			DEFINE(DEF_MULTANNPATH) ? "��" : "��");
	prints(
			"%s",
			(pm->level & PERM_BOARDS) ? genbuf
					: "[1;44;31m[���ܼ�] [33m ˵�� h���뿪 q,�����ƶ��α� k,��,j,������ȡ���� Rtn,��               [m");
}

//�򾫻�����ǰĿ¼����һ����¼
//	��ʾ����Ϊ title
//	Ӳ�̴洢��Ϊ fname
//	����ֻ�������ڴ���,��δʵ�ʴ洢��Ӳ����
void a_additem(MENU *pm, char *title, char *fname) {
	ITEM *newitem;
	if (pm->num < MAXITEMS) {
		newitem = (ITEM *) malloc(sizeof(ITEM));
		memset(newitem, 0, sizeof(ITEM));
		strcpy(newitem->title, title);
		strcpy(newitem->fname, fname);
		pm->item[(pm->num)++] = newitem;
	}
}

//	�������뾫�������߽��뾫������ĳ��Ŀ¼,��������Ҫ����Ϣ
int a_loadnames(MENU *pm) {
	FILE *fn;
	ITEM litem;
	char buf[PATHLEN], *ptr;

	pm->num = 0;
	sprintf(buf, "%s/.Names", pm->path);
	if ((fn = fopen(buf, "r")) == NULL)
		return 0;
	// Add by wujian
	/*   sprintf(buf, "%s/counter.person",pm->path);
	 if (!dashf(buf))
	 {
	 fp=fopen(buf,"w+");
	 if(fp){
	 FLOCK(fileno(fp),LOCK_EX);
	 fprintf(fp,"%d",counter+1);
	 FLOCK(fileno(fp),LOCK_UN);
	 fclose(fp);
	 }
	 }
	 else
	 {
	 fp = fopen(buf,"r");
	 if(fp) { 
	 fscanf(fp,"%d",&counter);
	 fclose(fp); 
	 
	 if (counter > 0){
	 fp=fopen(buf,"w+");         //moved here from out of this judge by money
	 if(fp){			  //if fp is opened in R mod and fp == NULL
	 FLOCK(fileno(fp),LOCK_EX);//counter is 0, but the file is exited
	 fprintf(fp,"%d",counter+1); //so the count is changed to 1
	 FLOCK(fileno(fp),LOCK_UN);
	 fclose(fp);
	 }
	 }				  //moved end 2003.12.31.
	 }
	 }
	 */
	// Add end.
	while (fgets(buf, sizeof(buf), fn) != NULL) {
		//		memset(litem, 0, sizeof(ITEM));
		if ((ptr = strchr(buf, '\n')) != NULL)
			*ptr = '\0';
		if (strncmp(buf, "Name=", 5) == 0) {
			strlcpy(litem.title, buf + 5, 72);
			litem.title[71] = '\0';
		} else if (strncmp(buf, "Path=", 5) == 0) {
			if (strncmp(buf, "Path=~/", 7) == 0)
				strlcpy(litem.fname, buf + 7, 80);
			else
				strlcpy(litem.fname, buf + 5, 80);
			litem.fname[79] = '\0';
			// add judgement of OBOARDS by roly 02.02.26
			//modified by iamfat 2002.06.09
			//if ((!strstr(litem.title+38,"(BM: BMS)")||HAS_PERM(PERM_BOARDS))&&
			if ((!strstr(litem.title, "(BM: BMS)")||chk_currBM(currBM, 0))
					&&(!strstr(litem.title, "(BM: OBOARDS")
							||HAS_PERM(PERM_OBOARDS)) &&(!strstr(
					litem.title, "(BM: SYSOPS)") ||HAS_PERM(PERM_SYSOPS))
					&&(strncmp(litem.title, "<HIDE>", 6) ||!strncmp(
							litem.title+6, currentuser.userid,
							strlen(currentuser.userid)))) {//if
				a_additem(pm, litem.title, litem.fname);
			}
		} else if (strncmp(buf, "# Title=", 8) == 0) {
			//if (pm->mtitle[0] == '\0')
			strlcpy(pm->mtitle, buf + 8, STRLEN);
		}//if (strncmp(buf, "Name=", 5) == 0) {
	}
	fclose(fn);
	return 1;
}

//��pm����������Ϣд�뵽Ӳ��
void a_savenames(MENU *pm) {
	FILE *fn;
	ITEM *item;
	char fpath[PATHLEN];
	int n;

	sprintf(fpath, "%s/.Names", pm->path);
	if ((fn = fopen(fpath, "w")) == NULL)
		return;
	fprintf(fn, "#\n");
	if (!strncmp(pm->mtitle, "[Ŀ¼] ", 7) || !strncmp(pm->mtitle, "[�ļ�] ",
			7) || !strncmp(pm->mtitle, "[��Ŀ] ", 7) || !strncmp(pm->mtitle,
			"[����] ", 7)) {
		fprintf(fn, "# Title=%s\n", pm->mtitle + 7);
	} else {
		fprintf(fn, "# Title=%s\n", pm->mtitle);
	}
	fprintf(fn, "#\n");
	for (n = 0; n < pm->num; n++) {
		item = pm->item[n];
		if (!strncmp(item->title, "[Ŀ¼] ", 7)||!strncmp(item->title,
				"[�ļ�] ", 7) ||!strncmp(item->title, "[��Ŀ] ", 7)||!strncmp(
				item->title, "[����] ", 7)) {
			fprintf(fn, "Name=%s\n", item->title + 7);
		} else {
			fprintf(fn, "Name=%s\n", item->title);
		}
		fprintf(fn, "Path=~/%s\n", item->fname);
		fprintf(fn, "Numb=%d\n", n + 1);
		fprintf(fn, "#\n");
	}
	fclose(fn);
	chmod(fpath, 0644);
}

void a_prompt(int bot, char* pmt, char* buf, int len) {
	saveline(t_lines + bot, 0);
	move(t_lines + bot, 0);
	clrtoeol();
	getdata(t_lines + bot, 0, pmt, buf, len, DOECHO, YEA);
	saveline(t_lines + bot, 1);
}

/* added by netty to handle post saving into (0)Announce */

int a_Save(char *path, char* key, struct fileheader *fileinfo, int nomsg,
		int full) {
	FILE *fp;
	char board[40];
	int ans = NA;
	if (!nomsg) {
		sprintf(genbuf, "ȷ���� [%-.40s] �����ݴ浵��", fileinfo->title);
		if (askyn(genbuf, NA, YEA) == NA)
			return FULLUPDATE;
	}
	sprintf(board, "bm/%s", currentuser.userid);
	if (dashf(board)) {
		if (nomsg)
			ans = YEA;
		else
			ans = askyn("Ҫ�����ھ��ݴ浵֮����", NA, YEA);
	}
	if (in_mail)
		sprintf(genbuf, "mail/%c/%s/%s", toupper(currentuser.userid[0]),
				currentuser.userid, fileinfo->filename);
	else
		sprintf(genbuf, "boards/%s/%s", key, fileinfo->filename);
	if (full)
		f_cp(genbuf, board, (ans ) ? O_APPEND : O_CREAT);
	else
		part_cp(genbuf, board, (ans ) ? "a+" : "w+");
	fp=fopen(board, "a");//���ӿ��� added by money 2002.3.22
	fprintf(fp, "\r\n");
	fclose(fp);
	if (!nomsg)
		presskeyfor("�ѽ������´����ݴ浵, �밴<Enter>����...", t_lines-1);
	return FULLUPDATE;
}

int a_Save2(char *path, char* key, struct fileheader* fileinfo, int nomsg) {

	char board[40];
	int ans = NA;
	FILE *fp;

	if (!nomsg) {
		sprintf(genbuf, "ȷ���� [%-.40s] �����ݴ浵��", fileinfo->title);
		if (askyn(genbuf, NA, YEA) == NA)
			return FULLUPDATE;
	}
	sprintf(board, "bm/%s", currentuser.userid);
	if (dashf(board)) {
		if (nomsg)
			ans = YEA;
		else
			ans = askyn("Ҫ�����ھ��ݴ浵֮����", NA, YEA);
	}
	if (in_mail)
		sprintf(genbuf, "/bin/%s mail/%c/%s/%s %s %s", (ans) ? "cat"
				: "cp -r", toupper(currentuser.userid[0]),
				currentuser.userid, fileinfo->filename, (ans) ? ">>" : "",
				board);
	else
		sprintf(genbuf, "/bin/%s boards/%s/%s %s %s", (ans) ? "cat"
				: "cp -r", key, fileinfo->filename, (ans) ? ">>" : "",
				board);
	system(genbuf);
	fp=fopen(board, "a");//���ӿ��� added by money 2002.3.22
	fprintf(fp, "\r\n");
	fclose(fp);
	if (!nomsg)
		presskeyfor("�ѽ������´����ݴ浵, �밴<Enter>����...", t_lines-1);
	return FULLUPDATE;
}

/* Add by shun . 1999.11.2.  to qutoesave to clipboard; change by money. 2002.1.12.*/
int quote_save(char *path, char *key, struct fileheader *fileinfo,
		int nomsg) {
	FILE *inf, *outf;
	char buf[256];
	//, *ptr;
	//char        op;
	//int         bflag;
	char qfile[STRLEN];
	char board[STRLEN ];
	char tmpfile[STRLEN];
	char ans[STRLEN ];

	sprintf(board, "bm/%s", currentuser.userid);
	sprintf(tmpfile, "bm/bmtmp.%s", currentuser.userid);
	sprintf(qfile, "boards/%s/%s", key, fileinfo->filename);
	outf = fopen(tmpfile, "w");
	if ( *qfile != '\0' && (inf = fopen(qfile, "r")) != NULL) {
		fgets(buf, 256, inf);
		fprintf(outf, "%s", buf);
		while (fgets(buf, 256, inf) != NULL)
			if (buf[0] == '\n')
				break;
		while (fgets(buf, 256, inf) != NULL) {
			if (strcmp(buf, "--\n") == 0)
				break;
			if (buf[ 250 ] != '\0')
				strcpy(buf+250, "\n");
			if ( !garbage_line(buf) )
				fprintf(outf, "%s", buf);
		}
		fprintf(outf, "\n");
		fclose(inf);
	}
	fclose(outf);
	/* ȥͷ��βȥ���ԣ����µĲ��ִ���ļ� */
	if (dashf(board) ) {
		sprintf(genbuf, "Ҫ�����ھ��ݴ浵֮����?(Y/N) [N]: ");
		if (!nomsg)
			a_prompt( -1, genbuf, ans, 39);
		if (ans[0] == 'Y' || ans[0] == 'y' ||nomsg)
			sprintf(genbuf, "/bin/cat bm/bmtmp.%s >> %s",
					currentuser.userid, board);
		else
			sprintf(genbuf, "/bin/cp -r bm/bmtmp.%s %s",
					currentuser.userid, board);
	} else {
		sprintf(genbuf, "/bin/cp -r bm/bmtmp.%s  %s", currentuser.userid,
				board);
	}
	system(genbuf);
	sprintf(genbuf, " �ѽ������´����ݴ浵, �밴�κμ��Լ��� << ");
	if (!nomsg)
		a_prompt( -1, genbuf, ans, 39);
	return 1;
}
/* End */

/* added by netty to handle post saving into (0)Announce */
int a_Import(char *path, char* key, int ent, struct fileheader *fileinfo,
		char * direct, int nomsg) {
	char fname[STRLEN], *ip, bname[PATHLEN], buf[ 256 ];
	int ch;
	MENU pm;
	FILE *fn;
	//Added by IAMFAT 2002-05-30
	char title[STRLEN];
	char currBM_bak[BM_LEN-1];

	modify_user_mode(DIGEST);
	sethomefile(buf, currentuser.userid, ".announcepath");
	if ((fn = fopen(buf, "r")) == NULL) {
		presskeyfor("�Բ���, ��û���趨˿·. �����趨˿·.", t_lines-1);
		return DONOTHING;
	}
	fscanf(fn, "%s", buf);
	fclose(fn);
	if (!dashd(buf)) {
		presskeyfor("���趨��˿·�Ѷ�ʧ, �������趨.", t_lines-1);
		return DONOTHING;
	}
	pm.path = buf;
	strcpy(pm.mtitle, "");

	/* added 2 sentences for change currBM before load .Names and
	 change it back after load .Names
	 by money 04.02.11
	 */
	memcpy(currBM_bak, currBM, BM_LEN - 1);
	sprintf(currBM, "%s", currentuser.userid);
	a_loadnames(&pm);
	memcpy(currBM, currBM_bak, BM_LEN - 1);

	strcpy(fname, fileinfo->filename);
	sprintf(bname, "%s/%s", pm.path, fname);
	ip = &fname[strlen(fname) - 1];
	while (dashf(bname)) {
		if (*ip == 'Z')
			ip++, *ip = 'A', *(ip + 1) = '\0';
		else
			(*ip)++;
		sprintf(bname, "%s/%s", pm.path, fname);
	}

	//Added by IAMFAT 2002-05-30
	ansi_filter(title, fileinfo->title);
	ellipsis(title, 38);
	sprintf(genbuf, "%-38.38s %s ", title, currentuser.userid);
	//sprintf(genbuf, "%-38.38s %s ", fileinfo->title, fileinfo->owner);
	//modified by roly 02.03.30
	//Removed by IAMFAT 2002-05-30
	//sprintf(genbuf, "%-38.38s %s ", fileinfo->title, currentuser.userid);

	a_additem(&pm, genbuf, fname);
	a_savenames(&pm);
	if (in_mail)
		sprintf(genbuf, "/bin/cp -r mail/%c/%s/%s %s",
				toupper(currentuser.userid[0]), currentuser.userid,
				fileinfo->filename, bname);
	else
		sprintf(genbuf, "/bin/cp -r boards/%s/%s %s", key,
				fileinfo->filename, bname);
	system(genbuf);
	if (!(fileinfo->accessed[1] & FILE_NOTICE)) {
		fileinfo->accessed[1] |= FILE_IMPORTED;
		substitute_record(direct, fileinfo, sizeof(*fileinfo), ent);
	}
	if (!nomsg && !DEFINE(DEF_MULTANNPATH)) {
		presskeyfor("�ѽ������·Ž�������, �밴<Enter>����...", t_lines-1);
	}
	for (ch = 0; ch < pm.num; ch++)
		free(pm.item[ch]);
	//add by fangu 2003.2.26, add log
	//sprintf(genbuf, "ANN IMP file:%s -floss:%s", fname,buf); //ǰ����source,������˿·
	//modified by iamfat 2003.2.28
	bm_log(currentuser.userid, currboard, BMLOG_ANNOUNCE, 1);
	sprintf(genbuf, "%s %s��¼ '%-20.20s..'\n", get_short_date(time(0)),
			currentuser.userid, fileinfo->title);
	file_append(ANN_LOG_PATH, genbuf);
	return FULLUPDATE;
}

// deardragon 0921

int a_menusearch(char *path, char* key, char * found) {
	FILE *fn;
	char bname[20], flag='0';
	char buf[PATHLEN], *ptr;
	int searchmode = 0;
	struct boardheader *bp;
	if (key == NULL) {
		key = bname;
		a_prompt(-1, "��������Ѱ֮����������: ", key, 18);
		searchmode = 1;
	}
	sprintf(buf, "0Announce/.Search");
	if (key[0] != '\0' && (fn = fopen(buf, "r")) != NULL) {
		while (fgets(buf, sizeof(buf), fn) != NULL) {
			if (searchmode && !strstr(buf, "groups/"))
				continue;
			ptr = strchr(buf, ':');
			if (!ptr)
				return 0;
			else {
				*ptr = '\0';
				ptr = strtok(ptr + 1, " \t\n");
			}
			if (!strcasecmp(buf, key)) {
				bp = getbcache(key);
				if (hasreadperm(&currentuser, bp))
				{
					if ((bp->flag & BOARD_CLUB_FLAG)
						&& (bp->flag & BOARD_READ_FLAG)
						&& !chk_currBM(bp->BM, 1)
						&& !isclubmember(currentuser.userid, bp->filename))
						break;
					sprintf(found, "0Announce/%s", ptr);
					flag = '1';
					break;
				}
			}
		}
		fclose(fn);
		if (flag == '0') {
			presskeyfor("�Ҳ������������������, ��<Enter>����...", t_lines-1);
			return 0;
		}
		return 1;
	}

	return 0;
}
int b_menusearch(MENU *pm) {
	char key[20];
	int i;
	a_prompt(-1, "��������ѰĿ¼֮����: ", key, 18);
	if (key[0] == '\0' || pm->num == 0)
		return 0;
	for (i=0; i< (pm->num); i++) {
		if (strstr(pm->item[i]->title, key) )
			return i;
	}
	return 0;
}

#ifdef INTERNET_EMAIL
void a_forward(char *path,ITEM* pitem,int mode) {
	struct boardheader fhdr;
	char fname[PATHLEN], *mesg;
	sprintf(fname, "%s/%s", path, pitem->fname);
	if (dashf(fname)) {
		strlcpy(fhdr.title, pitem->title, sizeof(fhdr.title));
		strlcpy(fhdr.filename, pitem->fname, sizeof(fhdr.filename));
		switch (doforward(path, &fhdr, mode)) {
			case 0:
				mesg = "����ת�����!\n";
				break;
			case BBS_EINTNL:
				mesg = "System error!!.\n";
				break;
			case -2:
				mesg = "Invalid address.\n";
				break;
			case BBS_EBLKLST:
				mesg = "�Է������յ������ż�.\n";
				break;
			default:
				mesg = "ȡ��ת�Ķ���.\n";
		}
		outs(mesg);
	} else {
		move(t_lines - 1, 0);
		prints("�޷�ת�Ĵ���Ŀ.\n");
	}
	pressanykey();

}
#endif

void a_newitem(MENU *pm, int mode) {
	char uident[STRLEN];
	char board[STRLEN], title[STRLEN];
	char fname[STRLEN], fpath[PATHLEN], fpath2[PATHLEN];
	char *mesg;
	FILE *pn;
	char head;
	srand(time(0));

	pm->page = 9999;
	head = 'X';
	switch (mode) {
		case ADDITEM:
			head = 'A'; /* article */
			break;
		case ADDGROUP:
			head = 'D'; /* directory */
			break;
		case ADDMAIL:
			sprintf(board, "bm/%s", currentuser.userid);
			if (!dashf(board)) {
				//a_prompt(-1, "�������������������´����ݴ浵, ��<Enter>����...", ans, 2);
				presskeyfor("�������������������´����ݴ浵, ��<Enter>����...", t_lines-1);
				return;
			}
			mesg = "�����뵵��֮Ӣ������(�ɺ�����)��";
			break;
	}
	/* edwardc.990320 system will assign a filename for you .. */
	sprintf(fname, "%c%lX", head, time(0) + getpid() + getppid() + rand());
	sprintf(fpath, "%s/%s", pm->path, fname);
	mesg = "�������ļ���Ŀ¼֮�������ƣ� ";
	a_prompt(-1, mesg, title, 35);
	if (*title == '\0')
		return;
	switch (mode) {
		case ADDITEM:
			if (vedit(fpath, 0, YEA) == -1)
				return;
			chmod(fpath, 0644);
			break;
		case ADDGROUP:
			if (strlen(fpath)>=230) {
				//a_prompt(-1,"�Բ���, Ŀ¼���̫��, ȡ������!",ans,2);
				presskeyfor("�Բ���, Ŀ¼���̫��, ȡ������!", t_lines-1);
				return;
			}
			mkdir(fpath, 0755);
			chmod(fpath, 0755);
			break;
		case ADDMAIL:
			rename(board, fpath);
			break;
	}
	if (mode != ADDGROUP)
		sprintf(genbuf, "%-38.38s %s", title, currentuser.userid);
	else {
		/*Add by SmallPig*/
		move(1, 0);
		clrtoeol();
		getdata(1, 0, "����: ", uident, 35, DOECHO, YEA);
		if (uident[0] != '\0')
			sprintf(genbuf, "%-38.38s(BM: %s)", title, uident);
		else
			sprintf(genbuf, "%-38.38s", title);
	}
	a_additem(pm, genbuf, fname);
	a_savenames(pm);
	if (mode == ADDGROUP) {
		sprintf(fpath2, "%s/%s/.Names", pm->path, fname);
		if ((pn = fopen(fpath2, "w")) != NULL) {
			fprintf(pn, "#\n");
			fprintf(pn, "# Title=%s\n", genbuf);
			fprintf(pn, "#\n");
			fclose(pn);
		}
	}
	//add by fangu 2003.2.26, add log
	//sprintf(genbuf, "ANN newitem pm:%s - fname:%s - mode:%d", pm->path,fname,mode);
	sprintf(genbuf, "%s %s����%s '%-20.20s..'\n", get_short_date(time(0)),
			currentuser.userid, (head=='D') ? "Ŀ¼" : "����", title);
	file_append(ANN_LOG_PATH, genbuf);
	bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
}

void a_moveitem(MENU *pm) {
	ITEM *tmp;
	char newnum[STRLEN];
	int num, n;
	sprintf(genbuf, "������� %d ����´���: ", pm->now + 1);
	a_prompt(-2, genbuf, newnum, 6);
	num = (newnum[0] == '$') ? 9999 : atoi(newnum) - 1;
	if (num >= pm->num)
		num = pm->num - 1;
	else if (num < 0)
		return;
	tmp = pm->item[pm->now];
	if (num > pm->now) {
		for (n = pm->now; n < num; n++)
			pm->item[n] = pm->item[n + 1];
	} else {
		for (n = pm->now; n > num; n--)
			pm->item[n] = pm->item[n - 1];
	}
	pm->item[num] = tmp;
	pm->now = num;
	a_savenames(pm);
	bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
}

void a_copypaste(MENU *pm, int paste) {
	ITEM *item;
	static char title[sizeof(item->title)], filename[STRLEN], fpath[PATHLEN];
	char newpath[PATHLEN]/*,ans[5]*/;
	FILE *fn;
	move(t_lines - 1, 0);
	sethomefile(fpath, currentuser.userid, ".copypaste");

	if (!paste) {
		fn = fopen(fpath, "w+");
		item = pm->item[pm->now];
		fwrite(item->title, sizeof(item->title), 1, fn);
		fwrite(item->fname, sizeof(item->fname), 1, fn);
		sprintf(genbuf, "%s/%s", pm->path, item->fname);
		strlcpy(fpath, genbuf, PATHLEN);
		fpath[PATHLEN - 1] = '\0';
		fwrite(fpath, sizeof(fpath), 1, fn);
		fclose(fn);
		//a_prompt(-1,"������ʶ���. (ע��! ճ����������ܽ����� delete!)",ans,2);
		presskeyfor("������ʶ���. (ע��! ճ����������ܽ����� delete!)", t_lines-1);
	} else {
		if ((fn = fopen(fpath /*genbuf*/, "r")) == NULL) {
			//a_prompt(-1,"����ʹ�� copy ������ʹ�� paste ����.",ans,2);
			presskeyfor("����ʹ�� copy ������ʹ�� paste ����.", t_lines-1);
			return;
		}
		fread(title, sizeof(item->title), 1, fn);
		fread(filename, sizeof(item->fname), 1, fn);
		fread(fpath, sizeof(fpath), 1, fn);
		fclose(fn);
		sprintf(newpath, "%s/%s", pm->path, filename);
		if (*title == '\0' || *filename == '\0') {
			//a_prompt(-1,"����ʹ�� copy ������ʹ�� paste ����. ",ans,2);
			presskeyfor("����ʹ�� copy ������ʹ�� paste ����.", t_lines-1);
		} else if (!(dashf(fpath) || dashd(fpath))) {
			sprintf(genbuf, "��������%s����/Ŀ¼������,���ܱ�ɾ��,ȡ��ճ��.", filename);
			//a_prompt(-1,genbuf,ans,2);
			presskeyfor(genbuf, t_lines-1);
		} else if (dashf(newpath) || dashd(newpath)) {
			sprintf(genbuf, "%s ����/Ŀ¼�Ѿ�����. ", filename);
			//a_prompt(-1,genbuf,ans,2);
			presskeyfor(genbuf, t_lines-1);
		} else if (strstr(newpath, fpath) != NULL) {
			//a_prompt(-1,"�޷���Ŀ¼����Լ�����Ŀ¼��, ���������Ȧ.",ans,2);
			presskeyfor("�޷���Ŀ¼����Լ�����Ŀ¼��, ���������Ȧ.", t_lines-1);
		} else { /*
		 sprintf(genbuf, "��ȷ��Ҫճ�� %s ����/Ŀ¼��", filename);
		 if (askyn(genbuf, NA, YEA) == YEA) { */
			if (dashd(fpath)) { /* ��Ŀ¼ */
				sprintf(genbuf, "/bin/cp -rp %s %s", fpath, newpath);
				system(genbuf);
			} else if (paste == 1) { //copy
				f_cp(fpath, newpath, O_CREAT);
			} else if (paste == 2) { //link
				f_ln(fpath, newpath);
			} else if (paste == 3) {//cut
				f_cp(fpath, newpath, O_CREAT);
			}
			a_additem(pm, title, filename);
			a_savenames(pm);
			//add by fangu 2003.2.26, add log
			sprintf(genbuf, "%s %sճ�� '%-20.20s..' �� '%-20.20s..'\n",
					get_short_date(time(0)), currentuser.userid, title,
					pm->mtitle);
			file_append(ANN_LOG_PATH, genbuf);
			bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
			//	                        sprintf(genbuf, "ANN PASTE  '%s'",title);// ��¼ ԴĿ¼ - ��Ŀ¼λ��
			//	                        report(genbuf);
			//}
		}
	}

	pm->page = 9999;
}

//ɾ���������ĵ�ǰ��,Ŀ¼���ļ�
void a_delete(MENU *pm) {
	ITEM *item;
	char fpath[PATHLEN], ans[5];
	int n;
	FILE *fn;
	item = pm->item[pm->now];
	move(t_lines - 2, 0);
	prints("%5d  %-50s\n", pm->now + 1, item->title);
	sethomefile(fpath, currentuser.userid, ".copypaste");
	if ((fn = fopen(fpath /*genbuf*/, "r")) != NULL) {
		//�ж��ļ��Ƿ����,����ͼ����������Ϣ,title,fname,fpath
		fread(genbuf, sizeof(item->title), 1, fn);
		fread(genbuf, sizeof(item->fname), 1, fn);
		fread(genbuf, sizeof(fpath), 1, fn);
		fclose(fn);
	}
	sprintf(fpath, "%s/%s", pm->path, item->fname);
	if (!strncmp(genbuf, fpath, sizeof(fpath)))
		a_prompt(
				-1,
				"[1;5;31m����[m: �õ���/Ŀ¼�Ǳ��������˱��, [1;33m���ɾ��, ������ճ��������!![m",
				ans, 2);
	if (dashf(fpath)) {
		if (askyn("ɾ���˵���, ȷ����", NA, YEA) == NA)
			return;
		unlink(fpath);
	} else if (dashd(fpath)) {
		if (askyn("ɾ��������Ŀ¼, ����ЦŶ, ȷ����", NA, YEA) == NA)
			return;
		deltree(fpath);
	}
	//add by iamfat 2003.2.26, add log
	sprintf(genbuf, "%s %s�� '%-20.20s..' ɾ�� '%-20.20s..'\n",
			get_short_date(time(0)), currentuser.userid, pm->mtitle,
			item->title);
	file_append(ANN_LOG_PATH, genbuf);
	bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
	//	sprintf(genbuf, "ANN DEL '%s'", item->title);
	//	report(genbuf);
	free(item);
	(pm->num)--;
	for (n = pm->now; n < pm->num; n++)
		pm->item[n] = pm->item[n + 1];
	a_savenames(pm);
}

/* addedd by roly 02.05.15 */
void a_RangeDelete(MENU *pm, int num1, int num2) {
	ITEM *item;
	char fpath[PATHLEN];
	int i, n;
	for (n=num1; n<=num2; n++) {
		item = pm->item[n];
		sprintf(fpath, "%s/%s", pm->path, item->fname);
		if (dashf(fpath)) {
			unlink(fpath);
		} else if (dashd(fpath)) {
			deltree(fpath);
		}
	}

	for (n=num1; n<=num2; n++) {
		free(pm->item[n]);
		(pm->num)--;
	}

	for (i = num1; i < pm->num; i++)
		pm->item[i] = pm->item[i + num2-num1+1];
	a_savenames(pm);

	//add by fangu 2003.2.26, add log
	sprintf(genbuf, "%s %s�� '%10.10s..' ɾ����%d����%dƪ\n",
			get_short_date(time(0)), currentuser.userid, pm->mtitle, num1, num2);
	file_append(ANN_LOG_PATH, genbuf);
	bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
	//sprintf(genbuf, "ANN RANGE_DEL %s",fpath);
	//report(genbuf);

}

int a_Rangefunc(MENU *pm) {
	struct fileheader;
	char annpath[512];
	char buf[STRLEN], ans[8], info[STRLEN];

	ITEM *item;

	char items[2][20] = { "ɾ��", "������˿·" };
	int type, num1, num2, i, max=2;

	saveline(t_lines - 1, 0);
	strcpy(info, "����:");
	for (i=0; i<max; i++) {
		sprintf(buf, " %d)%s", i+1, items[i]);
		strcat(info, buf);
	}
	strcat(info, " [0]: ");
	getdata(t_lines-1, 0, info, ans, 2, DOECHO, YEA);
	type = atoi(ans);
	if (type <= 0 || type > max) {
		saveline(t_lines - 1, 1);
		return DONOTHING;
	}

	move(t_lines-1, 0);
	clrtoeol();
	prints("���β���: %s", items[type-1]);
	getdata(t_lines-1, 20, "��ƪ���±��: ", ans, 6, DOECHO, YEA);
	num1=atoi(ans);
	if (num1>0) {
		getdata(t_lines-1, 40, "ĩƬ���±��: ", ans, 6, DOECHO, YEA);
		num2=atoi(ans);
	}
	if (num1<=0||num2<=0||num2<=num1 ||num2>pm->num) {
		move(t_lines-1, 60);
		prints("��������...");
		egetch();
		saveline(t_lines - 1, 1);
		return DONOTHING;
	}

	sprintf(info, "���� [%s] ������Χ [ %d -- %d ]��ȷ����", items[type-1], num1,
			num2);
	if (askyn(info, NA, YEA)==NA) {
		saveline(t_lines - 1, 1);
		return DONOTHING;
	}

	if (type == 2) {
		FILE *fn;
		char genbuf[512];
		if (DEFINE(DEF_MULTANNPATH) && set_ann_path(NULL, NULL,
				ANNPATH_GETMODE) == 0)
			return DONOTHING;

		sethomefile(annpath, currentuser.userid, ".announcepath");
		if ((fn = fopen(annpath, "r")) == NULL) {
			presskeyfor("�Բ���, ��û���趨˿·. ������ f �趨˿·.", t_lines-1);
			saveline(t_lines - 1, 1);
			return DONOTHING;
		}
		fscanf(fn, "%s", annpath);
		fclose(fn);
		if (!dashd(annpath)) {
			presskeyfor("���趨��˿·�Ѷ�ʧ, �������� f �趨.", t_lines-1);
			saveline(t_lines - 1, 1);
			return DONOTHING;
		}
		if (!strcmp(annpath, pm->path)) {
			presskeyfor("���趨��˿·�뵱ǰĿ¼��ͬ�����β���ȡ��.", t_lines-1);
			return DONOTHING;
		}

		for (i=num1-1; i<=num2-1; i++) {
			item = pm->item[i];
			sprintf(genbuf, "%s/%s", pm->path, item->fname);
			if (dashd(genbuf))
				if (strstr(annpath, genbuf)!=NULL) {
					presskeyfor("���趨��˿·λ����ѡĳ��Ŀ¼�У��ᵼ��Ƕ��Ŀ¼�����β���ȡ��.", t_lines
							-1);
					return DONOTHING;
				}
		}
	}

	switch (type) {
		case 1:
			sprintf(info, "�����Ҫɾ����Щ�ļ���Ŀ¼\?\?!!��ȷ����??");
			if (askyn(info, NA, YEA)==NA) {
				saveline(t_lines - 1, 1);
				return DONOTHING;
			}
			a_RangeDelete(pm, num1-1, num2-1);
			pm->page = 9999;
			break;
		case 2:
			for (i=num1-1; i<=num2-1; i++) {
				a_a_Import(pm, NA, i);
			}

			break;
	}
	bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
	saveline(t_lines - 1, 1);
	return DIRCHANGED;
}

/* add end */

/* added by roly 02.05.15 */
/* msg�����ж��Ƿ���ʾ����ɲ��� */
/* menuitem ������ִ��range a_a_Import���Ƿ���pm->now��ֵ */
int a_a_Import(MENU *pm, int msg, int menuitem) {
	ITEM *item;
	char fpath[PATHLEN], annpath[512], fname[512], dfname[512], *ip,
			genbuf[1024];
	int ch;
	FILE *fn;
	MENU newpm;

	if (menuitem<0)
		item = pm->item[pm->now];
	else
		item = pm->item[menuitem];

	sethomefile(annpath, currentuser.userid, ".announcepath");
	if ((fn = fopen(annpath, "r")) == NULL) {
		presskeyfor("�Բ���, ��û���趨˿·. ������ f �趨˿·.", t_lines-1);
		return 1;
	}
	fscanf(fn, "%s", annpath);
	fclose(fn);
	if (!dashd(annpath)) {
		presskeyfor("���趨��˿·�Ѷ�ʧ, �������� f �趨.", t_lines-1);
		return 1;
	}

	if (!strcmp(annpath, pm->path)) {
		presskeyfor("���趨��˿·�뵱ǰĿ¼��ͬ�����β���ȡ��.", t_lines-1);
		return 1;
	}

	sprintf(genbuf, "%s/%s", pm->path, item->fname);

	if (dashd(genbuf))
		if (strstr(annpath, genbuf)!=NULL) {
			presskeyfor("���趨��˿·λ����ѡĿ¼�У��ᵼ��Ƕ��Ŀ¼�����β���ȡ��.", t_lines-1);
			return 1;
		}

	//���ϼ��˿·


	newpm.path = annpath;
	strcpy(newpm.mtitle, "");
	a_loadnames(&newpm);
	strcpy(fname, item->fname);
	sprintf(dfname, "%s/%s", newpm.path, fname);
	ip = &fname[strlen(fname) - 1];
	if (dashf(dfname)||dashd(dfname)) {
		ip++, *ip = 'A', *(ip + 1) = '\0';
	}
	while (dashf(dfname)||dashd(dfname)) {
		if (*ip == 'Z')
			ip++, *ip = 'A', *(ip + 1) = '\0';
		else
			(*ip)++;
		sprintf(dfname, "%s/%s", newpm.path, fname);
	}
	sprintf(genbuf, "%-72.72s", item->title);
	//sprintf(genbuf, "%-38.38s %s ", item->title, currentuser.userid);
	a_additem(&newpm, genbuf, fname);
	a_savenames(&newpm);

	//����Ϊ���newpm

	sprintf(fpath, "%s/%s", pm->path, item->fname);
	sprintf(genbuf, "/bin/cp -r %s %s", fpath, dfname);
	system(genbuf);

	if (msg)
		presskeyfor("�ѽ�������/Ŀ¼�Ž�˿·, �밴<Enter>����...", t_lines-1);

	for (ch = 0; ch < newpm.num; ch++)
		free(newpm.item[ch]);
	pm->page=9999;
	//add by fangu 2003.2.26, add log
	sprintf(genbuf, "%s %s�� '%-20.20s..' ��¼ '%-20.20s..'\n",
			get_short_date(time(0)), currentuser.userid, pm->mtitle,
			item->title);
	file_append(ANN_LOG_PATH, genbuf);
	bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
	//sprintf(genbuf, "ANN AA_IMP %s ", dfname);//��¼ ˿·-Դ�ļ�
	//report(genbuf);
	return 1;

}
/* add end */

void a_newname(MENU *pm) {
	ITEM *item;
	char fname[STRLEN];
	char fpath[PATHLEN];
	char *mesg;
	item = pm->item[pm->now];
	a_prompt(-2, "�µ���: ", fname, 12);
	if (*fname == '\0')
		return;
	sprintf(fpath, "%s/%s", pm->path, fname);
	if (!valid_fname(fname)) {
		mesg = "���Ϸ���������.";
	} else if (dashf(fpath) || dashd(fpath)) {
		mesg = "ϵͳ�����д˵���������.";
	} else {
		sprintf(genbuf, "%s/%s", pm->path, item->fname);
		if (rename(genbuf, fpath) == 0) {
			//add by fangu 2003.2.26 add log
			//sprintf(genbuf, "ANN RENAME %s >> %s", item->fname,fname,pm->path);
			//��¼ ���� - ���� -�ļ�
			//report(genbuf);
			strcpy(item->fname, fname);
			a_savenames(pm);
			return;
		}
		mesg = "��������ʧ��!!";
	}
	outs(mesg);
	egetch();

}

/* added by roly 02.05.20,convert path to title */
void ann_to_title(char *annpath) {
	FILE *fn;
	int ipos, ipos2, ipos3, ipos4, icount=0;
	char *ptr;
	char tmppath[1000], buf[1000], result[1000];
	char dirname[STRLEN], titlename[STRLEN];
	for (ipos=0; ipos<strlen(annpath); ipos++) {
		if (annpath[ipos]=='/') {
			icount++;
			if (icount==3)
				break;
		}
	}
	if (icount<3)
		return;

	//���'/'����3������ʾ���Ǵ��ڰ��澫����������

	ipos++;
	ipos2=ipos;
	sprintf(tmppath, "%s", annpath);
	for (; ipos<strlen(annpath); ipos++)
		if (annpath[ipos]=='/')
			break;
	tmppath[ipos]='\0';
	sprintf(result, "%s��/", tmppath+ipos2);
	//��ȡ����

	if (ipos==strlen(annpath)) { //λ��ĳ�����Ŀ¼
		sprintf(annpath, "%s", result);
		return;
	}

	sprintf(buf, "%s/.Names", tmppath);
	tmppath[ipos]='/';
	ipos++;
	ipos2=ipos;
	size_t len = strlen(annpath);
	for (; ipos < len; ipos++) {
		if (annpath[ipos] == '/' || annpath[ipos + 1] == '\0') {
			if (annpath[ipos]=='/') {
				strlcpy(dirname, annpath + ipos2, ipos - ipos2 + 1);
				ipos4 = ipos - ipos2;
			} else {
				strlcpy(dirname, annpath + ipos2, ipos - ipos2 + 2);
				ipos4=ipos-ipos2+1;
			}

			if ((fn = fopen(buf, "r")) == NULL)
				break;
			while (fgets(buf, sizeof(buf), fn) != NULL) {
				if ((ptr = strchr(buf, '\n')) != NULL)
					*ptr = '\0';
				if (strncmp(buf, "Name=", 5) == 0) {
					strlcpy(titlename, buf + 5, 72);
				} else if (strncmp(buf, "Path=", 5) == 0) {

					if (((strncmp(buf, "Path=~/", 7) == 0) && (strncmp(buf
							+7, dirname, ipos4)==0)) ||(strncmp(buf+5,
							dirname, ipos4)==0)) {

						for (ipos3=36; ipos3>=1; ipos3--) {
							if (titlename[ipos3]!=' ') {
								titlename[ipos3+1]='/';
								titlename[ipos3+2]='\0';
								break;
							}
						}
						strcat(result, titlename);
						report(titlename, currentuser.userid);
						break;
					}
				}
			}
			fclose(fn);
			tmppath[ipos]='\0';
			sprintf(buf, "%s/.Names", tmppath);
			tmppath[ipos]='/';
			ipos2=ipos+1;
		}
	}

	sprintf(annpath, "%s", result);
	return;
}

/* add end */
//���û��ھ������İ������з�Ӧ
void a_manager(MENU *pm, int ch) {
	char uident[STRLEN];
	ITEM *item=NULL;
	MENU xpm;
	char fpath[PATHLEN], changed_T[STRLEN];//, ans[5];
	char fowner[IDLEN]=""; //add by Danielfree 06.2.20
	int i=0;
	if (pm->num > 0) {
		item = pm->item[pm->now];
		sprintf(fpath, "%s/%s", pm->path, item->fname);
	}
	if (item && strstr(item->title+38, "ֻ��"))
		readonly=YEA;
	else
		readonly=NA;

	switch (ch) {
		case 'a':
			a_newitem(pm, ADDITEM);
			break;
		case 'g':
			a_newitem(pm, ADDGROUP);
			break;
		case 'i':
			a_newitem(pm, ADDMAIL);
			break;
		case 'p':
			a_copypaste(pm, 1);
			break;
			/* added by roly 02.05.15 */
		case 'l':
			a_copypaste(pm, 2);
			break;
		case 'X':
			a_copypaste(pm, 3);
			break;
		case 't':
			if (DEFINE(DEF_MULTANNPATH))
				currentuser.userdefine &= ~DEF_MULTANNPATH;
			else
				currentuser.userdefine |= DEF_MULTANNPATH;
			substitut_record(PASSFILE, &currentuser, sizeof(currentuser),
					usernum);
			pm->page = 9999;
			break;
		case 'S': {
			FILE *fn;
			pm->page = 9999;
			sethomefile(genbuf, currentuser.userid, ".announcepath");
			fn = fopen(genbuf, "r");
			if (fn) {
				char tmpath[1000];
				fgets(tmpath, 1000, fn);
				fclose(fn);
				ann_to_title(tmpath);
				sprintf(genbuf, "��ǰ˿·: %s", tmpath);
			} else
				strcpy(genbuf, "����û���� f ����˿·.");
			presskeyfor(genbuf, t_lines-1);
			break;
		}
			/* add end */

			/* add by roly 02.05.15*/
		case 'I':
			pm->page = 9999;
			if (DEFINE(DEF_MULTANNPATH) && set_ann_path(NULL, NULL,
					ANNPATH_GETMODE) == 0)
				break;
			a_a_Import(pm, YEA, -1);
			break;
		case 'L':
			pm->page = 9999;
			a_Rangefunc(pm);
			break;
			/* add end */
		case 'f':
			pm->page = 9999;
			if (DEFINE(DEF_MULTANNPATH)) {
				set_ann_path(pm->mtitle, pm->path, ANNPATH_SETMODE);
			} else {
				FILE *fn;
				sethomefile(genbuf, currentuser.userid, ".announcepath");
				if ((fn = fopen(genbuf, "w+")) != NULL) {
					fprintf(fn, "%s", pm->path);
					fclose(fn);
					presskeyfor("�ѽ���·����Ϊ˿·, �밴<Enter>����...", t_lines-1);
				}
			}
			break;
			/* add by jacobson,20050515 */
			/* Ѱ�Ҷ�ʧ��Ŀ */
		case 'z':
			if (HAS_PERM(PERM_SYSOPS)) {
				int i;
				i=a_repair(pm);
				if (i>0) {
					sprintf(genbuf, "���� %d ����ʧ��Ŀ,�ѻָ�,�밴Enter����...", i);
				} else {
					sprintf(genbuf, "δ���ֶ�ʧ��Ŀ,�밴Enter����...");
				}
				//  a_prompt(-1,genbuf,ans,39);
				presskeyfor(genbuf, t_lines-1);
				pm->page = 9999;
			}
			break;
			/* add end */
	}
	if (pm->num > 0)
		switch (ch) {
			case 's':
				//if (++a_fmode >= 3)
				//	a_fmode = 1;
				a_fmode = !a_fmode;
				pm->page = 9999;
				break;
			case 'm':
				a_moveitem(pm);
				pm->page = 9999;
				break;
			case 'd':
			case 'D':
				a_delete(pm);
				pm->page = 9999;
				break;
			case 'V':
			case 'v':
				if (HAS_PERM(PERM_SYSOPS)) {
					if (ch == 'v')
						sprintf(fpath, "%s/.Names", pm->path);
					else
						sprintf(fpath, "0Announce/.Search");

					if (dashf(fpath)) {
						modify_user_mode(EDITANN);
						vedit(fpath, 0, YEA);
						modify_user_mode(DIGEST);
					}
					pm->page = 9999;
				}
				break;
			case 'T':
				if (readonly==YEA)
					break;
				//Modified by IAMFAT 2002-05-25
				ansi_filter(changed_T, item->title);
				//add by Danielfree 06.2.20
				for (i =0; i<IDLEN; i++) {
					if (changed_T[39+i]) {
						fowner[i]=changed_T[39+i];
					}
				}
				fowner[sizeof(fowner) - 1] = '\0';
				//add end
				changed_T[38]='\0';
				rtrim(changed_T);
				//a_prompt(-2, "�±���: ", changed_T, 35);
				saveline(t_lines-2, 0);
				move(t_lines-2, 0);
				clrtoeol();
				//Modified by IAMFAT 2002-05-30
				getdata(t_lines-2, 0, "�±���: ", changed_T, 39, DOECHO, NA);
				saveline(t_lines-2, 1);
				//Modify End.
				/*
				 * modified by netty to properly handle title
				 * change,add bm by SmallPig
				 */
				if (*changed_T) {
					sprintf(genbuf,
							"%s %s�� '%-20.20s..' ����Ϊ '%-20.20s..'\n",
							get_short_date(time(0)), currentuser.userid,
							item->title, changed_T);
					file_append(ANN_LOG_PATH, genbuf);
					bm_log(currentuser.userid, currboard, BMLOG_DOANN, 1);
					if (dashf(fpath)) {
						sprintf(genbuf, "%-38.38s %s", changed_T, fowner); //suggest by Humorous
						//sprintf(genbuf, "%-38.38s %s", changed_T, currentuser.userid);
						strlcpy(item->title, genbuf, 72);
						item->title[71] = '\0';
					} else if (dashd(fpath)) {
						/*Modified by IAMFAT 2002-05-25*/
						move(1, 0);
						clrtoeol();
						/*
						 * usercomplete("����:
						 * ",uident) ;
						 */
						getdata(1, 0, "����: ", uident, 35, DOECHO, YEA);
						if (uident[0] != '\0')
							sprintf(genbuf, "%-38.38s(BM: %s)", changed_T,
									uident);
						else
							sprintf(genbuf, "%-38.38s", changed_T);
						xpm.path=fpath;
						a_loadnames(&xpm);
						strcpy(xpm.mtitle, genbuf);
						a_savenames(&xpm);

						strlcpy(item->title, genbuf, 72);
					}
					item->title[71] = '\0';
					a_savenames(pm);
				}
				pm->page = 9999;
				break;
			case 'E':
				if (readonly==YEA)
					break;
				if (dashf(fpath)) {
					modify_user_mode(EDITANN);
					//vedit(fpath,0,NA);
					vedit(fpath, 0, YEA);
					modify_user_mode(DIGEST);
				}
				pm->page = 9999;
				break;
			case 'n':
				a_newname(pm);
				pm->page = 9999;
				break;
			case 'c':
				a_copypaste(pm, 0);
				break;

		}
}

void a_menu(char *maintitle, char* path, int lastlevel, int lastbmonly) {
	MENU me;
	char fname[PATHLEN], tmp[STRLEN];
	int ch;
	char *bmstr;
	char buf[STRLEN];
	int bmonly;
	int number = 0;
	int savemode;
	char something[PATHLEN+20];//add by wujian

	modify_user_mode(DIGEST);
	strcpy(something, path);
	strcat(something, "/welcome");
	me.path = path;
	if (dashf(something))
		show_help(something);
	strcpy(me.mtitle, maintitle);
	me.level = lastlevel;
	bmonly = lastbmonly;
	a_loadnames(&me);

	memset(buf, 0, STRLEN);
	strlcpy(buf, me.mtitle, STRLEN);
	bmstr = strstr(buf, "(BM:");
	if (bmstr != NULL) {
		if (chk_currBM(bmstr + 4, 0))
			me.level |= PERM_BOARDS;
		else if (bmonly == 1 && !HAS_PERM(PERM_BOARDS))
			return;
	}
	if ((strstr(me.mtitle, "(BM: BMS)") && !chk_currBM(currBM, 0))
			|| (strstr(me.mtitle, "(BM: SYSOPS)")
					&& !HAS_PERM(PERM_SYSOPS)) || (strstr(me.mtitle,
			"(BM: OBOARDS)") && !HAS_PERM(PERM_OBOARDS)) || (strstr(
			me.mtitle, "(BM: SECRET)") && !chk_currBM(currBM, 0))) {
		for (ch = 0; ch < me.num; ch++)
			free(me.item[ch]);
		return;
	}

	strcpy(buf, me.mtitle);
	bmstr = strstr(buf, "(BM:");

	me.page = 9999;
	me.now = 0;
	while (1) {
		if (me.now >= me.num && me.num > 0) {
			me.now = me.num - 1;
		} else if (me.now < 0) {
			me.now = 0;
		}
		if (me.now < me.page || me.now >= me.page + A_PAGESIZE) {
			me.page = me.now - (me.now % A_PAGESIZE);
			a_showmenu(&me);
		}
		move(3 + me.now - me.page, 0);
		prints("->");
		ch = egetch();
		move(3 + me.now - me.page, 0);
		prints("  ");
		if (ch == 'Q' || ch == 'q' || ch == KEY_LEFT || ch == EOF)
			break;
		EXPRESS: /* Leeward 98.09.13 */
		switch (ch) {
			/*Added by kit 2001.01.29*/
			case '*':
				if (me.num > 0) {
					a_file_info(&me);
					me.page = 9999;
				}
				break;
			case 'o':
				online_users_show();
				a_showmenu(&me);
				break;
				/* End */
			case KEY_UP:
			case 'K':
			case 'k':
				if (--me.now < 0)
					me.now = me.num - 1;
				break;
			case KEY_DOWN:
			case 'J':
			case 'j':
				if (++me.now >= me.num)
					me.now = 0;
				break;
			case KEY_PGUP:
			case Ctrl('B'):
				if (me.now >= A_PAGESIZE)
					me.now -= A_PAGESIZE;
				else if (me.now > 0)
					me.now = 0;
				else
					me.now = me.num - 1;
				break;
			case KEY_PGDN:
			case Ctrl('F'):
			case ' ':
				if (me.now < me.num - A_PAGESIZE)
					me.now += A_PAGESIZE;
				else if (me.now < me.num - 1)
					me.now = me.num - 1;
				else
					me.now = 0;
				break;
			case KEY_HOME:
				me.now = 0;
				break;
			case KEY_END:
			case '$':
				me.now = me.num - 1;
				break;
			case Ctrl('C'):
				if (me.num==0)
					break;
				set_safe_record();
				if (!HAS_PERM(PERM_POST))
					break;
				sprintf(fname, "%s/%s", path, me.item[me.now]->fname);
				if (!dashf(fname))
					break;
				if (me.now < me.num) {
					char bname[30];
					clear();
					if (get_a_boardname(bname, "������Ҫת��������������: ")) {
						move(1, 0);
						struct boardheader *bp = getbcache(bname);
						if (bp == NULL || !haspostperm(&currentuser, bp)) {
							prints("\n\n������Ȩ���� %s ��������.", bname);
							pressreturn();
							me.page = 9999;
							break;
						}
						sprintf(tmp, "��ȷ��Ҫת���� %s ����", bname);
						if (askyn(tmp, NA, NA) == 1) {
							Postfile(fname, bname, me.item[me.now]->title,
									4);
							move(3, 0);
							sprintf(tmp, "[1m�Ѿ�����ת���� %s ����[m", bname);
							outs(tmp);
							refresh();
							sleep(1);
						}
					}
					me.page = 9999;
				}
				show_message(NULL);
				break;
			case 'M':
				savemode = uinfo.mode;
				m_new();
				modify_user_mode(savemode);
				me.page = 9999;
				break;
#if 0

				case 'L':
				savemode = uinfo.mode;
				m_read();
				modify_user_mode(savemode);
				me.page = 9999;
				break;
#endif

			case 'h':
				show_help("help/announcereadhelp");
				me.page = 9999;
				break;
			case '\n':
			case '\r':
				if (number > 0) {
					me.now = number - 1;
					number = 0;
					continue;
				}
			case 'R':
			case 'r':
			case KEY_RIGHT:
				if (me.now < me.num) {
					sprintf(fname, "%s/%s", path, me.item[me.now]->fname);
					if (dashf(fname) ) {
						/* Leeward 98.09:�ھ������Ķ����µ�ĩβʱ�����ϣ��¼�ͷֱ����ת��ǰ����һ�� */
						if (ansimore(fname, NA)==KEY_UP) {
							if (--me.now < 0)
								me.now = me.num -1;
							ch = KEY_RIGHT;
							goto EXPRESS;
						}
						//Modified by IAMFAT 2002-05-28
						//Roll Back by IAMFAT 2002-05-29
						//		     prints("[1m[44m[31m[�Ķ�����������]  [33m����Q, �� �� ��һ������ U,���� ��һ������ <Enter>,<Space>,�� [m");
						//Modified by IAMFAT 2002.06.11
						prints("[1;44;31m[�Ķ�����������]  [33m����Q, �� �� ��һ������ U,���� ��һ������ <Enter>,<Space>,�� [m");
						switch (ch = egetch() ) {
							case KEY_DOWN:
							case ' ':
							case '\n':
								if ( ++me.now >= me.num)
									me.now = 0;
								ch = KEY_RIGHT;
								goto EXPRESS;
							case KEY_UP:
							case 'u':
							case 'U':
								if (--me.now < 0)
									me.now = me.num - 1;
								ch = KEY_RIGHT;
								goto EXPRESS;
							case 'h':
								goto EXPRESS;
							default:
								break;
						}
					} else if (dashd(fname)) {
						//add by Danielfree 06.12.6
						char *end = nowpath + strlen(nowpath);
						sprintf(end, "-%d", (me.now) + 1);
						a_menu(me.item[me.now]->title, fname, me.level,
								bmonly);
					}
					me.page = 9999;
				}
				break;
			case '/': {
				int found;
				found=b_menusearch(&me);
				if (found)
					me.now=found;
			}
				break;
#ifdef INTERNET_EMAIL

				case 'F':
				case 'U':
				if (me.now < me.num && HAS_PERM(PERM_MAIL)) {
					a_forward(path, me.item[me.now], ch == 'U');
					me.page = 9999;
				}
				break;
#endif
			case '!':
				if (!Q_Goodbye())
					break; /* youzi leave */
		}
		if (ch >= '0' && ch <= '9') {
			number = number * 10 + (ch - '0');
			ch = '\0';
		} else {
			number = 0;
		}
		if (me.level & PERM_BOARDS)
			a_manager(&me, ch);
	}
	for (ch = 0; ch < me.num; ch++)
		free(me.item[ch]);
	//add by Danielfree 06.12.6 
	char oldpath[STRLEN-17]="\0"; //for safety
	if (strlen(nowpath) >=1) {
		char *tmpchar;
		tmpchar = strrchr(nowpath, '-');//find the last "->"
		strncpy(oldpath, nowpath, tmpchar-nowpath);
	}
	snprintf(nowpath, STRLEN-17, "%s", oldpath);
}

//���ļ���Ϊfname,����Ϊtitle���ļ����뵽��������·��Ϊpath������
int linkto(char *path, char *fname, char *title) {
	MENU pm;
	pm.path = path;

	//strcpy(pm.mtitle, f_title);
	a_loadnames(&pm);
	if (pm.mtitle[0] == '\0')
		strcpy(pm.mtitle, title);
	a_additem(&pm, title, fname);
	a_savenames(&pm);
}

int add_grp(char group[STRLEN], char gname[STRLEN], char bname[STRLEN],
		char title[STRLEN]) {
	FILE *fn;
	char buf[PATHLEN];
	char searchname[STRLEN];
	char gpath[STRLEN * 2]; //�߲�Ŀ¼
	char bpath[STRLEN * 2]; //�Ͳ�Ŀ¼
	sprintf(buf, "0Announce/.Search");
	sprintf(searchname, "%s: groups/%s/%s\n", bname, group, bname);
	sprintf(gpath, "0Announce/groups/%s", group);
	sprintf(bpath, "%s/%s", gpath, bname);
	if (!dashd("0Announce")) {
		mkdir("0Announce", 0755);
		chmod("0Announce", 0755);
		if ((fn = fopen("0Announce/.Names", "w")) == NULL)
			return;
		fprintf(fn, "#\n");
		fprintf(fn, "# Title=%s ������������\n", BoardName);
		fprintf(fn, "#\n");
		fclose(fn);
	}
	if (!seek_in_file(buf, bname))
		file_append(buf, searchname);
	if (!dashd("0Announce/groups")) {
		mkdir("0Announce/groups", 0755);
		chmod("0Announce/groups", 0755);

		if ( (fn = fopen("0Announce/groups/.Names", "w")) == NULL)
			return;
		fprintf(fn, "#\n");
		fprintf(fn, "# Title=����������\n");
		fprintf(fn, "#\n");
		fclose(fn);

		//linkto("0Announce", "����������","groups", "����������");
		linkto("0Announce", "groups", "����������");
	}
	if (!dashd(gpath)) {
		mkdir(gpath, 0755);
		chmod(gpath, 0755);
		sprintf(buf, "%s/.Names", gpath);
		if ( (fn = fopen(buf, "w")) == NULL)
			return;
		fprintf(fn, "#\n");
		fprintf(fn, "# Title=%s\n", gname);
		fprintf(fn, "#\n");
		fclose(fn);
		//linkto("0Announce/groups", gname, group, gname);
		linkto("0Announce/groups", group, gname);
	}
	if (!dashd(bpath)) {
		mkdir(bpath, 0755);
		chmod(bpath, 0755);
		//linkto(gpath, title, bname, title);
		linkto(gpath, bname, title);
		sprintf(buf, "%s/.Names", bpath);
		if ((fn = fopen(buf, "w")) == NULL) {
			return -1;
		}
		fprintf(fn, "#\n");
		fprintf(fn, "# Title=%s\n", title);
		fprintf(fn, "#\n");
		fclose(fn);
	}
	//add by fangu 2003.2.26 ,add log
	//sprintf(genbuf, "ANN add_grp %s", bpath);
	//report(genbuf);
	return 1;

}

int del_grp(char grp[STRLEN], char bname[STRLEN], char title[STRLEN]) {
	char buf[STRLEN], buf2[STRLEN], buf3[30];
	char gpath[STRLEN * 2];
	char bpath[STRLEN * 2];
	char check[30];
	int i, n;
	MENU pm;
	strlcpy(buf3, grp, 29);
	buf3[29] = '\0';
	sprintf(buf, "0Announce/.Search");
	sprintf(gpath, "0Announce/groups/%s", buf3);
	sprintf(bpath, "%s/%s", gpath, bname);
	deltree(bpath);

	pm.path = gpath;
	a_loadnames(&pm);
	for (i = 0; i < pm.num; i++) {
		strcpy(buf2, pm.item[i]->title);
		strcpy(check, strtok(pm.item[i]->fname, "/~\n\b"));
		if (strstr(buf2, title) && !strcmp(check, bname)) {
			free(pm.item[i]);
			(pm.num)--;
			for (n = i; n < pm.num; n++)
				pm.item[n] = pm.item[n + 1];
			a_savenames(&pm);
			break;
		}
	}
	//add by fangu 2003.2.26, add log
	//sprintf(genbuf, "ANN del_grp %s", bpath);
	//report(genbuf);
}

int edit_grp(char bname[STRLEN], char grp[STRLEN], char title[STRLEN],
		char newtitle[STRLEN]) {
	char buf[STRLEN], buf2[STRLEN], buf3[30];
	char gpath[STRLEN * 2];
	char bpath[STRLEN * 2];
	int i;
	MENU pm;
	strlcpy(buf3, grp, 29);
	buf3[29] = '\0';
	sprintf(buf, "0Announce/.Search");
	sprintf(gpath, "0Announce/groups/%s", buf3);
	sprintf(bpath, "%s/%s", gpath, bname);
	if (!seek_in_file(buf, bname))
		return 0;

	pm.path = gpath;
	a_loadnames(&pm);
	for (i = 0; i < pm.num; i++) {
		strlcpy(buf2, pm.item[i]->title, STRLEN);
		buf2[STRLEN - 1] = '\0';
		if (strstr(buf2, title) && strstr(pm.item[i]->fname, bname)) {
			//add by fangu 2003.2.26, add log
			//sprintf(genbuf, "ANN edit_grp file:%s-old title:%s -
			//new title:%s", gpath,pm.item[i]->title,newtitle);
			//report(genbuf);
			strlcpy(pm.item[i]->title, newtitle, STRLEN);
			break;
		}
	}
	a_savenames(&pm);
	pm.path = bpath;
	a_loadnames(&pm);
	strlcpy(pm.mtitle, newtitle, STRLEN);
	a_savenames(&pm);

}

int AddPCorpus() {
	FILE *fn;
	char digestpath[80] = "0Announce/groups/GROUP_0/PersonalCorpus";
	char personalpath[80], Log[200], title[200], ftitle[80];
	char *title1 = title;
	time_t now;
	int id;
	modify_user_mode(DIGEST);
	sprintf(Log, "%s/Log", digestpath);
	if (!check_systempasswd()) {
		return 1;
	}
	clear();
	stand_title("���������ļ�");

	move(4, 0);
	if (!gettheuserid(1, "������ʹ���ߴ���: ", &id))
		return 1;

	sprintf(personalpath, "%s/%c/%s", digestpath,
			toupper(lookupuser.userid[0]), lookupuser.userid);
	if (dashd(personalpath)) {
		presskeyfor("���û��ĸ����ļ�Ŀ¼�Ѵ���, �������ȡ��..", 4);
		return 1;
	}

	move(4, 0);
	if (askyn("ȷ��ҪΪ���û�����һ�������ļ���?", YEA, NA)==NA) {
		presskeyfor("��ѡ��ȡ������. �������ȡ��...", 6);
		return 1;
	}
	mkdir(personalpath, 0755);
	chmod(personalpath, 0755);

	move(7, 0);
	prints("[ֱ�Ӱ� ENTER ��, �����ȱʡΪ: [32m%s �ĸ����ļ�[m]", lookupuser.userid);
	getdata(6, 0, "����������ļ�֮����: ", title, 39, DOECHO, YEA);
	if (title[0] == '\0')
		title1 += sprintf(title, "%s �ĸ����ļ�", lookupuser.userid);
	sprintf(title1, "%-38.38s(BM: %s)", title, lookupuser.userid);
	sprintf(digestpath, "0Announce/groups/GROUP_0/PersonalCorpus/%c", 
			toupper(lookupuser.userid[0]));
	sprintf(ftitle, "�����ļ� ���� (����ĸ %c)", toupper(lookupuser.userid[0]));
	//linkto(digestpath, ftitle, lookupuser.userid, title);
	linkto(digestpath, lookupuser.userid, title);
	sprintf(personalpath, "%s/.Names", personalpath);
	if ((fn = fopen(personalpath, "w")) == NULL) {
		return -1;
	}
	fprintf(fn, "#\n");
	fprintf(fn, "# Title=%s\n", title);
	fprintf(fn, "#\n");
	fclose(fn);
	if (!(lookupuser.userlevel & PERM_BOARDS)) {
		char secu[STRLEN];
		lookupuser.userlevel |= PERM_BOARDS;
		substitut_record(PASSFILE, &lookupuser, sizeof(struct userec), id);
		sprintf(secu, "���������ļ�, ���� %s ����Ȩ��", lookupuser.userid);
		securityreport(secu, 0, 0);
		move( 9, 0);
		outs(secu);
		sethomefile(genbuf, lookupuser.userid, ".announcepath");
		report(genbuf, currentuser.userid);
		if ((fn = fopen(genbuf, "w+")) != NULL) {
			fprintf(fn, "%s/%s", digestpath, lookupuser.userid);
			fclose(fn);
		}
	}
	now = time(NULL);
	sprintf(genbuf, "\033[36m%-12.12s\033[m %14.14s \033[32m %.38s\033[m\n",
			lookupuser.userid, getdatestring(now, DATE_ZH), title);
	file_append(Log, genbuf);
	presskeyfor("�Ѿ����������ļ�, �밴���������...", 12);
	return 0;
}

void Announce() {
	sprintf(genbuf, "%s ������������", BoardName);
	a_menu(genbuf, "0Announce", HAS_PERM(PERM_ANNOUNCE) ? PERM_BOARDS : 0,
			0);
	clear();
}

int a_file_info(MENU *pm) {
	char fname[1024], weblink[1024], tmp[80], type[10];
	struct stat st;
	int i, len;

	/*������*/

	sprintf(fname, "%s/%s", pm->path, pm->item[pm->now]->fname);
	if (dashf(fname) ) {
		snprintf(weblink, 1024, "http://%s/cgi-bin/bbs/bbsanc?path=%s\n",
				BBSHOST, fname + 9);
		strcpy(type, "�ļ�");
	} else if (dashd(fname) ) {
		snprintf(weblink, 1024, "http://%s/cgi-bin/bbs/bbs0an?path=%s\n",
				BBSHOST, fname + 9);
		strcpy(type, "Ŀ¼");
	} else {
		return;
	}
	len = strlen(weblink);
	stat(fname, &st);

	clear();
	move(0, 0);

	prints("������%s��ϸ��Ϣ:\n\n", type);
	prints("��    ��:     �� %d ƪ\n", pm->now);
	prints("��    ��:     %s\n", type);
	strlcpy(tmp, pm->item[pm->now]->title, 38);
	tmp[38] = '\0';
	prints("��    ��:     %s\n", tmp);
	prints("�� �� ��:     %s\n", pm->item[pm->now]->title + 39);
	prints("��    ��:     %s\n", pm->item[pm->now]->fname);
	prints("�༭����:     %s\n", getdatestring(st.st_mtime, DATE_ZH));
	prints("��    С:     %d �ֽ�\n", st.st_size);
	prints("URL ��ַ:\n");
	for (i = 0; i < len; i +=78) {
		strlcpy(tmp, weblink+i, 78);
		tmp[78] = '\n';
		tmp[79] = '\0';
		outs(tmp);
	}

	pressanykey();
}

/* coded by stiger, immigrate to fudan by jacobson 2005.5.21 */

/* Ѱ�Ҷ�ʧ��Ŀ */
/* ����ֵ������˼��� */

int a_repair(MENU *pm) {
	DIR *dirp;
	struct dirent *direntp;
	int i, changed;

	changed=0;
	dirp=opendir(pm->path);
	if (dirp==NULL)
		return 0;

	while ( (direntp=readdir(dirp)) != NULL) {
		if (direntp->d_name[0]=='.')
			continue;
		if (strcasecmp(direntp->d_name, "counter.person")==0)
			continue;
		for (i=0; i < pm->num; i++) {
			if (strcmp(pm->item[i]->fname, direntp->d_name)==0) {
				i=-1;
				break;
			}
		}
		if (i!=-1) {
			a_additem(pm, direntp->d_name, direntp->d_name);
			changed++;
		}
	}
	closedir(dirp);
	if (changed>0) {
		//if(a_savenames(pm) != 0)
		a_savenames(pm);
		//changed = 0 - changed;
		return changed;
	}
}
/* add end */

int seekannpath(char *ann_index, char *buf) {
	FILE* fp;
	char index[IDLEN+1];
	char line[4200];
	sethomefile(genbuf, currentuser.userid, "annpath");
	if (!(fp=fopen(genbuf, "r")))
		return 0;
	while (fgets(line, 4200, fp)) {
		strlcpy(index, line, IDLEN);
		index[IDLEN] = '\0';
		if (strcasecmp(ann_index, index)==0) {
			strcpy(buf, line+13);
			buf[256] = '\0';
			strtok(buf, " \r\n\t");
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
}

struct AnnPath {
	int num;
	char title[STRLEN];
	char path[PATHLEN];
} import_path[MAXANNPATHS];
int curr_annpath;
int ann_mode;
int show_mode = 0;
char show_items[3][8] = { "[����]", "[Ŀ¼]", "[·��]" };

void ann_set_title_show(char* title) {
	char buf[256], path[256];
	move(0, 0);
	rtrim(title);
	sprintf(path, "[%-1.56s] %s", title, show_items[show_mode]);
	sprintf(buf, "[1;44;33m �趨˿· %69.69s[m\n", path);
	outs(buf);
	prints(" �˳�[[1;32m��[m] ����/�滻[[1;32m�� Enter[m] ����[[1;32mt[m] ɾ��[[1;32md[m] ���[[1;32mc[m] �ƶ�[[1;32mm[m] ����[[1;32mg[m] ����[[1;32mh[m]\n");
	prints("[1;44;37m ���  ˿·                                                                    [m\n");
}

void ann_get_title_show() {
	char buf[256];
	move(0, 0);
	sprintf(buf, "[1;44;33m ʹ��˿· %69.69s[m\n", show_items[show_mode]);
	outs(buf);
	prints(" �˳�[[1;32m��[m] �鿴[[1;32m��[m] �ƶ�[[1;32m��[m,[1;32m��[m] �鿴[[1;32mS[m] ģʽ[[1;32mf[m] ��Ѱ[[1;32m/[m] ����[[1;32mh[m]\n");
	prints("[1;44;37m ���  ˿·                                                                    [m\n");
}

int save_import_path() {
	FILE *fp;
	sethomefile(genbuf, currentuser.userid, "annpath");
	if ((fp = fopen(genbuf, "w")) != NULL) {
		fwrite(import_path, sizeof(struct AnnPath), MAXANNPATHS, fp);
		fclose(fp);
	}
	return 1;
}

int load_import_path() {
	FILE *fp;
	int count = 0;
	sethomefile(genbuf, currentuser.userid, "annpath");
	if ((fp = fopen(genbuf, "r")) != NULL) {
		count = fread(import_path, sizeof(struct AnnPath), MAXANNPATHS,	fp);
		fclose(fp);
	}
	if (count != MAXANNPATHS)
		clear_ann_path();
	return 1;
}

int change_ann_path(int index, char* title, char *path, int mode) {
	import_path[index].num = index;
	char anntitle[STRLEN];
	strlcpy(anntitle, title, STRLEN);
	if (index != -1) {
		move(1, 0);
		rtrim(anntitle);
		getdata(1, 0, "�趨˿·��:", anntitle, STRLEN, DOECHO, NA);
		if (anntitle[0]=='\0')
			return;
		strlcpy(import_path[index].title, anntitle, STRLEN);
		if (mode == 0) {
			strlcpy(import_path[index].path, path, PATHLEN);
		}
	}
	return save_import_path();
}

int del_ann_path(int index) {
	import_path[index].num = -1;
	import_path[index].title[0] = '\0';
	import_path[index].path[0] = '\0';
	return save_import_path();
}

int clear_ann_path() {
	int i;
	for (i = 0; i < MAXANNPATHS; i++) {
		import_path[i].num = -1;
		import_path[i].title[0] = '\0';
		import_path[i].path[0] = '\0';
	}
	return save_import_path();
}

//mode == 0 f�趨˿·
//mode == 1 Iѡ��˿·
//return 0	˿·�趨ʧ��
//return 1	˿·�趨�ɹ�
//show_mode == 0	˿·��
//show_mode == 1	˿·������Ŀ¼
//show_mode == 2	˿·����
int set_ann_path(char *title, char *path, int mode) {
	int from = curr_annpath / A_PAGESIZE * A_PAGESIZE, to = 0;
	int ch;
	int redrawflag = 1;
	int y = 3, number=0;
	char buf[1024];
	FILE *fn;

	ann_mode = mode;

	load_import_path();
	show_mode = 0;

	while (1) {
		if (redrawflag) {
			clear();

			if (ann_mode == ANNPATH_SETMODE) {
				strcpy(buf, path);
				ann_to_title(buf);
				ann_set_title_show(buf);
			} else if (ann_mode == ANNPATH_GETMODE)
				ann_get_title_show();

			to=from;
			y=3;
			move(y, 0);
			while (y < t_lines-1 && to < MAXANNPATHS) {
				if (show_mode == 0)
					sprintf(genbuf, "%4d  %-72.72s", to+1,
							import_path[to].num == -1 ? "[32m<��δ�趨>[m"
									: import_path[to].title);
				else if (show_mode == 2)
					sprintf(genbuf, "%4d  %-72.72s", to+1,
							import_path[to].num == -1 ? "[32m<��δ�趨>[m"
									: import_path[to].path);
				else if (show_mode = 1) {
					strlcpy(buf, import_path[to].path, PATHLEN);
					ann_to_title(buf);
					sprintf(genbuf, "%4d  %-72.72s", to+1,
							import_path[to].num == -1 ? "[32m<��δ�趨>[m"
									: buf);
				}

				strtok(genbuf, "\n");
				prints(" %-78s\n", genbuf);
				to++;
				y++;
			}//while
			if (from==to) {
				from-=(A_PAGESIZE-1);
				if (from<0)
					from=0;
				curr_annpath=from;
				continue;
			}
			if (curr_annpath>to-1)
				curr_annpath= to -1;
			update_endline();
		}//if 
		move(3+curr_annpath-from, 0);
		prints(">");
		ch = egetch();
		redrawflag=0;
		move(3+curr_annpath-from, 0);
		prints(" ");

		switch (ch) {
			case 'h':
			case 'H':
				show_help("help/import_announcehelp");
				redrawflag=1;
				break;
			case 'k':
			case 'K':
			case KEY_UP:
				curr_annpath--;
				if (curr_annpath < 0) {
					curr_annpath = MAXANNPATHS-1;
					from += MAXANNPATHS - A_PAGESIZE;
					redrawflag=1;
				} else if (curr_annpath < from) {
					from -= A_PAGESIZE;
					if (from < 0)
						from = 0;
					if (curr_annpath<from)
						curr_annpath=from;
					redrawflag=1;
				}
				break;
			case 'j':
			case 'J':
			case KEY_DOWN:
				curr_annpath++;
				if (curr_annpath > MAXANNPATHS-1) {
					from = 0;
					curr_annpath = 0;
					redrawflag=1;
				} else if (curr_annpath > to - 1) {
					from += A_PAGESIZE;
					if (from > MAXANNPATHS-1)
						from = 0;
					if (curr_annpath<from)
						curr_annpath=from;
					redrawflag=1;
				}
				break;
			case Ctrl('B'):
			case KEY_PGUP:
				from-=A_PAGESIZE;
				curr_annpath-=A_PAGESIZE;
				if (from == -A_PAGESIZE) {
					from+=MAXANNPATHS;
					curr_annpath= (curr_annpath+A_PAGESIZE) % A_PAGESIZE;

				} else if (from<0) {
					from=0;
				}
				if (curr_annpath<from)
					curr_annpath=from;
				redrawflag=1;
				break;
			case Ctrl('F'):
			case KEY_PGDN:
				from+=A_PAGESIZE;
				curr_annpath+=A_PAGESIZE;
				if (from > MAXANNPATHS-1) {
					from = 0;
					curr_annpath %= A_PAGESIZE;
				}
				if (curr_annpath<from)
					curr_annpath=from;
				redrawflag=1;
				break;
			case KEY_HOME:
				from = 0;
				curr_annpath = 0;
				redrawflag=1;
				break;
			case KEY_END:
				from = MAXANNPATHS-A_PAGESIZE;
				curr_annpath = MAXANNPATHS-1;
				redrawflag=1;
				break;
			case KEY_LEFT:
			case KEY_ESC:
				return 0;
			case 'd': //ɾ��
			case 'D': //ɾ��
				move(1, 0);
				redrawflag=1;
				if (askyn("ɾ��˿·��", NA, NA)==NA)
					break;
				del_ann_path(curr_annpath);
				break;
			case 'c': //���
			case 'C': //���
				move(1, 0);
				redrawflag=1;
				if (askyn("�������˿·��?", NA, NA)==NA)
					break;
				clear_ann_path();
				break;
			case 'S':
				sethomefile(genbuf, currentuser.userid, ".announcepath");
				fn = fopen(genbuf, "r");
				if (fn) {
					fgets(buf, 1000, fn);
					fclose(fn);
					ann_to_title(buf);
					sprintf(genbuf, "��ǰ˿·: %s", buf);
				} else
					strcpy(genbuf, "����û���� f ����˿·.");
				presskeyfor(genbuf, t_lines-1);
				redrawflag=1;
				break;
			case 't'://������
			case 'T'://������
				if (import_path[curr_annpath].num == -1)
					break;
				if (ann_mode == ANNPATH_SETMODE) {
					move(1, 0);
					change_ann_path(curr_annpath,
							import_path[curr_annpath].title, NULL, 1);
				}
				redrawflag=1;
				break;
			case 's':
				show_mode = !show_mode;
				redrawflag=1;
				break;
			case 'f'://�޸�ģʽ
			case 'F'://�޸�ģʽ
				show_mode = (show_mode + 1) % 3;
				redrawflag=1;
				break;
			case 'g'://����˿·
			case 'G'://����˿·
				if (import_path[curr_annpath].num == -1)
					break;
				if (ann_mode == ANNPATH_SETMODE) {
					if (dashd(import_path[curr_annpath].path)) {
						a_menu("", import_path[curr_annpath].path,
								PERM_BOARDS, 0);
						return 0;
					} else
						presskeyfor("��˿·�Ѷ�ʧ, �������趨", t_lines-1);

				}
				break;
			case 'm':
			case 'M':
				if (import_path[curr_annpath].num != -1) {
					buf[0] = '\0';
					getdata(1, 0, "Ҫ�ƶ���λ��:", buf, 4, DOECHO, NA);
					if ((buf[0] != 0) && isdigit(buf[0])) {
						int new_pos;
						new_pos = atoi(buf);
						new_pos--;
						if ((new_pos >= 0) && (new_pos < MAXANNPATHS)
								&& (new_pos != curr_annpath)) {
							if (import_path[new_pos].num != -1) {
								strlcpy(buf,
										import_path[curr_annpath].title,
										STRLEN);
								strlcpy(import_path[curr_annpath].title,
										import_path[new_pos].title, STRLEN);
								strlcpy(import_path[new_pos].title, buf,
										STRLEN);

								strlcpy(buf,
										import_path[curr_annpath].path,
										STRLEN);
								strlcpy(import_path[curr_annpath].path,
										import_path[new_pos].path, STRLEN);
								strlcpy(import_path[new_pos].path, buf,
										STRLEN);
							} else {
								strlcpy(import_path[new_pos].title,
										import_path[curr_annpath].title,
										STRLEN);
								strlcpy(import_path[new_pos].path,
										import_path[curr_annpath].path,
										STRLEN);
								import_path[new_pos].num = new_pos;
								import_path[curr_annpath].num = -1;
							}
							save_import_path();
							curr_annpath = new_pos;
						}
					}
					redrawflag=1;
				}
				break;
			case KEY_RIGHT:
			case '\n':
				redrawflag=1;
				if (number > 0) {
					curr_annpath = number - 1;
					from = curr_annpath /A_PAGESIZE * A_PAGESIZE;
					number = 0;

				} else {
					move(1, 0);
					if (ann_mode == ANNPATH_SETMODE) {
						if (import_path[curr_annpath].num == -1) {
							change_ann_path(curr_annpath, title, path, 0);
						} else if (askyn("Ҫ�������е�˿·ô��", NA, NA)== YEA)
							change_ann_path(curr_annpath, title, path, 0);
					} else if (ann_mode == ANNPATH_GETMODE) {
						if (import_path[curr_annpath].num != -1) {
							sprintf(genbuf, "����˿·%d [%-1.50s]��?",
									import_path[curr_annpath].num + 1,
									import_path[curr_annpath].title);
							if (askyn(genbuf, YEA, NA)==YEA) {
								sethomefile(genbuf, currentuser.userid,
										".announcepath");
								if ((fn = fopen(genbuf, "w+")) != NULL) {
									fprintf(fn, "%s",
											import_path[curr_annpath].path);
									fclose(fn);
								}
								return 1;
							}
						}

					}
				}
				break;
			default:
				if (ch >= '0' && ch <= '9') {
					number = number * 10 + (ch - '0');
					ch = '\0';
				} else {
					number = 0;
				}
				break;
		}// switch
	}//while (1)
	return 0;
}

