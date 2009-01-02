#include "libweb.h"

int main() {
	char board[80], start[80], buf[256];
	init_all();
	printf("<b>�������δ�� �� %s </b><br>\n",BBSNAME);
	printpretable_lite();

	strsncpy(board, getparm("board"), 32);
	strsncpy(start, getparm("start"), 32);
	if(!loginok) http_fatal("�Ҵҹ����޷�ִ�д������, ���ȵ�¼");
	if(!has_read_perm(&currentuser, board)) http_fatal("�����������");
	brc_initial(currentuser.userid, board);
	brc_clear(NA, NULL, YEA);
	brc_update(currentuser.userid, board);
	sprintf(buf, "bbsdoc?board=%s&start=%s", board, start);
	refreshto(buf, 0);
	http_quit();
}
