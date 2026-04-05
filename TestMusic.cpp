#include <windows.h>
#include <tchar.h>
#include <stdio.h>
void DebugLog(const char * s)
{
	printf(s);
	putchar('\n');
}
#define LODWORD(x)  (*((DWORD*)&x))  // low dword
#define HIDWORD(x)  (*((DWORD*)&x+1))
#include "cplayer.h"
int main()
{
	CPlayer cplayer;
	CPlayer cplayer2;
	BOOL ret = cplayer.Open("mus_vsasgore.wav");
	printf("Opening return value = %2d\n",ret);
	cplayer.Play();
	DWORD len = cplayer.GetLength();
	/*while(cplayer.GetPos() < len)
        Sleep(1000);*/
    Sleep(3000);
    BOOL ret2 = cplayer2.Open("snd_asgore_voice.wav");
	printf("Opening return value = %2d\n",ret2);
	cplayer2.Play();
	
    cplayer.Stop();
    cplayer.Close();
    cplayer2.Stop();
    cplayer2.Close();
    return 0;
}
