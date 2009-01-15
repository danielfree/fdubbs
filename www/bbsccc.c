#include "libweb.h"

int main() {
	struct fileheader *x;
	char board[80], file[80], target[80];
	init_all();
	strlcpy(board, getparm("board"), 30);
	strlcpy(file, getparm("file"), 32);
	strlcpy(target, getparm("target"), 30);
	if(!loginok) 
	{
		printf("<b>ת������ �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�Ҵҹ��Ͳ��ܽ��б������");
	}
	if(!has_read_perm(&currentuser, board)) 
	{
		printf("<b>ת������ �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("�����������");
	}
	x=get_file_ent(board, file);
	if(x==0) 
	{
		printf("<b>ת������ �� %s </b><br>\n",BBSNAME);
		printpretable_lite();
		http_fatal("������ļ���");
	}
	printf("<b>ת������ �� %s [ʹ����: %s]</b><br>\n", BBSNAME, currentuser.userid);
	printpretable_lite();
	if(target[0]) {
		if(!has_post_perm(&currentuser, target)) http_fatal("��������������ƻ���û���ڸð淢�ĵ�Ȩ��");
		return do_ccc(x, board, target);
	}
	printf("<table><tr><td>\n");
	printf("<font color=red>ת������ע������:<br>\n");
	printf("��վ�涨ͬ�����ݵ������Ͻ��� 4 ���� 4 ���������������ظ�����");
	printf("Υ�߽�������ڱ�վ���ĵ�Ȩ��<br><br></font>\n");
	printf("���±���: %s<br>\n", nohtml(x->title));
	printf("��������: %s<br>\n", x->owner);
	printf("ԭ������: %s<br>\n", board);
	printf("<form action=bbsccc method=post>\n");
	printf("<input type=hidden name=board value=%s>", board);
	printf("<input type=hidden name=file value=%s>", file);
	printf("ת�ص� <input name=target size=30 maxlength=30> ������. ");
	printf("<input type=submit value=ȷ��></form>");
}

int do_ccc(struct fileheader *x, char *board, char *board2) {
	FILE *fp, *fp2;
	struct boardheader *brc = NULL;
	brc = getbcache(board2);
	if (brc -> flag & BOARD_DIR_FLAG) {  //����ת�ص�Ŀ¼ Danielfree 06.3.5
	        http_fatal("��ѡ����һ��Ŀ¼");
        }
	if ((brc->flag & BOARD_CLUB_FLAG)&& (brc->flag & BOARD_READ_FLAG )&& !has_BM_perm(&currentuser, brc->filename)&& !isclubmember(currentuser.userid, brc->filename)) {
			http_fatal ("��������������ƻ���û���ڸð淢�ĵ�Ȩ��");
	}
	char title[512], buf[512], path[200], path2[200], i;
	sprintf(path, "boards/%s/%s", board, x->filename);
	fp=fopen(path, "r");
	if(fp==0) http_fatal("�ļ������Ѷ�ʧ, �޷�ת��");
	sprintf(path2, "tmp/%d.tmp", getpid());
	fp2=fopen(path2, "w");
	for(i=0; i<3; i++)
		if(fgets(buf, 256, fp)==0) break;
	fprintf(fp2, "[37;1m�� ��������ת���� [32m%s [37m������ ��\n", board);
	fprintf(fp2, "[37;1m�� ԭ���� [32m%s [37m������ ��[m\n\n", x->owner);
	while(1) {
		if(fgets(buf, 256, fp)==0) break;
		fprintf(fp2, "%s", buf);
	}
	fclose(fp);
	fclose(fp2);
	if((!strncmp(x->title, "[ת��]", 6))||
		(!strncmp(x->title, "Re: [ת��]", 10))){
		//modified by money 04.01.17 for judge Re & cross
		sprintf(title, x->title);
	} else {
		sprintf(title, "[ת��]%.55s", x->title);
	}
	post_article(board2, title, path2, currentuser.userid, currentuser.username, fromhost, -1, -1, -1);
	unlink(path2);
	printf("'%s' ��ת���� %s ��.<br>\n", nohtml(title), board2);
	printf("[<a href='javascript:history.go(-2)'>����</a>]");
}
