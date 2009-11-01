#include "libweb.h"
int main() {
	FILE *fp;
	char board[80], dir[80], *ptr;
	struct boardheader *x1;
	struct fileheader x;
	char path[256];
	int i, start, total;
 	init_all();
	strlcpy(board, getparm("board"), 32);
	x1=getbcache(board);
	if(x1==0)
	{
		printf("<b>��ժ�� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������");
	}
	strcpy(board, x1->filename);
	if(!has_read_perm(&currentuser, board)) 
	{
		printf("<b>��ժ�� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������");
	}
	sprintf(dir, "boards/%s/.DIGEST", board);
    fp=fopen(dir, "r");
    if(fp==0)
	{
		printf("<b>��ժ�� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������Ŀ¼");
	}
	total=file_size(dir)/sizeof(struct fileheader);
	start=atoi(getparm("start"));
	if(strlen(getparm("start"))==0 || start>total-20) 
		start=total-19;
  	if(start<1) 
		start=1;
	printf("<nobr>\n");

        printf("<table width=100%% border=0 ><tr><td width=85%% align=left>\n");

        sprintf(path, "%s/info/boards/%s/icon.jpg",BBSHOME,board);
        if(dashf(path))
        {
                printf("<img src=/info/boards/%s/icon.jpg align=absmiddle width=32 height=32>",board);
        }

        //printf("<nobr>\n");
        printf("<b>");

        sprintf(path,"%s/info/boards/%s/banner.jpg",BBSHOME, board);
        if(dashf(path))
                printf("<img src=/info/boards/%s/banner.jpg align=absmiddle height=32>", board);
        else
                printf("<font style='font-size: 18pt'>%s</font> ��",board);

	printf(" %s ��ժ�� ����[%s] ������[%d]</b><br>\n", BBSNAME, userid_str(x1->BM), total);


        printf("<td width=15%% align=right>\n");
        printf("<form name=form1 action=bbsgdoc?board=%s method=post>", board);
        printf("<input border=0 src=/images/button/forward.gif type=image align=absmiddle> �� <input class=thinborder type=text name=start size=4> ƪ");
        printf("</form></td></tr></table>\n");


	if(total<=0) 
	{
		printpretable_lite();
		http_fatal("��������Ŀǰû������");
	}
	printf("<center>\n");
	printpretable();
	printf("<table width=100%% border=0  bgcolor=#ffffff>\n");
    printf("<tr class=pt9h ><td><font color=white>���<td><font color=white>״̬<td><font color=white>����<td><font color=white>����<td><font color=white>����<td>\n");//<font color=white>����\n");
	fseek(fp, (start-1)*sizeof(struct fileheader), SEEK_SET);
	int cc=0;
    for(i=0; i<20; i++) 
	{
		if(fread(&x, sizeof(x), 1, fp)<=0) 
			break;
		//added by iamfat 2002.08.10
		//check_anonymous(x.owner);
		//added end.
		printf("<tr class=%s><td>%d<td>%s<td>%s",((cc++)%2)?"pt9dc":"pt9lc" , start+i, flag_str(x.accessed[0]), userid_str(x.owner));
		printf("<td>%12.12s", Ctime(atoi(x.filename+2))+4);
		//fix bug: ��ʾbug by DeepOcean
		printf("<td><a href=bbsgcon?board=%s&file=%s&num=%d>%s%s</a>\n",	board, x.filename, start+i,	strncmp(x.title, "Re: ", 4) ? "�� " : "",
			nohtml(x.title));
		/*
		#ifdef SPARC
			,*(int*)(x.title+72));//modified by roly from 73 to 72 for sparc solaris
		#else
			,*(int*)(x.title+73));//modified by roly from 73 to 72 for sparc solaris
		#endif*/
	}
	printf("</table>\n");
	printposttable();
	printf("</center>\n");
	if(start>0) 
		printf("<a href=bbsgdoc?board=%s&start=%d><img border=0 src=/images/button/up.gif align=absmiddle>��һҳ</a>  ",	board, start<20 ? 0 : start-20);
	if(start<total-19) 
		printf("<a href=bbsgdoc?board=%s&start=%d><img border=0 src=/images/button/down.gif align=absmiddle>��һҳ</a>  ",	board, start+20);
	printf("<a href=bbsdoc?board=%s>��������</a>  ", board);
	printf("<a href=bbs0an?path=%s>������</a>  ", anno_path_of(board));
	printf("<a href=bbspst?board=%s><img border=0 src=/images/button/edit.gif align=absmiddle>��������</a>  ", board);
	fclose(fp);

	http_quit();
}
