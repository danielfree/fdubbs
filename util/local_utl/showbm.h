#ifndef _SHOWBM_H_
#define _SHOWBM_H_

#define NOFILEDAYS      30      /* ��������������, >NOFILEDAYS�Ժ�ɫ��ʾ */
#define NO0FILEDAYS     30      /* ������δ��������, >NO0FILEDAYS�Ժ�ɫ��ʾ*/
#define BMNOLOGIN       30      /* ���δ��վ����, >BMNOLOGIN�Ժ�ɫ��ʾ */
#define BMNOFILEDAYS    30      /* BM����δ��������, >BMNOFILEDAYS�Ժ�ɫ��ʾ*/
#define DAYSN           7       /* �޸ı�������ͬ���޸�����head���� */
#define REDMARK		"\x1B[1;31m"	/* ʹ����������ɺ�ɫ*/
#define ENDREDMARK	"\x1B[m"	/* ʹ������ֱ��ԭ������ɫ*/

/* ����б�Ҫ�޸���������, ��ע��ո����Ŀ */
/*
  const char *head =
"\x1B[1;33;44m��������            ���ܰ���������      ��������������  δ������������         ����ID             ��������������      ����δ��վ����   ����δ�������� \x1B[0m\n\n";
*/

const char *head=
"\033[1;33;44m"
 "                  ����  ������ δ����               ����   ����  δ��վ δ����\033[m\n"
"\033[1;33;44m"
 "��������         ������ ������  ����  ����ID       ������ ������  ����   ���� \033[m\n";
//BBS_Game             65     49      1 lemonyu          17    151      0      0

#endif

