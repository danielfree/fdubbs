#include "bbs.h"

#define BBS_PAGESIZE    (t_lines - 4)

extern time_t login_start_time;

typedef struct {
	char *name, *title, *BM;
	unsigned int flag;
	int pos, total;
	bool unread, zap;
	char status;
} board_data_t;

typedef struct {
	board_data_t *brds;
	int *zapbuf;
	char *prefix;
	int num;
	bool yank;
	int parent;
	bool mode;
	bool newflag;
} choose_board_t;

struct goodboard GoodBrd;

//int inGoodBrds (char *bname)
int inGoodBrds(int pos) //modified by cometcaptor 2007-04-21 �򻯲�������
{
	int i;
	for (i = 0; i < GoodBrd.num && i < GOOD_BRC_NUM; i++)
		//if (!strcmp (bname, GoodBrd.ID[i]))
		if ((GoodBrd.boards[i].pid == GoodBrd.nowpid)
				&&(!(GoodBrd.boards[i].flag & BOARD_CUSTOM_FLAG))&&(pos
				== GoodBrd.boards[i].pos)) //modified by cometcaptor
			return i + 1;
	return 0;
}

/**
 *
 */
static int load_zapbuf(choose_board_t *cbrd)
{
	if (cbrd->zapbuf == NULL) {
		cbrd->zapbuf = malloc(sizeof(*cbrd->zapbuf) * MAXBOARD);
		if (cbrd->zapbuf == NULL)
			return -1;
	} else {
		return 0;
	}

	char file[HOMELEN];
	sethomefile(file, currentuser.userid, ".lastread");
	int fd = open(file, O_RDONLY, 0600);
	if (fd < 0) {
		int n;
		for (n = 0; n < MAXBOARD; n++) {
			cbrd->zapbuf[n] = 1;
		}
	} else {
		read(fd, cbrd->zapbuf, sizeof(int) * numboards);
		close(fd);
	}
	return 0;
}

/**
 *
 */
int save_zapbuf(const choose_board_t *cbrd)
{
	char file[HOMELEN];
	sethomefile(file, currentuser.userid, ".lastread");
	int fd = open(file, O_WRONLY | O_CREAT, 0600);
	if (fd >= 0) {
		write(fd, cbrd->zapbuf, sizeof(int) * numboards);
		close(fd);
		return 0;
	}
	return -1;
}

void load_GoodBrd() {
	int i;
	char fname[STRLEN];
	FILE *fp;

	GoodBrd.num = 0;
	setuserfile(fname, ".goodbrd");
	//modified by cometcaptor 2007-04-23 �ղؼ��Զ���Ŀ¼
	if ((fp = fopen(fname, "rb"))) {
		while (fread(&GoodBrd.boards[GoodBrd.num],
				sizeof(struct goodbrdheader), 1, fp)) {
			if (GoodBrd.boards[GoodBrd.num].flag & BOARD_CUSTOM_FLAG)
				GoodBrd.num++;
			else {
				i = GoodBrd.boards[GoodBrd.num].pos;
				if ((bcache[i].filename[0]) && (bcache[i].flag
						& BOARD_POST_FLAG //p���ư���
						|| HAS_PERM(bcache[i].level) //Ȩ���㹻
				||(bcache[i].flag & BOARD_NOZAP_FLAG))) //����zap
					GoodBrd.num++;
			}
			if (GoodBrd.num == GOOD_BRC_NUM)
				break;
		}
		fclose(fp);
	}

	if (GoodBrd.num == 0) {
		GoodBrd.num = 1;
		i = getbnum(DEFAULTBOARD, &currentuser);
		if (i == 0)
			i = getbnum(currboard, &currentuser);
		GoodBrd.boards[0].id = 1;
		GoodBrd.boards[0].pid = 0;
		GoodBrd.boards[0].pos = i-1;
		GoodBrd.boards[0].flag = bcache[i-1].flag;
		strcpy(GoodBrd.boards[0].filename, bcache[i-1].filename);
		strcpy(GoodBrd.boards[0].title, bcache[i-1].title);
	}
}

void save_GoodBrd() {
	int i;
	FILE *fp;
	char fname[STRLEN];

	//modified by cometcaptor 2007-04-21
	if (GoodBrd.num == 0) {
		GoodBrd.num = 1;
		i = getbnum(DEFAULTBOARD, &currentuser);
		if (i == 0)
			i = getbnum(currboard, &currentuser);
		GoodBrd.boards[0].id = 1;
		GoodBrd.boards[0].pid = 0;
		GoodBrd.boards[0].pos = i-1;
		GoodBrd.boards[0].flag = bcache[i-1].flag;
		strcpy(GoodBrd.boards[0].filename, bcache[i-1].filename);
		strcpy(GoodBrd.boards[0].title, bcache[i-1].title);
	}
	setuserfile(fname, ".goodbrd");
	if ((fp = fopen(fname, "wb")) != NULL) {
		for (i = 0; i < GoodBrd.num; i++) {
			fwrite(&GoodBrd.boards[i], sizeof(struct goodbrdheader), 1, fp);
			report(GoodBrd.boards[i].filename, currentuser.userid);
		}
		fclose(fp);
	}
}

void add_GoodBrd(char *bname, int pid) //cometcaptor 2007-04-21
{
	int i = getbnum(bname, &currentuser);
	if ((i > 0)&&(GoodBrd.num < GOOD_BRC_NUM)) {
		i--;
		GoodBrd.boards[GoodBrd.num].pid = pid;
		GoodBrd.boards[GoodBrd.num].pos = i;
		strcpy(GoodBrd.boards[GoodBrd.num].filename, bcache[i].filename);
		strcpy(GoodBrd.boards[GoodBrd.num].title, bcache[i].title);
		GoodBrd.boards[GoodBrd.num].flag = bcache[i].flag;
		//���ǲ�Ҫ����ô���ӵ�ѭ���ҿ��ˣ�û��Ҫ
		if (GoodBrd.num)
			GoodBrd.boards[GoodBrd.num].id
					= GoodBrd.boards[GoodBrd.num-1].id + 1;
		else
			GoodBrd.boards[GoodBrd.num].id = 1;
		GoodBrd.num++;
		save_GoodBrd();
	}
}

void mkdir_GoodBrd(char *dirname, char *dirtitle, int pid)//cometcaptor 2007-04-21
{
	//pid��ʱ�Ǹ���ʹ�õĲ�������Ϊ�����㽨����Ŀ¼��ɾ��Ŀ¼�Ĵ���Ŀ¼�Բ����ƣ�
	if (GoodBrd.num < GOOD_BRC_NUM) {
		GoodBrd.boards[GoodBrd.num].pid = 0;
		strcpy(GoodBrd.boards[GoodBrd.num].filename, dirname);
		strcpy(GoodBrd.boards[GoodBrd.num].title, "~[�ղ�] �� ");
		if (dirtitle[0] != '\0')
			strcpy(GoodBrd.boards[GoodBrd.num].title+11, dirtitle);
		else
			strcpy(GoodBrd.boards[GoodBrd.num].title+11, "�Զ���Ŀ¼");
		GoodBrd.boards[GoodBrd.num].flag = BOARD_DIR_FLAG
				| BOARD_CUSTOM_FLAG;
		GoodBrd.boards[GoodBrd.num].pos = -1;
		if (GoodBrd.num)
			GoodBrd.boards[GoodBrd.num].id
					= GoodBrd.boards[GoodBrd.num-1].id + 1;
		else
			GoodBrd.boards[GoodBrd.num].id = 1;
		GoodBrd.num++;
		save_GoodBrd();
	}
}

void rmdir_GoodBrd(int id)//cometcaptor 2007-04-21
{
	//Ŀ¼û�жԶ���Ŀ¼Ƕ��ɾ���Ĺ��ܣ�Ҳ��Ϊ������ƣ��ղؼ�Ŀ¼������������Ŀ¼
	int i, n = 0;
	for (i = 0; i < GoodBrd.num; i++) {
		if (((GoodBrd.boards[i].flag & BOARD_CUSTOM_FLAG)
				&&(GoodBrd.boards[i].id == id)) ||(GoodBrd.boards[i].pid
				== id))
			continue;
		else {
			if (i != n)
				memcpy(&GoodBrd.boards[n], &GoodBrd.boards[i],
						sizeof(struct goodbrdheader));
			n++;
		}
	}
	GoodBrd.num = n;
	save_GoodBrd();
}

/**
 *
 */
static int load_boards(choose_board_t *cbrd)
{
	struct boardheader *bptr;
	board_data_t *ptr;
	int addto = 0;
	bool goodbrd = false;

	if (load_zapbuf(cbrd) != 0)
		return -1;
	cbrd->num = 0;

	if (GoodBrd.nowpid >= 0) {
		load_GoodBrd();
		goodbrd = true;
	}

	int n;
	for (n = 0; n < numboards; n++) {
		bptr = bcache + n;
		if (!(bptr->filename[0]))
			continue;
		if (!goodbrd) {
			if (!(bptr->flag & BOARD_POST_FLAG) && !HAS_PERM(bptr->level)
					&& !(bptr->flag & BOARD_NOZAP_FLAG))
				continue;
			if ((bptr->flag & BOARD_CLUB_FLAG)&& (bptr->flag
					& BOARD_READ_FLAG )&& !chkBM(bptr, &currentuser)
					&& !isclubmember(currentuser.userid, bptr->filename))
				continue;
			if (cbrd->mode == 0) {
				if (cbrd->parent > 0 && cbrd->parent != bptr->group - 1)
					continue;
				if (cbrd->parent == 0 && bptr->group != 0)
					continue;
				if (cbrd->parent > 0 && bptr->title[0] == '*')
					continue;
			} else {
				if (cbrd->prefix != NULL
						&& !strchr(cbrd->prefix, bptr->title[0])
						&& cbrd->prefix[0] != '*')
					continue;
				if (cbrd->prefix != NULL && cbrd->prefix[0] == '*') {
					if (!strstr(bptr->title, "��")
							&& !strstr(bptr->title, "��")
							&& bptr->title[0] != '*')
						continue;
				}
				if (cbrd->prefix == NULL && bptr->title[0] == '*')
					continue;
			}
			addto = cbrd->yank || cbrd->zapbuf[n] != 0
					|| (bptr->flag & BOARD_NOZAP_FLAG);
		} else {
			addto = inGoodBrds(n);
		}

		if (addto) {
			ptr = cbrd->brds + cbrd->num++;
			ptr->name = bptr->filename;
			ptr->title = bptr->title;
			ptr->BM = bptr->BM;
			ptr->flag = bptr->flag;
			ptr->pos = n;
			ptr->total = -1;
			ptr->zap = (cbrd->zapbuf[n] == 0);
			if (bptr ->flag & BOARD_DIR_FLAG) {
				if (bptr->level != 0)
					ptr->status = 'r';
				else
					ptr->status = ' ';
			} else {
				if (bptr->flag & BOARD_NOZAP_FLAG)
					ptr->status = 'z';
				else if (bptr->flag & BOARD_POST_FLAG)
					ptr->status = 'p';
				else if (bptr->flag & BOARD_NOREPLY_FLAG)
					ptr->status = 'x';
				else if (bptr->level != 0)
					ptr->status = 'r';
				else
					ptr->status = ' ';
			}
		}
	}

	//added by cometcaptor 2007-04-21 ��ȡ�Զ���Ŀ¼
	if (goodbrd) {
		for (n = 0; n<GoodBrd.num && n<GOOD_BRC_NUM; n++) {
			if ((GoodBrd.boards[n].flag & BOARD_CUSTOM_FLAG)
					&&(GoodBrd.boards[n].pid == GoodBrd.nowpid)) {
				ptr = cbrd->brds + cbrd->num++;
				ptr->name = GoodBrd.boards[n].filename;
				ptr->title = GoodBrd.boards[n].title;
				ptr->BM = NULL;
				ptr->flag = GoodBrd.boards[n].flag;
				ptr->pos = GoodBrd.boards[n].id;
				ptr->zap = 0;
				ptr->total = 0;
				ptr->status = ' ';
			}
		}
	}
	//add end

	if (cbrd->num == 0 && !cbrd->yank && cbrd->parent == -1) {
		cbrd->num = -1;
		cbrd->yank = true;
		return -1;
	}
	return 0;
}

static int search_board(const choose_board_t *cbrd, int *num)
{
	static int i = 0, find = YEA;
	static char bname[STRLEN];
	int n, ch, tmpn = NA;

	if (find == YEA) {
		bzero(bname, sizeof (bname));
		find = NA;
		i = 0;
	}

	while (1) {
		move(t_lines - 1, 0);
		clrtoeol();
		prints("������Ҫ���ҵİ������ƣ�%s", bname);
		ch = egetch();

		if (isprint2(ch)) {
			bname[i++] = ch;
			for (n = 0; n < cbrd->num; n++) {
				if (!strncasecmp(cbrd->brds[n].name, bname, i)) {
					tmpn = YEA;
					*num = n;
					if (!strcmp(cbrd->brds[n].name, bname))
						return 1 /* �ҵ����Ƶİ棬�����ػ�
						 */;
				}
			}
			if (tmpn)
				return 1;
			if (find == NA) {
				bname[--i] = '\0';
			}
			continue;
		} else if (ch == Ctrl('H') || ch == KEY_LEFT || ch == KEY_DEL
				|| ch == '\177') {
			i--;
			if (i < 0) {
				find = YEA;
				break;
			} else {
				bname[i] = '\0';
				continue;
			}
		} else if (ch == '\t') {
			find = YEA;
			break;
		} else if (ch == '\n' || ch == '\r' || ch == KEY_RIGHT) {
			find = YEA;
			break;
		}
		bell(1);
	}
	if (find) {
		move(t_lines - 1, 0);
		clrtoeol();
		return 2 /* ������ */;
	}
	return 1;
}

static bool check_newpost(board_data_t *ptr)
{
	ptr->unread = false;
	ptr->total = (brdshm->bstatus[ptr->pos]).total;
	if (!brc_initial(currentuser.userid, ptr->name)) {
		ptr->unread = true;
	} else {
		if (brc_unread1((brdshm->bstatus[ptr->pos]).lastpost)) {
			ptr->unread = true;
		}
	}
	return ptr->unread;
}

int unread_position(char *dirfile, board_data_t *ptr)
{
	struct fileheader fh;
	char filename[STRLEN];
	int fd, offset, step, num;
	num = ptr->total + 1;
	if (ptr->unread && (fd = open (dirfile, O_RDWR))> 0) {
		if (!brc_initial (currentuser.userid, ptr->name)) {
			num = 1;
		} else {
			offset = (int) ((char *) &(fh.filename[0]) - (char *) &(fh));
			num = ptr->total - 1;
			step = 4;
			while (num> 0) {
				lseek (fd, (off_t) (offset + num * sizeof (fh)), SEEK_SET);
				if (read (fd, filename, STRLEN) <= 0 || !brc_unread (filename))
				break;
				num -= step;
				if (step < 32)
				step += step / 2;
			}
			if (num < 0)
			num = 0;
			while (num < ptr->total) {
				lseek (fd, (off_t) (offset + num * sizeof (fh)), SEEK_SET);
				if (read (fd, filename, STRLEN) <= 0 || brc_unread (filename))
				break;
				num++;
			}
		}
		close (fd);
	}
	if (num < 0)
	num = 0;
	return num;
}

static void show_brdlist(choose_board_t *cbrd, int page, bool clsflag)
{
	board_data_t *ptr;
	char tmpBM[BM_LEN - 1];

	char cate[7], title[STRLEN];

	const char *sort = "����";
	char flag = currentuser.flags[0];
	if (flag & BRDSORT_FLAG) {
		sort = "��ĸ";
	} else if (flag & BRDSORT_ONLINE) {
		sort = "����";
	} else if (flag & BRDSORT_UDEF) {
		sort = "�Զ�";
	} else if (flag & BRDSORT_UPDATE) {
		sort = "����";
	}
	char buf[32];
	snprintf(buf, sizeof(buf), "[�������б�] [%s]", sort);

	if (clsflag) {
		clear();
		docmdtitle(buf, " \033[m��ѡ��[\033[1;32m��\033[m,\033[1;32me\033[m] "
				"�Ķ�[\033[1;32m��\033[m,\033[1;32mRtn\033[m] ѡ��[\033[1;32m"
				"��\033[m,\033[1;32m��\033[m] �г�[\033[1;32my\033[m] ����"
				"[\033[1;32ms\033[m] ��Ѱ[\033[1;32m/\033[m] �л�[\033[1;32mc"
				"\033[m] ����[\033[1;32mh\033[m]\n");
		prints("\033[1;44;37m %s ����������        V  ���  %-20s S ��  ��"
				"        ���� \033[m\n",
				cbrd->newflag ? "ȫ ��  δ" : "�� ��  δ", "��  ��  ��  ��");
	}

	move (3, 0);
	int n;
	for (n = page; n < page + BBS_PAGESIZE; n++) {
		if (n >= cbrd->num) {
			prints ("\n");
			continue;
		}
		ptr = cbrd->brds + n;
		if (ptr->total == -1) {
			refresh();
			check_newpost(ptr);
		}

		if (!cbrd->newflag)
			prints(" %5d", n + 1);
		else if (ptr->flag & BOARD_DIR_FLAG)
			prints("  Ŀ¼");
		else
			prints(" %5d", ptr->total);

		if (ptr->flag & BOARD_DIR_FLAG)
			prints("  ��");
		else
			prints("  %s", ptr->unread ? "��" : "��");

		if (!(ptr->flag & BOARD_CUSTOM_FLAG))
			strcpy(tmpBM, ptr->BM);

		strlcpy(cate, ptr->title + 1, sizeof(cate));
		strlcpy(title, ptr->title + 11, sizeof(title));
		ellipsis(title, 20);

		prints("%c%-17s %s%s%6s %-20s %c ",
				(ptr->zap && !(ptr->flag & BOARD_NOZAP_FLAG)) ? '*' : ' ',
				ptr->name,
				(ptr->flag & BOARD_VOTE_FLAG) ? "\033[1;31mV\033[m" : " ",
				(ptr->flag & BOARD_CLUB_FLAG) ? (ptr->flag & BOARD_READ_FLAG)
				? "\033[1;31mc\033[m" : "\033[1;33mc\033[m" : " ",
				cate, title, HAS_PERM (PERM_POST) ? ptr->status : ' ');

		if (ptr->flag & BOARD_DIR_FLAG)
			prints("[Ŀ¼]\n");
		else
			prints("%-12s %4d\n",
					ptr->BM[0] <= ' ' ? "����������" : strtok(tmpBM, " "),
					brdshm->bstatus[ptr->pos].inboard);
	}
	refresh();
}

static int cmpboard(const void *b1, const void *b2)
{
	const board_data_t *brd = b1;
	const board_data_t *tmp = b2;

	if (currentuser.flags[0] & BRDSORT_FLAG) {
		return strcasecmp(brd->name, tmp->name);
	} else if (currentuser.flags[0] & BRDSORT_ONLINE) {
		return brdshm->bstatus[tmp->pos].inboard - brdshm->bstatus[brd->pos].inboard;
	}

	int type = brd->title[0] - tmp->title[0];
	if (type == 0)
		type = strncasecmp(brd->title + 1, tmp->title + 1, 6);
	if (type == 0)
		type = strcasecmp(brd->name, tmp->name);
	return type;
}

static int show_board_info(const char *board)
{
	int i;
	struct boardheader *bp;
	struct bstat *bs;
	char secu[40];
	bp = getbcache(board);
	bs = getbstat(board);
	clear();
	prints("������ϸ��Ϣ:\n\n");
	prints("number  :     %d\n", getbnum(bp->filename, &currentuser));
	prints("Ӣ������:     %s\n", bp->filename);
	prints("��������:     %s\n", (HAS_PERM(PERM_SPECIAL0)) ? bp->title
			: (bp->title + 11));
	prints("��    ��:     %s\n", bp->BM);
	prints("����������:   %s\n", bp->group ? bcache[bp->group - 1].filename
			: "��");
	prints("�Ƿ�Ŀ¼:     %s\n", (bp->flag & BOARD_DIR_FLAG) ? "Ŀ¼" : "����");
	prints("���� ZAP:     %s\n", (bp->flag & BOARD_NOZAP_FLAG) ? "������"
			: "����");

	if (!(bp->flag & BOARD_DIR_FLAG)) {
		prints("��������:     %d ��\n", bs->inboard);
		prints("�� �� ��:     %s\n", (bp->flag & BOARD_JUNK_FLAG) ? "������"
				: "����");
		prints("���Իظ�:     %s\n", (bp->flag & BOARD_NOREPLY_FLAG) ? "������"
				: "����");
		//prints ("���� ZAP:     %s\n",
		//	    (bp->flag & BOARD_NOZAP_FLAG) ? "������" : "����");
		prints("�� �� ��:     %s\n", (bp->flag & BOARD_ANONY_FLAG) ? "��"
				: "��");
#ifdef ENABLE_PREFIX
		prints ("ǿ��ǰ׺:     %s\n",
				(bp->flag & BOARD_PREFIX_FLAG) ? "����" : "����");
#endif
		prints("�� �� ��:     %s\n", (bp->flag & BOARD_CLUB_FLAG) ? (bp-> flag
				& BOARD_READ_FLAG) ? "�����ƾ��ֲ�" : "��ͨ���ֲ�" : "�Ǿ��ֲ�");
		prints("now id  :     %d\n", bs->nowid);
		prints("��д����:     %s\n", (bp->flag & BOARD_POST_FLAG) ? "���Ʒ���"
				: (bp->level ==0) ? "û������" : "�����Ķ�");
	}
	if (HAS_PERM(PERM_SPECIAL0) && bp->level != 0) {
		prints("Ȩ    ��:     ");
		strcpy(secu, "ltmprbBOCAMURS#@XLEast0123456789");
		for (i = 0; i < 32; i++) {
			if (!(bp->level & (1 << i)))
				secu[i] = '-';
			else {
				prints("%s\n              ", permstrings[i]);
			}

		}
		prints("\nȨ �� λ:     %s\n", secu);
	}

	prints("URL ��ַ:     http://"BBSHOST"/bbs/doc?bid=%d\n",
			bp - bcache + 1);
	pressanykey();
	return FULLUPDATE;

}

static int choose_board(choose_board_t *cbrd);

/**
 *
 */
static void read_board(choose_board_t *cbrd, int pos)
{
	board_data_t *ptr = cbrd->brds + pos;
	if (ptr->flag & BOARD_DIR_FLAG) {
		int tmpgrp, tmpmode;
		int oldpid;
		tmpgrp = cbrd->parent;
		tmpmode = cbrd->mode;
		cbrd->mode = 0;
		cbrd->parent = getbnum(ptr->name, &currentuser) - 1;
		oldpid = GoodBrd.nowpid;
		if (ptr->flag & BOARD_CUSTOM_FLAG)
			GoodBrd.nowpid = ptr->pos;
		else
			GoodBrd.nowpid = -1;
		choose_board(cbrd);
		GoodBrd.nowpid = oldpid;
		cbrd->parent = tmpgrp;
		cbrd->mode = tmpmode;
		cbrd->num = -1;
	} else {
		brc_initial(currentuser.userid, ptr->name);
		changeboard(&currbp, currboard, ptr->name);
		memcpy(currBM, ptr->BM, BM_LEN - 1);

		char buf[STRLEN];
		if (DEFINE(DEF_FIRSTNEW)) {
			setbdir(buf, currboard);
			int tmp = unread_position(buf, ptr);
			int page = tmp - t_lines / 2;
			getkeep(buf, page > 1 ? page : 1, tmp + 1);
		}
		Read();
		brc_zapbuf(cbrd->zapbuf + ptr->pos);
		ptr->total = -1;
		currBM[0] = '\0';
	}
}

/**
 *
 */
static int choose_board(choose_board_t *cbrd)
{
	static int num;
	board_data_t *ptr;
	int page, ch, tmp, number, tmpnum;
	int loop_mode = 0;
	char ans[2];
	static char addname[STRLEN-8];

	cbrd->brds = malloc(sizeof(board_data_t) * MAXBOARD);
	if (cbrd->brds == NULL)
		return -1;
	if (!strcmp(currentuser.userid, "guest"))
		cbrd->yank = true;

	modify_user_mode(cbrd->newflag ? READNEW : READBRD);

	cbrd->num = number = 0;

	clear();
	while (1) {
		digestmode = NA;
		if (cbrd->num <= 0) {
			if (load_boards(cbrd) == -1)
				continue;
			qsort(cbrd->brds, cbrd->num, sizeof(cbrd->brds[0]), cmpboard);
			page = -1;
			if (cbrd->num < 0)
				break;
		}

		if (num < 0)
			num = 0;
		if (num >= cbrd->num)
			num = cbrd->num - 1;

		if (page < 0) {
			if (cbrd->newflag) {
				tmp = num;
				while (num < cbrd->num) {
					ptr = cbrd->brds + num;
					if (!(ptr->flag & BOARD_DIR_FLAG)) {
						if (ptr->total == -1)
							check_newpost(ptr);
						if (ptr->unread)
							break;
					}
					num++;
				}
				if (num >= cbrd->num)
					num = tmp;
			}
			page = (num / BBS_PAGESIZE) * BBS_PAGESIZE;
			show_brdlist(cbrd, page, true);
			update_endline();
		}

		if (num < page || num >= page + BBS_PAGESIZE) {
			page = (num / BBS_PAGESIZE) * BBS_PAGESIZE;
			show_brdlist(cbrd, page, false);
			update_endline();
		}

		move(3 + num - page, 0);
		prints(">", number);
		if (loop_mode == 0) {
			ch = egetch();
		}
		move(3 + num - page, 0);
		prints(" ");
		if (ch == 'q' || ch == 'e' || ch == KEY_LEFT || ch == EOF)
			break;
		switch (ch) {
			case '*':
				if (cbrd->brds[num].flag & BOARD_CUSTOM_FLAG)
					break;
				ptr = cbrd->brds + num;
				show_board_info(ptr->name);
				page = -1;
				break;
			case 'b':
			case Ctrl('B'):
			case KEY_PGUP:
				if (num == 0)
					num = cbrd->num - 1;
				else
					num -= BBS_PAGESIZE;
				break;
			case 'C':
				if (!HAS_PERM(PERM_LOGIN))
					break;
				if ((GoodBrd.num == 0) && (GoodBrd.nowpid == -1))
					break;
				if (cbrd->brds[num].flag & BOARD_CUSTOM_FLAG)
					break;
				strlcpy(addname, cbrd->brds[num].name, sizeof(addname));
				presskeyfor("�����Ѹ��� �밴Pճ��", t_lines - 1);
				cbrd->num = -1;
				break;
			case 'c':
				cbrd->newflag = !cbrd->newflag;
				show_brdlist(cbrd, page, true);
				break;
			case 'L':
				m_read();
				page = -1;
				break;
			case 'M':
				m_new();
				page = -1;
				break;
			case 'u':
				modify_user_mode(QUERY);
				t_query();
				page = -1;
				break;
			case 'H':
				getdata(t_lines - 1, 0, "��ѡ��? (1) ����ʮ��  (2) ϵͳ�ȵ� [1]",
						ans, 2, DOECHO, YEA);
				if (ans[0] == '2')
					show_help("etc/hotspot");
				else
					show_help("0Announce/bbslist/day");
				page = -1;
				break;
			case 'l':
				msg_more();
				page = -1;
				break;
			case 'N':
			case ' ':
			case Ctrl('F'):
			case KEY_PGDN:
				if (num == cbrd->num - 1)
					num = 0;
				else
					num += BBS_PAGESIZE;
				break;
			case 'P':
				if (!HAS_PERM(PERM_LOGIN))
					break;
				if ((GoodBrd.num==0)&&(GoodBrd.nowpid == -1))
					break;
				add_GoodBrd(addname, GoodBrd.nowpid);
				addname[0]='\0';
				cbrd->num = -1;
				break;
			case 'p':
			case 'k':
			case KEY_UP:
				if (num-- <= 0)
					num = cbrd->num - 1;
				break;
			case 'n':
			case 'j':
			case KEY_DOWN:
				if (++num >= cbrd->num)
					num = 0;
				break;
			case '$':
				num = cbrd->num - 1;
				break;
			case '!':
				save_zapbuf(cbrd);
				free(cbrd->brds);
				return Goodbye();
				break;
			case 'h':
				show_help("help/boardreadhelp");
				page = -1;
				break;
			case '/':
				move(3 + num - page, 0);
				prints(">", number);
				tmpnum = num;
				tmp = search_board(cbrd, &num);
				move(3 + tmpnum - page, 0);
				prints(" ", number);
				if (tmp == 1)
					loop_mode = 1;
				else {
					loop_mode = 0;
					update_endline();
				}
				break;
			case 'i': //sort by online num
				currentuser.flags[0] ^= BRDSORT_ONLINE;
				qsort(cbrd->brds, cbrd->num, sizeof(*cbrd->brds), cmpboard);
				page = -1;
				substitut_record(PASSFILE, &currentuser,
						sizeof(currentuser), usernum);
				break;
			case 's': /* sort/unsort -mfchen */
				if (currentuser.flags[0] & BRDSORT_FLAG) {
					currentuser.flags[0] ^= BRDSORT_FLAG;
					currentuser.flags[0] |= BRDSORT_ONLINE;
				} else if (currentuser.flags[0] & BRDSORT_ONLINE) {
					currentuser.flags[0] ^= BRDSORT_ONLINE;
				} else {
					currentuser.flags[0] ^= BRDSORT_FLAG;
				}
				qsort(cbrd->brds, cbrd->num, sizeof(*cbrd->brds), cmpboard);
				substitut_record(PASSFILE, &currentuser,
						sizeof(currentuser), usernum);
				page = -1;
				break;
			case 'y':
				if (GoodBrd.num)
					break;
				cbrd->yank = !cbrd->yank;
				cbrd->num = -1;
				break;
			case 'z':
				if (GoodBrd.num)
					break;
				if (HAS_PERM(PERM_LOGIN)
						&& !(cbrd->brds[num].flag & BOARD_NOZAP_FLAG)) {
					ptr = cbrd->brds + num;
					ptr->zap = !ptr->zap;
					ptr->total = -1;
					cbrd->zapbuf[ptr->pos] = (ptr->zap ? 0 : login_start_time);
					page = 999;
				}
				break;
			case 'a':
				if (!HAS_PERM(PERM_LOGIN))
					break;
				if ((GoodBrd.num) && (GoodBrd.nowpid == -1))
					break; //added by cometcaptor 2007-04-24 ��ֹ�ڷ��Զ���Ŀ¼�Ӱ���
				if (GoodBrd.num >= GOOD_BRC_NUM) {
					presskeyfor("�������Ű����Ѿ�������", t_lines - 1);
					//�ղؼ�
				} else if (GoodBrd.num) {
					int pos;
					char bname[STRLEN];
					struct boardheader fh;
					if (gettheboardname(1, "������������ (���հ׼��Զ���Ѱ): ",
							&pos, &fh, bname, 1)) {
						if (!inGoodBrds(getbnum(bname, &currentuser)-1)) {
							add_GoodBrd(bname, GoodBrd.nowpid); //modified by cometcaptor 2007-04-21 
							cbrd->num = -1;
							break;
						}
					}
					page = -1;
				} else {
					load_GoodBrd();
					if (GoodBrd.num >= GOOD_BRC_NUM) {
						presskeyfor("�������Ű����Ѿ�������", t_lines - 1);
					} else if (!inGoodBrds(getbnum(cbrd->brds[num].name, &currentuser)-1)) {
						sprintf(genbuf, "��ȷ��Ҫ���%s���ղؼ���?", cbrd->brds[num].name);
						if (askyn(genbuf, NA, YEA) == YEA) {
							add_GoodBrd(cbrd->brds[num].name, 0); //modified by cometcaptor 2007-04-21
						}
						cbrd->num = -1;
					}
					GoodBrd.num = 0;
				}
				break;
			case 'A':
				//added by cometcaptor 2007-04-22 ����д����Ǵ����Զ���Ŀ¼�Ĵ���
				if (!HAS_PERM(PERM_LOGIN))
					break;
				if (GoodBrd.nowpid == 0) {
					if (GoodBrd.num >= GOOD_BRC_NUM)
						presskeyfor("�������Ű����Ѿ�������", t_lines - 1);
					else {
						//Ҫ������Ŀ¼��
						char dirname[STRLEN];
						char dirtitle[STRLEN];
						dirname[0] = '\0'; //�����һ�δ�����Ŀ¼��
						getdata(t_lines - 1, 0, "�����Զ���Ŀ¼: ", dirname, 17,
								DOECHO, NA); //���17����Ϊֻ��ʾ16���ַ���Ϊ�˷�ֹ������Ŀ¼������ʾ�Ĳ�ͬ���޶�
						if (dirname[0] != '\0') {
							strcpy(dirtitle, "�Զ���Ŀ¼");
							getdata(t_lines - 1, 0, "�Զ���Ŀ¼����: ", dirtitle,
									21, DOECHO, NA);
							mkdir_GoodBrd(dirname, dirtitle, 0);
						}
						cbrd->num = -1;
					}
				}
				break;
			case 'T':
				//added by cometcaptor 2007-04-25 �޸��Զ���Ŀ¼��
				if (!HAS_PERM(PERM_LOGIN))
					break;
				if ((GoodBrd.nowpid == 0)&& cbrd->num
						&& (cbrd->brds[num].flag & BOARD_CUSTOM_FLAG)) {
					char dirname[STRLEN];
					char dirtitle[STRLEN];
					int gbid = 0;
					for (gbid = 0; gbid < GoodBrd.num; gbid++)
						if (GoodBrd.boards[gbid].id == cbrd->brds[num].pos)
							break;
					if (gbid == GoodBrd.num)
						break;
					strcpy(dirname, GoodBrd.boards[gbid].filename);
					getdata(t_lines - 1, 0, "�޸��Զ���Ŀ¼��: ", dirname, 17,
							DOECHO, NA);
					if (dirname[0] != '\0') {
						strcpy(dirtitle, GoodBrd.boards[gbid].title+11);
						getdata(t_lines - 1, 0, "�Զ���Ŀ¼����: ", dirtitle, 21,
								DOECHO, NA);
						if (dirtitle[0] == '\0')
							strcpy(dirtitle, GoodBrd.boards[gbid].title+11);
						strcpy(GoodBrd.boards[gbid].filename, dirname);
						strcpy(GoodBrd.boards[gbid].title+11, dirtitle);
						save_GoodBrd();
					}
					cbrd->num = -1;
				}
				break;
			case 'd':
				if ((GoodBrd.num)&&(cbrd->num > 0)) {
					int i, pos;
					char ans[5];
					sprintf(genbuf, "Ҫ�� %s ���ղؼ���ȥ����[y/N]",
							cbrd->brds[num].name);
					getdata(t_lines - 1, 0, genbuf, ans, 2, DOECHO, YEA);
					if (ans[0] == 'y' || ans[0] == 'Y') {
						if (cbrd->brds[num].flag & BOARD_CUSTOM_FLAG)
							rmdir_GoodBrd(cbrd->brds[num].pos);
						else {
							pos = inGoodBrds(cbrd->brds[num].pos);
							if (pos) {
								for (i = pos-1; i < GoodBrd.num-1; i++)
									memcpy(&GoodBrd.boards[i],
											&GoodBrd.boards[i+1],
											sizeof(struct goodbrdheader));
								GoodBrd.num--;
							}
							save_GoodBrd();
						}
						cbrd->num = -1;
					} else {
						page = -1;
						break;
					}
				}
				break;
			case KEY_HOME:
				num = 0;
				break;
			case KEY_END:
				num = cbrd->num - 1;
				break;
			case '\n':
			case '\r':
				if (number > 0) {
					num = number - 1;
					break;
				}
				/* fall through */
			case KEY_RIGHT:
				tmp = num;
				num = 0;
				if (cbrd->num > 0)
					read_board(cbrd, tmp);
				num = tmp;
				page = -1;
				break;
			case 'S':
				if (!HAS_PERM(PERM_TALK))
					break;
				s_msg();
				page = -1;
				break;
			case 'o':
				if (!HAS_PERM(PERM_LOGIN))
					break;
				t_friends();
				page = -1;
				break;
			default:
				break;
		}
		modify_user_mode(cbrd->newflag ? READNEW : READBRD);
		if (ch >= '0' && ch <= '9') {
			number = number * 10 + (ch - '0');
			ch = '\0';
		} else {
			number = 0;
		}
	}
	clear();
	save_zapbuf(cbrd);
	free(cbrd->brds);
	return 0;
}

void EGroup(const char *cmd)
{
	char buf[16];
	snprintf(buf, sizeof(buf), "EGROUP%c", *cmd);

	choose_board_t cbrd;
	memset(&cbrd, 0, sizeof(cbrd));
	cbrd.mode = true;
	cbrd.prefix = sysconf_str(buf);
	cbrd.newflag = DEFINE(DEF_NEWPOST);
	GoodBrd.num = 0;
	GoodBrd.nowpid = -1;

	choose_board(&cbrd);
}

void BoardGroup(void)
{
	choose_board_t cbrd;
	memset(&cbrd, 0, sizeof(cbrd));
	cbrd.newflag = DEFINE(DEF_NEWPOST);
	GoodBrd.num = 0;
	GoodBrd.nowpid = -1;

	choose_board(&cbrd);
}

void Boards(void)
{
	choose_board_t cbrd;
	memset(&cbrd, 0, sizeof(cbrd));
	cbrd.parent = -1;
	GoodBrd.num = 0;
	GoodBrd.nowpid = -1;

	choose_board(&cbrd);
}

void GoodBrds(void)
{
	if (!strcmp(currentuser.userid, "guest"))
		return;

	choose_board_t cbrd;
	memset(&cbrd, 0, sizeof(cbrd));
	cbrd.parent = -2;
	cbrd.newflag = true;
	GoodBrd.nowpid = 0;

	choose_board(&cbrd);

	GoodBrd.nowpid = -1;
	GoodBrd.num = 0;
}

void New(void)
{
	choose_board_t cbrd;
	memset(&cbrd, 0, sizeof(cbrd));
	cbrd.parent = -1;
	cbrd.newflag = true;
	GoodBrd.num = 0;
	GoodBrd.nowpid = -1;

	choose_board(&cbrd);
}

