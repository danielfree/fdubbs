#include "config.h"
#include "modes.h"
#include <stdbool.h>

/**
 * Get descriptions of user mode.
 * @param mode user mode.
 * @return a string describing the mode.
 */
const char *mode_type(int mode)
{
	switch (mode & 0x3fffffff) {
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
			return "��԰ҹ��";
		case LAUSERS:
			return "̽������";
		case XMENU:
			return "ϵͳ��Ѷ";
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
			return "�鿴ѶϢ";
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
		case GIVEUPBBS:
			return "������";
		case BBSST_UPLOAD:
			return "�ϴ��ļ�";
		default:
			return "ȥ������!?";
	}
}

/**
 * Get whether user is web browsing.
 * @param mode user mode.
 * @return true if web browsing, false otherwise.
 */
bool is_web_user(int mode)
{
	return (mode & WWW);
}

/**
 * Get web mode.
 * @param mode user mode.
 * @return correspoding web mode.
 */
int get_web_mode(int mode)
{
	return (mode | WWW);
}
