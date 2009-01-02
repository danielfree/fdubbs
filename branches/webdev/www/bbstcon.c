#include "libweb.h"

int no_re=0;
/*	bbscon?board=xx&file=xx&start=xx 	*/

int main() 
{
	FILE *fp;
	char buf[512], board[80], dir[80], file[80], filename[80];
	struct fileheader x;
    struct boardheader *x1;//added by polygon
	int i, num=0, found=0;
	init_all();
	strsncpy(board, getparm("board"), 32);
	strsncpy(file, getparm("file"), 32);
    /*
     *  �����������������ӵİ�����֤����bbsdoc.c�е���֤��������,added by polygon
     * */
    x1=getbcache(board);
         if ((x1->flag & BOARD_CLUB_FLAG)
                          && (x1->flag & BOARD_READ_FLAG )
                                       && !has_BM_perm(&currentuser, board)
                                                        && !isclubmember(currentuser.userid, board))
                                        http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸð���", board);
	/*
     * end polygon
     * */
    printf("<center>\n");
	if(!has_read_perm(&currentuser, board)) 
	{
		printf("<b>ͬ���������Ķ� �� %s </b></center><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������");
	}
	strcpy(board, getbcache(board)->filename);
	if(loginok)
		brc_initial(currentuser.userid, board);
	printf("<a name=pagetop ></a>");
	printf("<b>ͬ���������Ķ� �� %s[������: %s]</center>", BBSNAME, board);
	if(strncmp(file, "M.", 2) && strncmp(file, "G.", 2)) 
	{
		printpretable_lite();
		http_fatal("����Ĳ���1");
	}
	if(strstr(file, "..") || strstr(file, "/")) 
	{
		printpretable_lite();
		http_fatal("����Ĳ���2");
	}
	sprintf(dir, "boards/%s/.DIR", board);
	if(!strcmp(board, "noticeboard")) 
		no_re=1;
	fp=fopen(dir, "r+");
	if(fp==0) 
	{
		printpretable_lite();
		http_fatal("Ŀ¼����");
	}
	while(1) 
	{
		if(fread(&x, sizeof(x), 1, fp)<=0) 
			break;
		//added by iamfat 2002.08.10
		//check_anonymous(x.owner);
		//added end.
		num++;
		if(!strcmp(x.filename, file)) 
		{
			int gid = x.gid;
			#ifdef SPARC
				(*(int*)(x.title+72))++;//modified by roly from 73 to 72 for sparc solaris
			#else
				(*(int*)(x.title+73))++;//modified by roly from 73 to 72 for sparc solaris
			#endif
			fseek(fp, -1*sizeof(x), SEEK_CUR);
			//fwrite(&x, sizeof(x), 1, fp);
			found=1;
			while(1) 
			{
				if(fread(&x, sizeof(x), 1, fp)<=0) 
					break;
				num++;
				if(x.gid == gid)
					show_file(board, &x, num-1);
			}
		}
	}
	fclose(fp);
	if(found==0) 
	{
		printpretable_lite();
		http_fatal("������ļ���");
	}
   	//if(!no_re) printf("[<a href='bbspst?board=%s&file=%s&userid=%s&title=%s'>������</a>] ",
	//	board, file, x.owner, title);
	printf("<center>\n");
	printf("[<a href='javascript:history.go(-1)'>������һҳ</a>]");
	printf("[<a href=bbsdoc?board=%s>��������</a>]", board);
   	printf("</center>\n"); 
	if(loginok) 
		brc_update(currentuser.userid, board);
	http_quit();
}

int show_file(char *board, struct fileheader *x, int n) 
{
	FILE *fp;
	char path[80], buf[512],*ptr;
	#ifdef CERTIFYMODE
		if(x->accessed[1]&FILE_UNCERTIFIED)
		{
			printpretable_lite();
			http_fatal("������δͨ������");
		}
	#endif
	if(loginok) 
		brc_addlist(x->filename);
	sprintf(path, "boards/%s/%s", board, x->filename);
	fp=fopen(path, "r");
	if(fp==0) 
	{
		printpretable_lite();
		return;
	}
	ptr=x->title;
	if(!strncmp(ptr, "Re: ", 4)) 
		ptr+=4;
	ptr[60]=0;
	printf("   <a href=bbscon?b=%s&f=%s&n=%d><img border=0 src=/images/button/content.gif align=absmiddle>��ƪȫ��</a>", board, x->filename, n);
	printf("   <a href='bbspst?board=%s&file=%s&userid=%s&id=%d&gid=%d&title=Re: %s'><img border=0 src=/images/button/edit.gif align=absmiddle>�ظ�����</a>  ", board, x->filename, x->owner, x->id, x->gid, entity_char(ptr));	
	//printf("   <a href='bbspst?board=%s&file=%s&title=%s&userid=%s'><img border=0 src=/images/button/edit.gif align=absmiddle>�ظ�����</a>  ", board, x->filename, x->title, x->owner);
	printf("[��ƪ����: %s]  ", userid_str(x->owner));
	#ifdef SPARC	
		printf("[��ƪ����: %d] ", *(int*)(x->title+72));//modified by roly
	#else
		printf("[��ƪ����: %d] ", *(int*)(x->title+73));//modified by roly
	#endif
	printf("<a href=#pagetop ><img border=0 src=/images/button/up.gif align=absmiddle>��ҳ��</a>  \n");
	printpretable();
	printf("<table width=100%%><pre class=ansi>\n");
	while(1) 
	{
		if(fgets(buf, 500, fp)==0) 
			break;
	//The following lines are commented by polygon
    //	if(!strncmp(buf, ": ", 2)) 
	//		continue;
	//	if(!strncmp(buf, "�� �� ", 4)) 
	//		continue;
        //polygon<<
        if(!strncmp(buf, ": ", 2))
            printf("<font color=808080>");
		hhprintf("%s", buf);
        if(!strncmp(buf, ": ", 2))
            printf("</font>");
	    //polygon>>
    }
	fclose(fp);
	printf("</pre>\n</table>\n");
	printposttable();
}
