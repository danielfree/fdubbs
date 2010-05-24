#include <bbs.h>
#include <glossary.h>

#define MAX_PERF (1000)

//	������ֵת������ʾ�ַ�,�� [---      ]��,[]����������
//  ��ʾ���ַ���2000Ϊ��λ,�ֱ���-=+*#A
//	��ʾ�ĸ����ǽ�����2000�����ĸ�����200Ϊ��λ�ٴλ���,���10��
char *cexpstr(int exp) {
	static char ce[11];
	char* c= "-=+*#A";
	int i;
	int j;
	strcpy(ce, "          ");
	if (exp < 0)
		return ce;
	i = exp / 2000;
	i = i > 5 ? 5 : i;
	j = (exp - i * 2000) / 200;
	j = j > 9 ? 9 : j;
	memset(ce, c[i], j + 1);
	return ce;
}

#ifdef ALLOWGAME
char *cnummedals(int num)
{
	if(num <= 0)
		return GLY_MEDAL0;
	if(num <= 300)
		return GLY_MEDAL1;
	if(num <= 1000)
		return GLY_MEDAL2;
	if(num <= 3000)
		return GLY_MEDAL3;
	return GLY_MEDAL4;
}

char *cmoney(int num)
{
	if(num <= 100)
		return GLY_MONEY0;
	if(num <= 3000)
		return GLY_MONEY1;
	if(num <= 10000)
		return GLY_MONEY2;
	if(num <= 50000)
		return GLY_MONEY3;
	if(num <= 150000)
		return GLY_MONEY4;
	if(num <= 300000)
		return GLY_MONEY5;
	if(num <= 500000)
		return GLY_MONEY6;
	return GLY_MONEY7;
}
#endif

//	���ݱ���ֵperf�������ֵ��ʾ��ʽ
char *cperf(int perf)
{
	if (perf <= 100)
		return GLY_CPERF0;
	if (perf <= 200)
		return GLY_CPERF1;
	if (perf <= 400)
		return GLY_CPERF2;
	if (perf <= 500)
		return GLY_CPERF3;
	if (perf <= 550)
		return GLY_CPERF4;
	if (perf <= 600)
		return GLY_CPERF5;
	if (perf <= 650)
		return GLY_CPERF6;
	if (perf <= 700)
		return GLY_CPERF7;
	if (perf <= 800)
		return GLY_CPERF8;
	if (perf <= 900)
		return GLY_CPERF9;
	if (perf < 1000)
		return GLY_CPERFA;
	return GLY_CPERFB;
}

//���㾭��ֵ,������+��½����1/5+��ע�������+ͣ����ʱ��
//	���ֵ��12000
int countexp(const struct userec *udata)
{
	int exp;
	if (!strcmp(udata->userid, "guest"))
		return 0;
	exp = udata->numposts + udata->numlogins / 5 + (time(0) 
		- udata->firstlogin) / 86400 + udata->stay / 3600;
	if (exp > 12000)
		exp = 12000;
	return exp > 0 ? exp : 0;
}

//	���� ����ֵ
int countperf(const struct userec *udata) {
	int perf;
	int reg_days;
	if (!strcmp(udata->userid, "guest"))
		return 0;
	reg_days = (time(0) - udata->firstlogin) / 86400 + 1;
	perf = (reg_days / 4 > 250 ? 250 : reg_days / 4);
	perf += (udata->stay / 14400 > 250 ? 250 : (udata->stay / 14400));
	perf += (udata->stay / (36 * reg_days) > 500 ? 500 : (udata->stay / (36
			*reg_days)));
	perf = (perf > MAX_PERF) ? MAX_PERF : perf;
	perf = (perf < 0) ? 0 : perf;

	return perf;
}

// Calculates Julian Date (after Oct. 1582) at noon UTC of given date.
int julian_day(int year, int month, int day)
{
	int a = (14 - month) / 12;
	int y = year + 4800 - a;
	int m = month + 12 * a - 3;
	return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

// Calculates differences between given date (after Oct. 1582) and now.
int days_elapsed(int year, int month, int day, time_t now)
{
	//Epoch 1970.1.1 0:0:0 UTC = 2440587.5 (Julian Day)
	return (now - 43200) / 86400 + 2440588 - julian_day(year, month, day);
}

const char *horoscope(char month, char day)
{
	static const char *name[12] = {
		"Ħ����", "ˮƿ��", "˫����", "ĵ����", "��ţ��", "˫����",
		"��з��", "ʨ����", "��Ů��", "�����", "��Ы��", "������" };
	switch (month) {
		case 1:
			if (day < 21)
				return (name[0]);
			else
				return (name[1]);
		case 2:
			if (day < 19)
				return (name[1]);
			else
				return (name[2]);
		case 3:
			if (day < 21)
				return (name[2]);
			else
				return (name[3]);
		case 4:
			if (day < 21)
				return (name[3]);
			else
				return (name[4]);
		case 5:
			if (day < 21)
				return (name[4]);
			else
				return (name[5]);
		case 6:
			if (day < 22)
				return (name[5]);
			else
				return (name[6]);
		case 7:
			if (day < 23)
				return (name[6]);
			else
				return (name[7]);
		case 8:
			if (day < 23)
				return (name[7]);
			else
				return (name[8]);
		case 9:
			if (day < 23)
				return (name[8]);
			else
				return (name[9]);
		case 10:
			if (day < 24)
				return (name[9]);
			else
				return (name[10]);
		case 11:
			if (day < 23)
				return (name[10]);
			else
				return (name[11]);
		case 12:
			if (day < 22)
				return (name[11]);
			else
				return (name[0]);
	}
	return ("������");
}

/**
 * Compute user's hp.
 * @param urec User record.
 * @return user's hp.
 */
int compute_user_value(const struct userec *urec)
{
	int value, value2;
	time_t now = time(NULL);
	value = (now - urec->lastlogin);
	value2 = (now - urec->firstlogin);
	// new user should register in 30 mins
	if (strcmp(urec->userid, "new") == 0) {
		return 30 * 60 - value;
	}
#ifdef FDQUAN
	if ((urec->userlevel & PERM_XEMPT)
			|| strcmp(urec->userid, "SYSOP") == 0
			|| strcmp(urec->userid, "guest") == 0)
		return 999;
	if (!(urec->userlevel & PERM_REGISTER))
		return 14 - value / (24 * 60 * 60);
	if (value2 >= 5 * 365 * 24 * 60 * 60)
		return 666 - value / (24 * 60 * 60);
	if (value2 >= 2 * 365 * 24 * 60 * 60)
		return 365 - value / (24 * 60 * 60);
	return 150 - value / (24 * 60 * 60);
#else
	if (((urec->userlevel & PERM_XEMPT) 
			&& (urec->userlevel & PERM_LONGLIFE))
			|| strcmp(urec->userid, "SYSOP") == 0
			|| strcmp(urec->userid, "guest") == 0)
		return 999;
	if ((urec->userlevel & PERM_XEMPT) 
			&& !(urec->userlevel & PERM_LONGLIFE))
		return 666;
	if (!(urec->userlevel & PERM_REGISTER))
		return 14 - value / (24 * 60 * 60);
	if (urec->userlevel & PERM_SPECIAL1)
		return 527 - value / (24 * 60 * 60);
	if (!(urec->userlevel & PERM_XEMPT)
			&& (urec->userlevel	& PERM_LONGLIFE))
		return 365 - value / (24 * 60 * 60);
	if (value2 >= 3 * 365 * 24 * 60 * 60)
		return 180 - value / (24 * 60 * 60);
	return 120 - value / (24 * 60 * 60);
#endif
}

static int show_volunteer(char *userid, char **buf, size_t *size)
{
	char file[HOMELEN], tmp[STRLEN];
	sethomefile(file, userid, ".volunteer");
	FILE *fp = fopen(file, "r");
	if (fp) {
		strappend(buf, size, "[\033[1;33m");
		while (fgets(tmp, sizeof(tmp), fp) != NULL) {
			tmp[strlen(tmp)] = '\0';
			strappend(buf, size, tmp);
		}
		strappend(buf, size, "\033[m]");
		fclose(fp);
		return 1;
	}
	return 0;
}

static int show_bm(char *userid, char **buf, size_t *size)
{
	char file[HOMELEN], tmp[STRLEN];
	sethomefile(file, userid, ".bmfile");
	FILE *fp = fopen(file, "r");
	if (fp) {
		strappend(buf, size, "[\033[1;33m");
		while (fgets(tmp, sizeof(tmp), fp) != NULL) {
			tmp[strlen(tmp) - 1] = ' ';
			strappend(buf, size, tmp);
		}
		strappend(buf, size, "\033[32m�����\033[m]");
		fclose(fp);
		return 1;
	}
	return 0;
}

void show_position(const struct userec *user, char *buf, size_t size)
{
	if (user->userlevel & PERM_SPECIAL9) {
		if (user->userlevel & PERM_SYSOPS) {
			strappend(&buf, &size, "[\033[1;32mվ��\033[m]");
		} else if (user->userlevel & PERM_ANNOUNCE) {
			strappend(&buf, &size, "[\033[1;32mվ��\033[m]");
		} else if (user->userlevel & PERM_OCHAT) {
			strappend(&buf, &size, "[\033[1;32mʵϰվ��\033[m]");
		} else if (user->userlevel & PERM_SPECIAL0) {
			strappend(&buf, &size, "[\033[1;32mվ��ίԱ������\033[m]");
		} else {
			strappend(&buf, &size, "[\033[1;32m����վ��\033[m]");
		}
	} else {
		int normal = 1;
		if ((user->userlevel & PERM_XEMPT)
				&& (user->userlevel & PERM_LONGLIFE)
				&& (user->userlevel & PERM_LARGEMAIL)) {
			strappend(&buf, &size, "[\033[1;32m��������\033[m]");
			normal = 0;
		}
		if (show_volunteer(user->userid, &buf, &size))
			normal = 0;
		if ((user->userlevel & PERM_BOARDS)
				&& show_bm(user->userid, &buf, &size)) {
			normal = 0;
		}
		if (user->userlevel & PERM_ARBI) {
			strappend(&buf, &size, "[\033[1;32m�ٲ���\033[m]");
			normal = 0;
		}
		if (user->userlevel & PERM_SERV) {
			strappend(&buf, &size, "[\033[1;32m��ѵ��\033[m]");
			normal = 0;
		}
		if (user->userlevel & PERM_SPECIAL3) {
			strappend(&buf, &size, "[\033[1;32m������\033[m]");
			normal = 0;
		}
		if (user->userlevel & PERM_TECH) {
			strappend(&buf, &size, "[\033[1;32m������\033[m]");
			normal = 0;
		}
		if (normal) {
#ifndef FDQUAN
			snprintf(buf, size, "[\033[1;32m�⻪����\033[m]");
#else
			snprintf(buf, size, "[\033[1;32mȪվ����\033[m]");
#endif
		}
	}
}
