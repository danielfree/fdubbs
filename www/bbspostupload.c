#include "libweb.h"

#ifndef UPLOAD_MAX
	#define UPLOAD_MAX	(1*1024*1024)
#endif


int main() 
{
	char file[100],dir[100],url_filename[256],board[20];
	int i;
	static struct dir x;
	FILE * fp;

	init_all();
	printf("<title>�ϴ��ļ�</title>\n");
	if(!loginok) 
		http_fatal("�Ҵҹ����޷�ִ�б����������ȵ�¼");
	x.reid=1;
	strsncpy(board, getparm("board"), 20);
	strsncpy(x.owner, currentuser.userid, 13);
	strsncpy(x.filename, getparm("name"), 100);
	strtourl(url_filename,x.filename);
	x.timeDeleted=time(0);
	sprintf(file,"%s/upload/%s/%s",BBSHOME,board,x.filename);
	sprintf(dir,"%s/upload/%s/.DIR",BBSHOME,board);
	if(!has_post_perm(&currentuser, board)) 
		http_fatal("���������������Ȩ�ϴ��ļ�����������");
	if(!file_exist(file)) 
		http_fatal("������ļ���");
	x.id=file_size(file);
	if(x.id>UPLOAD_MAX) 
	{
		unlink(file);
		http_fatal("�ļ���С��������ļ�����");
	}
	
	fp=fopen(dir, "a");
	if(fp==NULL)
	{
		unlink(file);
		http_fatal("�ڲ�����:д�ļ�����");
	}
	fwrite(&x, sizeof(struct dir), 1, fp);
	fclose(fp);
	{
		char buf[256],log[100];
		sprintf(buf, "UP [%s] %s %dB %s %s FILE:%s\n",cn_Ctime(time(0)), currentuser.userid, x.id, fromhost, board, x.filename);
		sprintf(log,"%s/upload.log",BBSHOME);
		f_append(log, buf);
	}

	printf("�ļ��ϴ��ɹ�, ��ϸ��Ϣ����:");
	printpretable_lite();
	{
		float my_size=x.id;
		char sizestr[10];
		if(my_size>1024)
		{
			my_size=my_size/1024;
			if(my_size>1024)
			{
				my_size=my_size/1024;
				sprintf(sizestr,"%-6.2fMB",my_size);
			}else{
				sprintf(sizestr,"%-4.2fKB",my_size);
			}
			
		}else{
			sprintf(sizestr,"%dB",(int)(my_size));
		}
		printf("�ļ���С: %s<br>\n", sizestr);
	}
	printf("�ļ�����: %s<br>\n", x.filename);
	printf("�ϴ���ID: %s<br>\n", x.owner);
	printf("�ϴ�ʱ��: %s<br>\n", cn_Ctime(time(0)));
	printf("�ϴ�����: %s<br>\n", board);
	printf("�ϴ��ļ����Զ������������http://ת��,<br>\n");
	printf("�뱣���Զ���Ӳ���ԭ��(��Ȼ������������),<br>\n");
	printf("��www������ת�岿�ֽ��Զ�ת��Ϊ��Ӧ������/ͼƬ.\n");
	printposttable_lite();
	printf("<a href='#' onclick='return closewin()'>����</a>\n");
	printf("<script language='JavaScript'>\n");
	printf("<!--					\n");
	printf("function closewin()		\n");
	printf("{						\n");
    	printf("    opener.document.forms['postform'].elements['text'].value +='\\nhttp://%s/upload/%s/%s\\n';\n",BBSHOST, board,url_filename);
	printf("	return window.close();		\n");
	printf("}						\n");
	printf("-->						\n");
	printf("</script>\n");
	http_quit();
}

