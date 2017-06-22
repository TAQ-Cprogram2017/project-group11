#include "StdAfx.h"
#include "Song.h"
#include <iostream>
#pragma warning(disable: 4996)
using namespace std;


void gotoxy(int a,int b)
{
	int x=0x0b; 
    HANDLE hOutput; 
    COORD loc; 
    loc.X = a; loc.Y=b;  
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleCursorPosition(hOutput, loc); 
}


int Song::flag = 0;

Song::Song(void)
{
}

Song::~Song(void)
{
}

string Song::getName()
{
	return name;
}

Song::Song(string name, string path)
{
	this->name = name;
	this->path = path;
}

void Song::init(int now)
{
	if(now == index)
	{
		printf("                        --->    ");
		printf("%s                 \n",name.c_str());
	}else
	{
		printf("                                ");
		printf("%s\n",name.c_str());
	}
	printf("                 * --------------------------------------*\n");
}

void Song::pauseSong()
{
	TCHAR fileName[255] = { '\0' };
	MultiByteToWideChar(CP_ACP, 0,path.c_str(),-1,fileName,strlen(path.c_str()));
	TCHAR shortName[255] = { '\0' };
	GetShortPathName(fileName,shortName,1000);
	TCHAR cmd[265] = { '\0' };
	wsprintf(cmd,L"pause %s",shortName);
	mciSendString(cmd,NULL,0,NULL);
}

void Song::playSong()
{

	TCHAR fileName[255] = { '\0' };
	MultiByteToWideChar(CP_ACP, 0,path.c_str(),-1,fileName,strlen(path.c_str()));
	TCHAR shortName[255] = { '\0' };
	GetShortPathName(fileName,shortName,1000);
	TCHAR cmd[265] = { '\0' };
	wsprintf(cmd,L"play %s repeat",shortName);
	mciSendString(cmd,NULL,0,NULL);

}

void Song::stopSong()
{
	TCHAR fileName[255] = { '\0' };
	MultiByteToWideChar(CP_ACP, 0,path.c_str(),-1,fileName,strlen(path.c_str()));
	TCHAR shortName[255] = { '\0' };
	GetShortPathName(fileName,shortName,1000);
	TCHAR cmd[265] = { '\0' };
	wsprintf(cmd,L"stop %s",shortName);
	mciSendString(cmd,NULL,0,NULL);
}

void Song::replaySong()
{
	TCHAR fileName[255] = { '\0' };
	MultiByteToWideChar(CP_ACP, 0,path.c_str(),-1,fileName,strlen(path.c_str()));
	TCHAR shortName[255] = { '\0' };
	GetShortPathName(fileName,shortName,1000);
	TCHAR cmd[265] = { '\0' };
	wsprintf(cmd,L"resume %s",shortName);
	mciSendString(cmd,NULL,0,NULL);
}

string Song::getPath()
{
	return path;
}

void Song::setIndex(int index)
{
	this->index = index;
}

