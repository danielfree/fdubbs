/***************************************************************************
                          poststat.c  -  description
                             -------------------
    begin                : ��  3�� 10 2005
    copyright            : (C) 2005 by SpiritRain
    email                : SpiritRain@citiz.net
	last modified        : ��  4�� 26 2005
 ***************************************************************************/

/*
*   ����: poststat [dir] [type]
*   type: 
*   0 ��ʾͳ����ʮ��
*   1 ��ʾͳ������ʮ��
*   2 ��ʾͳ����һ�ٴ�
*   3 ��ʾͳ����һ����ʮ��
*/

//#define DEBUG			1

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE		2048
#define AUTHORSIZE		13
#define BOARDSIZE		18
#define TITLESIZE		62

char *myfile[4] = {"day", "week", "month", "year"};
int mytop[4] = {10, 50, 100, 200};
char *mytitle[4] = {"��ʮ", "����ʮ", "��һ��", "��ȶ���"};


time_t now;

/* 105 bytes */
struct postrec {
    char author[AUTHORSIZE];              /* author name */
    char board[BOARDSIZE];               /* board name */
    char title[TITLESIZE];               /* title name */
    unsigned int gid;
    time_t date;                  /* last post's date */
    int number;                   /* post number */
};

struct postlist {
    char author[AUTHORSIZE];              /* author name */
    char board[BOARDSIZE];               /* board name */
    char title[TITLESIZE];               /* title name */
    unsigned int gid;
    time_t date;                  /* last post's date */
    int number;                   /* post number */
	struct postlist  *bnext, *prev, *next;
}*bucket[HASHSIZE],*toppost;

static struct boards_bucket_node {
	char board[BOARDSIZE];
	int times;
	struct boards_bucket_node *next;
} *boards_bucket[HASHSIZE];

int	hash(char *key, int len) {
    int i, value = 0;

    for (i = 0; key[i] && i < len; i++)
        value += key[i] < 0 ? -key[i] : key[i];

    value = value % HASHSIZE;
    return value;
}


/* ---------------------------------- */
/* hash structure : array + link list */
/* ---------------------------------- */
static __inline__ void add_rec(struct postlist * rec, struct postlist * prev,struct postlist * next)
{
	next->prev = rec;
	rec->next = next;
	rec->prev = prev;
	prev->next = rec;
}
static __inline__ void del_rec(struct postlist * prev,
				  struct postlist * next)
{
	next->prev = prev;
	prev->next = next;
}


void swap(struct postlist *i, struct postlist * j){
	del_rec(i->prev, i->next);
	del_rec(j->prev, j->next);
	add_rec(i, j->prev, j->next);
	add_rec(j, i->prev, i->next);
}

void sort(struct postlist *rec)
{
	struct postlist *tmppost = rec->prev;

	while (rec->number > tmppost->number && tmppost != toppost)
	{
		tmppost = tmppost->prev;
	}
	del_rec(rec->prev, rec->next);
	add_rec(rec, tmppost, tmppost->next);
}

void search(struct postlist *rec)
{
    struct postlist *t;
    int h;

	h = rec->gid % HASHSIZE;
	t = bucket[h];
	while (t) {
		if (t->gid == rec->gid &&
			!strncmp(t->board, rec->board, BOARDSIZE)) {
			t->number += rec->number;
			if (t->date < rec->date)
				t->date = rec->date;
			sort(t);
			free(rec);
			return;
		}
		t = t->bnext;
	}
	rec->bnext = bucket[h];
	bucket[h] = t;
	add_rec(rec, toppost->prev, toppost);
	sort(rec);
}

//��fname�ж�ȡ��¼����Ϊÿ����¼��list���ҵ��ʵ���λ��
void load_stat(char *fname, size_t size) {
    FILE *fp;
	struct postlist *post;
	int listsize = sizeof(struct postlist); 
	long count = 0;
    //��fname
	if( (fp = fopen(fname, "r")) != NULL) {
		//����һ���ڴ��
		post = (struct postlist *)malloc(listsize);
		post->prev = post;
		post->next = post;
		post->bnext = NULL;
		//���ļ��ж�ȡ��¼
		while (fread(post, sizeof(struct postrec), 1, fp) && count < size)
		{
			//�����б�����¼����
			search(post);
			//���·���һ���ڴ��
			count++;
			post = (struct postlist *)malloc(listsize);
			post->prev = post;
			post->next = post;
			post->bnext = NULL;
		};
		//�ͷŶ�����ڴ��
		free(post);
		//�ر�fp
        fclose(fp);
    }
}

static int boards_get_times(char *board_name)
{
	struct boards_bucket_node *new_node, *t;
	int h;

	h = hash(board_name, BOARDSIZE);
	t = boards_bucket[h];
	while (t) {
		if (!strncmp(t->board, board_name, BOARDSIZE))
			return ++t->times;
		t = t->next;
	}

	new_node = malloc(sizeof(struct boards_bucket_node));
	if (!new_node)
		return 0;
	strncpy(new_node->board, board_name, BOARDSIZE);
	new_node->times = 1;
	new_node->next = boards_bucket[h];
	boards_bucket[h] = new_node;

	return new_node->times;
}

/*
 * every board can only on top ten posts at most 3 times
 */
void remove_duplicate_boards()
{
	struct postlist *cur;
	int i;

	cur = toppost->next;
	while (cur != toppost)
	{
		if (boards_get_times(cur->board) > 3) {
			/* delete this post from bucket */
			struct postlist **t, *tmp;
			t = &bucket[cur->gid % HASHSIZE];
			while (*t) {
				if (cur->gid == (*t)->gid &&
					!strncmp(cur->board, (*t)->board, BOARDSIZE)) {
					*t = (*t)->bnext;
					break;
				}
			}

			/* delete this post from toppost */
			del_rec(cur->prev, cur->next);
			tmp = cur;
			cur = tmp->next;

			/* free the space */
			free(tmp);

			continue;
		}
		cur = cur->next;
	}

	/* free boards_bucket */
	for (i = 0; i < HASHSIZE; i++) {
		struct boards_bucket_node *node, *t;

		node = boards_bucket[i];
		while (node) {
			t = node->next;
			free(node);
			node = t;
		}
	}
}

void poststat(int mytype) {
	FILE *fp;
	char srcfile[64], dstfile[64], *p;
    struct postlist *tmppost;
    int i, top, count;


	//ʮ��ͳ��ƪ��
	top = mytop[mytype];
	//��ʼ��toppost���ݽṹ
	//toppost��һ���յ�list
	toppost = malloc(sizeof(struct postlist));
	toppost->prev = toppost;
	toppost->next = toppost;
	toppost->bnext = NULL;
	toppost->number = 0;


	//ͳ����ʮ��
	if (mytype == 0)
	{
#ifdef DEBUG
		sprintf(srcfile,  "tmp/post");
		sprintf(dstfile,  "tmp/post.old");
#else
		sprintf(srcfile,  "tmp/.post");
		sprintf(dstfile,  "tmp/.post.old");
#endif
		//�Ƴ� tmp/.post.old
		remove(dstfile);
		
		//�� tmp/.post ������Ϊ tmp/.post.old
		rename(srcfile, dstfile);
		
		//�ȶ�ȡetc/posts/day.0
		sprintf(srcfile, "etc/posts/day.0");
		load_stat(srcfile, -1);
		
		//�ٶ�ȡ tmp/.post.old
		load_stat(dstfile, -1);

		remove_duplicate_boards();
	}
	//ͳ������ʮ��
	else if (mytype > 0 && mytype < 4)
	{
		//�ȶ�ȡ����top��¼etc/posts/week.0
		sprintf(srcfile,  "etc/posts/%s.0", myfile[mytype]);
		load_stat(srcfile, -1);

		//�ٶ�ȡ���ռ�¼etc/posts/day.0
		sprintf(dstfile, "etc/posts/day.0");
		load_stat(dstfile, -1);
	}
	//ͳ����һ�ٴ�
	else
	{
		return;
	}

	//��ʮ��д��
	if( (fp = fopen(srcfile, "w"))!=NULL) {
		count = 0;
		tmppost = toppost->next;
		while (tmppost != toppost)
		{
			fwrite(tmppost, sizeof(struct postrec), 1, fp);
			tmppost = tmppost->next;
		}
		fclose(fp);
	}

    sprintf(srcfile, "etc/posts/%s", myfile[mytype]);
	fp = fopen(srcfile, "w");
	if (!fp)
		fprintf(stderr, "file %s not exists!\n", srcfile);
	else {
        fprintf(fp, "                [1;34m-----[37m=====[41m ��%s�����Ż��� [40m=====[34m-----[0m\n\n", mytitle[mytype]);

		tmppost = toppost->next;
        for (i = 0; i < top && tmppost != toppost; i++) {
            strcpy(dstfile, ctime(&tmppost->date));
            dstfile[20] = '\0';
            p = dstfile + 4;
            fprintf(fp,
                    "[1;37m��[1;31m%3d[37m �� [37m���� : [33m%-18s[37m"
                    "�� [32m%s[37m��[36m%4d [37mƪ[35m%13.13s\n"
                    "     [37m���� : [1;44;37m%-60.60s[40m\n"
                    ,(int)(i+1),tmppost->board,p,tmppost->number, tmppost->author, tmppost->title);
    		tmppost = tmppost->next;
		}
        fclose(fp);
    }

    /* free statistics */
	do
	{
		tmppost = toppost->next;
		del_rec(toppost,tmppost->next);
		free(tmppost);
	}
	while  (tmppost != toppost);
	for (i = 0; i < HASHSIZE ; i++ )
	{
		bucket[i] = NULL;
	}
}

//ɾ��ԭ�ȵ� etc/posts/????.0
//�� etc/posts/day.0 ������Ϊ etc/posts/day.x
//�����ʼ���¼���ʮ��
void reset(int type)
{
	char srcfile[64];
	sprintf(srcfile, "etc/posts/%s.0",myfile[type]);
	remove(srcfile);
}


int	main(int argc,char ** argv) {
#ifdef DEBUG
	int i, j;
	char buf[64];
	time_t t;
#endif
    struct tm *today;

#ifndef DEBUG
	if (argc < 2) {
		printf("Usage:\t%s bbshome [day]\n", argv[0]);
		return (-1);
	}
#endif
	chdir(argv[1]);
	if (argc == 3) {
		poststat(atoi(argv[2]));
		return (0);
	}
	time(&now);
    today = localtime(&now);
#ifdef DEBUG
	printf("mon:%x\tday:%d\thour:%d\tmin:%d\n", today->tm_mon , today->tm_mday , today->tm_hour,today->tm_min);
#endif
/*
	for (i = 0 ; i < 24 ; i++ )
	{
		for (j = 0; j < 60 ; j+=15 )
		{
			sprintf(buf, "tmp/.post.71%d%d", i, j);
			rename(buf,"tmp/post");
			poststat(0);
	printf("mon:%x\tday:%d\thour:%d\tmin:%d\n", today->tm_mon , today->tm_mday , today->tm_hour,today->tm_min);
		}
	}
	*/
#ifdef DEBUG
	printf("mon:%x\tday:%d\thour:%d\tmin:%d\n", today->tm_mon , today->tm_mday , today->tm_hour,today->tm_min);
#endif


// ���hour��0��,����0���ǰ10���ӵ��õ�main
    //
	//���ɵ���ʮ��
    poststat(0);
    today = localtime(&now);
//    today = localtime(&now);
	//�賿0���ʱ��
	if (today->tm_hour == 0 && today->tm_min <10) {
		//���ɱ�����ʮ��
		poststat(1);
		//���ɱ���һ�ٴ�
		poststat(2);
		//���ɱ���һ����ʮ��
		poststat(3);

		today = localtime(&now);
#ifdef DEBUG
		printf("mon:%x\tday:%d\thour:%d\tmin:%d\n", today->tm_mon , today->tm_mday , today->tm_hour,today->tm_min);
#endif
		//ɾ���ɵ�day.0��־�ļ������¿�ʼͳ��ÿ��ʮ��
		reset(0);

        //���week day������,ɾ��week.0��־�ļ������¿�ʼͳ��ÿ����ʮ��
       if (today->tm_wday == 0){
			reset(1);
		}

        //���month day��1��,ɾ��month.0��־�ļ������¿�ʼͳ��ÿ��һ�ٴ�
        if (today->tm_mday == 1){
            reset(2);

		}
		
		//�����1��1�ţ�ɾ��year.0��־�ļ������¿�ʼͳ��ÿ��һ����ʮ��
	//	today = localtime(&now);
		if (today->tm_yday == 0)
		{
			reset(3);
		}
    }
#ifdef DEBUG
   time(&t);
   printf("time: %d\n", t - now);
#endif
   return 0;
	
}
