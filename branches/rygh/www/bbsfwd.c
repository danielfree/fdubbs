#include "BBSLIB.inc"

int main() {
	struct fileheader *x;
	char board[80], file[80], target[80], filename[80];
	struct userec *user;
		
	init_all();
	strsncpy(board, getparm("board"), 30);
	strsncpy(file, getparm("file"), 32);
	strsncpy(target, getparm("target"), 30);
	if(!loginok) 
	{
		printf("<b>ת��/�Ƽ������� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�Ҵҹ��Ͳ��ܽ��б������");
	}
	/* Added by Amigo 2002.06.19. For mail right check. */
	if (!HAS_PERM(PERM_MAIL)) 
	{
		printf("<b>ת��/�Ƽ������� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("����δ���ע�ᣬ���߷����ż���Ȩ�ޱ����");
	}
	/* Add end. */
	/* added by roly for mail check */
	if (!mailnum_under_limit(currentuser.userid) || !mailsize_under_limit(currentuser.userid)) 
	{
		printf("<b>ת��/�Ƽ������� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����ż��������꣬�޷�����");
	}
	/* add end */
	if(!has_read_perm(&currentuser, board)) 
	{
		printf("<b>ת��/�Ƽ������� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������");
	}
	x=get_file_ent(board, file);
	if(x==0) 
	{
		printf("<b>ת��/�Ƽ������� �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("������ļ���");
	}
	printf("<b>ת��/�Ƽ������� �� %s [ʹ����: %s]</b>\n", BBSNAME, currentuser.userid);
	printpretable_lite();
	if(target[0]) {
		if(!strstr(target, "@")) {
			if(!getuser(target)) http_fatal("�����ʹ�����ʺ�");
			//add by Danielfree 06.2.5
			if (!( (getuser(target) )-> userlevel & PERM_READMAIL))
				  http_fatal("�Է��޷�����");
			strcpy(target, getuser(target)->userid);
			if (!mailsize_under_limit(target)|| !mailnum_under_limit(target))
				http_fatal("�������ż��������꣬�޷�����");
			//add end
		}
		user=getuser(target);
		
		sprintf(filename, "home/%c/%s/rejects", toupper(target[0]), user->userid);
		if(file_has_word(filename, currentuser.userid))
		    http_fatal("�Է������յ������ż�");
			
		return do_fwd(x, board, target);
	}
	printf("<table><tr><td>\n");
	printf("���±���: %s<br>\n", nohtml(x->title));
	printf("��������: %s<br>\n", x->owner);
	printf("ԭ������: %s<br>\n", board);
	printf("<form action=bbsfwd method=post>\n");
	printf("<input type=hidden name=board value=%s>", board);
	printf("<input type=hidden name=file value=%s>", file);
	/*
	printf("������ת�ĸ� <input name=target size=30 maxlength=30 value=%s> (������Է���id��email��ַ). <br>\n",
		currentuser.email);
	*/
	// modified by roly to deny internet mail
	printf("������ת�ĸ� <input name=target size=12 maxlength=12 value=%s> (������Է���id). <br>\n",
		currentuser.userid);
	//modified end
	printf("<input type=submit value=ȷ��ת��></form>");
}

int do_fwd(struct fileheader *x, char *board, char *target) {
	FILE *fp, *fp2;
	char title[512], buf[512], path[200], i;
	sprintf(path, "boards/%s/%s", board, x->filename);
	if(!file_exist(path)) http_fatal("�ļ������Ѷ�ʧ, �޷�ת��");
	sprintf(title, "[ת��] %s", x->title);
	title[60]=0;
	post_mail(target, title, path, currentuser.userid, currentuser.username, fromhost, -1);
	printf("������ת�ĸ�'%s'<br>\n", nohtml(target));
	printf("[<a href='javascript:history.go(-2)'>����</a>]");
}
