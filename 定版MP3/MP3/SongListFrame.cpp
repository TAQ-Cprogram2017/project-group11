#include "StdAfx.h"
#include "SongListFrame.h"

void setVolume(unsigned int &vol)
{
	HWAVEOUT hWave;
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM; 
	waveFormat.cbSize = sizeof( WAVEFORMATEX );
	waveFormat.nChannels = 2;  
	waveFormat.nSamplesPerSec = 44100L;
	waveFormat.nAvgBytesPerSec = 176400L;  
	waveFormat.nBlockAlign = 4;
	waveFormat.wBitsPerSample = 16;
	if( waveOutOpen( &hWave , WAVE_MAPPER , &waveFormat ,NULL , 0 , CALLBACK_WINDOW ) )
	{
		printf("Open Failed!");
		exit(0);
	}
	DWORD currentVolume;
	waveOutGetVolume( hWave , &currentVolume );	
	currentVolume = vol;
	waveOutSetVolume( hWave , currentVolume ); 
}

void showList(int index,int current_page,int allSong, Song s[],int songCount,string name)
{
	int i;
	
	cout << "	   * * * * * * * * * * * * * * * * * * * * * * * * * * * " << endl;;
	cout << "	   *            Welcome to the Mp3 player!             *" << endl;
	cout << "	   * * * * * * * * * * * * * * * * * * * * * * * * * * * " << endl << endl;
	cout << "                 *                                       *" << endl;
	cout << "                 *        enter  :choose                 * " << endl;
	cout << "                 *         q     :stop song              *" << endl;
	cout << "                * *       space  :play&pause             * * " << endl;
	cout << "               * * *      ←     :previous song        * * * " << endl;
	cout << "                * *       →     :next song             * * " << endl;
	cout << "                 *        ↑     :up                     * " << endl;
	cout << "                 *        ↓     :down                   * " << endl;
	cout << "                 *        s      :sound-                 * " << endl;
	cout << "                 *        w      :sound+                 * " << endl;
	cout << "                 *        a      :add songs              * " << endl;
	cout << "                 *        n      :next page              * " << endl;
	cout << "                 *        p      :last page              * " << endl;
	cout << "                 *                                       * " << endl << endl;
	for( i=0;i<songCount&& (( current_page - 1 )*NUM_PER_PAGE + i)<allSong;i++)
	{
		s[i].init( index );
	}
	cout << "now is  " << current_page << "  page,in total  " << GetMaxPage( allSong ) << "  page," <<allSong<<"  music"<< endl;
	cout<<"\n\nPlaying song:"<<name;
}

void songListFrame(vector<Song> &s,int allSongs,int showSongs)
{
	
	int max_page = GetMaxPage( allSongs );
	int current_page = 1;
	Song temp[NUM_PER_PAGE];
	int nowIndex=0;//the index of song(now)
	int tempIndex=0;//the index of song(now showing in the list)
	int playIndex=-1;//the index of song(playing)
	int hitCount = 0;//hit? times the song pause
	for(int i=0;i < allSongs;i++)
	{
		s[NUM_PER_PAGE*(current_page-1)+i].setIndex(i);
	}
	for(int i=0;i < showSongs;i++)
	{
		temp[i]=s[NUM_PER_PAGE*(current_page-1) +i ];
	}
	unsigned int vol = 0xFFFFFFFF;
	showList(nowIndex,current_page,allSongs,temp,showSongs,"NO");
	while(1)
	{
		char c = _getch();
		//↓
		if(c==80)
		{
			if(nowIndex==(allSongs-1)){
				nowIndex=0;
				tempIndex=0;
				NextPage( s, temp, allSongs, current_page );
				int j;
				if(playIndex == -1)
				{
					system("cls");
					showList(nowIndex,current_page, allSongs, temp,showSongs,"NO");
				}else
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
				}
			}else if(tempIndex==3)
			{
				nowIndex=nowIndex+1;
				tempIndex = 0;
				NextPage( s, temp, allSongs, current_page );
				int j,i=nowIndex-3;
				if(playIndex == -1)
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp,showSongs,"无");
				}else
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
				}
			}else
			{
				nowIndex=nowIndex+1;
				tempIndex=tempIndex+1;
				if(playIndex == -1)
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp,showSongs,"无");
				}else
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
				}
			}
		//↑
		}else if(c==72)
		{
			if(tempIndex==0)
			{
				if ( current_page == 1 ) {
					nowIndex = allSongs-1;
					tempIndex = allSongs-(max_page-1)*NUM_PER_PAGE-1;
				}
				else {
					nowIndex--;
					tempIndex = 3;
				}
				PrevPage( s, temp, allSongs, current_page );
				
				int j,i=(nowIndex-3)>=0?nowIndex-3:0;
				if(playIndex == -1)
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp,showSongs,"无");
				}else
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
				}
			}else
			{
				nowIndex=nowIndex-1;
				tempIndex=tempIndex-1;
				if(playIndex == -1)
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp,showSongs,"无");
				}else
				{
					system("cls");
					showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
				}
			}
		//enter
		}else if(c==13)
		{
			if (playIndex == nowIndex || playIndex == -1)
			{
				s[nowIndex].playSong();
				hitCount = 0;
			}
			else{
				s[playIndex].stopSong();
				s[nowIndex].playSong();
				hitCount = 0;
			}
			playIndex = nowIndex;
			showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());

		//space
		}else if(c==32)
		{
			if(playIndex == -1)
				continue;
			++hitCount;
			if(1 == hitCount%2)
			{
				s[playIndex].pauseSong();
			}else
			{
				s[playIndex].replaySong();
			}
		//q
		}else if(c=='q')
		{
			s[playIndex].stopSong();
			showList(nowIndex, current_page, allSongs, temp, showSongs, "NO");
		//->
		}else if(c==77)
		{
			if (playIndex + 1 == s.size())
			{
				s[playIndex].stopSong();
				s[0].playSong();
				playIndex = 0;
			}
			else if (playIndex == -1)
			{
				s[0].playSong();
				playIndex = 0;
			}
			else{
				s[playIndex].stopSong();
				s[playIndex + 1].playSong();
				playIndex = playIndex + 1;
			}
			showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
		//<-
		}else if(c==75)
		{
			if (playIndex == 0)
			{
				s[playIndex].stopSong();
				playIndex = s.size() - 1;
				s[playIndex].playSong();
			}
			else if (playIndex == -1)
			{
				s[0].playSong();
				playIndex = 0;
			}
			else{
				s[playIndex].stopSong();
				playIndex = playIndex - 1;
				s[playIndex].playSong();
			}
			showList(nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName());
		
		}
		else if ( c == 'a' ) {
			AddSongs( s,allSongs );
			system( "pause" );
			if ( playIndex == -1 )
			{
				showList( nowIndex, current_page, allSongs, temp, showSongs, "NO" );
			}
			else
			{
				showList( nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName() );
			}
		}
		//s
		else if(c == 's')
		{
			if(vol != 0x0000000f)
			{
				vol = vol >> 4;
			}else{
				vol = 0x0000000f;
			}
			setVolume(vol);
		//w
		}else if(c == 'w')
		{
			if(vol != 0xffffffff)
			{
				vol = (vol << 4) | 0x0f;
			}else{
				vol = 0xffffffff;
			}
			setVolume(vol);
		}
		else if ( c == 'n' ) {
			NextPage( s, temp, allSongs, current_page );
			nowIndex = (current_page-1)*NUM_PER_PAGE;
			tempIndex = 0;
			if ( playIndex == -1 )
			{
				showList( nowIndex, current_page, allSongs, temp, showSongs, "NO" );
			}
			else
			{
				showList( nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName() );
			}
		}
		else if ( c == 'p' ) {
			PrevPage( s, temp, allSongs, current_page );
			nowIndex = (current_page-1)*NUM_PER_PAGE - 1;
			tempIndex = 0;
			if ( playIndex == -1 )
			{
				showList( nowIndex, current_page, allSongs, temp, showSongs, "NO" );
			}
			else
			{
				showList( nowIndex, current_page, allSongs, temp, showSongs, s[playIndex].getName() );
			}
		}
	}
}

void AddSongs( vector<Song>& songs, int& allsongs )
{
	printf("\nPlease input the address of the file\n" );
	string folder, filePath;
	cin >> filePath;
	folder = filePath + "\\*.mp3";
	struct _finddata_t fd;
	long handle = _findfirst( folder.c_str(), &fd );
	if ( handle == -1 ) {
		printf("Error or No mp3 in the file！\n" );
		return;
	}
	int sum = 1;
	Song temp( fd.name, filePath + "\\" + fd.name );
	temp.setIndex( songs.size() );
	allsongs++;
	songs.push_back( temp );
	cout << "FIND" << fd.name << endl;
	while ( !_findnext( handle, &fd ) ) 
	{
		Song temp( fd.name, filePath + "\\" + fd.name );
		temp.setIndex( songs.size() );
		songs.push_back( temp );
		cout << "FIND" << fd.name << endl;
		sum++;
		allsongs++;
	}
	printf("All the songs are added，in total %d music!",sum);

	_findclose( handle );

}

void NextPage( vector<Song>& songs,Song temp[], int allsongs, int& current_page ) {
	system("cls");
	int max_page = GetMaxPage( allsongs );
	if ( current_page == max_page ) current_page = 1; 
	else { current_page++; }
	int start = NUM_PER_PAGE * (current_page-1);
	for ( int i = 0; start < allsongs && i < NUM_PER_PAGE  ; i++,start++ ) {
		temp[i] = songs[start];
	}
}

void PrevPage( vector<Song>& songs, Song temp[], int allsongs, int& current_page ) {
	system("cls");
	int max_page = GetMaxPage( allsongs );
	if ( current_page == 1 ) current_page = max_page;
	else { current_page--; }
	int start = NUM_PER_PAGE * (current_page-1);
	for ( int i = 0; start < allsongs && i < NUM_PER_PAGE; i++, start++ ) {
		temp[i] = songs[start];
	}
}

int GetMaxPage( int allsongs ) {
	double cmp1 = ((double)allsongs) / ((double)NUM_PER_PAGE);
	double cmp2 = ( double ) ( int ) (allsongs / NUM_PER_PAGE);
	if ( cmp1 == cmp2 ) return ( int ) cmp1;
	return ( int ) ( cmp2 + 1 );
}