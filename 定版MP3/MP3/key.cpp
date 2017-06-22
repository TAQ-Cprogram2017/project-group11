
#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include "MP3.h"


char KEYS[10] = { 0 };
void second_key();
void gaimi();

void red()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);
}

void green()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
}

void yellow()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN+FOREGROUND_RED);
}

void djs(void)
{
    int m=0;
    static int s=30;
    int z;
    z=s;
    while(m>=0&&z>=0)
    {
		system("cls");
        printf("Count down:%d:%dlater try again",m,z);
        Sleep(1000);
        z--;
        if(z==0&&m>0)
        {
            z=60;
            m--;
        }
        else if(z>=60)
        {
            m++;
            z=z-60;
        }
        else if(z==-1&&m==0)
            break;

    }
    s*=2;
    system("pause");
    second_key();

}
void second_key(void)
{
    char keynd[10]={0};
    int keycountnd=0;
    while(strcmp(KEYS,keynd)!=0&&keycountnd!=-1 )
    {
        printf("\a");
        while(keycountnd>=1)
        {
            djs();
			keycountnd=-1;
        }
        if(keycountnd!=-1)
        {

          printf("please input the password again£¨you still have%dchance£©£º",1-keycountnd);
          keycountnd++;
          scanf("%s",keynd);
        }
    }
    system("cls");
    printf("password is right\n");
}


void change_key(void)
{
	FILE* cfPtr = NULL;
	cfPtr = fopen("keys.txt", "w");
	char keys[10] = { 0 };
	char keyss[10] = { 0 };
	printf("please input the new password£º");
	scanf("%s", keys);
	printf("please input the new password again£º");
	scanf("%s", keyss);
	if (strcmp(keys, keyss) != 0)
	{
		printf("error!please do it again\n");
		gaimi();

	}
	else
	{
		system("cls");
		printf("The new password works now\nPlease restart the system\n");
		system("pause");
		fprintf(cfPtr, "%s\n", keys);
		fclose(cfPtr);
	}
}
int main()
{
	FILE* cfPtr = NULL;
	cfPtr = fopen("keys.txt", "r");
	char input[10] = { 0 };
	char Keys[10];
	fscanf(cfPtr, "%s", Keys);
	fscanf(cfPtr, "%s", KEYS);
	int keycount = 0;
	green();
	printf("Welcome to the Mp3 player\n");
    printf("please input the password(3 chance£©:\n");
    scanf("%s",&input);
    while(strcmp(Keys,input)!=0&&keycount!=-1 )
    {
		red();
        printf("\a");
        printf("password is wrong\n");
        while(keycount>=2)
        {
            djs();
			keycount=-1;
        }
        if(keycount!=-1)
        {

          printf("\t\t\t\t\t input password again£¨you still have%dchance£©£º",2-keycount);
          keycount++;
          scanf("%s",input);
        }
    }
    system("cls");
	green();
    printf("password is right\n");
   gaimi();
   return 0;
}

void gaimi(void)
{
     int y;
	 yellow();
     printf("select0:enter the Mp3 player\n");
	 printf("select1:change the password\n");
     scanf("%d",&y);
     if(y!=0&&y!=1)
     {
		 red();
         printf("error\n");
         gaimi();
     }
    else if(y==1)
       {
            change_key();
       }
	else if(y==0)
	{
	 mainUI();
	 system("pause");
	}
}


