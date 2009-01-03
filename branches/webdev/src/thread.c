/*
 Pirate Bulletin Board System
 Copyright (C) 1990, Edward Luke, lush@Athena.EE.MsState.EDU
 Eagles Bulletin Board System
 Copyright (C) 1992, Raymond Rocker, rocker@rock.b11.ingr.com
 Guy Vega, gtvega@seabass.st.usm.edu
 Dominic Tynes, dbtynes@seabass.st.usm.edu
 Firebird Bulletin Board System
 Copyright (C) 1996, Hsien-Tsung Chang, Smallpig.bbs@bbs.cs.ccu.edu.tw
 Peng Piaw Foong, ppfoong@csie.ncu.edu.tw
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 1, or (at your option)
 any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 */
/*
 $Id: thread.c 2 2005-07-14 15:06:08Z root $
 */

#include "bbs.h"

char tname[STRLEN]; //.THREAD�ļ���,�ݴ��м���
char fname[STRLEN]; //.DIR2�ļ�,�ݴ��м���

struct postnode { //��¼ͬһ����Ĳ�ͬpost
	int num;
	struct postnode *next;
};

struct titlenode { //ÿ��titlenode��¼��ͬ����
	char *title;
	struct titlenode *next;
	struct postnode *post;
};

struct titlenode toptitle; //ͬ���ⷽʽ��ͷ���

// �ͷ����ж�̬������ڴ�ռ�
int FreeTitleMem() {
	struct titlenode *t=toptitle.next;
	struct postnode *p=NULL;

	while (t !=NULL) {
		p = t->post;
		while (p!=NULL) {
			t->post = p->next;
			free(p); //�ͷ�ÿ��post��ռ�õĿռ�
			p= t->post;
		}
		toptitle.next=t->next;
		free(t->title); //�ͷű����ַ�����ռ�õĿռ�		
		free(t); //�ͷű�����������ռ�õĿռ�
		t = toptitle.next;
	}

	return 0;
}

//����num����¼post���뵽ͬ�����б���
//����ֵ��������
int thread(struct fileheader *post, int num) {
	struct titlenode *tmp;
	char *ntitle;
	tmp = &toptitle;

	//ȡ��post�ı���
	if (post->title[0] == 'R' && post->title[1] == 'e' && post->title[2]
			== ':') {
		ntitle = post->title + 4; //re�Ĵӵ��ĸ��ַ���ʼ����
	} else {
		ntitle = post->title;
	}

	while (1) {
		if (tmp->next == NULL) {
			struct titlenode *titletmp;
			titletmp
					= (struct titlenode *) malloc(sizeof(struct titlenode));
			titletmp->title = malloc(sizeof(char) * (strlen(ntitle) + 1));
			strcpy(titletmp->title, ntitle);
			titletmp->post = NULL;
			titletmp->next = NULL;
			tmp->next = titletmp;
		}
		if (!strcmp(tmp->next->title, ntitle)) { //�ҵ�����������
			struct postnode *tmppost, *first = tmp->next->post;
			if (first == NULL) {
				tmppost
						= (struct postnode *) malloc(sizeof(struct postnode));
				tmppost->num = num;
				tmppost->next = NULL;
				tmp->next->post = tmppost;
				return 1;
			}
			while (1) {
				if (first->next == NULL) {
					tmppost
							=(struct postnode *)malloc(sizeof(struct postnode));
					tmppost->num = num;
					tmppost->next = NULL;
					first->next = tmppost;
					return 2;
				}
				first = first->next;
			}//while(1)
		} else {
			tmp = tmp->next; //tmp��ʱ�������ntitle,������
		} //else
	} //while(1)

	return 0;
}
//����toptitle������post��¼,��fname�м�ȡ��¼,�޸ĺ󱣴浽tname��
int visit_all() {
	struct titlenode *tmp;
	struct fileheader post;
	int i = 0;
	tmp = toptitle.next;

	while (tmp) {
		struct postnode *tmppost;
		i++;
		tmppost = tmp->post;
		while (tmppost) {
			get_record(fname, &post, sizeof(post), tmppost->num);
			//added by iamfat ����ͬ���� 2002.3.11
			if (!tmppost->next) { //���һ��,���õ�ʱʹ��?
				post.accessed[1]|=FILE_LASTONE;
			}
			append_record(tname, &post, sizeof(post));
			tmppost = tmppost->next;
		} //while (tmppost)
		tmp = tmp->next;
	}//while(tmp)

	return 0;
}

int main(int argc, char *argv[]) {
	FILE *tf;
	int i = 0;
	struct fileheader post;
	char dname[STRLEN];
	char buf[256];
	struct stat st1, st2;
	sprintf(dname, "boards/%s/%s", argv[1], DOT_DIR); //.DIR�ļ�
	sprintf(fname, "boards/%s/%s2", argv[1], DOT_DIR); //.DIR2�ļ�
	sprintf(tname, "boards/%s/%s", argv[1], THREAD_DIR);//.THREAD�ļ�

	if (stat(dname, &st1) == -1) {
		return 1;
	}
	if (stat(tname, &st2) != -1) {
		if (st2.st_mtime >= st1.st_mtime)
			return 2;
	}
	unlink(tname);
	sprintf(buf, "cp %s %s", dname, fname);
	system(buf);

	if ((tf = fopen(fname, "rb")) == NULL) {
		printf(".DIR can't open...");
		return 3;
	}
	toptitle.next = NULL;
	toptitle.post = NULL;
	while (1) {
		i++;
		if (fread(&post, sizeof(post), 1, tf) <= 0)
			break;
		thread(&post, i);
	}
	visit_all();
	fclose(tf);
	FreeTitleMem();
	unlink(fname);

	return 0;
}
