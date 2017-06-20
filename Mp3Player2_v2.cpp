// Mp3Player2.cpp : 定义控制台应用程序的入口点。
//



#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include "atlbase.h"
#include "atlstr.h"
#pragma comment(lib,"winmm.lib")


void BackPlay(UINT DeviceID);
void ForwardPlay(UINT DeviceID);
void GetCruutPos(UINT DeviceID);


int _tmain(int argc, _TCHAR* argv[])
{
	printf(" 请输入mp3文件全路径，按回车键播放\n");
	printf(" 例如  C:\\\\1.mp3\n");
	char szFilePath[256] = {0};
	scanf("%s",szFilePath);
	getchar();	
	WCHAR   wstrPath[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, szFilePath, -1, wstrPath, sizeof(wstrPath));
	MCI_OPEN_PARMS mciOpen;	
	mciOpen.lpstrDeviceType = TEXT("mpegvideo");
	
	mciOpen.lpstrElementName = wstrPath;
	MCIERROR mciError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD)&mciOpen);
	if(mciError)
	{
		char szMsg[128]={0};
		mciGetErrorString(mciError,(WCHAR*)szMsg,128);
		printf("%s/n",szMsg);
		goto Error;
	}
	UINT DeviceID = mciOpen.wDeviceID ;
	MCI_PLAY_PARMS mciPlay;

	mciError = mciSendCommand(DeviceID,MCI_PLAY,0 ,(DWORD)&mciPlay);
	if(mciError)
	{		printf("发送 MCI_PLAY 命令失败\n");
		goto Error;
	}

	printf("请输入数字，按Enter确认: 1 退出,2 暂停,3继续,4 后退,5 前进，6 播放进度，7 显示命令提示\n");


	while(1)
	{
		char ch = getchar();
		if(ch == '1')
		{
			break;
		}
		if(ch == '2')
		{
			mciError = mciSendCommand(DeviceID,MCI_PAUSE,0,0);
			printf("已暂停\n");
		}
		if(ch == '3')
		{
			mciError = mciSendCommand(DeviceID,MCI_RESUME,0,0);
			printf("已继续\n");
		}

		if(ch == '4')
		{
			BackPlay(DeviceID);
			
		}
		if(ch == '5')
		{
			ForwardPlay(DeviceID);

		}

		
		if(ch == '6')
		{
			GetCruutPos(DeviceID);

		}

		if(ch == '7')
		{
			printf("请输入数字，按Enter确认: 1 退出,2 暂停,3继续,4 后退,5 前进，6 播放进度，7 显示命令提示\n");
		}
	}

Error:

	
	return 0;
}


DWORD GetTotalMp3Length(UINT DeviceID)
{
	MCI_STATUS_PARMS statusParms;
	statusParms.dwItem=MCI_STATUS_LENGTH;
	mciSendCommand( DeviceID, MCI_STATUS, MCI_STATUS_ITEM|MCI_WAIT,(DWORD)(LPVOID)&statusParms);
	
	return statusParms.dwReturn;

}


void BackPlay(UINT DeviceID)
{ 
	DWORD  dwLength = GetTotalMp3Length(DeviceID);

	MCI_STATUS_PARMS statusParms; 
	MCI_PLAY_PARMS playParms; 
	statusParms.dwItem=MCI_STATUS_POSITION;
	mciSendCommand(DeviceID,MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms);
	DWORD dwCurrentPos=statusParms.dwReturn;
	if(dwCurrentPos<=(dwLength/10))
	{ 
		mciSendCommand(DeviceID,MCI_SEEK,MCI_SEEK_TO_START,NULL);
		mciSendCommand(DeviceID,MCI_PLAY,0,(DWORD)(LPVOID)&playParms);
		printf("已后退到文件首部\n");
	}
	else
	{ 
		playParms.dwFrom=dwCurrentPos-(DWORD)(dwLength/10);
		mciSendCommand(DeviceID, MCI_PLAY,MCI_FROM, (DWORD)(LPVOID)&playParms);
		printf("已后退10%%\n");
	}
}


void ForwardPlay(UINT DeviceID)
{ 
	DWORD  dwLength = GetTotalMp3Length(DeviceID);

//	if(m_PSign)
//	{
		//只有又文件播放时时才可以快进,跳转到文件末尾 
		MCI_STATUS_PARMS statusParms;
		MCI_PLAY_PARMS playParms;
		statusParms.dwItem=MCI_STATUS_POSITION;
		mciSendCommand(DeviceID,MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms); 
		DWORD dwCurrentPos=statusParms.dwReturn;
		if((dwLength-dwCurrentPos)<=(dwLength/10))
		{	
			mciSendCommand(DeviceID,MCI_SEEK,MCI_SEEK_TO_END,NULL); 
			mciSendCommand(DeviceID,MCI_PLAY,0,(DWORD)(LPVOID)&playParms);
			printf("已前进到到文件尾部\n");
		}
		else
		{
			playParms.dwFrom=dwCurrentPos+(DWORD)(dwLength/10);
			mciSendCommand(DeviceID, MCI_PLAY,MCI_FROM, (DWORD)(LPVOID)&playParms);
			printf("已前进10%%\n");
		}
//	}
}




void GetCruutPos(UINT DeviceID)
{ 
	DWORD  dwLength = GetTotalMp3Length(DeviceID);

	 
	MCI_STATUS_PARMS statusParms;
	MCI_PLAY_PARMS playParms;
	statusParms.dwItem=MCI_STATUS_POSITION;
	mciSendCommand(DeviceID,MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms); 
	DWORD dwCurrentPos=statusParms.dwReturn;

	printf("当前播放进度%d%%\n",dwCurrentPos * 100/dwLength );	
	
}