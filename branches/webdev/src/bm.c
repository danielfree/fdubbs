#include "bbs.h"

//��Ҫ�õ����ⲿ����
extern struct boardheader *getbcache();

//����ҳ���С
#define BBS_PAGESIZE (t_lines-4)

char club_uid[IDLEN+1];

int addtoclub(char *uident, char *msg) {
	char strtosave[512], buf[50];
	int seek;
	if (!strcmp(uident, "guest")) {
		move(t_lines-1, 0);
		prints("��������guest������ֲ�");
		egetch();
		return -1;
	}
	seek = isclubmember(uident, currboard);
	if (seek) {
		move(1, 0);
		prints(" %s �Ѿ��ھ��ֲ������С�", uident);
		egetch();
		return -1;
	}
	getdata(1, 0, "���벹��˵��:", buf, 50, DOECHO, YEA);
	move(1, 0);
	sprintf(strtosave, "����%s������ֲ���?", uident);
	if (askyn(strtosave, YEA, NA)==NA)
		return -1;
	time_t daytime= time(0);
	struct tm* tmtime=localtime(&daytime);
	sprintf(strtosave, "%-12s %-40s %04d.%02d.%02d %-12s", uident, buf,
			1900+tmtime->tm_year, tmtime->tm_mon+1, tmtime->tm_mday,
			currentuser.userid);

	sprintf(msg, "%s:\n\n    �������������ֲ��� %s\n\n����˵����%s\n\n������: %s\n",
			uident, currboard, buf, currentuser.userid);
	setbfile(genbuf, currboard, "club_users");
	bm_log(currentuser.userid, currboard, BMLOG_ADDCLUB, 1);
	return add_to_file(genbuf, strtosave);
}

int delclub(char *uident) {
	char fn[STRLEN];
	setbfile(fn, currboard, "club_users");
	bm_log(currentuser.userid, currboard, BMLOG_DELCLUB, 1);
	return del_from_file(fn, uident);
}

void club_title_show() {
	move(0, 0);
	prints("[1;44;36m �趨���ֲ�������                                                               [m\n");
	prints("�뿪[[1;32m��[m] ѡ��[[1;32m��[m,[1;32m��[m] ���[[1;32ma[m] ɾ��[[1;32md[m] ����[[1;32m/[m]\n");
	prints("[1;44m�û�����               ����˵��                         ��������     ������     [m\n");
}

int club_key_deal(char* fname, int ch, char* line) {
	char msgbuf[4096];
	char repbuf[500];
	if (line) {
		strlcpy(club_uid, line, sizeof(club_uid));
		strtok(club_uid, " \n\r\t");
	}
	switch (ch) {
		case 'a': //����
			move(1, 0);
			usercomplete("���Ӿ��ֲ���Ա: ", club_uid);
			if (*club_uid!='\0' && getuser(club_uid)) {
				if (addtoclub(club_uid, msgbuf)==1) {
					sprintf(repbuf, "%s����%s������ֲ���%s", currentuser.userid,
							club_uid, currboard);
					autoreport(repbuf, msgbuf, YEA, club_uid, 2);
					Poststring(msgbuf, "club", repbuf, 2);
					log_DOTFILE(club_uid, repbuf);
				}
			}
			break;
		case 'd': //ɾ����Ա
			if (!line)
				return 0;
			move(1, 0);
			sprintf(msgbuf, "ɾ�����ֲ���Ա%s��?", club_uid);
			if (askyn(msgbuf, NA, NA)==NA)
				return 1;
			if (delclub(club_uid)) {
				sprintf(repbuf, "%sȡ��%s�ھ��ֲ���%s��Ȩ��", currentuser.userid,
						club_uid, currboard);
				msgbuf[0] = '\0';
				autoreport(repbuf, msgbuf, YEA, club_uid, 2);
				Poststring(msgbuf, "club", repbuf, 2);
				log_DOTFILE(club_uid, repbuf);
			}
			break;
		case Ctrl('A'):
		case KEY_RIGHT: //�û���Ϣ
			if (!line)
				return 0;
			t_query(club_uid);
			break;
	}
	return 1;
}

int club_user() {
	struct boardheader *bp;
	extern struct boardheader *getbcache();
	bp = getbcache(currboard);

	if ((bp->flag & BOARD_CLUB_FLAG) && chkBM(currbp, &currentuser)) {
		setbfile(genbuf, currboard, "club_users");
		list_text(genbuf, club_title_show, club_key_deal, NULL);
		return FULLUPDATE;
	} else
		return DONOTHING;
}

int bm_log(char *id, char *boardname, int type, int value) {
	int fd, data[BMLOGLEN];
	struct flock ldata;
	struct stat buf;
	struct boardheader *btemp;
	char direct[STRLEN], BM[BM_LEN];
	char *ptr;

	btemp = getbcache(boardname);
	if (btemp == NULL)
		return 0;
	strlcpy(BM, btemp->BM, sizeof(BM) - 1);
	BM[sizeof(BM) - 1] = '\0';
	ptr = strtok(BM, ",: ;|&()\0\n");
	while (ptr) {
		if (!strcmp(ptr, currentuser.userid))
			break;
		ptr = strtok(NULL, ",: ;|&()\0\n");
	}
	if (!ptr)
		return 0;
	sprintf(direct, "boards/%s/.bm.%s", boardname, id);
	if ((fd = open(direct, O_RDWR | O_CREAT, 0644)) == -1)
		return 0;
	ldata.l_type = F_RDLCK;
	ldata.l_whence = 0;
	ldata.l_len = 0;
	ldata.l_start = 0;
	if (fcntl(fd, F_SETLKW, &ldata) == -1) {
		close(fd);
		return 0;
	}
	fstat(fd, &buf);
	if (buf.st_size < BMLOGLEN * sizeof(int)) {
		memset(data, 0, sizeof(int) * BMLOGLEN);
	} else {
		read(fd, data, sizeof(int) * BMLOGLEN);
	}
	if (type >= 0 && type < BMLOGLEN)
		data[type] += value;
	lseek(fd, 0, SEEK_SET);
	write(fd, data, sizeof(int) * BMLOGLEN);
	ldata.l_type = F_UNLCK;
	fcntl(fd, F_SETLKW, &ldata);
	close(fd);
	return 0;
}

