#include "libweb.h"

//add by Danielfree to check filesize limit 06.11.23
int maxlen(char *board)
{
	char path[256];
	int	limit=1*1024*1024;
	sprintf(path,"%s/upload/%s/.maxlen",BBSHOME,board);
	if (dashf(path))
	{
		FILE *fp;
		fp=fopen(path,"r");
		if(fp)
			{
				fscanf(fp,"%d",&limit);
				fclose(fp);
			}
	}
	return	limit;
}

int main() 
{
	char board[80];
	init_all();
	strsncpy(board, getparm("board"), 30);
	printf("<title>�ϴ��ļ�</title>\n");
	if(!loginok) 
		http_fatal("�Ҵҹ�����Ȩ���ϴ��ļ�");
	if(!has_read_perm(&currentuser, board)) 
		http_fatal("�����������");
	if(!has_post_perm(&currentuser, board)) 
		http_fatal("���û������������Ȩ���ϴ��ļ�");
	strsncpy(board, getparm("board"), 80);
	printf("<script lang='Javascript'>	\n");
	printf("function clickup()			\n");
	printf("{	if(document.forms['upform'].elements['up'].value)	\n");
	printf("		document.forms['upform'].submit();	\n");
	printf("	else	\n");
	printf("		alert('�㻹û��ѡ���ϴ��ļ���,:)')\n");
	printf("}	\n");
	printf("</script>	\n");
	printf("<b>�ϴ��ļ���%s������ �� %s [ʹ����: %s] <b><br>\n", board, BBSNAME, currentuser.userid);
	printpretable_lite();
	printf("��������Դ���ޣ�Ϊ��ʡ�ռ䣬�����ϴ�������ļ��������ϴ�������޹ص��ļ���<br>\n");
	printf("Ŀǰ���浥���ϴ��ļ���С����Ϊ%-2.1fK�ֽ�. <br>\n",(float)(maxlen(board)/1024));
	printf("�����ع��ҷ��ɣ�<font color=red>�Ͻ��ϴ��Ƿ����ϺͿ��ܵ��¾��׵�����</font>��<br>\n");
	printf("<form method=post name=upform action=bbsupload enctype='multipart/form-data'>\n");
	printf("<table>\n");
	printf("<tr><td>�ϴ��ļ�: <td><input type=file name=up>");
	printf("<input type=hidden name=board value='%s'>", board);
	printf("</table>\n");
	printposttable_lite();
	printf("<input type=button value='�ϴ�' onclick='clickup()'> \n");
	printf("</form>");
	http_quit();
}
