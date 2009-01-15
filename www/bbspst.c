#include "libweb.h"

int main() {
    /* added by roly 02.01.25 to disable reply article which is marked as unrepliable */
    int noreply = 0;
    int replymode = 0; //added by money 04.01.17 for judge reply mode
    struct fileheader *fileinfo; 
    struct boardheader *bp;
    /* add end */
	
   	FILE *fp;
   	int i;
	char userid[80], buf[512], path[512], file[512], board[512], title[80]="";


   	init_all();

	if(!loginok)
	{
		printf("<b>�������� �� %s </b><br>\n", BBSNAME);
		printpretable_lite();
		http_fatal("�Ҵҹ��Ͳ��ܷ������£����ȵ�¼");
	}
	strlcpy(board, getparm("board"), 20);
	strlcpy(file, getparm("file"), 80);
	//02.09.07 modified by stephen to fit the title buffer,changed from 50 to 80
	//strsncpy(title, getparm("title"), 50);
	strlcpy(title, getparm("title"), 66);
	if(title[0])  
	{	
		if(strncmp(title, "Re: ", 4))
			sprintf(title, "Re: %s", getparm("title"));
		replymode = 1;
	}
	strlcpy(userid, getparm("userid"), 40);
	if(file[0]!='M' && file[0])
	{
		printf("<b>�������� �� %s </b><br>\n", BBSNAME);
		printpretable_lite();
		http_fatal("������ļ���");
	}
	
/* added by roly 02.01.25 to disable ... */
	bp = getbcache(board);
    if(strlen(file)!=0){
	    fileinfo=get_file_ent(board, file); 
	    noreply = fileinfo->accessed[0] & FILE_NOREPLY ||bp->flag & BOARD_NOREPLY_FLAG; 
	    if(!(!noreply ||has_BM_perm(&currentuser, board))) 
		{
			printf("<b>�������� �� %s </b><br>\n", BBSNAME);
			printpretable_lite();
	        http_fatal("�Բ���, �������в��� RE ����, �����ܻظ�(RE) ��ƪ����."); 
		}
    }
/* add end */  
    if(!has_post_perm(&currentuser, board))
	{
		printf("<b>�������� �� %s </b><br>\n", BBSNAME);
		printpretable_lite();
		http_fatal("�������������������Ȩ�ڴ���������������");
	}
    
    if ((bp->flag & BOARD_CLUB_FLAG)
	    && !isclubmember(currentuser.userid, board)
		&& !has_BM_perm(&currentuser, board))
	    http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ�ڴ���������������", board);
	    printf("<body onload=javascript:document.postform.title.focus();>");
        printf("<b>�������� �� %s [ʹ����: %s]</b>\n", BBSNAME, currentuser.userid);
   	printf("<center>\n");

	printpretable_lite();
	
	printf("<table border=0>\n");
	printf("<tr><td>");
	printf("<font color=green>����ע������: <br>\n");
	printf("����ʱӦ���ؿ������������Ƿ��ʺϹ������Ϸ������������ˮ��лл���ĺ�����<br>\n</font>");
//	printf("<tr><td><form name=postform method=post action=bbssnd?board=%s>\n", board);
    printf("<tr><td><form name=postform method=post onsubmit='wait_post()' action=bbssnd?board=%s>\n", board);

	  
/* added by money 04.01.17 for judge Reply mode when post article */
	if (replymode == 1)
		printf("<input type=hidden name=replymode value=1>");
	else
		printf("<input type=hidden name=replymode value=2>");
	printf("<input type=hidden name=id value=%s>", getparm("id"));
	printf("<input type=hidden name=gid value=%s>", getparm("gid"));
	/* added end */
   	printf("ʹ�ñ���: <input class=thinborder type=text name=title size=60 maxlength=50 value='%s' %s>", 
		title, (replymode==1)?"READONLY":"");
	printf(" ������: [%s]<br>\n",board);
   	printf("���ߣ�<b>%s</b>", currentuser.userid);
   	printf("  &nbsp; &nbsp; ʹ��ǩ���� ");
   	printf("<input type=radio name=signature value=1 checked>1");
   	printf("<input type=radio name=signature value=2>2");
   	printf("<input type=radio name=signature value=3>3");
	printf("<input type=radio name=signature value=4>4");
	printf("<input type=radio name=signature value=5>5");
   	printf("<input type=radio name=signature value=0>0"); 
   	printf(" [<a target=_balnk href=bbssig>�鿴ǩ����</a>] ");
	printf("<br>\n<textarea class=thinborder name=text rows=20 cols=80 wrap=virtual>\n\n");

 
	
	if(file[0]) {
		int lines=0;
		printf("�� �� %s �Ĵ������ᵽ: ��\n", userid);
		sprintf(path, "boards/%s/%s", board, file);
		fp=fopen(path, "r");
		if(fp) {
			for(i=0; i<3; i++)
				if(fgets(buf, 500, fp)==0) break;
			while(1) {
				if(fgets(buf, 500, fp)==0) break;
				if(!strncmp(buf, ": ��", 4)) continue;
				if(!strncmp(buf, ": : ", 4)) continue;
				if(!strncmp(buf, "--\n", 3)) break;
				if(buf[0]=='\n') continue;
				if(lines++>=20) {
					printf(": (��������ʡ��...)\n");
					break;
				}
				if(!strcasestr_gbk(buf, "</textarea>")) printf(": %s", buf);
			}
			fclose(fp);
		}
	}
   	printf("</textarea>\n");
   	printf("<tr><td class=post align=center>");
//	printf("<input type=submit value=����> ");
    printf("<input type=submit value=&nbsp;��&nbsp;&nbsp;��&nbsp; id=btnPost> ");
	
   	printf("<input type=reset value=���>  ");
	{
		char attach[256];
		sprintf(attach, "%s/upload/%s", BBSHOME, board);
		if(dashd(attach))
			printf("<input type='button' name='attach' value='����' onclick='return opnewwin() ' />");
	}
	printf("<br>");
	printf("<script language='JavaScript'>\n");
	printf("<!--					\n");
    printf("var objPost=document.getElementById('btnPost');\n");
	printf("document.onkeydown=function(evt){\n");
	printf("  document.onkeypress=function(){return true;};\n");
	printf("  evt=evt?evt:event;\n");
	printf("  if((evt.keyCode==119 || evt.keyCode==87) && evt.ctrlKey){\n");
	printf("    document.onkeypress=function(){return false;};\n");
	printf("    document.postform.submit();\n");
	printf("   return false; }};\n");							
	printf("function wait_post() \n");
	printf("{          \n");
	printf("     objPost.disabled=true;\n");
	printf("     objPost.value='���ڷ���,�����ĵȴ�...';\n");
	printf("     setTimeout('enable_post()',20000);\n");
	printf("}           \n");
	printf("function enable_post() \n");
	printf("{           \n");
	printf("     objPost.value='���ͳ�ʱ,������';\n");
	printf("     objPost.disabled=false;\n");
	printf("}           \n");
	printf("function opnewwin()		\n");
	printf("{						\n");
	printf("	var mywin = window.open('bbspreupload?board=%s','_blank','width=600,height=300,scrollbars=yes');\n",board);
	printf("	if ((document.window != null) && (!mywin.opener))\n");
	printf("		mywin.opener = document.window;\n");
	printf("	mywin.focus();		\n");
	printf("	return false;		\n");
	printf("}						\n");
	printf("-->						\n");
	printf("</script>\n");
	printf("</form>\n");
	printf("</table>");

	printposttable_lite();
	printf("</center>");
	printf("</body>");
    http_quit();
}
