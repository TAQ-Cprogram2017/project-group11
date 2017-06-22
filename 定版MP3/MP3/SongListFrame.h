#pragma once
#include <vector>
#include <conio.h>
#include <io.h>
#include "Song.h"

const int NUM_PER_PAGE = 4;
using namespace std;

void songListFrame(vector<Song> &s,int allSongs,int showSongs);

void showList(int index,int current_page,int allSong, Song s[],int songCount,string name);
void AddSongs( vector<Song>&,int& allsongs );
void NextPage( vector<Song>& songs, Song temp[], int allsongs, int& current_page );
void PrevPage( vector<Song>& songs, Song temp[], int allsongs, int& current_page );
int GetMaxPage( int allsongs );
