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
#include "config.h"
#include "modes.h"

// ����ģʽmode����Ӧ����������
char *ModeType(int mode) {
	switch (mode) {
		case IDLE:
			return "��������";
		case NEW:
			return "��վ��ע��";
		case LOGIN:
			return "���뱾վ";
		case DIGEST:
			return "��ȡ����";
		case MMENU:
			return "�δ��";
		case ADMIN:
			return "��·����";
		case SELECT:
			return "ѡ��������";
		case READBRD:
			return "��������";
		case READNEW:
			return "��������";
		case READING:
			return "Ʒζ����";
		case POSTING:
			return "�ĺ��ӱ�";
		case MAIL:
			return "�����ż�";
		case SMAIL:
			return "�����Ÿ�";
		case RMAIL:
			return "�����ż�";
		case TMENU:
			return "��ȵ��";
		case LUSERS:
			return "�����ķ�";
		case FRIEND:
			return "ҹ̽����";
		case MONITOR:
			return "̽������";
		case QUERY:
			return "��ѯ����";
		case TALK:
			return "ȵ��ϸ��";
		case PAGE:
			return "��ϿԳ��";
		case CHAT2:
			return "��԰ҹ��";
		case CHAT1:
			return "��Բҹ��";
		case LAUSERS:
			return "̽������";
		case XMENU:
			return "ϵͳ��Ѷ";
		case VOTING:
			return "ͶƱ";
		case BBSNET:
#ifdef FDQUAN
			return "��Ȫ����";
#else
			return "������Ȫ";
#endif
		case EDITUFILE:
			return "�༭���˵�";
		case EDITSFILE:
			return "���ֶ���";
		case GAME:
			return "��������";
		case SYSINFO:
			return "���ϵͳ";
		case DICT:
			return "�����ֵ�";
		case LOCKSCREEN:
			return "��Ļ����";
		case NOTEPAD:
			return "���԰�";
		case GMENU:
			return "������";
		case MSG:
			return "��ѶϢ";
		case USERDEF:
			return "�Զ�����";
		case EDIT:
			return "�޸�����";
		case OFFLINE:
			return "��ɱ��..";
		case EDITANN:
			return "���޾���";
		case LOOKMSGS:
			return "�쿴ѶϢ";
		case WFRIEND:
			return "Ѱ������";
		case WNOTEPAD:
			return "���߻���";
		case BBSPAGER:
			return "��·����";
		case M_BLACKJACK:
			return "��ڼ׿ˡ�";
		case M_XAXB:
			return "������֡�";
		case M_DICE:
			return "����������";
		case M_GP:
			return "���˿����";
		case M_NINE:
			return "��ؾž�";
		case WINMINE:
			return "����ɨ��";
		case M_BINGO:
			return "��������";
		case FIVE:
			return "��ս������";
		case MARKET:
			return "�����г�";
		case PAGE_FIVE:
			return "��������";
		case CHICK:
			return "����С��";
		case MARY:
			return "��������";
		case CHICKEN:
			return "�ǿ�ս����";
		case GOODWISH:
			return "������ף��";
			/*2003.04.22 added by stephen*/
		case GIVEUPBBS:
			return "������";
			/*2003.04.22 stephen add end*/
			/* added by roly */
		case 10001:
			return "WWW���";
		case 10002:
			return "JABBER";
			/* added end */
		default:
			return "ȥ������!?";
	}
}
