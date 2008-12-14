#include "libweb.h"
/*check whether the user is valid or not
 *
 *@author polygon
 *@return 0 if valid, otherwise return -1
 */
int check_user(char * userid)
{
	char filename[80];
	struct userec *u;
//	strcpy(userid,user);
//	printf("%s<br>\n",userid);
	//first check whether user is valid
	if(!strstr(userid, "@")) {
                u=getuser(userid);
                if(u==0) 
			//http_fatal("������������ʺ�");
			return -1;
                 //add by Danielfree 06.2.5
                 else if (!(u -> userlevel & PERM_READMAIL))
                      //http_fatal("�Է��޷�����");
		      return -1;
                //add  end
                strcpy(userid, u->userid);
                //added by roly to test mail size
                //modified by Danielfree to test mail num
                if (!mailsize_under_limit(userid)||!mailnum_under_limit(userid))
		//http_fatal("�ռ����ż��������꣬�޷�����");
                return -1;
		//add end
		sprintf(filename, "home/%c/%s/rejects", toupper(userid[0]), userid);
		if(file_has_word(filename, currentuser.userid))
		 // http_fatal("�Է������յ������ż�");
	      		return -1;
       }
        /* added by roly to deny internet mail */
        else {
                http_fatal("��վ��֧�ֶ��ⷢ��");
		return -1;
        }
	

	return 0;
}
int main() {
   	FILE *fp;
	char userid[80], filename[80], dir[80], title[80], title2[80], buf[80], *content;
	int t, i, sig, backup;
	struct fileheader x;
	init_all();
	printf("<b>�����ż� �� %s </b><br>\n",BBSNAME);
	printpretable_lite();
	if(!loginok) http_fatal("�Ҵҹ��Ͳ���д�ţ����ȵ�¼");
	/* Added by Amigo 2002.06.19. For mail right check. */
	if (!HAS_PERM(PERM_MAIL)) 
		http_fatal("����δ���ע�ᣬ���߷����ż���Ȩ�ޱ����");
	/* Add end. */
	if(!mailnum_under_limit(currentuser.userid) || !mailsize_under_limit(currentuser.userid))
		http_fatal("�����ż��������꣬�޷�����");
   	
	//prepare the message...
	strsncpy(title, noansi(getparm("title")), 50);
	backup=strlen(getparm("backup"));
  	for(i=0; i<strlen(title); i++)
		if(title[i]<27 && title[i]>=-1) title[i]=' ';
   	sig=atoi(getparm("signature"));
   	content=getparm("text");
   	if(title[0]==0)
      		strcpy(title, "û����");
	sprintf(filename, "tmp/%d.tmp", getpid());
	f_append(filename, content);
	sprintf(title2, "{����} %s", title);
	title2[70]=0;
	
	/* 
	 * added by polygon, send to a group of people ...
	 * 
	 */
	//first get the users to be sent to.
	strsncpy(userid, getparm("userid"), 40);
	//then split it into tokens
	char * token;
	token = strtok(userid,";");
	if(token)
	{
		if(check_user(token)==0){
	  		post_mail(token, title, filename, currentuser.userid, currentuser.username, fromhost, sig-1);
			printf("�ż��Ѽĸ�%s.<br>\n", token);
		}else{
			printf("�޷����ż��ĸ�%s.<br>\n",token);
		}
	}
	while(token)
	{
		token = strtok(NULL,";");
		if(token)
		{
			if(check_user(token)==0){
	  			post_mail(token, title, filename, currentuser.userid, currentuser.username, fromhost, sig-1);
				printf("�ż��Ѽĸ�%s.<br>\n", token);
			}else{
				printf("�޷����ż��ĸ�%s.<br>\n",token);
			}
			
		}

	}
	//printf(userid);
//	http_quit();


	/*
	 *ended by polygon
	 */

	if(backup)
		post_mail(currentuser.userid, title2, filename, currentuser.userid, currentuser.username, fromhost, sig-1);
	if(backup) printf("�ż��Ѿ�����.<br>\n");
	unlink(filename);
	//added by iamfat 2002.10.20
	sprintf(title2, "mailed %s: %s", userid, title);
	trace(title2);
	//added end
	printf("<a href='javascript:history.go(-2)'>����</a>");
	http_quit();
}
