/*******************************************
     annotator : DeepOcean, Oct 20,2005
*******************************************/

#include "BBSLIB.inc"
#include "boardrc.inc"
char *eff_size();
int my_t_lines;

struct user_doccount {
	char userid[IDLEN+50];
	int  doccount;
	int  gcount;
	int  mcount;
	int  xcount;
	int  othercount;
	struct user_doccount * next;
};


int main()
{
	struct user_doccount *udcount,*udcounthead,*tail, *prev, *swap;

	FILE *fp;
	int type=0,docstart,docend,total,i,docmin,sortmode;
	char dir[80],  board[80], userid[80];
	struct boardheader *brd;
	struct fileheader x;
	init_all();
	printf("<b><font style='font-size: 18pt'>����ͳ��</font> �� %s </b><br>\n", BBSNAME);
	type=atoi(getparm("type"));					//��ȡ����ͳ�Ƶ�ͳ�Ʒ�ʽ��1:M��,2:G��,3:ˮ��,4:δ���,5:����,6:ID
	strsncpy(board, getparm("board"), 30);		//��ȡ����ͳ�Ƶİ�������
	brd=getbcache(board);						//�жϸð��������Ƿ����
	if(brd==0) http_fatal("�����������");		//���治����
	if(!has_read_perm(&currentuser, board)) http_fatal("���������������");		//�жϵ�ǰ�û��Ƿ��иð���Ķ�Ȩ��
	if(type==0) return show_form(board);		//û���ύͳ�Ʒ�ʽ����һ�ν����ҳ���Ǳ�ҳ������ύ�˴�������ݣ�ʱ��ͳ�ƣ������ύ����ͳ�Ƶı��

	if(type>6) return http_fatal("�Ƿ�����ҳ�棡");		//added by DeepOcean. Oct 22,2005 �û��ӷǱ�ҳ���ύ�Ƿ�����
if (brd ->flag & BOARD_DIR_FLAG)
		      http_fatal("��ѡ�����һ��Ŀ¼"); //add by Danielfree 06.3.5
if ((brd->flag & BOARD_CLUB_FLAG)&& (brd->flag & BOARD_READ_FLAG )&& !has_BM_perm(&currentuser, board)&& !isclubmember(currentuser.userid, board))
			        http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸð���", board);
	sortmode=atoi(getparm("mode"));				//��ȡ����ʽ��0:����1:����
	docstart=atoi(getparm("start"));			//��ȡ��ʼƪ��
	docend=atoi(getparm("end"));				//��ȡ��ֹƪ��
	docmin=atoi(getparm("min"));				//��ȡͳ����ֵ��3ƪ��ͳ��

	sprintf(dir, "boards/%s/.DIR", board);

	fp=fopen(dir, "r");
	if (!fp) http_fatal("���������������");	//.DIRָ�����Ŀ¼
	total=file_size(dir)/sizeof(struct fileheader);

//	if(docstart<=0) docstart=0;
//	if(docend<=0 || docend>total) docend=total;

	//modified by DeepOcean, Oct 20,2005  ������ʼƪ������ֹƪ��
	if(docstart<=0 || docstart>total) docstart=0;
	if(docend<docstart || docend>total) docend=total;

	if(docmin<3 || docmin>total) docmin=3;		//ͳ����ֵ��3ƪ��ͳ��

	printf("<br>ͳ��������'%s'��, %d �� %d �ķ��ĳ���%d��ID", board, docstart,docend,docmin);

	sprintf(dir, "boards/%s/.DIR", board);

	//printf("docstart:%d,docend:%d",docstart,docend);

	if(fp)
	{
		udcount=malloc(sizeof(struct user_doccount));
		udcount->next = NULL;
		udcounthead=udcount;
		fseek(fp, docstart*sizeof(struct fileheader), SEEK_SET);

		for(i=0; i<=docend-docstart; i++)
		{
			char filename[80];
			char *ptr;
			if(fread(&x, sizeof(x), 1, fp)<=0) break;
			//added by iamfat 2002.08.10
			//check_anonymous(x.owner);
			//added end.
			//printf(userid_str(x.owner));

			//ͳ�Ʒ�������
			udcount=udcounthead;
			while (udcount->next!=NULL)
			{
				if (!strcmp(udcount->next->userid,userid_str(x.owner)))
				{
					udcount->next->doccount++;
/*					if(x.accessed[0] & FILE_DIGEST) udcount->next->gcount++;
					if(x.accessed[0]& FILE_MARKED) udcount->next->mcount++;
					if(x.accessed[0]& FILE_DELETED) udcount->next->xcount++;
					if(!(x.accessed[0]& FILE_DELETED || x.accessed[0] & FILE_DIGEST
					 || x.accessed[0] & FILE_MARKED)) udcount->next->othercount++;
*/
					//modified by DeepOcean. Oct 22,2005
					if (x.accessed[0] & (FILE_DIGEST | FILE_MARKED))
					{
						if (x.accessed[0] & (FILE_DIGEST)) udcount->next->gcount++;
						if (x.accessed[0] & (FILE_MARKED)) udcount->next->mcount++;
					}
					else
						if (x.accessed[0] & (FILE_DELETED)) udcount->next->xcount++;
						else udcount->next->othercount++;
					//finished.

					break;
				}
				udcount=udcount->next;
			}
			if (udcount->next==NULL)
			{
				udcount->next=malloc(sizeof(struct user_doccount));
				udcount->next->next=NULL;
				udcount->next->doccount=1;
				udcount->next->gcount=0;
				udcount->next->mcount=0;
				udcount->next->xcount=0;
				udcount->next->othercount=0;
/*
				if(x.accessed[0] & FILE_DIGEST) udcount->next->gcount++;
				if(x.accessed[0]& FILE_MARKED) udcount->next->mcount++;
				if(x.accessed[0]& FILE_DELETED) udcount->next->xcount++;
				if(!(x.accessed[0]& FILE_DELETED || x.accessed[0] & FILE_DIGEST
				 || x.accessed[0] & FILE_MARKED))udcount->next->othercount++;
*/
				//modified by DeepOcean. Oct 22,2005
				if (x.accessed[0] & (FILE_DIGEST | FILE_MARKED))
				{
					if (x.accessed[0] & (FILE_DIGEST)) udcount->next->gcount++;
					if (x.accessed[0] & (FILE_MARKED)) udcount->next->mcount++;
				}
				else
					if (x.accessed[0] & (FILE_DELETED)) udcount->next->xcount++;
					else udcount->next->othercount++;
				//finished.

				sprintf(udcount->next->userid,userid_str(x.owner));
			}
		}
	}
	//����
	tail = NULL;
	while (1)
	{
		udcount = udcounthead;
		prev = NULL;
		while (udcount && udcount->next != tail)
		{
			if ((type == 5 && (sortmode ? (udcount->doccount > udcount->next->doccount) : (udcount->doccount < udcount->next->doccount)))
				|| (type == 1 && (sortmode ? (udcount->mcount > udcount->next->mcount) : (udcount->mcount < udcount->next->mcount)))
				|| (type == 2 && (sortmode ? (udcount->gcount > udcount->next->gcount) : (udcount->gcount < udcount->next->gcount)))
				|| (type == 3 && (sortmode ? (udcount->xcount > udcount->next->xcount) : (udcount->xcount < udcount->next->xcount)))
				|| (type == 6 && (sortmode ? (toupper(udcount->userid[22]) > toupper(udcount->next->userid[22])) : (toupper(udcount->userid[22]) < toupper(udcount->next->userid[22]))))
				|| (type == 4 && (sortmode ? (udcount->othercount > udcount->next->othercount) : (udcount->othercount < udcount->next->othercount))))
			{
				if (prev)
					prev->next = udcount->next;
				else
					udcounthead = udcount->next;
				swap = udcount->next->next;
				udcount->next->next = udcount;
				udcount->next = swap;
				if (prev)
					udcount = prev->next;
				else
					udcount = udcounthead;
			}
			prev = udcount;
			udcount = udcount->next;
		}
		if (udcount == udcounthead) break;
		if (udcount) tail = udcount;
	}

	printpretable();
	printf("<table width=100%%>\n");
	printf("<tr  class=pt9h ><td><font color=white>�ʺ�</td><td><font color=white>����</td><td><font color=white>M��</td><td><font color=white>G��</td><td><font color=white>ˮ��</td><td><font color=white>�ޱ��</td></tr>\n");
	int cc=0;
	while (udcounthead->next!=NULL)
	{
		udcount=udcounthead->next;
		if (udcount->doccount>=docmin)
		{
			printf("<tr class=%s><td> %s</td><td> %d</td><td> %d</td><td> %d</td><td> %d</td><td> %d</td></tr>",((cc++)%2)?"pt9dc":"pt9lc" ,udcount->userid,udcount->doccount,udcount->mcount,udcount->gcount,udcount->xcount,
			udcount->othercount );
		}
		free(udcounthead);
		udcounthead=udcount;
	}
	printf("</table>");
	printposttable();
	printf("<br>[<a href='bbsdoc?board=%s'>����������</a>] ", board);
	http_quit();
}


int show_form(char *board)
{
	printpretable_lite();
/*
	printf("<table><form action=bbsacount method=post>\n");
	printf("<tr><td>��������: <input type=text maxlength=24 size=24 name=board value='%s'/><br /></td></tr>\n", board);
	printf("<tr><td>��ʼƪ��: <input type=text maxlength=8 size=8 name=start /><br /></td></tr>\n");
	printf("<tr><td>��ֹƪ��: <input type=text maxlength=8 size=8 name=end /></td></tr>\n");
	printf("<tr><td>ͳ����ֵ: <input type=text maxlength=3 size=8 name=min value=5 /></td></tr>\n");
	printf("<tr><td>ͳ�Ʒ�ʽ: <select name=type><option value='5' >����</option><option value='1' >M��</option><option value='2' >G��</option><option value='3' >ˮ��</option><option value='4' >δ���</option><option value='6' >id</option></select></td></tr>\n");
	printf("<tr><td>����ʽ: <select name=mode><option value='0'>����</option><option value='1' >����</option></select></td></tr>\n");
	printf("<tr><td><input type=submit value=�ݽ���ѯ��� /></td></tr>\n");
*/
//modified by DeepOcean, Oct 20,2005
	printf("<script language=JavaScript src=/javascript/datachk.js></script>\n");	//�����������ݵ���֤�ɿͻ��˵�javascript��ɣ�������ν�ĸ���
	printf("<table><form action=bbsacount method=post onSubmit='return DataCheck(this)';>\n");
	printf("<tr><td>��������: <input class=thinborder type=text maxlength=24 size=24 name=board value='%s'></td></tr>\n", board);
	printf("<tr><td>��ʼƪ��: <input class=thinborder type=text maxlength=8 size=8 name=start></td></tr>\n");
	printf("<tr><td>��ֹƪ��: <input class=thinborder type=text maxlength=8 size=8 name=end></td></tr>\n");
	printf("<tr><td>ͳ����ֵ: <input class=thinborder type=text maxlength=3 size=8 name=min value=5></td></tr>\n");
	printf("<tr><td>ͳ�Ʒ�ʽ: <select class=thinborder name=type><option value='5'>����</option><option value='1'>M��</option><option value='2'>G��</option><option value='3'>ˮ��</option><option value='4'>δ���</option><option value='6'>id</option></select></td></tr>\n");
	printf("<tr><td>����ʽ: <select class=thinborder name=mode><option value='0'>����</option><option value='1'>����</option></select></td></tr>\n");
	printf("<tr><td><input type=submit value=�ݽ���ѯ��� name=submit></td></tr>");
//finished.
	printf("</form></table>");
	printposttable_lite();
	printf("[<a href='bbsdoc?board=%s'>������һҳ</a>] ", board);
	http_quit();
}
