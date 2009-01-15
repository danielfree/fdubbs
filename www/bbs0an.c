#include "libweb.h"

int main() {
	FILE *fp;
	int count, i, index=0, total=0;
	char *ptr, path[512], names[512], name[1024][80], file[1024][80], buf[512], title[256]=" ";
	char *board;
	struct boardheader *x1;
	init_all();
	
	/* added by roly  2002.01.03 ȥ��cache */
	printf("<meta http-equiv=\"pragma\" content=\"no-cache\">");
	/* add end */
	
	strlcpy(path, getparm("path"), 511);
	if(strstr(path, "..") || strstr(path, "SYSHome")) http_fatal("��Ŀ¼������");
	sprintf(names, "0Announce%s/.Names", path);
	fp=fopen(names, "r");
	board=getbfroma(path);
	if(board[0]) {
		if(!has_read_perm(&currentuser, board)) http_fatal("Ŀ¼������");
		x1=getbcache(board);
		if ((x1->flag & BOARD_CLUB_FLAG)
			&& (x1->flag & BOARD_READ_FLAG )
			&& !has_BM_perm(&currentuser, board)
			&& !isclubmember(currentuser.userid, board))
				http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸ�Ŀ¼", board);
		
	}
	if(fp==0) http_fatal("Ŀ¼������");
	while(1) {
		if(fgets(buf, 511, fp)==0) break;
		if(!strncmp(buf, "# Title=", 8)) strcpy(title, buf+8);
		if(!strncmp(buf, "Name=", 5) && total<1023) {
			strcpy(name[total], trim(buf+5));
			strcpy(file[total], "");
			total++;
		}
		if(!strncmp(buf, "Path=~", 6) && total>0) {
			sprintf(file[total-1], "%s", trim(buf+6));
		}
	}
	if(strstr(title, "BM: SYSOPS") && !(currentuser.userlevel & PERM_SYSOPS)) http_fatal("�����Ŀ¼");
	
	/*  added by roly 2002.01.03*/
	/*  ��Ӷ�Ŀ¼�ķ���Ȩ�޿��ƣ���ֹ����Ŀ¼��ֱ�Ӷ�������*/
	if(strstr(title, "BM: OBOARDS") && !(currentuser.userlevel & PERM_OBOARDS)) http_fatal("�����Ŀ¼");
	//modified by iamfat 2002.10.18 ��֤BMSĿ¼��������ɼ�
	if(strstr(title, "BM: BMS") && !has_BM_perm(&currentuser, board)) http_fatal("�����Ŀ¼");
	//if(strstr(title, "BM: BMS") && !(currentuser.userlevel & PERM_BOARDS)) http_fatal("�����Ŀ¼");
	/* added end   */
	
	buf[0]=0;
	if(board[0]) 
		sprintf(buf, "%s", board);
	{
		char path[256];
		sprintf(path, "%s/info/boards/%s/icon.jpg",BBSHOME,board);
		if(dashf(path))
		{
			printf("<img src='/info/boards/%s/icon.jpg' align=absmiddle>",board);
		}
	}

    printf("<b><font style='font-size: 18pt'>%s</font> �� %s ������ [��Ŀ¼�������: %d]</b>\n", buf, BBSNAME, get_count(path));
	
	printf("<center>\n");
	printf("<table width=100%% height=80%%><tr valign=top><td>");
	printpretable();
	if(total<=0) {
		printf("<br><< Ŀǰû������ >>\n");
		printposttable();
		printf("</center>\n");
		http_quit();
	}
       	printf("<table border=0 width=100%%>\n");
       	printf("<tr class=pt9h ><th nowrap>���<th nowrap>����<th nowrap>������<th nowrap>����");
	int cc=0;
	for(i=0; i<total; i++) {
		char *id;
		/* if(strstr(name[i], "SYSOPS")) continue; */
		//���ϴ�����rolyע�͵�����Ϊվ�����Է��ʸ�Ŀ¼  2002.01.03
		
		index++;
		if(strlen(name[i])<=39) {
			id="";
		} else {
			name[i][38]=0;
			id=name[i]+39;
			if(!strncmp(id, "BM: ", 4)) id+=4;
			ptr=strchr(id, ')');
			if(ptr) ptr[0]=0;
		}
		
		/*  add by roly 2002.01.03
		 *  ����Ŀ¼�������Լ��û�Ȩ���ж��Ƿ���ʾ 
		 */		
		if (!strncmp(id,"SYSOPS",6) && !(currentuser.userlevel & PERM_SYSOPS)) continue;
		//modified by iamfat 2002.10.18 ��֤BMSĿ¼��������ɼ�
		if (!strncmp(id,"BMS",3) && !has_BM_perm(&currentuser, board))continue;
		//if(!strncmp(id,"BMS",3) && !(currentuser.userlevel & PERM_BOARDS)) continue;
		if (!strncmp(id,"OBOARDS",7) && !(currentuser.userlevel & PERM_OBOARDS)) continue;
		/*  add end     */ 
		
		printf("<tr class=%s><td align=right>%d</td>", ((cc++)%2)?"pt9dc":"pt9lc", index);
		sprintf(buf, "0Announce%s%s", path, file[i]);
		if(!file_exist(buf)) {
			printf("<td width=100%%><img src=/images/types/error.gif border=0 align=absmiddle>  %s", nohtml(name[i]));
		} else if(file_isdir(buf)) {
			printf("<td width=100%%><img src=/images/types/folder.gif border=0 align=absmiddle> <a href=bbs0an?path=%s%s>%s</a>", path, file[i], nohtml(name[i]));
		} else {
			printf("<td width=100%%><img src=/images/types/text.gif border=0 align=absmiddle> <a href=bbsanc?path=%s%s>%s</a>", path, file[i], nohtml(name[i]));
		}
		if(id[0]) {
			printf("<td nowrap>%s", userid_str(id));
		} else {
			printf("<td nowrap>");
		}
		printf("<td nowrap>%6.6s %s", Ctime(file_time(buf))+4, Ctime(file_time(buf))+20);
	}
	printf("</table>");
	printposttable();
        sprintf(path,"%s/info/announce/code",BBSHOME, board);
	if(dashf(path))
	{
	     printf("</td><td width=250>");
	     showrawcontent(path);
	}
	printf("</td></tr></table>");
	printf("</center>\n");
	printf("<br>[<a href='javascript:history.go(-1)'><img border=0 src=/images/button/back.gif align=absmiddle>������һҳ</a>] ");
	if(board[0]) printf("[<a href=bbsdoc?board=%s><img border=0 src=/images/button/home.gif align=absmiddle>��������</a>]", board);
	http_quit();
}

int get_count(char *path) {
	FILE *fp;
	char buf[256];
	int counts=0;
	sprintf(buf, "0Announce%s/.counts", path);
	if(!file_exist(buf)) {
		fp=fopen(buf, "w+");
	} else {
		fp=fopen(buf, "r+");
	}
	FLOCK(fileno(fp), LOCK_EX);
	fscanf(fp, "%d", &counts);
	counts++;
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d\n", counts);
	FLOCK(fileno(fp), LOCK_UN);
	fclose(fp);
	return counts;
}
