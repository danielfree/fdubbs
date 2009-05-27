#include "libweb.h"

static int check_multi(const struct userec *user)
{
	int i, total = 0;
	int uid = searchuser(user->userid);
	for (i = 0; i < MAXACTIVE; i++) {
		if (utmpshm->uinfo[i].active == 0)
			continue;
		if (utmpshm->uinfo[i].uid == uid)
			total++;
	}
	return total;
}

static void add_msg(int notused)
{
	int i;
	FILE *fp;
	char buf[129], file[256], *id=currentuser.userid;
	sprintf(file, "touch home/%c/%s/wwwmsg.flush", toupper(id[0]), id);
	system(file);
	sprintf(file, "home/%c/%s/msgfile", toupper(id[0]), id);
	i=file_size(file)/129;
	if (get_record(file, buf, 129, i) <= 0)
		return;
	sprintf(file, "home/%c/%s/wwwmsg", toupper(id[0]), id);
	fp=fopen(file, "a");
	fwrite(buf, 129, 1, fp);
	fclose(fp);
}

static int wwwlogin(struct userec *user) {
	FILE *fp;
	char buf[STRLEN];
	int n, tmp;
	struct user_info *u;

	if(!(currentuser.userlevel & PERM_REGISTER)) { 
		char file[HOMELEN]; 
		sethomefile(file, currentuser.userid, "register");
		if(dashf(file)) { 
			currentuser.userlevel |=PERM_DEFAULT; 
			save_user_data(&currentuser);
		}
	}

	fp=fopen("tmp/.UTMP.lock", "a");
	FLOCK(fileno(fp), LOCK_EX);
	for(n = 0; n < MAXACTIVE; n++) {
		if(utmpshm->uinfo[n].active == 0) {
			u = &(utmpshm->uinfo[n]);
			u_info = u;
			bzero(u, sizeof(struct user_info));
			u->active = 1;
			u->uid = searchuser(user->userid);
			u->pid = getpid();
			u->mode = WWW;
			if (HAS_PERM(PERM_LOGINCLOAK)
					&& (currentuser.flags[0] & CLOAK_FLAG))
				u->invisible = YEA;
			u->pager = 0;
			if (DEFINE(DEF_FRIENDCALL))
				u->pager |= FRIEND_PAGER;
			if (DEFINE(PAGER_FLAG)) {
				u->pager |= ALL_PAGER;
				u->pager |= FRIEND_PAGER;
			}
			if (DEFINE(DEF_FRIENDMSG))
				u->pager |= FRIENDMSG_PAGER;
			if (DEFINE(DEF_ALLMSG)) {
				u->pager |= ALLMSG_PAGER;
				u->pager |= FRIENDMSG_PAGER;
			}

			SpecialID(u->userid, fromhost, IPLEN);
			strlcpy(u->from, fromhost, 24);//???
#ifdef SPARC
			*(int*)(u->from + 30)=time(NULL);
#else
			*(int*)(u->from + 32)=time(NULL);
#endif
			u->from[22] = DEFINE(DEF_NOTHIDEIP) ? 'S' : 'H';

			u->idle_time = time(NULL);
			strlcpy(u->username, user->username, sizeof(u->username));
			strlcpy(u->userid, user->userid, sizeof(u->userid));

			tmp = rand() % 100000000;
			u->utmpkey = tmp;
			FLOCK(fileno(fp), LOCK_UN);
			fclose(fp);
			http_header();
			sprintf(buf, "%d", n + 1);
			setcookie("utmpnum", buf);
			sprintf(buf, "%d", tmp);
			setcookie("utmpkey", buf);
			setcookie("utmpuserid", currentuser.userid);
			set_my_cookie();
			refreshto(3, FIRST_PAGE);
			printf("</head>\n<body>��¼�ɹ���3���Ӻ��Զ�ת��"
				"<a href='"FIRST_PAGE"'>web��ҳ</a>\n</body>\n</html>\n");

			uidshm->status[u->uid - 1]++;
			return 0;
		}
	}
	FLOCK(fileno(fp), LOCK_UN);
	fclose(fp);
	http_fatal2(HTTP_STATUS_SERVICE_UNAVAILABLE, "��Ǹ��Ŀǰ�����û����Ѵ����ޣ��޷���¼�����Ժ�������");
	return 0;
}

int bbslogin_main(void)
{
	char fname[STRLEN];
	char buf[256], id[IDLEN + 1], pw[PASSLEN];
	struct userec user;

	parse_post_data();
	strlcpy(id, getparm("id"), sizeof(id));
	strlcpy(pw, getparm("pw"), sizeof(pw));
	if(loginok && strcasecmp(id, currentuser.userid)) {
		http_fatal("ϵͳ��⵽Ŀǰ���ļ�������Ѿ���¼��һ���ʺţ�"
				"�����˳�(ѡ��ע����¼�����߹ر��������������)��");
	}
	if (getuserec(id, &user) == 0)
		http_fatal("����֤���޴� ID��");

	user.numlogins++;
	if (strcasecmp(id, "guest")) {
		int total;
		time_t stay, recent, now, t;
		if (!checkpasswd(user.passwd, pw)) {
			sprintf(buf, "%-12.12s %s @%s\n",
					user.userid, cn_Ctime(time(NULL)), fromhost);
			sethomefile(fname, user.userid, "logins.bad"); 
			file_append(fname, buf);
			file_append("logins.bad", buf);
			http_fatal("�����������...");
		}

		total = check_multi(&user);
		if (!HAS_PERM2(PERM_SYSOPS, &user) && total >= 2)
			http_fatal("���Ѿ���¼��2�����ڡ�"
					"Ϊ�˱�֤�������棬�˴����߽���ȡ����");

		if (!HAS_PERM2(PERM_LOGIN, &user))
			http_fatal("���ʺ���ͣ�����뵽Notice���ѯԭ��");

		now = time(NULL);
		// Do not count frequent logins.
		if (now - user.lastlogin < 20 * 60
				&& user.numlogins >= 100)
				user.numlogins--;
		if (total > 1) {
			recent = user.lastlogout;
			if (user.lastlogin > recent)
				recent = user.lastlogin;
			stay = now - recent;
			if (stay < 0)
				stay = 0;
		} else {
			stay = 0;
		}
		t = user.lastlogin;
		user.lastlogin = now;
		user.stay += stay;
#ifdef CHECK_FREQUENTLOGIN
		if (!HAS_PERM(PERM_SYSOPS)
				&& abs(t - time(NULL)) < 10) {
			report("Too Frequent", user.userid);
			http_fatal("��¼����Ƶ�������Ժ�������");
		}
#endif
		strlcpy(user.lasthost, fromhost, sizeof(user.lasthost));
		save_user_data(&user);
		currentuser = user;
	}

	log_usies("ENTER", fromhost, &user);
	if(!loginok && strcasecmp(id, "guest"))
		wwwlogin(&user);
	return 0;
}
