#include "BBSLIB.inc"

char mybrd[22][80];
int mybrdnum=0;
struct boardheader x;

int ismybrd(char *board) {
	int n;
	for(n=0; n<mybrdnum; n++) 
	{
		if(!strcasecmp(mybrd[n], board)) 
			return n;
	}
	return -1;
}

int main() {
	FILE *fp;
	char file[200], board[200];
	int i=0;
	init_all();
	printf("<b>�ղؼ� �� %s </b><br>\n",BBSNAME);
	printpretable_lite();
	strsncpy(board, getparm("board"), 32);
	if(!loginok) http_fatal("��ʱ��δ��¼��������login");
	sprintf(file, "home/%c/%s/.goodbrd", toupper(currentuser.userid[0]), currentuser.userid);
	fp=fopen(file, "r");
        mybrdnum=0;
        if(fp!=NULL)
        {
                while(fscanf(fp, "%s\n", mybrd[mybrdnum])!= EOF)
                {
                        mybrdnum++;
                        if(mybrdnum>19)
                                break;
                }
                fclose(fp);
        }
	if(mybrdnum>=40) http_fatal("��Ԥ����������Ŀ�Ѵ����ޣ���������Ԥ��");
	if(ismybrd(board)>=0) http_fatal("���Ѿ�Ԥ�������������");
	if(!has_read_perm(&currentuser, board)) http_fatal("��������������");
	strcpy(mybrd[mybrdnum], board);
	mybrdnum++;
	fp=fopen(file, "w");
        if(fp!=NULL)
        {
        	for(i=0;i<mybrdnum;i++)
                {
			fprintf(fp,"%s\n",mybrd[i]);
		}
                fclose(fp);
        }
	printf("<script>top.f2.location='bbsleft'</script>\n");
	printf("Ԥ���������ɹ�<br><a href='javascript:history.go(-1)'>���ٷ���</a>");
	http_quit();
}
