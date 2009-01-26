#include "libweb.h"

int main() {
	FILE *fp;
	struct boardheader *brd;
	struct fileheader f;
	struct userec *u;
	char buf[80], dir[80], path[80], board[80], file[80], *id;
	int num=0;
	init_all();
	printf("<b>ɾ������ �� %s </b><br>\n",BBSNAME);
	printpretable_lite();
	if(!loginok) http_fatal("���ȵ�¼");
	id=currentuser.userid;
	strlcpy(board, getparm("board"), 60);
	strlcpy(file, getparm("file"), 32);
	brd=getbcache(board);
	if(strncmp(file, "M.", 2) && strncmp(file, "G.", 2)) http_fatal("����Ĳ���");
	if(strstr(file, "..")) http_fatal("����Ĳ���");
	if(brd==0) http_fatal("�������");
	if(!has_post_perm(&currentuser, board)) http_fatal("�����������");
	#ifdef USE_SHMFS
		sprintf(dir, "%s/boards/%s/.DIR", SHM_HOMEDIR, board);
		sprintf(path, "%s/boards/%s/%s", SHM_HOMEDIR, board, file);
	#else
		sprintf(dir, "boards/%s/.DIR", board);
		sprintf(path, "boards/%s/%s", board, file);
	#endif
	fp=fopen(dir, "r");
	if(fp==0) http_fatal("����Ĳ���");
	while(1) {
		if(fread(&f, sizeof(struct fileheader), 1, fp)<=0) break;
		//added by iamfat 2002.08.10
		//check_anonymous(f.owner);
		//added end.
		if(!strcmp(f.filename, file)) {
        		if(strcasecmp(id, f.owner) && !has_BM_perm(&currentuser, board))
                		http_fatal("����Ȩɾ������");
			del_record(dir, sizeof(struct fileheader), num);
//			sprintf(buf, "\n�� %s �� %s ɾ���E[FROM: %s]", currentuser.userid, Ctime(time(0))+4, fromhost);
//modified by iamfat 2002.08.01
//			sprintf(buf, "\n�� %s �� %16.16s ɾ���E[FROM: %s]", currentuser.userid, cn_Ctime(time(0))+6, fromhost);
//			f_append(path, buf);
		#ifdef USE_SHMFS
			sprintf(dir, strcmp(id, f.owner)?"%s/boards/%s/.TRASH":"%s/boards/%s/.JUNK", SHM_HOMEDIR, board);
		#else
			sprintf(dir, strcmp(id, f.owner)?"boards/%s/.TRASH":"boards/%s/.JUNK", board);
		#endif
			strcpy(f.szEraser, id);
			f.timeDeleted=time(0);
			append_record(dir, &f, sizeof(f));
			updatelastpost(board);
			printf("ɾ���ɹ�.<br><a href='bbsdoc?board=%s'>���ر�������</a>", board);
			u=getuser(f.owner);
			if(!junkboard(board) && u) {
				if(u->numposts>0) u->numposts--;
				save_user_data(u);
			}
			//sprintf(buf, "%-12s %s bbsdel %s\n", id, Ctime(time(0))+4, board);
			//modified by iamfat 2002.08.01
			sprintf(buf, "deleted[www] '%s' on %s\n", f.title, board);
			//do_report("trace", buf);
			trace(buf);	//modify by iamfat to use syslog 2004.01.07
			http_quit();
		}
		num++;
	}
	fclose(fp);
	printf("�ļ�������, ɾ��ʧ��.<br>\n");
	printf("<a href='bbsdoc?board=%s'>���ر�������</a>", board);
	http_quit();
}
