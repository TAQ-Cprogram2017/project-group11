// MP3.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <conio.h>
#include <vector>
#include <direct.h>
#include <io.h>

#include "song.h"
#include "MP3.h"
#include "SongListFrame.h"
#pragma warning(disable: 4996)

using namespace std;

unsigned int nShowSongs = 4;
unsigned int nAllSongs = 0;
void key(void);

void InitSongs(vector<Song> &songs)
{
	char path_buffer[MAX_PATH] = { '\0' };
	getcwd(path_buffer, MAX_PATH);	
	string str = "dir /d ";	
	str += path_buffer;
	str += "\\music";
	str += " /s/B >information.txt";

	system(str.c_str());	
	FILE *fp = fopen("information.txt", "r");
		char cname[1000] = "";
			while(fgets(cname,1000,fp)!=NULL)
			{
		
		if (0 == strlen(cname))
			break;
		string _path = (string)cname;
		string path = _path.substr(0, _path.length()-1);;
		string tmp2 = _path.substr(_path.find_last_of('\\') + 1, _path.length());
		if (tmp2.find(".") != -1)
			{
			string _name = tmp2.substr(0, tmp2.length() - 5);
			++nAllSongs;
			Song ss(_name, path);
			songs.push_back(ss);
			cout<<_name<<endl;
			}
	}

}



void mainUI()
{
	SMALL_RECT winPon = { 0, 0, 75, 30 };	
	HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD buf = { 76, 31 };
	SetConsoleWindowInfo(con, 1, &winPon);
	SetConsoleScreenBufferSize(con, buf);


	void key(void);
	vector<Song> vecSongs;
	InitSongs(vecSongs);

	if (vecSongs.size() < nShowSongs)
	{
		nShowSongs = vecSongs.size();
	}

	songListFrame(vecSongs, nAllSongs, nShowSongs);	
}