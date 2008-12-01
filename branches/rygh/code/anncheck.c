#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BBSDISKPATH "/home/bbs/0Announce/groups/"
#define LOGPATH "/home/bbs/0Announce/groups/system.faq/Yyzabdc/auto/"
#define DOTNAMES ".Names"
#define MAXQUEUEDEEP 8192
#define MAXDIRFILES 1024
#define DIRPATHLEN 1024
#define STRLEN 255

char arrDirs[MAXQUEUEDEEP][DIRPATHLEN];
int dirLayers[MAXQUEUEDEEP];
int intArrDirsHead;
int intArrDirsTail;
FILE *noexist, *toolong, *toodeep, *loop;

//�ж�һ��·���Ƿ��������Ŀ¼������Ǵ��ڵ�Ŀ¼�򷵻�1�����򷵻�0��
int isDir(char *strFileName){
	struct stat st;
	return ( stat( strFileName, &st ) == 0 && S_ISDIR( st.st_mode ) );
}

//�ж�һ��·���Ƿ���ڣ�����Ǵ��ڵ��򷵻�1�����򷵻�0��
int isExist(char *strPath){
	struct stat st;
	return ( stat( strPath, &st ) == 0 );
}

//���һ��Ŀ¼
convertDir(int intDirIndex){

	char strBuf[STRLEN], strHtmDirName[STRLEN], strPath[STRLEN], *ptr;
	FILE *fl;

	//.Names�ж����ļ��б�
	sprintf( strBuf, "%s/%s", arrDirs[ intDirIndex ], DOTNAMES );
	if( fl = fopen( strBuf, "rt" ) ){

		while( !feof( fl ) ){

			//�õ���һ�е�����
			fgets( strBuf, STRLEN, fl );
			if( feof( fl ) ) break;

			//ȡ��һ���ļ���ʾ����ȷ����ʽ����ȷ��
			strBuf[ strlen( strBuf ) - 1 ] = '\0';
			if( ( ptr = strstr( strBuf, "Name=" ) ) && strlen( strBuf ) > 5 ){

				//�õ������� Name= ����һ�е�����
				fgets( strBuf, STRLEN, fl );
				if( feof( fl ) ) break;
				strBuf[ strlen( strBuf ) - 1 ] = '\0';

				//ȡ��һ���ļ�������ʽӦ���� Path=~/ �� �����Ƿ���� .. �޳�ѭ��Ŀ¼
				if( ( ptr = strstr( strBuf, "Path=~/" ) ) && strlen( strBuf ) > 7 ){
					strcpy( strPath, ptr + 7 );
					if( strstr( strPath, "/" ) || strstr( strPath, "\\" ) ){
						//����������Ŀ¼������
						fprintf( loop, "�����ļ���ѭ��Ŀ¼��%s\n\n", arrDirs[ intDirIndex ] );
					}else{

						//����ȫ·��
						sprintf( strBuf , "%s/%s", arrDirs[ intDirIndex ], strPath );

						//�ж�����ļ��Ƿ���Ŀ¼�������Ŀ¼����������Ŀ¼�����У��������������ĵ������С�
						if( isDir( strBuf ) ){
							if( strlen( strBuf ) > 130 )
								fprintf( toolong, "·����������%s\n\n", strBuf );
							//��Ŀ¼��д�������Ŀ¼����
							strcpy( arrDirs[ intArrDirsTail ], strBuf );
							dirLayers[ intArrDirsTail ] = dirLayers[ intDirIndex ] + 1;
							if( dirLayers[ intArrDirsTail ] > 9 )
								fprintf( toodeep, "Ŀ¼���̫�%s\n\n", strBuf );
							intArrDirsTail ++;
						}else if( isExist( strBuf ) ){
							if( strlen( strBuf ) > 130 )
								fprintf( toolong, "·����������%s\n\n", strBuf );
						}else{
							//����Ŀ¼���
							fprintf( noexist, "�����ڵ�·����%s\n\n", strBuf );
						}

					}
				}else if( strlen( strBuf ) == 7 ){
					//������������Ŀ¼����
					fprintf( noexist, "�����ļ��п�Ŀ¼��%s\n\n", arrDirs[ intDirIndex ] );
				}
			}

		}

	}

	intArrDirsHead ++;

	//�ر�.Names
	fclose( fl );

}

//����һ����ľ�����
void convertBoard(char *strBoardName){

	char strBuf[STRLEN];

	intArrDirsHead = 0;
	intArrDirsTail = 0;
	sprintf( strBuf, "%s%s", BBSDISKPATH, strBoardName );
	strcpy( arrDirs[0], strBuf );
	dirLayers[0] = 0;
	intArrDirsTail ++;

	while( intArrDirsHead < intArrDirsTail )
		convertDir(intArrDirsHead);

}

//����һ���������а���
void convertPart(char *strPartName){

	char strBuf[STRLEN], strBoardName[STRLEN];
	FILE *fl;

	//��ls��Ŀ¼�е��ļ����б����BBSDISKPATH�е�list_txt�ļ���
	sprintf( strBuf, "ls %s%s > %sboard_txt", BBSDISKPATH, strPartName, BBSDISKPATH );
	system( strBuf );

	//��list_txt�ж����ļ��б�
	sprintf( strBuf, "%sboard_txt", BBSDISKPATH );
	if( fl = fopen( strBuf, "rt" ) )
		while( !feof( fl ) ){

			//ȡ��һ���ַ���
			fgets( strBuf, STRLEN, fl );
			if( feof( fl ) ) break;

			//�õ����������İ����������·����
			strBuf[ strlen(strBuf) - 1 ] = '\0';
			sprintf( strBoardName , "%s/%s", strPartName, strBuf );
			sprintf( strBuf , "%s%s", BBSDISKPATH, strBoardName );

			//�ж�����ļ��Ƿ���Ŀ¼�������Ŀ¼�����convertBoard����������ľ�������
			if( isDir( strBuf ) ) convertBoard( strBoardName );

		}

	//�ر�list_txt
	fclose(fl);

}

main(){

	char strBuf[STRLEN];

	//�򿪴�Ų�ѯ����ĸ����ļ�
	sprintf( strBuf, "%snoexist", LOGPATH );
	if( ( noexist = fopen(strBuf, "w") ) == NULL ) exit(0);
	fprintf( noexist, "Ŀ¼�б�ʼ��\n\n" );

	sprintf( strBuf, "%stoolong", LOGPATH );
	if( ( toolong = fopen(strBuf, "w") ) == NULL ) exit(0);
	fprintf( toolong, "Ŀ¼�б�ʼ��\n\n" );

	sprintf( strBuf, "%stoodeep", LOGPATH );
	if( ( toodeep = fopen(strBuf, "w") ) == NULL ) exit(0);
	fprintf( toodeep, "Ŀ¼�б�ʼ��\n\n" );

	sprintf( strBuf, "%sloop", LOGPATH );
	if( ( loop = fopen(strBuf, "w") ) == NULL ) exit(0);
	fprintf( loop, "Ŀ¼�б�ʼ��\n\n" );

	//������о�������Ŀ¼
	convertPart( "system.faq" );
	convertPart( "ccu.faq" );
	convertPart( "comp.faq" );
	convertPart( "rec.faq" );
	convertPart( "music.faq" );
	convertPart( "literal.faq" );
	convertPart( "sport.faq" );
	convertPart( "talk.faq" );
	convertPart( "news.faq" );
	convertPart( "sci.faq" );
	convertPart( "other.faq" );
	convertPart( "soc.faq" );

	//�ر��ļ�
	fprintf( noexist, "Ŀ¼�б��ա�\n\n" );
	fclose(noexist);
	fprintf( toolong, "Ŀ¼�б��ա�\n\n" );
	fclose(toolong);
	fprintf( toodeep, "Ŀ¼�б��ա�\n\n" );
	fclose(toodeep);
	fprintf( loop, "Ŀ¼�б��ա�\n\n" );
	fclose(loop);
}
