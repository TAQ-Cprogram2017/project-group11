//
//  Fward/BackPlay.cpp
//  MP3 Player


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
    scanf("%s",szFilePath);//输入字符串即mp3文件全路径
    getchar();//取字符
    WCHAR   wstrPath[MAX_PATH] = { 0 };//WCHAR即为wchar_t,wchar_t数据类型一般为16位或32位,
    MultiByteToWideChar(CP_ACP, 0, szFilePath, -1, wstrPath, sizeof(wstrPath));/* MultiByteToWideChar映射一个字符串到一个宽字符（unicode）的字符串,由该函数映射的字符串没必要是多字节字符组。
     为什么要使用Unicode？
    （1） 可以很容易地在不同语言之间进行数据交换。
    （2） 使你能够分配支持所有语言的单个二进制.exe文件或DLL文件。
    （3） 提高应用程序的运行效率。*/
    MCI_OPEN_PARMS mciOpen;
    /*MCI_OPEN_PARMS是一结构体类型，其结构体定义为
     typedef struct {
     DWORD_PTR   dwCallback;
     MCIDEVICEID wDeviceID;//设别ID
     LPCTSTR     lpstrDeviceType;//设备类型
     LPCTSTR     lpstrElementName;//文件名
     LPCTSTR     lpstrAlias;//别名
     } MCI_OPEN_PARMS;*/
    mciOpen.lpstrDeviceType = TEXT("mpegvideo");//只能播放wav格式，如果想播放MP3格式的，选择"mpegvideo"
    
    mciOpen.lpstrElementName = wstrPath;// 播放指定文件
    MCIERROR mciError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD)&mciOpen);//MCI全称是Media control interface即多媒体控制接口,mci错误是是声卡驱动问题

    if(mciError)
    {
        char szMsg[128]={0};
        mciGetErrorString(mciError,(WCHAR*)szMsg,128);//函数返回的错误码可以用mciGetErrorString函数进行分析
        printf("%s/n",szMsg);
        goto Error;
    }
    UINT DeviceID = mciOpen.wDeviceID ;//保存设备ID
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
            mciError = mciSendCommand(DeviceID,MCI_PAUSE,0,0);//stop 停止设备的播放或记录
            printf("已暂停\n");
        }
        if(ch == '3')
        {
            mciError = mciSendCommand(DeviceID,MCI_RESUME,0,0);//resume 恢复暂停播放或记录的设备
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
            printf("请输入数字，按Enter确认: 1 退出,2 暂停,3 继续,4 后退,5 前进，6 播放进度，7 显示命令提示\n");
        }
    }
    
Error:
    
    
    return 0;
}


DWORD GetTotalMp3Length(UINT DeviceID)//DWORD 就是 Double Word,DWORD 现在表示 32bit 无符号整数，即使以后 Windows 升级到64位，DWORD 仍然是 32bit 无符号整数
{
    MCI_STATUS_PARMS statusParms;/* 通过MCI_STATUS命令可以得到媒体控制接口设备的参数，它的返回值的类型就是MCI_STATUS_PARMS    
                                  typedef struct {
                                  DWORD_PTR dwCallback;
                                  DWORD     dwReturn;
                                  DWORD     dwItem;
                                  DWORD     dwTrack; 
                                  } MCI_STATUS_PARMS; */
    statusParms.dwItem=MCI_STATUS_LENGTH;//获得文件长度,返回信息存放于StatusParms.dwReturn中
    mciSendCommand( DeviceID, MCI_STATUS, MCI_STATUS_ITEM|MCI_WAIT,(DWORD)(LPVOID)&statusParms);
    
    return statusParms.dwReturn;
    
}


void BackPlay(UINT DeviceID)
{
    DWORD  dwLength = GetTotalMp3Length(DeviceID);
    
    MCI_STATUS_PARMS statusParms;
    MCI_PLAY_PARMS playParms;/* 定义播放参数
                                结构体：
                                typedef struct {
                                DWORD_PTR dwCallback;
                                DWORD     dwFrom;
                                DWORD     dwTo; */
    } MCI_PLAY_PARMS;
    statusParms.dwItem=MCI_STATUS_POSITION;//获取信息statusParms.dwItem，定义为MCI_STATUS_POSITION，表示让返回的statusParms.dwItem是歌曲的长度值
    mciSendCommand(DeviceID,MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms);//所有MCI命令消息都是通过mciSendCommand函数发送的，此为函数声明,获取到的歌曲长度信息放到statusParms数据结构中，
    DWORD dwCurrentPos=statusParms.dwReturn;//得到结束的长度，也就是当前播放的歌曲长度
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
    //只有有文件播放时时才可以快进,跳转到文件末尾
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

/*
 跳转到任意地方   
 MCI_SEEK_PARMS SeekParms;   
 SeekParms.dwTo = (nMinute * 60 + nSecond) * 1000;  //跳转的目标时间，时间单位为毫秒   
 mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_TO  | MCI_WAIT,(DWORD)(LPVOID)  &SeekParms)；
 */


void GetCruutPos(UINT DeviceID)//播放进度
{ 
    DWORD  dwLength = GetTotalMp3Length(DeviceID);
    
    
    MCI_STATUS_PARMS statusParms;
    MCI_PLAY_PARMS playParms;
    statusParms.dwItem=MCI_STATUS_POSITION;
    mciSendCommand(DeviceID,MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms); 
    DWORD dwCurrentPos=statusParms.dwReturn;
    
    printf("当前播放进度%d%%\n",dwCurrentPos * 100/dwLength );	
    
    
