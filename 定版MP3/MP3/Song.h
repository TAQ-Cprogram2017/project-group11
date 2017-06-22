#pragma once
#include <windows.h>
#pragma comment(lib,"winmm.lib")
#include <string>
#include <iostream>
#include <vector>
#include <atlstr.h>
#include <time.h>
#include <process.h>

using namespace std;

class Song
{
public:
	static int flag;
	
	Song(void);
	~Song(void);
	Song(string name,string path);
	void setIndex(int name);
	string getPath();
	string getName();
	void init(int now);
	void pauseSong();
	void playSong();
	void stopSong();
	void replaySong();
	

private:
	string name;
	string path;
	int index;
};
