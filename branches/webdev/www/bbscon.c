#include "libweb.h"

int inboard(char *board, char* dirname , char* file){
	char dir[256];
	int total;
	struct fileheader x;
	FILE *fp;
	sprintf(dir, "boards/%s/%s", board, dirname);
	total=file_size(dir)/sizeof(x);
	if(total)
	{
		fp=fopen(dir, "r+");
		while(fread (&x, sizeof (x), 1, fp)&& strncmp(x.filename, file, 19));
		if (feof (fp) && strncmp (x.filename, file, 19) ){
			fclose(fp);
			return 0;
		}
		fclose(fp);
	}
	return 1;
					
}

int main() {
	FILE *fp;
	char buf[512], board[80], dir[80], file[80], filename[80], *ptr;
	struct fileheader x;
	int num, tmp, total, in = 0;
	init_all();
	strsncpy(board, getparm("b"), 32);
	strsncpy(file, getparm("f"), 32);
    /*
     * �����������������ӵİ�����֤����bbsdoc.c�е���֤��������,added by polygon
     */
	struct boardheader *x1;
        x1=getbcache(board);
             if ((x1->flag & BOARD_CLUB_FLAG)
                         && (x1->flag & BOARD_READ_FLAG )
                             && !has_BM_perm(&currentuser, board)
                                 && !isclubmember(currentuser.userid, board))
                        http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸð���", board);
    /*
     * ended --polygon
     * */
    num=atoi(getparm("n"));
	printf("<center>\n");
	if(!has_read_perm(&currentuser, board))
	{
		printf("<b>�����Ķ� �� %s </b></center><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������");
	}
	strcpy(board, getbcache(board)->filename);
	printf("<b>�����Ķ� �� %s [������: %s]</b></center><br>\n", BBSNAME, board);
	if(strncmp(file, "M.", 2) && strncmp(file, "G.", 2)&&strncmp(file,"T.",2) )
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
	total=file_size(dir)/sizeof(x);
	if(total<=0) 
	{
		printpretable_lite();
		http_fatal("�������������ڻ���Ϊ��");
	}
	in = inboard(board, ".DIR", file);
	in += inboard(board, ".NOTICE", file);
	if (in == 0){
		printpretable_lite();
		http_fatal("���������");
	}
	#ifdef CERTIFYMODE
		fp=fopen(dir, "r+");
		if(fp==0) 
		{
			printpretable_lite();
			http_fatal("dir error2");
		}
		fseek(fp, sizeof(x)*num, SEEK_SET);
	    fread(&x, sizeof(x), 1, fp);
		fclose(fp);
		if(x.accessed[1]&FILE_UNCERTIFIED)
		{
			printpretable_lite();
			http_fatal("������δͨ������");
		}
	#endif
	printpretable();
	printf("<table width=100%% border=0 cellspacing=3>\n");
	sprintf(filename, "boards/%s/%s", board, file);
	if(!showcontent(filename))
	{
		printf("���Ĳ����ڻ����ѱ�ɾ��");
		printf("</pre>\n</table>\n");
		printposttable();
		printf("<br><center><a href=/cgi-bin/bbs/bbsdoc?board=%s><img border=0 src=/images/button/home.gif align=absmiddle> ��������</a></center>", board);
		http_quit();
	}
	printf("</table>\n");  //pre
	printposttable();
	printf("<center>\n");
	printf("<a href=/cgi-bin/bbs/bbsdoc?board=%s><img border=0 src=/images/button/home.gif align=absmiddle> ��������</a>  ", board);
	printf("<a href=/cgi-bin/bbs/bbsfwd?board=%s&file=%s>ת��/�Ƽ�</a>  ", board, file);
	printf("<a href=/cgi-bin/bbs/bbsccc?board=%s&file=%s>ת��</a>  ", board, file);
	printf("<a onclick='return confirm(\"�����Ҫɾ��������?\")' href=bbsdel?board=%s&file=%s>ɾ������</a>  ", board, file);
	printf("<a href=/cgi-bin/bbs/bbsedit?board=%s&file=%s><img border=0 src=/images/button/edit.gif align=absmiddle>�޸�����</a>  ", board, file);
	fp=fopen(dir, "r+");
	if(fp==0) 
		http_fatal("dir error2");
	if(num>1) 
	{
		fseek(fp, sizeof(x)*(num-2), SEEK_SET);
		fread(&x, sizeof(x), 1, fp);
		printf("<a href=/cgi-bin/bbs/bbscon?b=%s&f=%s&n=%d><img border=0 src=/images/button/up.gif align=absmiddle>��һƪ</a>  ", board, x.filename, num-1);
	}
	if(num<total) 
	{
		fseek(fp, sizeof(x)*(num), SEEK_SET);
    	fread(&x, sizeof(x), 1, fp);
    	printf("<a href=/cgi-bin/bbs/bbscon?b=%s&f=%s&n=%d><img border=0 src=/images/button/down.gif align=absmiddle>��һƪ</a>  ", board, x.filename, num+1);
	}
	if(num>0 && num<=total) 
	{
		//modified by roly from "num>0 && num<total"
		fseek(fp, sizeof(x)*(num-1), SEEK_SET);
		fread(&x, sizeof(x), 1, fp);
		#ifdef SPARC
			(*(int*)(x.title+72))++; //modified by roly from 73 to 72 for sparc solaris
			if(*(int*)(x.title+72)>1000000)
				(*(int*)(x.title+72))=0;//modified by roly from 73 to 72 for sparc solaris
		#else
			(*(int*)(x.title+73))++; //modified by roly from 73 to 72 for sparc solaris
			if(*(int*)(x.title+73)>1000000)
				(*(int*)(x.title+73))=0;//modified by roly from 73 to 72 for sparc solaris
		#endif
		//fseek(fp, sizeof(x)*(num-1), SEEK_SET);
		//fwrite(&x, sizeof(x), 1, fp);
		brc_initial(currentuser.userid, board);
		brc_addlist(x.filename);
		brc_update(currentuser.userid, board);
	}
	fclose(fp);
    ptr=x.title;
    if(!strncmp(ptr, "Re: ", 4)) 
		ptr+=4;
	ptr[60]=0;
	/* added by roly */
	
	/* add end */
		//added by iamfat 2002.08.10
		//check_anonymous(x.owner);
		//added end.
    printf("[<a href='/cgi-bin/bbs/bbspst?board=%s&file=%s&userid=%s&id=%d&gid=%d&title=Re: %s '><img border=0 src=/images/button/edit.gif align=absmiddle>������</a>] ",board, file, x.owner, x.id, x.gid, entity_char(ptr));
	printf("[<a href='/cgi-bin/bbs/bbsgfind?board=%s&gid=%d '>ͬ�����Ķ�</a>] \n", board, x.gid);
   	printf("</center>\n"); 
	http_quit();
}
