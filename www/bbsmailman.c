#include "libweb.h"

int main() {
	int i, total=0, mode;
	char  *ptr;
	struct boardheader *brd;
	init_all();
	printf("<b>ɾ���ż� �� %s </b><br>\n",BBSNAME);
	printpretable_lite();
	if(!loginok) 
		http_fatal("���ȵ�¼");
	mode=atoi(getparm("mode"));
	if(mode<=0 || mode>1) 
		http_fatal("����Ĳ���");
	printf("<table>");
	for(i=0; i<parm_num && i<40; i++) 
	{
		if(!strncmp(parm_name[i], "box", 3)) 
		{
			total++;
			if(mode==1) 
				do_del(parm_name[i]+3);
			/* for extension */
		}
	}
	printf("</table>");
	if(total<=0) 
		printf("����ѡ������<br>\n");
	printposttable_lite();
	printf("<br><a href=bbsmail>�����ż�ģʽ</a>");
	http_quit();
}

int do_del(char * file) { 
        FILE *fp; 
        struct fileheader f; 
        char path[80], *id,buf[256]; 
		
        int num=0; 
        if(loginok == 0) http_fatal("����δ��¼"); 
        id=currentuser.userid; 
        if(strncmp(file, "M.", 2) || strstr(file, "..")) http_fatal("����Ĳ���"); 
        sprintf(path, "mail/%c/%s/.DIR", toupper(id[0]), id); 
        fp=fopen(path, "r"); 
        if(fp==0) http_fatal("����Ĳ���2"); 
        while(1) { 
                if(fread(&f, sizeof(f), 1, fp)<=0) break; 
		//added by iamfat 2002.08.10
		//check_anonymous(f.owner);
		//added end.
                num++; 
                if(!strcmp(f.filename, file)) { 
                        fclose(fp); 
                        del_record(path, sizeof(struct fileheader), num-1); 
                        sprintf(path, "mail/%c/%s/%s", toupper(id[0]), id, f.filename); 
                        unlink(path); 
                        
						printf("<tr><td>%s  <td>����:%s <td>ɾ���ɹ�.\n", f.owner, nohtml(f.title));
					
						return 0;
			    } 
        } 
        fclose(fp); 
        http_fatal("�ż�������, �޷�ɾ��"); 
} 

