#include "BBSLIB.inc"

void recount_size(char *board)
{
      char path[256], cmd[256];
      sprintf(path, "%s/upload/%s", BBSHOME, board);
      if(dashd(path))
      {
          FILE *fp;
          sprintf(cmd , "du %s|cut -f1>%s/.size", path, path);
          system(cmd);
      }
}
												

int main() 
{
	FILE *fpdir,*fptrash;
	struct dir x;
	struct boardheader *brd;
	char file[80], board[80], filename[80];//,filename2[80];
	int start;
	int num=0;
	init_all();
	if(!loginok) http_fatal("�Ҵҹ����޷�ɾ���ļ�, ���ȵ�¼");
	strsncpy(file, getparm("file"), 40);
	start=atoi(getparm("start"));
	strsncpy(board, getparm("board"),80);
	sprintf(filename,"%s/upload/%s/.DIR",BBSHOME,board);
	fpdir=fopen(filename, "r");
	if(fpdir==0) 
		http_fatal("��Ŀ¼");
	while(1) 
	{
		if(fread(&x, sizeof(x), 1, fpdir)<=0) 
			break;
		if(!strncmp(x.filename, file, 36)) 
		{
			brd=getbcache(board);
			if(brd==0) 
				http_fatal("�ڲ�����10002");
			if(strcasecmp(x.owner,currentuser.userid) && !has_BM_perm(&currentuser, board) && !(currentuser.userlevel & PERM_OBOARDS)) 
			{
				http_fatal("û��Ȩ��ɾ���ļ�");
			}

			sprintf(filename,"%s/upload/%s/%s",BBSHOME,board,x.filename);
			if(unlink(filename))
			{
				http_fatal("ɾ���ļ�����");
			}
			/*
			sprintf(filename2,"%s_TRASH",filename);
			if(rename(filename,filename2))
			{
				http_fatal("ɾ���ļ�����");
			}
			strcat(x.showname,"_TRASH");			
			sprintf(filename,"%s/upload/%s/.TRASH", BBSHOME,x.board);
			fptrash=fopen(filename,"a");
			if(fptrash==NULL)
			{
				http_fatal("�ڲ�����:д�ļ�����");
			}
			fwrite(&x,sizeof(struct dir),1,fptrash);
			fclose(fptrash);	
			*/
			sprintf(filename,"%s/upload/%s/.DIR",BBSHOME, board);
			del_record(filename, sizeof(struct dir), num);
			{
				char buf[256],log[100];
				sprintf(buf, "DEL [%s] %s %dB %s %s FILE:%s\n",cn_Ctime(time(0)), currentuser.userid, x.id, fromhost, board, file);
				sprintf(log,"%s/upload.log",BBSHOME);
				f_append(log, buf);
			}
			recount_size(board);
			sprintf(filename, "bbsfdoc?board=%s&start=%d", board,start);
			redirect(filename);
			//printf("<script>history.go(-1);</script>");
			http_quit();
		}
		num++;
	}
	fclose(fpdir);
	http_fatal("������ļ���");
}

