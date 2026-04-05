/*****************************
 *        Underspace         *
 * Author:Wormwaker          *
 *    StartDate: 20220928    *
 * Description:              *
 *      Undertale-like game  *
 *      QQ: 2399347979       *
 *****************************
 */
//64位程序 。 
//#define NOSOUND
//#define NOSHUFFLE
#define CRT_VER "v1.0.0.3"
#define SKIP_TO_ROUND 141

//-lgdi32 -luser32 -mwindows -lwininet -lws2_32 -lgraphics64 -luuid -lmsimg32 -limm32 -lole32 -loleaut32 -lgdiplus -lwinmm
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <io.h>
#include <tchar.h>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
#include "graphics.h"	//ege
#define CJZAPI
void DebugLog(string text,bool bTime=true);
#define LODWORD(x)  (*((DWORD*)&x))  // low dword
#define HIDWORD(x)  (*((DWORD*)&x+1))
#define _USER_MCI_COMMAND
#include "str.cpp" 
#include "cplayer64.h"
//#define NOLOG

#define ENTITYDATA_PATH "EntityData.csv"

#define XCPT_L try{
#define XCPT_R }catch(exception& e)\
				{\
					ErrorTip(NULL, e.what(), __func__);\
				}
#define sqrt2 pow(2,0.5)
#define UT_ORIGIN_WHITE EGERGB(255,255,255)
#define UT_WHITE EGERGB(250,250,250)
inline bool IsUTWhite(color_t clr)
{
	return (EGEGET_R(UT_WHITE) <= EGEGET_R(clr) && EGEGET_G(UT_WHITE) <= EGEGET_G(clr) && EGEGET_B(UT_WHITE) <= EGEGET_B(clr)
		&&  EGEGET_R(UT_ORIGIN_WHITE) >= EGEGET_R(clr) && EGEGET_G(UT_ORIGIN_WHITE) >= EGEGET_G(clr) && EGEGET_B(UT_ORIGIN_WHITE) >= EGEGET_B(clr));
}
#define UT_ORIGIN_BLUE EGERGB(74,201,255)
#define UT_BLUE EGERGB(72,199,253)
#define UT_ORIGIN_ORANGE EGERGB(255,138,28)
#define UT_ORANGE EGERGB(253,136,26)

#define fzero(f) (abs(f-0.0f)<0.01f)
#define fequ(f1,f2)	(abs(f1-f2)<0.01f)

#ifndef KEY_DOWN
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)& 0x8000 ? 1:0))
#define KEY_DOWN_FOREMOST(hWnd,vk) (KEY_DOWN(vk) && GetForegroundWindow()==hWnd)	//最前面 
#		define KEY_DOWN_FOCUSED(hWnd,vk) KEY_DOWN_FOREMOST(hWnd,vk)	//带焦点 
#endif
#define K(sk) KEY_DOWN_FOCUSED(hwnd,sk)
#define LEFT_KEYS (K(VK_LEFT) || K('A'))
#define DOWN_KEYS (K(VK_DOWN) || K('S'))
#define RIGHT_KEYS (K(VK_RIGHT) || K('D'))
#define UP_KEYS (K(VK_UP) || K('W'))
#define QUICK_KEYS (K(VK_CONTROL))
#define SLOW_KEYS (K(VK_SHIFT))
#define ESCAPE_KEYS (K(VK_ESCAPE))

HWND hwnd=nullptr;
float g_uix,g_uiy;
clock_t lastDraw; 
clock_t lastFrame;
clock_t lastRound;
clock_t lastMsg;
string g_msg;
long g_round=0;
bool g_playing=false;



long g_temp[20] {0L};
size_t GetEntityCount(void);
float GetEntityX(short);
float GetEntityY(short);
#define blue_left abs(g_temp[0])
#define orange_top abs(g_temp[1])
#define blue_k g_temp[0]/abs(g_temp[0])
#define orange_k g_temp[1]/abs(g_temp[1])
inline void BlueLeft(const long& step)
{
	g_temp[0] += step;
}
inline void OrangeTop(const long& step)
{
	g_temp[1] += step;
}
typedef short ENTITY_ID, AI_ID;
void SpawnEntityEx(ENTITY_ID id, 
                   float x, float y, 
				   float v, float theta, 
                   float r=0.0f, float ox=-1.0f, float oy=-1.0f,const char attrib = '$', bool mute=false);

#define ROUND_T 12 * 1000	//Round Time

inline INT CJZAPI GetScreenHeight(void) //获取屏幕高度 
{
	return GetSystemMetrics(SM_CYSCREEN);
}
inline INT CJZAPI GetScreenWidth(void) //获取屏幕宽度 
{
	return GetSystemMetrics(SM_CXSCREEN);
}
inline RECT CJZAPI GetSystemWorkAreaRect(void) //获取工作区矩形 
{
	RECT rt;
    SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);    // 获得工作区大小
    return rt;
}
inline LONG CJZAPI GetSystemWorkAreaHeight(void) //高 
{
	RECT rt=GetSystemWorkAreaRect();
	return (rt.bottom - rt.top);
}
inline LONG CJZAPI GetSystemWorkAreaWidth(void)	//宽 
{
	RECT rt=GetSystemWorkAreaRect();
	return (rt.right - rt.left);
}
HANDLE CJZAPI CreateThread(LPTHREAD_START_ROUTINE func_name, LPVOID args=NULL)
{ //函数格式必须为 DWORD FUNC(LPVOID XXX)
	return CreateThread(NULL,0,func_name,args,0,NULL);
}
template <typename _T>
   inline string CJZAPI ToString(_T value)
   {
   	     stringstream ss;
   	     ss<<value;
         return ss.str(); 
	} 
template <typename _T>
_T CJZAPI Clamp(_T val,_T min=0,_T max=2147483647)	//限定范围 
{
	if(val < min) val = min;
	else if(val > max) val = max;
	return val;
}
template <typename _T>
_T CJZAPI ClampA(_T& val,_T min=0,_T max=2147483647)	//限定范围 
{
	if(val < min) val = min;
	else if(val > max) val = max;
	return val;
}
template <typename _Type>
	inline _Type CJZAPI pow2(_Type x)
	{
		return x * x;
	}
inline BOOL CJZAPI ExistFile(string strFile)
{	//文件或文件夹都可以 
	return !access(strFile.c_str(),S_OK);//S_OK表示只检查是否存在 
}


inline VOID CJZAPI ErrorTip(HWND hwnd, string txt, string title)
{
 	MessageBox(hwnd, txt.c_str(), title.c_str(),
	               MB_ICONERROR|MB_OK|MB_SYSTEMMODAL);
}
#define MSG_SND_TIP_SOUND MB_ICONINFORMATION
inline VOID CJZAPI MsgSndTip(VOID)
{
	MessageBeep(MSG_SND_TIP_SOUND);
}
vector<string> CJZAPI ReadFileLines(string filePath,int lineMaxCharCnt=65535)
{	//读取文件每一行 
	vector<string> ret {} ;
	fstream fin(filePath,ios::in);
	if(fin.fail())
		return ret;
	while(1)
	{
		CHAR s[lineMaxCharCnt];
		
		if(fin.eof())
		   break;
  
        fin.getline(s,lineMaxCharCnt);
//        cout<<s<<endl;
        ret.push_back(ToString(s));
	}
	fin.close();
	return ret;
}
vector<string> CJZAPI CSVCutLine(string line,char sep=',')
{	//数据分开，单独取出 
	//CSV 逗号分隔值文件 , 处理略有不同 
	vector<string> data {} ;
	int p1=0,p2=0;
	for(int i=0;i<line.size();i++)
	{
		if(line.at(i) == sep 
		|| /*line.at(i) == '\n' NO USE*/
		i==line.size()-1)	//别漏了最后一个数据 
		{
			p2=i;
			auto one = line.substr(p1,p2-p1
					  					  + ((i==line.size()-1
											&&	line.at(i)!=sep)	//防止最后多个逗号 
											?1:0) );	//别漏了最后一个字符 
			data.push_back(one);	//<!>警告：有可能会放空的str 
			p1=i+1;
		}
	}
	return data;
}
string CJZAPI ResplitLine(string line,char sep=',',char newSep=' ')
{	//重新分隔 并合并成一句 
	vector<string> data=CSVCutLine(line,sep);
	string ret;
	for(int i=0;i<data.size();i++)
	{
		ret += data.at(i);
		if(i != data.size()-1)
		    ret += newSep;
	}
	return ret;
}
//↓带缩放的getimage()函数（原来的zoomWidth/zoomHeight不管用，是个bug） 
VOID CJZAPI EGEAPI getimage_withzoom(PIMAGE pDstImg,
                                     LPCSTR lpFile,
									 UINT zoomWidth,
									 UINT zoomHeight)
{
	getimage(pDstImg,lpFile);
		//Resize image to pwidth*pheight
	PIMAGE save=gettarget();
		//Get image size
	settarget(pDstImg);
	int width,height;
	width=getwidth();
	height=getheight();
	settarget(save);
	PIMAGE backproc=newimage(zoomWidth,zoomHeight);
	//Resize
	putimage(backproc,0,0,zoomWidth,zoomHeight,pDstImg,0,0,width,height); 
	getimage(pDstImg,backproc,0,0,zoomWidth,zoomHeight);
	delimage(backproc);
}
void EGEAPI _getimage(PIMAGE pDstImg, LPCSTR lpFile)
{
	if (!ExistFile(lpFile))
	{
		string info = (string)"找不到指定图像:" + lpFile;
		ErrorTip(NULL, info.c_str(), "_getimage");
		return;
	}
	getimage(pDstImg, lpFile);
}
VOID EGEAPI _getimage_withzoom(PIMAGE pDstImg,
	LPCSTR lpFile,
	UINT zoomWidth,
	UINT zoomHeight)
{
	//先检查，更保险！
	if (!ExistFile(lpFile))
	{
		ErrorTip(NULL,"找不到图像：" + (string)lpFile, "_getimage_withzoom");
		return;
	}
	getimage_withzoom(pDstImg, lpFile, zoomWidth, zoomHeight);
}
int GetHour(void){
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_hour);
}
int GetMinute(void){
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_min);
}
int GetSecond(void){
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_sec);
}
#define LOG_NAME "prevPlaying.log"
#ifndef NOLOG
void DebugLog(string text,bool bTime/*=true*/)
{	//日志输出 
	FILE *fp=fopen(LOG_NAME,"a+");
	//fprintf(fp,"\n%s%s",bTime?(("["+string(ToTimeStr(time(0)))+"] ").c_str()):"",text.c_str());
	fprintf(fp, "\n%s%s", bTime ? (("[" + ToString(GetHour()) + ":" + ToString(GetMinute()) + ":" + ToString(GetSecond()) + "] ").c_str()) : "", text.c_str());
	fclose(fp);
}
void DebugLogPoint(POINT const& pt)
{
	DebugLog("点坐标("+ToString(pt.x)+","+ToString(pt.y)+")");
}
#else
#define DebugLog(...)
#define DebugLogPoint(...) 
#endif
//Sound ID
#define SND_CRACK1 1
#define SND_CRACK2 2
#define SND_ASGORE_VOICE 3
#define SND_HURT 4
#define SND_DING 5
#define SND_LOW_WARN 6
#define SND_SPEAR 7
#define SND_GBLASTER_1 8
#define SND_GBLASTER_2 9
#define SND_STRIKE 10
#define SND_HEAL 11
#define SND_EXCLAMATION 12
#define SND_BARK 13
#define SND_MAX 13

string GetSoundPath(short uSound)
{
	string name = "snd_";
	switch(uSound)
	{
		case SND_CRACK1:
			name += "crack1";break;
		case SND_CRACK2:
			name += "crack2";break;
		case SND_ASGORE_VOICE:
			name += "asgore_voice";break;
		case SND_HURT:
			name += "hurt";break;
		case SND_DING:
			name += "ding";break;
		case SND_LOW_WARN:
			name += "lowwarn";break;
		case SND_SPEAR:
			name += "spear";break;
		case SND_GBLASTER_1:
			name += "gblaster_1";break;
		case SND_GBLASTER_2:
			name += "gblaster_2";break;
		case SND_STRIKE:
			name += "strike";break;
		case SND_HEAL:
			name += "heal";break;
		case SND_EXCLAMATION:
			name += "exclamation";break;
		case SND_BARK:
			name += "bark";break;
		default:
			DebugLog("<!> Sound: Invalid sound id="+ToString(uSound));
			break;
	}
	if(name=="snd_")	return "";
	name+=".wav";
	if(!ExistFile(name))
	{	//404
		DebugLog("<!> Sound: no such file "+name);
	}
	return name; 
}

bool g_sndTrg[SND_MAX+1]{false};
short g_tmp_id=1;
DWORD SoundProc(LPVOID lpParam)
{
	short id = g_tmp_id++;	//++是最方便的 
//	DebugLog("SoundProc id="+ToString(id));
	string path = GetSoundPath(id);
	CPlayer player;
	player.Open(path.c_str());
//	DebugLog("SoundProc path="+path);
	while(is_run() && g_playing)
	{
		if(g_sndTrg[id])
		{
			player.Play(0);
			g_sndTrg[id] = false;
		}
		api_sleep(20);	//不然会死机 
	}
//	DebugLog("End SoundProc path="+path);
	return 1L;
}
string g_mus_path="";
bool g_mus_signal=false;
DWORD MusicProc(LPVOID args)
{
	if(g_mus_path.empty() || !ExistFile(g_mus_path))
						  return 0L;
    CPlayer mus;
    mus.Open(g_mus_path.c_str());
    g_mus_signal = true;
    clock_t start = clock();
    mus.Play();
    DWORD leng = mus.GetLength();
    while(clock() - start <= leng)
    	if(!g_mus_signal)
    	    break;	//exit
	    api_sleep(50);
    mus.Stop();
    mus.Close();
    g_mus_signal = false;
	return 1L;
}
class SoundBank {
	private:
//		vector<CPlayer> chorus;
		HANDLE hMus;
	
	public:
		SoundBank() { hMus=NULL;		}
		~SoundBank()
		{
			Dispose();
		}
		void Dispose()
		{
			/*if(chorus.empty())	return;
			for(short i = 0; i < chorus.size(); ++i)
			{
				chorus[i].Stop();
				chorus[i].Close();
			}
			chorus.clear();*/
		}
		/*bool Play(string path)
		{
			CPlayer member;
			BOOL ret = member.Open(path.c_str());
			if(!ret)
			{
				DebugLog("<!> SoundBank::Play: Failed in opening " + path);
				return false;
			}
			if(hwnd != nullptr && hwnd != INVALID_HANDLE_VALUE)
			    member.SetWindowHwnd(hwnd);
//            member.SetAudio(false);
//            member.SetVolume(50);
//            DebugLog(ToString(member.GetLength()));
			chorus.push_back(member);
			return chorus.at(chorus.size()-1).Play();
		}*/
		void Prepare(short id)
		{
			/*CPlayer member;
			chorus.push_back(member);
			bool ret = chorus.back().Open(path.c_str());
			DebugLog("Preparing "+ path + (ret?" Success":" Failed"));
			return ret;*/
//			DebugLog("Prepare "+ToString(id));
//			g_tmp_id = id;
			CreateThread((LPTHREAD_START_ROUTINE)SoundProc);
		}
		void Music(const char* path)
		{
			g_mus_path = path;
			hMus = CreateThread((LPTHREAD_START_ROUTINE)MusicProc);
		}
		inline bool ExistMusic(void) const
		{
			return g_mus_signal;
		}
		void StopMusic(void)
		{
			g_mus_signal = false;
			CloseHandle(hMus);
		}
		void Play(UINT uSnd)
		{
			/*if(uSnd >= chorus.size())
			{
				DebugLog("<!> SoundBank::Play: uSnd out of prepared range");
				return;
			}*/
//			DebugLog("SoundBank::Play: uSnd="+ToString(uSnd));
//			return chorus.at(uSnd-1).Play(0);
			g_sndTrg[uSnd] = true;
		}
		/*void Refresh()
		{
			if(chorus.empty())	return;
			for(short i = 0; i < chorus.size(); ++i)
			{
				if(chorus[i].GetPos() >= chorus[i].GetLength())
				{
					chorus[i].Stop();
					chorus[i].Close();
					
					auto iter = chorus.begin();
					short j = 0;
					while(j < i)
						++iter,++j;
					iter = chorus.erase(iter);	//Delete it in the vector
					continue;
				}
			}return;
		}*/
}soundBank;

#ifndef NOSOUND

void Sound(short uSound)
{
	//sndPlaySound(name.c_str(), SND_FILENAME|SND_ASYNC);
	soundBank.Play(uSound);
//	BOOL ret = soundBank.Play("snd_spear.wav");
	/*if(!ret)
	{
		DebugLog("<!> Sound: Failed in playing");
	}*/
}
void PrepareSounds()
{
	DebugLog("PrepareSounds: 正在加载音效");
//	if(!soundBank.Empty()) soundBank.Dispose();
	for(short i = 1; i <= SND_MAX; ++i)
	{
		soundBank.Prepare(i);
	}
}
#else
void Sound(UINT uSound){}
void PrepareSounds(){}
#endif
/*
int _seed=12;
int rand2(void)
{
	return (_seed = _seed * 237 + 114514) % INT_MAX;
}*/

int Randint(int Min,int Max,bool rchMin=true,bool rchMax=true)
{	//随机整数 
	UINT a = random(UINT_MAX);
	if(rchMin && rchMax)	//[a,b]
	   return (a%(Max - Min + 1)) + Min;
    else if(rchMin && !rchMax)		//[a,b)
       return (a%(Max - Min)) + Min;
    else if(!rchMin && rchMax)		//(a,b]
       return (a%(Max - Min)) + Min + 1;
    else							//(a,b)
       return (a%(Max - Min - 1)) + Min + 1;
}
template <typename _T1,typename _T2>
_T1 CJZAPI Varience2(_T1 val,_T2 single_offset)	//波动 
{
	val += Randint(0 - single_offset,single_offset,true,true);
	return val;
}
template <typename _T1,typename _T2>
_T1 CJZAPI VarienceA2(_T1& val,_T2 single_offset)	//波动 
{
	val += Randint(0 - single_offset,single_offset,true,true);
	return val;
}
template <typename _Tp>
_Tp CJZAPI Choice(initializer_list<_Tp> choices)
{
	vector<_Tp> vec(choices);
	return vec.at(Randint(0,vec.size()-1));
}
template <typename _Tp>
_Tp CJZAPI Nth(initializer_list<_Tp> seq, int nth_zerostart)
{
	vector<_Tp> vec(seq);
	return vec.at(nth_zerostart);
}
template <typename _T>
   int CJZAPI ToInt(_T value)
   {
   	     int ret;
   	     stringstream ss;
   	     ss<<value; ss>>ret;
         return ret;
	} 
template <typename _T>
   short CJZAPI ToShort(_T value)
   {
   	     short ret;
   	     stringstream ss;
   	     ss<<value; ss>>ret;
         return ret;
	} 
template <typename _T>
   long CJZAPI ToLong(_T value)
   {
   	     long ret;
   	     stringstream ss;
   	     ss<<value; ss>>ret;
         return ret;
	} 

inline float Distance2(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}
inline float ToRadian(const float& degree)
{
	return degree * PI / 180;
}
float StdRadian(float radian)
{
	while(radian >= 2 * PI)
		radian -= 2 * PI;
	while(radian < 0)
        radian += 2 * PI;
    return radian;
}
inline float OppoRadian(const float& radian)
{
	return StdRadian(radian + PI);
}

inline float Towards(float srcx,float srcy,float dstx,float dsty)
{	//朝着 
	return atan2(-dsty+srcy, dstx-srcx);
}
#define _OFFSET_X -300
RECT midprinty(string strText, int y)
{	//自定义 y
	int fy = textheight('1');	//font height
	int fx = textwidth('A');	//font width
	int sx = fx * strText.length() / 2;	//string width
	int x = g_uix / 2 - sx / 2  + _OFFSET_X  ;
	xyprintf(x, y, "%s", strText.c_str());
	
	RECT rt;
	rt.left = x;
	rt.right = x+sx;
	rt.top = y;
	rt.bottom = y+fy;
	return rt;	//返回矩形
}
void delayprint(int x,int y,LPCSTR lpText,DWORD dwInterval = 60)
{
	size_t sz = strlen(lpText)+1;
	string s;
	s += *lpText;
	for(int i = 1; i < sz; ++i)
	{
		xyprintf(x,y,s.c_str());
		s += lpText[i];
		Sleep(dwInterval);
	}
}


//Directions
typedef BYTE DIR,*PDIR;
#define UP 0
#define RIGHTUP 1
#define RIGHT 2
#define RIGHTDOWN 3
#define DOWN 4
#define LEFTDOWN 5
#define LEFT 6
#define LEFTUP 7
inline DIR RandomDir4(void)
{
	return Randint(0,3,true,true)*2;
}
inline DIR RandomDir8(void)
{
	return Randint(0,7,true,true);
}
inline DIR NthDir4(short right_zero)
{
	return DIR((2 + right_zero * 2) % 8);
}
inline DIR NthDir4_2(short rightup_zero)
{	//oblique direction
	return DIR((1 + rightup_zero * 2) % 8);
}
inline DIR NthDir8(short right_zero)
{
	return DIR((2 + right_zero) % 8);
}
inline DIR NthDir8_Sery(short right_zero)
{	//上下左右，然后斜4个方向 
	return array<DIR,8>{UP,RIGHT,DOWN,LEFT,RIGHTUP,RIGHTDOWN,LEFTDOWN,LEFTUP}[right_zero%8];
}
inline DIR OppoDir(DIR dir)
{
	return DIR((dir + 4) % 8);
}
inline string DirName(DIR dir)
{
	if(dir == LEFT)
		return "Left";
	else if(dir == LEFTUP)
		return "Leftup";
	else if(dir == UP)
		return "Up";
	else if(dir == RIGHTUP)
		return "Rightup"; 
	else if(dir == RIGHT)
		return "Right";
	else if(dir == RIGHTDOWN)
		return "Rightdown";
	else if(dir == DOWN)
		return "Down";
	else if(dir == LEFTDOWN)
		return "Leftdown";
	else
		return "Unknown";
}/*
DIR RadianToDir(float radian)
{	//<!> 0 -- right   PI/2 -- up  PI -- left  PI*3/2 -- down
//				float equal bug ——don't use
	if(radian == 0.0f)
	    return RIGHT;
    else if(radian == PI / 4)
       return RIGHTUP;
    else if(radian == PI / 2)
        return UP;
    else if(radian == PI * 3 / 4)
        return LEFTUP;
    else if(radian == PI)
        return LEFT;
    else if(radian == PI * 5 / 4)
        return LEFTDOWN;
    else if(radian == PI * 2 / 3)
    	 return DOWN;
    else if(radian == PI * 7 / 4)
        return RIGHTDOWN;
}*/
float DirToRadian(DIR dir)
{
	if(dir == LEFT)
		return PI;
	else if(dir == LEFTUP)
		return PI * 3 / 4;
	else if(dir == UP)
		return PI / 2;
	else if(dir == RIGHTUP)
		return PI / 4;
	else if(dir == RIGHT)
		return 0.0f;
	else if(dir == RIGHTDOWN)
		return PI * 7 / 4;
	else if(dir == DOWN)
		return PI * 3 / 2;
	else if(dir == LEFTDOWN)
		return PI * 5 / 4;
}
inline float RadianToDegree(float radian)
{	//rad -> deg
	return radian/PI*180.0f;
}

//Status
typedef BYTE STATUS,*PSTATUS;
#define ST_DEAD 0
#define ST_ALIVE 1
#define ST_DRAWONLY 2
inline float RadianForDrawing(float theta)
{
	return theta - PI/2;
}
color_t ColorOriginate(color_t clr)
{
	if(clr == UT_BLUE)
		return UT_ORIGIN_BLUE;
	else if(clr == UT_ORANGE)
		return UT_ORIGIN_ORANGE;
	else if(clr == UT_WHITE)
		return UT_ORIGIN_WHITE;
	else{
		DebugLog("<!> ColorOriginate: Invalid color");
//		MsgSndTip();
		return UT_ORIGIN_WHITE;
	}
}

namespace images
{
	const short p_w = 30, p_h = 30;
	const short go_w = 600, go_h = 266; 
	PIMAGE img_player;
	PIMAGE img_player_blue;
	PIMAGE img_player_green;
	PIMAGE img_gameover;
	PIMAGE img_shield;
	PIMAGE img_shield_1;
	PIMAGE img_white_bone_end;
	PIMAGE img_blue_bone_end;
	PIMAGE img_orange_bone_end;
	
	void LoadImages(void)
	{
		DebugLog("images::LoadImages: 加载一些图片...") ;
		img_player = newimage();
		_getimage_withzoom(img_player,_T("heart.png"),p_w,p_h);
		img_player_blue = newimage();
		_getimage_withzoom(img_player_blue,_T("heart_blue.png"),p_w,p_h);
		img_player_green = newimage();
		_getimage_withzoom(img_player_green,_T("heart_green.png"),p_w,p_h);
		img_gameover = newimage();
		_getimage_withzoom(img_gameover,_T("gameover.png"), go_w, go_h);
		img_shield = newimage();
		_getimage(img_shield,_T("shield.png"));
		img_shield_1 = newimage();
		_getimage(img_shield_1,_T("shield_1.png"));
		img_white_bone_end = newimage();
		_getimage(img_white_bone_end,_T("white_bone_end.png"));
		img_blue_bone_end = newimage();
		_getimage(img_blue_bone_end,_T("blue_bone_end.png"));
		img_orange_bone_end = newimage();
		_getimage(img_orange_bone_end,_T("orange_bone_end.png"));
	}
	
	struct LenBone {
		short len;
		PIMAGE img;
	};
	vector<LenBone> img_bones;
	vector<LenBone> img_blue_bones;
	vector<LenBone> img_orange_bones;
	const short bone_w = 11;	//骨宽 
	const short boneEnd_w = 21;	//骨头宽 
	const short boneEnd_h = 18;	//骨头高 
	
	inline vector<LenBone>& GetColorBones(color_t clr)
	{
		if(clr == UT_WHITE)	return img_bones;
		if(clr == UT_BLUE)	return img_blue_bones;
		if(clr == UT_ORANGE)	return img_orange_bones;
		DebugLog("<!> GetColorBones: Invalid color of bone");
		return img_bones;
	}
	inline PIMAGE GetColorBoneEnd(color_t clr)
	{
		if(clr == UT_WHITE)	return img_white_bone_end;
		if(clr == UT_BLUE)	return img_blue_bone_end;
		if(clr == UT_ORANGE)	return img_orange_bone_end;
		DebugLog("<!> GetColorBoneEnd: Invalid color of bone");
		return img_white_bone_end;
	}
	bool ExistLengthBone(short len, int* index, color_t clr = UT_WHITE)
	{
		if(GetColorBones(clr).empty())	return false;
		for(short i = 0 ; i < GetColorBones(clr).size(); ++i)
		{
			if(GetColorBones(clr).at(i).len == len)
			{
//				pimg = GetColorBones(clr).at(i).img;
				*index = i;
				return true;
			}
		}
//		pimg = nullptr;
		return false;
	}
	PIMAGE CreateLengthBone(short len, // len == 2 * r
                            color_t clr)
	{
		PIMAGE img = newimage(boneEnd_w,len);
		//Draw bone body
		setlinestyle(PS_SOLID,0,bone_w,img);
		setcolor(ColorOriginate(clr), img);
		line(boneEnd_w/2,boneEnd_h,boneEnd_w/2,len-boneEnd_h,img);
		//Draw bone ends
		putimage_withalpha(img, GetColorBoneEnd(clr),0,0);
		putimage_rotate(img, GetColorBoneEnd(clr),0,len-boneEnd_h,1,1,PI,1,-1);
		//Rotation
		PIMAGE img2 = newimage(len,boneEnd_w);
		putimage_rotate(img2,img,0,0,1,0,PI/2,1,-1);
		delimage(img);
//		if(!img2)	DebugLog("Creation NO");
		LenBone item {len,img2};
		GetColorBones(clr).push_back(item);
		return img2;
	}
	void ClearLengthBones(void)
	{
		img_bones.clear();
		img_orange_bones.clear();
		img_blue_bones.clear();
	}
}
#define pic ::images::

#define AI_STILL 0
#define AI_LINE 1
#define AI_LINE_ACC 2	//ACC表示加速 
#define AI_CIRCLE 3
#define AI_CIRCLE_ACC 4
#define AI_CIRCLE_IN 5
#define AI_CIRCLE_ACC_IN 6
#define AI_CIRCLE_NOROTATE 7
#define AI_FORTH_BACK 8		//线性往返 
#define AI_VIBRATION 9		//简谐振动 
#define AI_ZIGZAG 10		//之字形 
#define AI_SINE_WAVE 11			//正弦曲线 


inline bool IsAICircle(AI_ID ai)
{
	return (ai >= AI_CIRCLE && ai <= AI_CIRCLE_NOROTATE);
}
inline bool IsAIAcc(AI_ID ai)
{
	return (ai == AI_LINE_ACC || ai == AI_CIRCLE_ACC || AI_CIRCLE_ACC_IN);
}
class EntityData {
	public:
		ENTITY_ID id;
		string name;
		
		short damage;
		AI_ID ai;
		float v;
		float a;
		
		PIMAGE img;
		PIMAGE img2;
		
		EntityData()
		{
			id=99;
			name="子弹";
			damage=3;
			ai=AI_STILL;
			v=0.001f;
			a=0.0f;
			img = img2 = nullptr;
		}
		inline bool IsSpearArrow(void) const
		{
			return name.find("箭头")!=string::npos;
		}
		inline bool IsLaser(void) const
		{
			return name.find("激光")!=string::npos;
		}
		inline bool IsBone(void) const
		{
			return name.find("骨头")!=string::npos;
		}
		inline bool IsHorizBone(void) const
		{
			return name.find("横")!=string::npos && IsBone();
		}
		inline bool IsVertBone(void) const
		{
			return name.find("纵")!=string::npos && IsBone();
		}
		inline bool IsGBlaster(void) const
		{
			return name.find("龙骨炮")!=string::npos;
		}
		void GetImage()
		{
			XCPT_L
			if(IsLaser() || name == "last")	
				return;	
			img=newimage();
			string path="et_" + ToString(id) + ".png";
			//DebugLog("Entity::GetImage: 获取图片"+path);
			_getimage(img,path.c_str());
			if(IsSpearArrow() || IsGBlaster() || (id == 7/*lyingDog*/))
			{
				img2 = newimage();
				path = "et_"+ToString(id)+"_1.png";
				_getimage(img2,path.c_str());
			}
			XCPT_R
		}
};
vector<EntityData> edata;

#define CHECK_ENTITY_ID if(id <= 0 || id > edata.size())\
						{\
						    MsgSndTip();\
							DebugLog("ENTITY_ID Checking Failure in\""+(string)__func__+"\": id="+ToString(id));\
							id = 0;\
						}
PIMAGE GetEntityImage(ENTITY_ID id, short index=0)
{
	XCPT_L
	CHECK_ENTITY_ID
	if(index==0)
		return edata.at(id-1).img;
	else
		return edata.at(id-1).img2;
	XCPT_R
}
float GetEntitySpeed(ENTITY_ID id)
{
	XCPT_L
	CHECK_ENTITY_ID
	return edata.at(id-1).v;
	XCPT_R
}
AI_ID GetEntityAI(ENTITY_ID id)
{
	XCPT_L
	CHECK_ENTITY_ID
	return edata.at(id-1).ai;
	XCPT_R
}
float GetEntityAcc(ENTITY_ID id)
{
	XCPT_L
	CHECK_ENTITY_ID
	return edata.at(id-1).a;
	XCPT_R
}


void ReadEntityData()
{
	XCPT_L
	if(!ExistFile(ENTITYDATA_PATH))
	{
		ErrorTip(NULL, "EntityData.csv Not Found", "ReadEntityData");
		DebugLog("找不到EntityData.csv");
		exit(0);
		return;
	}
	if(!edata.empty()) edata.clear();
	DebugLog("ReadEntityData: 开始读取实体数据...");
	vector<string> lines = ReadFileLines(ENTITYDATA_PATH);
	//DebugLog("lines.size()=="+ToString(lines.size()));
	int i=1;
	while(i < lines.size())
	{
		string line = lines.at(i);
		line = ResplitLine(line,',',' ');
		if(line == "")
			break;
		
		stringstream ss;
		ss<<line;
		//DebugLog(line);
		
		EntityData edt;
		ss>>edt.id;
		ss>>edt.name;
		ss>>edt.damage;
		ss>>edt.ai;
		ss>>edt.v;
		if(edt.ai == AI_LINE_ACC)
			ss>>edt.a;
		
		if(!edt.IsHorizBone())	//横骨头自有处理方式 
		    edt.GetImage(); 
		
		edata.push_back(edt);
		++i;
	}
	//DebugLog("edata.size="+ToString(edata.size()));
	XCPT_R
}

template <typename _Tp>
void DirOffsetPos(_Tp& x, _Tp& y, DIR dir, _Tp v)
{	//根据方向与速度对坐标进行偏移 
	if(dir == UP)
		y -= v;
	else if(dir == RIGHTUP)
	{
		x += v/sqrt2;
		y -= v/sqrt2;
	}else if(dir == RIGHT)
		x += v;
	else if(dir == RIGHTDOWN)
	{
		x += v/sqrt2;
		y += v/sqrt2;
	}else if(dir == DOWN)
		y += v;
	else if(dir == LEFTDOWN)
	{
		x -= v/sqrt2;
		y += v/sqrt2;
	}else if(dir == LEFT)
		x -= v;
	else if(dir == LEFTUP)
	{
		x -= v/sqrt2;
		y -= v/sqrt2;
	}else{
		ErrorTip(NULL,"Invalid direction!","DirOffsetPos");
	}
}

//四个边界 
#define LEFT_BORDER (pic p_w/2)
#define UP_BORDER (pic p_h/2)
#define RIGHT_BORDER (g_uix - pic p_w/2)
#define DOWN_BORDER (g_uiy - pic p_h/2)
inline bool OuttaField(const float& x, const float& y, const float expand = 0.0)	//expand用于防止实体一角还可见时就被移除 
{	//是否出去了 
	if(x < LEFT_BORDER - expand || x > RIGHT_BORDER + expand	
	|| y < UP_BORDER - expand || y > DOWN_BORDER + expand)
		return true;
	return false;
}

inline float GetRealVelocity(const float& v)
{
	return v * (clock() - lastFrame);
//	return v * 0.02f;
}
#define TOUCH_T 100

//#define vert_k 8.0f
#define PL_DEF_SPEED 0.24f
#define PL_QUICK_SPEED 0.48f
#define PL_JUMP_SPEED 0.8f*8.0f
#define PL_SLOW_SPEED 0.15f
#define PL_DEF_MAX_HP 45
#define PL_INV_CD 1500	//ms
#define PL_BLUE_RATIO 3.0f
#define PL_JUMP_MAXDIST g_uiy * 0.5f

#define HEART_RED 0
#define HEART_BLUE 1
#define HEART_GREEN 2
#define BLUE_BOTTOM (g_uiy * 0.7f)
#define g 0.015f*25.0f	//重力加速度

class Player {	//玩家类 
	public:
		float x;
		float y;
		float v;	//velocity
		float v_g;	//velocity for instant gravity
		DIR dir;
		float y_d;
		
		short hp;	//health
		short max_hp;	//max health
		
		bool isMoving;
		
		clock_t lastMove;
		clock_t lastHurt; 
		clock_t lastTouched;	//shield touched
		UINT heartMode;
		
		Player()
		{	
			isMoving=false;
			heartMode = HEART_RED;
		}
		void Init()
		{
			x = g_uix/2;
			y = g_uiy/2;
			y_d = y;
			dir = RIGHT;
			v = PL_DEF_SPEED;
			hp = max_hp = PL_DEF_MAX_HP;
			lastMove = clock();
			lastTouched = clock() + TOUCH_T;
			heartMode = HEART_RED;
			v_g = 0.0f;
		}
		void SetPos(float _x, float _y, const bool checkLimit=true)
		{
			x = _x;
			y = _y;
			y_d = y;
			if(checkLimit)
			    LimitCoord();
		}
		inline UINT& HeartMode(void) 
		{
			return heartMode;
		}
		inline bool GreenMode(void) const
		{
			return heartMode==HEART_GREEN;
		}
		inline void RefreshTouch(void)
		{
			lastTouched = clock();
		}
		inline bool IsShieldTouched(void) const
		{
			return (clock() - lastTouched <= TOUCH_T);
		}
		bool IsMoving(void) const
		{
			return isMoving;
		}
		bool IsAlive(void)	const
		{
			return (hp > 0);
		}
		inline void LimitCoord(void)
		{
			if(GreenMode())
			{	//居中 
				x = g_uix / 2;
				y = g_uiy / 2;
				return;
			}else if(HeartMode() == HEART_BLUE)
			{
				ClampA(x,float(pic p_w/2),float(g_uix - pic p_w/2));
				ClampA(y,float(pic p_h/2),float(BLUE_BOTTOM - pic p_h/2));
				return;
			}
			//限制玩家坐标 
			ClampA(x,float(pic p_w/2),float(g_uix - pic p_w/2));
			ClampA(y,float(pic p_h/2),float(g_uiy - pic p_h/2));
		}
		void InstantMove(DIR dir)
		{
			if(GreenMode())
				return;
			else if(HeartMode() == HEART_BLUE)
			{	//重力蓝心 
				if(dir == DOWN)
					return;
				/*if(dir == LEFTDOWN)
					dir = LEFT;
				else if(dir == RIGHTDOWN)
					dir = RIGHT;*/
				else if(dir == UP 	//试图跳跃于 
				&& (!OnGround() 	//未着地 
				&& v_g > 0.0f))		//and下落 
					return;
				/*else if(dir == RIGHTUP && !OnGround())
					dir = RIGHT;
				else if(dir == LEFTUP && !OnGround())
					dir = LEFT;*/
				if(dir == UP)
				{
					if(abs(y - y_d) > PL_JUMP_MAXDIST)
						return;
					v_g = -PL_JUMP_SPEED;	//瞬时速率 
				}
			}
			float vreal = GetRealVelocity(v);
			if(HeartMode() == HEART_BLUE)
				vreal *= PL_BLUE_RATIO;
			if(HeartMode() == HEART_BLUE && dir != UP && dir != DOWN
			|| HeartMode() != HEART_BLUE)
				DirOffsetPos(x,y,dir,vreal);
		}
		bool Move(DIR dir)
		{
			if(!TimeToMove())
				return false;
			
			this->dir = dir;	//facing
			InstantMove(dir);
			LimitCoord();
			lastMove = clock();
			return true;
		}
		bool OnGround(void)
		{
			bool ret = (this->y >= BLUE_BOTTOM  - pic p_h/2 - 1
			/* && this->v_g <= 0.01f*/);
			if(ret)
				y_d = y;	//refresh
			return ret;
		}
		void Fall(void)	//Blue Heart
		{	//<!> Check heart mode outside
			if(HeartMode() != HEART_BLUE)
				return;
			if(OnGround() && v_g > 0.0f)
			{
				this->v_g = 0.0f;
				return;
			}
			if(!fequ(v_g,0.0f))
			{
				this->isMoving = true;
			}
//			g_temp[9]++;
			ClampA(v_g, -PL_JUMP_SPEED, 10.0f);
			if(!K(UP_KEYS))
			{
//				this->v_g += GetRealVelocity(g);	//acc
				this->v_g += g;	//acc
			}
//			float vreal = GetRealVelocity(v_g);
			float vreal = v_g;
			DirOffsetPos(x,y,DOWN,vreal);	//fall
			//LimitCoord();
		}
		bool TimeToMove(void) const
		{
			/*if(clock() - lastMove >= (10 / (float)v))
				return true;
			return false;*/
			return true;
		}
		void Hurt(short damage)
		{	//受伤 
		    if(damage <= 0)  return;
			hp -= damage;
			Sound(SND_HURT);
			lastHurt = clock();
			ClampA(hp,(short)0,max_hp);
		}
		void Heal(short heal)
		{	//治疗 
		    if(heal <= 0) return;
			hp += heal;
			Sound(SND_HEAL);
			ClampA(hp,(short)0,max_hp);
		}
		bool IsInvun(void) const
		{
			return (clock() - lastHurt <= PL_INV_CD);
		}
#define SHRINK_RATIO 0.85f
		bool JudgeCollide(void) const
		{
			XCPT_L
			bool flag = false; 
//			for(long _y = y - pic p_h/2; _y <= y + pic p_h/2; ++_y)
//				for(long _x = x - pic p_w/2; _x <= x + pic p_w/2; ++_x)
			for(long _y = y - pic p_h/2*SHRINK_RATIO; _y <= y + pic p_h/2*SHRINK_RATIO; ++_y)
				for(long _x = x - pic p_w/2*SHRINK_RATIO; _x <= x + pic p_w/2*SHRINK_RATIO; ++_x)
					if(IsUTWhite(getpixel(_x,_y)))	//巧妙的判定方法 
						flag = true;	//白色攻击 
					else if(getpixel(_x,_y) == UT_BLUE && IsMoving())
						flag = true;	//蓝色攻击 
					else if(getpixel(_x,_y) == UT_ORANGE && !IsMoving())
						flag = true;	//橙色攻击 
			return flag;	//avoiding this method invocation's time difference
			XCPT_R
		}
		
}p;

inline float GetScreenRadius()
{	//屏幕半径 
	return sqrt(pow2(g_uix/2.0f)+pow2(g_uiy/2.0f));
}
inline float GetScreenDiameter()
{	//屏幕直径 
	return GetScreenRadius()*2.0f;
}
struct LineEq{	//直线一般式 
	float A;
	float B;
	float C;
};
LineEq GetLine(float x1,float y1,float x2,float y2)
{
	LineEq para;
	if(fequ(x1,x2))
	{
		para.A = 1;
		para.B = 0;
		para.C = -x1;
	}
	else
	{
		para.B = 1;
		para.A = -(float)(y1-y2)/(float)(x1-x2);
		para.C = -(y1+para.A*x1);
	}
}

bool GetIntersectionPoint(LineEq para1, LineEq para2, long& x, long& y)
{	//求两直线交点 
//    para1.A = -para1.A;
//    para2.A = -para2.A;
	if(fequ(para1.A,para2.A) && fequ(para2.B,para2.B))
			   return false;
	if(fzero(para1.B)&&fzero(para2.A))
	{
		x = -para1.C;
		y = -para2.C;
	}
	else if(fzero(para1.A)&&fzero(para2.B))
	{
		x = -para2.C;
		y = -para1.C;
	}
	else
	{
		x = (para2.B*para1.C-para1.B*para2.C)/(para2.A*para1.B-para1.A*para2.B);
		y = (para1.A*para2.C-para2.A*para1.C)/(para2.A*para1.B-para1.A*para2.B);
	}
	return true;
}
void PositivePoint(POINT& pt)
{
	pt.x = abs(pt.x);
	pt.y = abs(pt.y);
	if(pt.x == -21473648)	pt.x=0;
	if(pt.y == -21473648)	pt.y=0;
}
void GetLineEndpoint(float x, float y, float theta, POINT* pt)
{
	if(!pt)
	    return;
	pt->x = x + GetScreenDiameter() * 2.0f * cos(theta);
	pt->y = y - GetScreenDiameter() * 2.0f * sin(theta);
}
float GetRelativeQuadrant(float theta)
{	//相对象限 
	theta = StdRadian(theta);
	if(theta > 0 && theta < PI / 2)
	    return 1.0f;
    else if(theta > PI / 2 && theta < PI)
        return 2.0f;
    else if(theta > PI && theta < PI * 3 / 2)
        return 3.0f;
    else if(theta > PI * 3 / 2 && theta < PI * 2)
        return 4.0f;
    else if(fequ(theta,0.0f))
        return 0.5f;
    else if(fequ(theta,PI / 2))
        return 1.5f;
    else if(fequ(theta,PI))
        return 2.5f;
    else if(fequ(theta,PI*3/2))
  	    return 3.5f;
    else
        return 0.0f;	//原点 
}
float GetRelativeQuadrant(float ox, float oy, float x, float y)
{
	return GetRelativeQuadrant(atan2(y-oy, x-ox));	//方便 
}
bool PermittedPoint(float quad1, float ox, float oy, float x, float y)
{
	float quad2 = GetRelativeQuadrant(ox,oy,x,y);
	if(fequ(quad2,0.0f) || fequ(quad1,0.0f))
	    return false;
	if(fequ(quad2,quad1))
	    return true;
    return false;
}
color_t GetColor(string& name)
{
	if(name.find("蓝色") != string::npos)
		return UT_BLUE;
	else if(name.find("橙色") != string::npos)
		return UT_ORANGE;
	else if(name.find("白色") != string::npos)
		return UT_WHITE;
	else{
		DebugLog("<!> GetColor: Invalid color name");
		return RED;
	}
}



//实体额外属性 
#define ETA_NONE '$'
#define ETA_DELAY_1s '1' //1秒静止后开始 
#define ETA_DELAY_2s '2' //2秒静止后开始 
#define ETA_DELAY_3s '3' //3秒静止后开始 
#define ETA_BLASTER 'G'		//Gaster Blaster
#define ETA_ROTATE 'r'	//旋转入场 
#define ETA_LIFE_1_5s	'!'	//1.5秒后消失 
#define ETA_FADE 'f'		//拥有淡入淡出效果 
#define ETA_NOROTATE '%'	//不旋转 
#define ETA_SEMIROTATE '#'	//横纵模式 

#define SANEAR_DISTANCE (pic p_w * 2.0f)
#define SANEAR_TOUCH (pic p_w / 2)
#define SANEAR_CIRCLE (pic p_w * 3.5f)
inline bool IsDelayAttrib(char attr)
{
	return (attr >= ETA_DELAY_1s && attr <= ETA_DELAY_3s);
}

class Entity {
		ENTITY_ID id;
		ENTITY_ID id_backup;
	public:
		float x;
		float y;
		
		float v;
		float v_r;	//转圈线速度 
		float a;
		short owner_i;
		float ox;
		float oy;
		float r;
		float theta;	//自转 
		float theta_r;	//公转 
		float theta_0;	//保留 
		AI_ID ai;
		
		string attrib;
		STATUS status;
		clock_t start;
		clock_t reserved_clk;
		char reserved1;
		
		Entity() : id(1)
		{
			id_backup = id;
			x=y=0.0;
			a=v=0.0;
			v_r = 0.0;
			ox=oy=0.0;
			r=0.0;
			theta=0.0;
			theta_0=theta_r=0.0;
			status = ST_ALIVE;
			owner_i = -1;
			start=0;
			reserved1 = 0;
			reserved_clk = clock();
			ai = AI_LINE;
		}
		ENTITY_ID& GetID(void)
		{
			return this->id;
		}
		void SetAI(AI_ID aid, float _v=0.0f)
		{
			this->ai = aid;
			if(!fequ(_v,0.0f))
				this->v = _v;
			else
				this->v = GetEntitySpeed(GetID());
			if(aid == AI_STILL)
				this->v = 0.0f;
			if(IsAIAcc(aid))
			{
				this->a = GetEntityAcc(GetID());
			}
		}
		#define ROTATION_T 1000	//ms
		#define ROTATION_W_BASE 0.08f
		float GetRotationOmega(void)
		{
//			return ROTATION_W_BASE * (1.0f / ((clock() - start) / 1000.0f));
//			v_r -= 0.00001f;
			v_r *= 0.9998f;
			ClampA(v_r,0.00001f,1.0f);
			return v_r;
		}
		#define THETA_PREDICT (.8f*PI)
//		#define THETA_PREDICT 0
		void FitTheta()
		{
			if(HasAttrib(ETA_ROTATE) && !fequ(ox,x))
			{
				this->v_r = ROTATION_W_BASE;
				this->ox = x, this->oy = y;
				this->theta_0 = StdRadian(this->theta);	//theta_0存储最终角度 
//				this->theta = Randint(0,15) * PI / 8;	//随机起始角 
				this->theta = StdRadian(this->theta_0 + THETA_PREDICT);	//change
			}
		}
		void SetID(ENTITY_ID _id)
		{
			id = _id;
		}
		Entity(ENTITY_ID _id) : id(_id)
		{
			id_backup = id;
			CheckID("Entity::Entity");
			x=y=0.0;
			a=v=0.0;
			v_r = 0.0;
			ox=oy=0.0;
			r=0.0;
			owner_i = -1;
			theta=0.0;
			status = ST_ALIVE;
			start=0;
			reserved1 = 0;
			reserved_clk = clock();
			ai = AI_LINE;
		}
		void FitAIType(float _v=0.0f)
		{
			SetAI(GetEntityAI(this->id),_v);
//			DebugLog("set ai="+ToString(this->ai));
		} 
		inline bool HasAttrib(char attr) const
		{
			for(short i = 0; i < attrib.size(); ++i)
				if(attrib[i] == attr)
					return true;
			return false;
		}
		/*inline bool HasAttrib(bool func(char)) const
		{	//以函数为参数 
			for(short i = 0; i < attrib.size(); ++i)
				if(func(attrib[i]))
					return true;
			return false;
		}*/
		bool AddAttrib(const char attr)
		{
			if(HasAttrib(attr))
				return false;
			attrib += attr;
			FitTheta();
			return true;
		}
		inline bool IsLaser(void) const
		{
			return edata.at(id-1).IsLaser();
		}
		inline bool IsBone(void) const
		{
			return edata.at(id-1).IsBone(); 
		}
		inline bool IsHorizBone(void) const
		{
			return edata.at(id-1).IsHorizBone(); 
		}
		#define LASER_T 1800	//laser life, ms
		bool IsAlive(void)
		{
			if(OuttaField(x,y,(GetScreenRadius()+100.0f-min(g_uix/2, g_uiy/2))))
//			if(OuttaField(x,y,g_uiy/2.0f))
			{
				status = ST_DEAD;
			}else if(IsLaser() && clock() - start >= LASER_T)
			{
				status = ST_DEAD;
			}
			if(HasAttrib(ETA_LIFE_1_5s) 
			&& clock() - start >= 1500.0f
			)
			{
				status = ST_DEAD;
			}
			if(status == ST_DEAD)
				return false;
			return true;
		}
		void CheckID(string tag="tag")
		{
			XCPT_L
			if(id <= 0 || id >= edata.size())
			{
				MsgSndTip();
				DebugLog("<!> Entity::CheckID in\"" + tag + "\" : Invalid Entity id ="+ToString(id)+" , id_backup="+ToString(id_backup));
				id = id_backup;
			}
			if(id_backup <= 0 || id_backup >= edata.size())
			{
				DebugLog("<!> Entity::CheckID in\"" + tag + "\" : Invalid Entity id_backup ="+ToString(id_backup));
				id = id_backup = 1;
				DebugLog("Now id="+ToString(id)+" id_backup="+ToString(id_backup));
			}
			XCPT_R
		}
		inline bool IsSpearArrow(void) const
		{
			return edata.at(this->id-1).IsSpearArrow();
		}
		inline bool SpearArrowNearby(void) const
		{
			return (IsSpearArrow() && Distance2(x,y, p.x,p.y) <= SANEAR_DISTANCE);
		}
		inline bool SpearArrowTouch(void) const
		{
			return (IsSpearArrow() && Distance2(x,y, p.x,p.y) <= SANEAR_TOUCH);
		}
		inline bool SpearArrowCircleDistance(void) const
		{	//point, not range
			return (IsSpearArrow() 
			&& Distance2(x, y, p.x, p.y) <= SANEAR_CIRCLE
//			&& Distance2(x,y, p.x,p.y) >= SANEAR_CIRCLE - 55.0f	//invalid
			&& ai == AI_LINE && reserved1 == 0
			);
		}
		#define GBLASTER_T 1000		//龙骨炮延时 ms
		bool GBlasterEmit(void) const
		{
			return ((IsLaser() || HasAttrib(ETA_BLASTER))
			&& clock() - start >= GBLASTER_T);
		}
		#define LWIDTH_A 15.0f
		#define LWIDTH_OMEGA 0.03f
		inline float GetLaserWidth(float const& r0) const
		{	//粗细不一  正弦函数 
			return Clamp<float>(LWIDTH_A * 
					sin(LWIDTH_OMEGA * 
						  (clock() - start)) + r0,
						   r0*0.7f, r0*2.0f);
		}
		bool HasDelayAttrib(void) const
		{
			for(short i = 0; i < attrib.size(); ++i)
				if(IsDelayAttrib(attrib.at(i)))
					return true;
			return false; 
		}
		void Attack()
		{	//攻击 
			if(id == 7)
			{
				float _x=this->x,_y=this->y;
				SpawnEntityEx(8,_x,_y,GetEntitySpeed(8),Towards(_x,_y,p.x,p.y));
				this->reserved_clk = clock();
				Sound(SND_BARK);
			}
		}
		#define FADEIN_T 500.0f
		#define FADEIN_SHORT_T 200.0f
		#define FADEOUT_T 250.0f
		void Draw()	//Entity::Dr aw
		{
			XCPT_L
			CheckID(__func__);
			if(!IsAlive())	return;
			
			PIMAGE img=nullptr;
			if(IsLaser())
			{
				setlinestyle(PS_SOLID, 0, GetLaserWidth(r));
				string _name = edata.at(id-1).name;
				color_t _clr = GetColor(_name);
//				DebugLog("_name="+_name+" _clr="+ToString(_clr));
				setcolor(_clr);
				POINT pt1{0,0};
//				GetLineEndpoints(x,y,theta,&pt1,&pt2); 
				GetLineEndpoint(x,y,theta,&pt1);
//				DebugLog("交点：A("+ToString(pt1.x)+","+ToString(pt1.y)+") B("+ToString(pt2.x)+","+ToString(pt2.y)+")");
				line(x,y, pt1.x, pt1.y);	//RAY
				return;
			}//--------------------------
			else if(IsHorizBone())
			{
				int _i;
				bool _ret = pic ExistLengthBone(2 * r, &_i, GetColor(edata.at(id-1).name));
				if(!_ret)	//no existing
				{
					img = pic CreateLengthBone(2 * r, GetColor(edata.at(id-1).name));
					/*if(img == nullptr)
					{
						DebugLog("no1");return;
					}*/
				}else{
					img = pic GetColorBones(GetColor(edata.at(id-1).name)).at(_i).img;
					/*if(img == nullptr)
						   DebugLog("nonono");*/
				}
				//go down
		#define DOGBARK_T 100
			}else{	//other Entities
				short _i=0;
				if(SpearArrowNearby() || GBlasterEmit()
				|| (id == 7 && (clock() - reserved_clk <= DOGBARK_T)))
				    _i = 1;
				img = GetEntityImage(id,_i);
			}
			if(img == nullptr)
			{
//				DebugLog("no2");
				return;
			}
			float radian = RadianForDrawing(this->theta);
			if(HasAttrib(ETA_NOROTATE))	//字头朝上 
				radian = 0.0f;
			else if(HasAttrib(ETA_SEMIROTATE))
			{	//横纵，只有两种旋转 
				if(fequ(this->theta,PI/2)|| fequ(this->theta,PI*1.5f))
					radian = PI*1.5f;
				else 
					radian = 0.0f;
			}
//			DebugLog("Entity::Draw x="+ToString(x)+" y="+ToString(y)+" theta="+ToString(theta));
			float alpha = -1;
			if(HasAttrib(ETA_FADE))
			{
				clock_t fadein_t = (HasDelayAttrib()?FADEIN_SHORT_T:FADEIN_T);
				clock_t fadeout_t = FADEOUT_T;
				if(clock() - start <= fadein_t)
				{	//Fadein effect
					alpha = 255 * (clock()-start)/fadein_t;
				}
				else if(HasAttrib(ETA_LIFE_1_5s) && (clock()-start >= 1500 - fadeout_t))
				{
					alpha = 255 * ((1500 - (clock()-start))/(float)fadeout_t);
	//				alpha = 100;
				}
			}
			putimage_rotate(NULL, img, x, y, 0.5f, 0.5f, radian, 1, alpha, 1);
			XCPT_R
		}
		void CheckSAShield()
		{	//抵挡 
			if(SpearArrowNearby() && 
			(abs((OppoRadian(this->theta)) - DirToRadian(p.dir)) < 0.1f)
			&& this->status == ST_ALIVE)
			{
				Sound(SND_DING);	//ding
				p.RefreshTouch();	//盾牌变红 
				this->status = ST_DEAD;
			}else{
//				DebugLog(ToString(OppoRadian(this->theta)));
//				DebugLog(DirName(RadianToDir(OppoRadian(this->theta))) + " and " + DirName(p.dir));
			}
		}
		void CheckSADamage()
		{	//spear arrow damage
			if(SpearArrowTouch() && this->status == ST_ALIVE)
			{
				p.Hurt(5);
				this->status = ST_DEAD;
			}
		}
		#define SACIR_SPEED_RATIO 2.0f
		void CheckSACircle()
		{	//instant operation
			if(id != 15 || this->status != ST_ALIVE)
				return;	//only for yellow spear arrow
			if(SpearArrowCircleDistance())
			{
//				MsgSndTip();
				//locate circle centre
				this->ox = p.x;
				this->oy = p.y;
				//rotation dimension
				this->theta_r = this->theta;
				this->theta_0 = this->theta;
				this->r = SANEAR_CIRCLE;	//radius
				this->ai = AI_CIRCLE_NOROTATE;	//ai change
				this->v_r = this->v * SACIR_SPEED_RATIO;
			}
			else if(this->ai == AI_CIRCLE_NOROTATE
			 && abs(theta_r - theta_0) >= PI)
			{	//转动结束 
				this->ai = AI_LINE; //恢复直线运动 
				this->reserved1 = 1;
			}
		}
		inline bool HasRotation() const
		{
			return (clock() - start <= ROTATION_T && HasAttrib(ETA_ROTATE));
		}
		void Move()	
		{//Entity::Mo ve
		    XCPT_L
		    CheckID(__func__);
		    /*AI_ID ai;
		    if(id-1 >= edata.size())	//<!>
		    {
		    	DebugLog("<!> Entity::Move: edata下标越界 "+ToString(id-1)+" >= "+ToString(edata.size()));
		    	ai = AI_LINE;
			}
            else
			    ai = GetEntityAI(id);*/
			/*if(IsLaser())
			{	//定位 
				if(owner_i < GetEntityCount())
				{	//valid index
					x = GetEntityX(owner_i);
					y = GetEntityY(owner_i);
				}
			}*/
			if(ai == AI_STILL)	//静止 
				return;
			if(
			(!HasAttrib(ETA_ROTATE) 
			&& HasAttrib(ETA_DELAY_1s) && (clock() - start <= 1000)
			|| HasAttrib(ETA_DELAY_2s) && (clock() - start <= 2000)
			|| HasAttrib(ETA_DELAY_3s) && (clock() - start <= 3000)
			)
			|| HasAttrib(ETA_BLASTER) && (clock() - start <= GBLASTER_T))
				return;	//具有延迟属性，N秒后开始运动 
				
			if(HasAttrib(ETA_ROTATE)
			 && (clock() - start > ROTATION_T) 
			 && (clock() - start < ROTATION_T * 1.5f))
			{	//旋转长矛丢出一刹那 
				if(!fequ(this->theta,this->theta_0))
				{
					this->theta = this->theta_0;	//复位 
					this->theta_r = this->theta;
					Sound(SND_SPEAR);
				}
			}
			float vreal = GetRealVelocity(v);
			if((ai == AI_LINE || ai == AI_LINE_ACC)
			&& (!HasAttrib(ETA_ROTATE) || !HasRotation()))
			{	//直线运动 
				float _theta = theta;
				if((id == 15 && !reserved1)
				|| HasAttrib(ETA_BLASTER) || IsLaser()
				)
				{	//<!> spec
					_theta = StdRadian(OppoRadian(theta));
				}
				x += vreal * cos(_theta);
				y -= vreal * sin(_theta);
				if(ai == AI_LINE_ACC)
				{	//匀加速直线 
//					DebugLog("acc");
					v += GetRealVelocity(a);
				}
			}
			else if(IsAICircle(ai) || HasRotation())
			{	//圆周运动 
//				DebugLog("circle");
				float vreal_r = GetRealVelocity(v_r);
				if(HasRotation())
				{	//自转 
					theta += GetRotationOmega();
				}else if(this->id != 15)	//注意：黄箭头不自转 
				{	//自转 
					theta += vreal_r / r;		//ω = v/r,   θ=  ωx Δt 
				}
				
				if(r >= 0.01f)
				{	//公转 包括自转↑ 
					theta_r += vreal_r / r;		//ω = v/r,   θ=  ωx Δt 
					if(ai == AI_CIRCLE_IN || ai == AI_CIRCLE_ACC_IN)
						r -= vreal;		//radius decrease
					x = ox + cos(theta_r) * r;
					y = oy - sin(theta_r) * r;
				}
				if(ai == AI_CIRCLE_ACC || ai == AI_CIRCLE_ACC_IN)
				{
					v_r += GetRealVelocity(a);
					if(v_r > 0)
						ClampA(v_r, 0.005f, 7.0f);
					else{
						ClampA(v_r, -7.0f, -0.005f);
					}
				}
					
			}else if(ai == AI_ZIGZAG || ai == AI_SINE_WAVE)
			{
				ox += vreal * cos(theta_r);
				oy -= vreal * sin(theta_r);
			}
			if(ai == AI_FORTH_BACK || ai == AI_ZIGZAG)
			{	//360degree
				clock_t period = (4.0 * r) / v_r;	//计算周期 
				float ratio = float(clock() - reserved_clk) / period + theta_0;	//计算比例
				while(ratio > 1.0)
					ratio -= 1.0;
				float k=0.0;
				//代入分段一次函数 
				if(ratio <= .25)
					k = ratio * 4.0;
				else if(ratio <= .75)
					k = 2.0 - 4.0 * ratio;
				else
					k = 4.0 * ratio - 4.0;
				this->x = ox + r * k * cos(theta_r+PI/2.0);
				this->y = oy - r * k * sin(theta_r+PI/2.0);
				if(reserved_clk > period)
					reserved_clk = reserved_clk % period;	//限制比例 
//					reserved_clk = clock();
			}else if(ai == AI_VIBRATION || ai == AI_SINE_WAVE)
			{
				clock_t period = (4.0 * r) / v_r;	//计算周期 
				float phase = StdRadian(2 * PI * float(clock() - reserved_clk) / period + theta_0);	//计算相位 
				this->x = ox + r * sin(phase) * cos(theta_r+PI/2.0);	//相位代入正弦函数 
				this->y = oy - r * sin(phase) * sin(theta_r+PI/2.0);
				if(reserved_clk > period)
					reserved_clk = reserved_clk % period;	//限制比例 
//					reserved_clk = clock();
			}
			if(IsSpearArrow())
			{
				CheckSACircle();
				CheckSAShield();
				CheckSADamage();
			}
			XCPT_R
		}
		void ExtraAction(short index);
		
};
vector<Entity> entities;

void Entity::ExtraAction(short index)
{
	if(HasAttrib(ETA_BLASTER) && GBlasterEmit() && !reserved1)
	{	//laser
//				MsgSndTip();
		Sound(SND_GBLASTER_2);	//Kahhhhh!
//				SpawnEntityEx(this->id + 1,x,y,1.0f,this->theta,this->r);
		Entity et(this->id + 1);
		et.x = x;
		et.y = y;
		et.owner_i = index;
		et.v = v;
		et.r = r;
		et.theta = theta;
		et.SetID(this->id + 1);
		et.FitAIType();
		et.status = ST_ALIVE;
		et.start = clock();
		entities.push_back(et);
		reserved1 = 1;	//book
	}
}
size_t GetEntityCount(void)
{
	return entities.size();
}
float GetEntityX(short i)
{
	return entities.at(i).x;
}
float GetEntityY(short i)
{
	return entities.at(i).y;
}
bool CheckPlayerCollision()
{	//检测玩家是否碰到弹幕 
	XCPT_L
	p.LimitCoord();
	if(entities.empty())	return false;
	if(p.JudgeCollide())
	{
		if(clock() - p.lastHurt >= PL_INV_CD)
			p.Hurt(3);
		return true;
	}
	return false;
	XCPT_R
}
void CheckPlayerMove()
{
	XCPT_L
	if(p.HeartMode() == HEART_BLUE)
		p.Fall();
	else{
		p.v_g = 0.0f;
	}
	XCPT_R
}
void DrawEntities()
{
	XCPT_L
	if(entities.empty())	return;
	for(long i = 0; i < entities.size(); ++i)
	{
		entities.at(i).Draw();
	}
	XCPT_R
}

#define IDC_BLINK_T 50
#define IDC_LINE_WIDTH 5
#define IDC_DEF_LIFE 1000
class Indicator {
	public:
		long left;
		long top;
		long right;
		long bottom;
		
		clock_t duration;
		clock_t start;
		
		Indicator() {	left=top=right=bottom=0;}
		Indicator(long _left, long _top, long _right, long _bottom, clock_t _life = IDC_DEF_LIFE)
		: left(_left) , top(_top) , right(_right), bottom(_bottom) , duration(_life) {start=clock();}
		inline long GetWidth(void) const
		{
			return abs(right - left);
		}
		inline long GetHeight(void) const
		{
			return abs(bottom - top);
		}
		inline long GetCenterX(void) const
		{
			return left + GetWidth()/2;
		}
		inline long GetCenterY(void) const
		{
			return top + GetHeight()/2;
		}
		void Start()
		{	//开始 
			start = clock();
		}
		inline bool IsActive(void) const
		{	//是否被激活 
			return (clock() - start <= duration);
		}
		inline color_t GetColor(void) const
		{	//瞬时颜色 
			return (((clock() - start) % IDC_BLINK_T >= IDC_BLINK_T / 2) ? RED : YELLOW);
		}
		void Draw()	//Indicator::Dr aw
		{
			XCPT_L
			if(!IsActive())	return;
			
			const int fs = 200;
			setfont(fs,0,"Berlin Sans FB Demi");
			setcolor(GetColor());
			setlinestyle(PS_SOLID, 0, IDC_LINE_WIDTH);
			line(left,top,right,top);
			line(left,bottom,right,bottom);
			line(left,top,left,bottom);
			line(right,top,right,bottom);
			xyprintf(GetCenterX() - fs/2, GetCenterY() - fs, "!");
			
			XCPT_R
		}
};
vector<Indicator> indicators;
void CreateIndicator(long left, long top, long right, long bottom, clock_t life)
{
	XCPT_L
	Indicator idc(left,top,right,bottom,life);
	idc.Start();
	Sound(SND_EXCLAMATION);
	indicators.push_back(idc);
	XCPT_R
}
void CheckIndicatorsLife()
{
	XCPT_L
	if(indicators.empty())	return;
	for(long i = 0; i < indicators.size(); ++i)
	{
		if(!indicators.at(i).IsActive())
		{
			auto iter = indicators.begin();
			short j=0;
			while(j < i)
				++iter,++j;
			iter = indicators.erase(iter);	//Delete it in the vector
		}
	}
	XCPT_R
}
void DrawIndicators()
{
	XCPT_L
	if(indicators.empty())	return;
	for(long i = 0; i < indicators.size(); ++i)
	{
		indicators.at(i).Draw();
	}
	XCPT_R
}

#define MSG_SHOW_T 1500
void AddMessage(string s)
{	//添加消息 
//	DebugLog("AddMessage="+s);
	g_msg = s;
	lastMsg = clock();
}
bool ShouldMessageShown(void)
{
	return (clock() - lastMsg <= MSG_SHOW_T);
}

#define INV_BLINK_T 200
void DrawPlayer()
{
	XCPT_L
	if(!p.IsAlive())
		return;
	bool is_invun = p.IsInvun();
	PIMAGE img;
	UINT hm = p.HeartMode();
	if(hm == HEART_BLUE)
		img = pic img_player_blue;
	else if(hm == HEART_GREEN)
        img = pic img_player_green;
	else
		img = pic img_player;
		
	if(is_invun && (clock() % INV_BLINK_T < INV_BLINK_T / 2))
		return;	//无敌时间有一半时间不画 
	putimage_withalpha(NULL,img, 
									p.x - pic p_w/2, p.y - pic p_h/2);
	if(hm == HEART_GREEN)
//	    putimage_withalpha(NULL, (p.IsShieldTouched()?pic img_shield_1:pic img_shield),
//									p.x - pic p_w/2, p.y - pic p_h/2);
        putimage_rotate(NULL, (p.IsShieldTouched()?pic img_shield_1:pic img_shield), p.x - pic p_w/2, p.y - pic p_h/2, 0.5f, 1.8f, 
			RadianForDrawing(DirToRadian(p.dir)), 1, -1, 1);
	XCPT_R
} 
void DrawScene()
{
	XCPT_L
	cleardevice();
	
	DrawIndicators();
	DrawPlayer();
	DrawEntities(); //这样的顺序才能判定伤害 
	
	setfont(25,0,"Lucida Console");
	setcolor(EGERGB(233,233,233));
	char hpmsg[30]{0};
	sprintf(hpmsg,"HP %d / %d", p.hp, p.max_hp);
	xyprintf(g_uix/2 - textheight('1') * strlen(hpmsg) / 2, g_uiy - 70,hpmsg);
	
	xyprintf(0,0,"X=%.1f  Y=%.1f  dir=%d  entities.size=%03d", p.x, p.y , UINT(p.dir), entities.size());
	xyprintf(0,25,"FPS=%.1f  v=%.3f  v_g=%.3f  onGND=%d", getfps(), p.v, p.v_g, int(p.OnGround()));
	setcolor(LIGHTGRAY);
	xyprintf(0,50,"Message=%s",g_msg.c_str());
	xyprintf(0,75,"IsMoving=%d",int(p.IsMoving()));
	if(entities.size() > 0)
//		xyprintf(0,100,"theta=%.1f theta_r=%.1f",entities.at(0).theta,entities.at(0).theta_r);
		xyprintf(0,100,"1stEntity: id=%02d x=%04.1lf y=%04.1lf v=%02.2lf a=%02.3lf v_r=%02.3lf θ0=%2f",entities.at(0).GetID(), entities.at(0).x, entities.at(0).y, entities.at(0).v, entities.at(0).a, entities.at(0).v_r, RadianToDegree(entities.at(0).theta_0));
//	xyprintf(0,125,"#%3d",g_temp[9]);
	xyprintf(0,125,"lenBones: %2d", pic img_bones.size());
	
	if(ShouldMessageShown())
	{
		setfont(70,0,"Agency FB");
		setcolor(YELLOW);
		midprinty(g_msg, g_uiy / 2 - 100);
	}
	
	delay_fps(60);
	XCPT_R
}


//Bullets Spawning Styles
#define SPN_CENTER 0			//in 2*PI
#define SPN_H_ATK 1
#define SPN_V_ATK 2
#define SPN_UP 3
#define SPN_RIGHTUP 4
#define SPN_RIGHT 5
#define SPN_RIGHTDOWN 6
#define SPN_DOWN 7
#define SPN_LEFTDOWN 8
#define SPN_LEFT 9
#define SPN_LEFTUP 10
#define SPN_4_CENTER 11		//k * PI / 2
#define SPN_8_CENTER 12			//k * PI / 4

clock_t lastSpawn1;
clock_t lastSpawn2;
clock_t lastSpawn3;
clock_t lastSpawn4;
short spawnMark=0;
short spawnMark2=0;
short spawnMark3=0;
short spawnMark4=0;
short spawnMark5=0;

inline bool IsSpear(ENTITY_ID id)
{
	return edata.at(id-1).name.find("矛")!=string::npos;
}
void ExtraEntityPreparation(Entity& et, const char attrib, bool mute=false)
{
	if(IsSpear(et.GetID()))
	{
		if(attrib == ETA_ROTATE)
		{
			et.AddAttrib(ETA_ROTATE);
			et.AddAttrib(ETA_FADE);
			if(!mute)
				Sound(SND_LOW_WARN);
		}else{
//			Sound(SND_SPEAR);
		}
	}
	if(IsDelayAttrib(attrib))
	{
		et.AddAttrib(attrib);
		et.AddAttrib(ETA_FADE);
		if(!mute && IsSpear(et.GetID()))
			Sound(SND_LOW_WARN);
	}
	if(et.GetID() == 8)
		et.AddAttrib(ETA_NOROTATE);
	if(et.GetID() == 7)
		et.AddAttrib(ETA_SEMIROTATE);
	if(et.ai == AI_SINE_WAVE || et.ai == AI_ZIGZAG)
	{	//<!>
		et.v_r = 1.2;
		et.theta_r = et.theta;
		et.ox = et.x;
		et.oy = et.y;
	}
 } 
#define DEF_BONE_R 50
void SpawnEntityEx(ENTITY_ID id, 
                   float x, float y, 
				   float v, float theta, 
                   float r/*=0.0f*/, float ox/*=-1.0f*/, float oy/*=-1.0f*/,
				   const char attrib/* = ETA_NONE*/, bool mute/*=false*/)
{
	XCPT_L
	
	Entity et(id);
	et.x = x;
	et.y = y;
	if(r != 0.0f)
		et.r = r;
	else if(edata.at(id-1).IsBone())
	{
		et.r = DEF_BONE_R;
	}
	if(ox > 0.0f)
	    et.ox = ox;
    if(oy > 0.0f)
	    et.oy = oy;
	    
	et.theta = theta;
	et.SetID(id);
	et.FitAIType();
	et.v = v;
//		AI_ID ai = GetEntityAI(et.GetID());
//		et.ai = ai;
	et.status = ST_ALIVE;
	et.start = clock();
	ExtraEntityPreparation(et,attrib,mute);
	entities.push_back(et);
	XCPT_R
}
void SpawnEntity(ENTITY_ID id, BYTE style = SPN_CENTER, const char attrib = ETA_NONE, bool mute=false)
{
	XCPT_L
	Entity et(id);
//	et.CheckID("SpawnEntity");
	if(style == SPN_CENTER)
	{
		short k1,k2;
		k1 = Choice({-1,0,1});
		if(k1 == 0)
			k2 = Choice({-1,1});	//防止在正中央刷 
		else
			k2 = Choice({-1,0,1});
		et.x = g_uix / 2 + k1 * Randint(g_uix / 2 - 60, g_uix / 2 - 10);
		et.y = g_uiy / 2 + k2 * Randint(g_uiy / 2 - 60, g_uiy / 2 - 10);
		et.v = Varience2(GetEntitySpeed(et.GetID()), 0.01);
//		et.theta = atan2(-p.y + et.y, p.x - et.x);	//太准了反而降低难度 
		et.theta = atan2(-p.y + et.y + Randint(-55,55), p.x - et.x + Randint(-105,105));
//		et.theta = Towards(et.x,et.y,p.x,p.y);
		et.theta_0 = et.theta;
		et.theta_r = et.theta;
	}else if(style == SPN_H_ATK)
	{	//横向攻击 
		bool left_zero_right_one = Choice({false,true});
		if(!left_zero_right_one)	//left
			et.x = Randint(g_uix, g_uix + 3);
		else
			et.x = Randint(-3,0);
		et.y = Randint(10, g_uiy - 10);
		et.v = Varience2(GetEntitySpeed(et.GetID()), 0.04);
		et.theta = (left_zero_right_one ? 0.0f : -PI);
	}else if(style == SPN_V_ATK)
	{	//纵向攻击 
		bool up_zero_down_one = Choice({false,true});
		if(up_zero_down_one)	//down
			et.y = Randint(-3, 0);
		else
			et.y = Randint(g_uiy, g_uiy + 3);
		et.x = Randint(10, g_uix - 10);
		et.v = Varience2(GetEntitySpeed(et.GetID()), 0.04);
		et.theta = (up_zero_down_one ? (3 * PI / 2) : (PI / 2));
	}else if(style >= SPN_UP && style <= SPN_LEFTUP)
	{	//八向一起处理 
		if(style == SPN_UP || style == SPN_DOWN)
		{
			et.x = Randint(10, g_uix - 10);
			if(style == SPN_UP)
			{
				et.y = Randint(g_uiy, g_uiy + 3);
				et.theta = PI / 2;
			}else{
				et.y = Randint(-3,0);
				et.theta = PI * 3 / 2;
			}
		}else if(style == SPN_RIGHT || style == SPN_LEFT)
		{
			et.y = Randint(10, g_uiy - 10);
			if(style == SPN_RIGHT)
			{
				et.x = Randint(-3,0);
				et.theta = 0.0f;
			}else{
				et.x = Randint(g_uix, g_uix + 3);
				et.theta = -PI;
			}
		}else if(style == SPN_RIGHTUP)
		{
			bool c1 = Choice({false,true});
			if(c1)
			{
				et.x = Randint(-3,0);
				et.y = Randint(g_uiy * 0.3f , g_uiy - 10);
			}else{
				et.x = Randint(10, g_uix * 0.7f);
				et.y = Randint(g_uiy, g_uiy + 3);
			}
			et.theta = PI / 4;
			VarienceA2(et.theta , PI / 36);
		}else if(style == SPN_RIGHTDOWN)
		{
			bool c2 = Choice({true,false});
			if(c2)
			{
				et.x = Randint(-3, 0);
				et.y = Randint(10 , g_uiy * 0.7f);
			}else{
				et.x = Randint(10, g_uix * 0.7f);
				et.y = Randint(-3, 0);
			}
			et.theta = PI * 7 / 4;	//-PI/4
			VarienceA2(et.theta , PI / 36);
		}else if(style == SPN_LEFTDOWN)
		{
			bool c3 = Choice({true,false});
			if(c3)
			{
				et.x = Randint(g_uiy, g_uiy + 3);
				et.y = Randint(10, g_uiy * 0.7f);
			}else{
				et.x = Randint(g_uix * 0.3f, g_uix - 10);
				et.y = Randint(-3, 0);
			}
			et.theta = PI * 5 / 4;
			VarienceA2(et.theta , PI / 36);
		}else if(style == SPN_LEFTUP)
		{
			bool c4 = Choice({false,true});
			if(c4)
			{
				et.x = Randint(g_uiy , g_uiy + 3);
				et.y = Randint(g_uiy * 0.7f, g_uiy - 10);
			}else{
				et.x = Randint(g_uix * 0.3f, g_uix - 10);
				et.y = Randint(g_uiy, g_uiy + 3);
			}
			et.theta = PI * 3 / 4;
			VarienceA2(et.theta , PI / 36);
		}
		et.v = Varience2(GetEntitySpeed(et.GetID()), 0.04);
	}
	else if(style == SPN_4_CENTER)
	{	//k * PI / 2 攻击 
		DIR dir = RandomDir4();
		if(dir == UP || dir == DOWN)
		{
			et.x = Randint(10, g_uix - 10);
			if(dir == UP)
			{
				et.y = Randint(g_uiy, g_uiy + 3);
				et.theta = PI / 2;
			}else{
				et.y = Randint(-3,0);
				et.theta = PI * 3 / 2;
			}
		}else if(dir == RIGHT || dir == LEFT)
		{
			et.y = Randint(10, g_uiy - 10);
			if(dir == RIGHT)
			{
				et.x = Randint(-3,0);
				et.theta = 0.0f;
			}else{
				et.x = Randint(g_uix, g_uix + 3);
				et.theta = -PI;
			}
		}
		et.v = Varience2(GetEntitySpeed(et.GetID()), 0.04);
	}else if(style == SPN_8_CENTER)
	{	// k * PI / 8 攻击 
		DIR dir = RandomDir8();
		if(dir == UP || dir == DOWN)
		{
			et.x = Randint(10, g_uix - 10);
			if(dir == UP)
			{
				et.y = Randint(g_uiy, g_uiy + 3);
				et.theta = PI / 2;
			}else{
				et.y = Randint(-3,0);
				et.theta = PI * 3 / 2;
			}
		}else if(dir == RIGHT || dir == LEFT)
		{
			et.y = Randint(10, g_uiy - 10);
			if(dir == RIGHT)
			{
				et.x = Randint(-3,0);
				et.theta = 0.0f;
			}else{
				et.x = Randint(g_uix, g_uix + 3);
				et.theta = -PI;
			}
		}	//↓斜向分开讨论 
		else if(dir == RIGHTUP)
		{
			bool c1 = Choice({true,false});
			if(c1)
			{
				et.x = Randint(-3,0);
				et.y = Randint(g_uiy * 0.3f , g_uiy - 10);
			}else{
				et.x = Randint(10, g_uix * 0.7f);
				et.y = Randint(g_uiy, g_uiy + 3);
			}
			et.theta = PI / 4;
			VarienceA2(et.theta , PI / 24);	//加大难度 
		}else if(dir == RIGHTDOWN)
		{
			bool c2 = Choice({true,false});
			if(c2)
			{
				et.x = Randint(-3, 0);
				et.y = Randint(10 , g_uiy * 0.7f);
			}else{
				et.x = Randint(10, g_uix * 0.7f);
				et.y = Randint(-3, 0);
			}
			et.theta = PI * 7 / 4;	//-PI/4
			VarienceA2(et.theta , PI / 24);	//加大难度 
		}else if(dir == LEFTDOWN)
		{
			bool c3 = Choice({true,false});
			if(c3)
			{
				et.x = Randint(g_uiy, g_uiy + 3);
				et.y = Randint(10, g_uiy * 0.7f);
			}else{
				et.x = Randint(g_uix * 0.3f, g_uix - 10);
				et.y = Randint(-3, 0);
			}
			et.theta = PI * 5 / 4;
			VarienceA2(et.theta , PI / 24);	//加大难度 
		}else if(dir == LEFTUP)
		{
			bool c4 = Choice({true,false});
			if(c4)
			{
				et.x = Randint(g_uiy , g_uiy + 3);
				et.y = Randint(g_uiy * 0.7f, g_uiy - 10);
			}else{
				et.x = Randint(g_uix * 0.3f, g_uix - 10);
				et.y = Randint(g_uiy, g_uiy + 3);
			}
			et.theta = PI * 3 / 4;
			VarienceA2(et.theta , PI / 24);	//加大难度 
		}
		et.v = Varience2(GetEntitySpeed(et.GetID()), 0.04);
	}
	else{
		DebugLog("SpawnEntity: Invalid Spawning Style ="+ToString(style));
		return;
	}
	
	et.SetID(id);
	AI_ID ai = GetEntityAI(et.GetID());
	et.SetAI(ai);
//	DebugLog(ToString(ai));
	et.status = ST_ALIVE;
	et.start = clock();
	ExtraEntityPreparation(et,attrib,mute);
	entities.push_back(et);
	XCPT_R
}
void BonePair(ENTITY_ID id, DIR dir, float vratio = 1.0f, float gapRatio = 0.5f , float gap = 140, bool y_whole = true)
{	//sans 的骨头对 
	short allx=g_uix, ally=(y_whole?g_uiy:BLUE_BOTTOM);
//	static const short gap = 150;
	short len1,len2;
	const short lmt=pic boneEnd_h * 0.2f;
	if(dir == RIGHT || dir == LEFT)
	{
		len1 = (ally * gapRatio - gap/2);
		len2 = ally - len1 - gap;
		if(len1 > lmt)
			SpawnEntityEx(id,(dir==RIGHT?0:g_uix),len1/2,GetEntitySpeed(id)*vratio,(dir==RIGHT?0.0f:PI),len1/2);
		if(len2 > lmt)
			SpawnEntityEx(id,(dir==RIGHT?0:g_uix),ally-len2/2,GetEntitySpeed(id)*vratio,(dir==RIGHT?0.0f:PI),len2/2);
	}else if(dir == DOWN || dir == UP)
	{
		len1 = (allx * gapRatio - gap/2);
		len2 = allx - len1 - gap;
		if(len1 > lmt)
			SpawnEntityEx(id,len1/2,(dir==DOWN?0:g_uiy),GetEntitySpeed(id)*vratio,(dir==DOWN?PI*3/2:PI/2),len1/2);
		if(len2 > lmt)
			SpawnEntityEx(id,allx-len2/2,(dir==DOWN?0:g_uiy),GetEntitySpeed(id)*vratio,(dir==DOWN?PI*3/2:PI/2),len2/2);
	}else{
		DebugLog("<!> BonePair: Invalid spawning dir!");
		return;
	}
}
float GetCornerX(DIR dir)
{
	if(dir == RIGHTUP || dir == RIGHTDOWN)
		return g_uix / 2 + (g_uix > g_uiy ? g_uiy / 2 : g_uix / 2);
	else if(dir == LEFTUP || dir == LEFTDOWN)
		return g_uix / 2 - (g_uix > g_uiy ? g_uiy / 2 : g_uix / 2);
	else{
		DebugLog("<!> GetCornerX: Invalid dir");
		return 20.0f;
	}
}
float GetCornerY(DIR dir)
{
	if(dir == RIGHTUP || dir == LEFTUP)
		return g_uiy / 2 - (g_uix > g_uiy ? g_uiy / 2 : g_uix / 2);
	else if(dir == RIGHTDOWN || dir == LEFTDOWN)
		return g_uiy / 2 + (g_uix > g_uiy ? g_uiy / 2 : g_uix / 2);
	else{
		DebugLog("<!> GetCornerY: Invalid dir");
		return 20.0f;
	}
}
float GetSAX(DIR dir)
{
	if(dir == RIGHT)
	    return g_uix;
    else if(dir == DOWN || dir == UP)
        return g_uix/2;
    else if(dir == LEFT)
        return 0;
    else
    	return GetCornerX(dir); 
}
float GetSAY(DIR dir)
{
	if(dir == RIGHT || dir == LEFT)
	    return g_uiy/2;
    else if(dir == UP)
        return 0;
    else if(dir == DOWN)
        return g_uiy;
    else
        return GetCornerY(dir);
}

float GetSACircleX(DIR dir)
{	//返回矛头圆圈X坐标 
	return (g_uix/2 + GetScreenRadius() * cos(DirToRadian(dir)));
}
float GetSACircleY(DIR dir)
{	//返回矛头圆圈Y坐标 
	return (g_uiy/2 - GetScreenRadius() * sin(DirToRadian(dir)));
}
#define DIR_V 1.0f	// |
#define DIR_OB (sqrt(2)/2.0f)	// /
#define DIR_H 0.0f	// __
inline float GetDirTrend(DIR dir)
{
	switch(dir)
	{
		case UP:case DOWN:	return DIR_V;break;
		case LEFT:case RIGHT: return DIR_H;break;
		default:  return DIR_OB;break;
	}
}
void SpawnSA(ENTITY_ID id, DIR dir, float v_rate=1.0f)
{
	DIR face = OppoDir(dir);
	if(id == 15)
		face = dir;
	SpawnEntityEx(id,
	              GetSACircleX(dir),GetSACircleY(dir),
				  GetEntitySpeed(id)* v_rate 
				  					/** (GetDirTrend(dir)==DIR_H ?
									  	 1.0f : (GetDirTrend(dir) * (g_uiy / (float)g_uix))),	//平衡屏幕纵横比 */,
				  DirToRadian(face));
}
void SpawnSAEx(ENTITY_ID id, float x, float y, DIR dir, float v_rate=1.0f)
{
	DIR face = OppoDir(dir);
	if(id == 15)
		face = dir;
	SpawnEntityEx(id,
	              x,y,
				  GetEntitySpeed(id)* v_rate 
				  					/** (GetDirTrend(dir)==DIR_H ?
									  	 1.0f : (GetDirTrend(dir) * (g_uiy / (float)g_uix))),	//平衡屏幕纵横比 */,
				  DirToRadian(face));
}
void SpawnGBlaster(ENTITY_ID id, float x, float y, float theta, float width)
{	//16
	Entity et(id);
	et.x = x;
	et.y = y;
	et.v = GetEntitySpeed(id);
	et.a = GetEntityAcc(et.GetID());	//don't forget
	et.r = width;
	et.theta = theta;
	et.SetID(id);
	et.FitAIType();
	et.AddAttrib(ETA_BLASTER);
	et.status = ST_ALIVE;
	et.start = clock();
	Sound(SND_GBLASTER_1);
	entities.push_back(et);
}

#define WOOL_CD 100
#define WOOL_WAVE_WARNING_T 1000
#define WOOL_WAVE_INTERNAL 10
#define BONE_SWING_INTERNAL 80
DWORD ThrSpawnChain(LPVOID lpParam)
{	//使用线程可以减少篇幅与耗时 
//	BYTE chain = *((unsigned char*)lpParam);
//	DebugLog("chain="+ToString(chain));
	XCPT_L
	string chain = string((char*)(lpParam));
	const int cnt = 10;
	const int _left = g_uix / 2 - cnt / 2 * getwidth(GetEntityImage(12-1)) * 3.0;
	const int _right = g_uix / 2 + cnt / 2 * getwidth(GetEntityImage(12-1)) * 3.0;
	const int _top = g_uiy / 2 - cnt / 2 * getheight(GetEntityImage(12-1)) * 3.0;
	const int _bottom = g_uiy / 2 + cnt / 2 * getheight(GetEntityImage(12-1)) * 3.0;
	
	const int wcnt = 64;
	const int bscnt = 10;
	
	const float _spr = 150.0f;
	const float _spr_delta = 50.0f;
	const float _spw = getwidth(GetEntityImage(1)) * 1.6f;
	
	if(chain == "sans_gblaster")
	{
		api_sleep(1000);
		//井字形 
		SpawnGBlaster(16,100,200,0.0,100);
		SpawnGBlaster(16,100,g_uiy-200,0.0,100);
		SpawnGBlaster(16,g_uix-100,200,PI,100);
		SpawnGBlaster(16,g_uix-100,g_uiy-200,PI,100);
		api_sleep(100);
		SpawnGBlaster(16,g_uix/2-200,100,PI*3/2.0,140);
		SpawnGBlaster(16,g_uix/2+200,100,PI*3/2.0,140);
		SpawnGBlaster(16,g_uix/2-300,g_uiy-100,PI/2.0,140);
		SpawnGBlaster(16,g_uix/2+300,g_uiy-100,PI/2.0,140);
		api_sleep(1700);
		//X字形 
		SpawnGBlaster(16,GetSAX(LEFTDOWN),GetSAY(LEFTDOWN),PI/4.0,190);
		SpawnGBlaster(16,GetSAX(RIGHTDOWN),GetSAY(RIGHTDOWN),PI*3/4.0,190);
		SpawnGBlaster(16,GetSAX(LEFTUP),GetSAY(LEFTUP),PI*7/4.0,190);
		SpawnGBlaster(16,GetSAX(RIGHTUP),GetSAY(RIGHTUP),PI*5/4.0,190);
		api_sleep(1400);
		//左右互打 
		SpawnGBlaster(16,100,g_uiy/2,0.0f,220);
		SpawnGBlaster(16,g_uix-100,g_uiy/2,PI,220);
		api_sleep(2200);
		//井字形 
		SpawnGBlaster(16,100,200,0.0,100);
		SpawnGBlaster(16,100,g_uiy-200,0.0,100);
		SpawnGBlaster(16,g_uix-100,200,PI,100);
		SpawnGBlaster(16,g_uix-100,g_uiy-200,PI,100);
		api_sleep(50);
		SpawnGBlaster(20,g_uix/2,150,PI*3/2.0,160);
		api_sleep(90);
		SpawnGBlaster(16,g_uix/2-200,100,PI*3/2.0,140);
		SpawnGBlaster(16,g_uix/2+200,100,PI*3/2.0,140);
		SpawnGBlaster(16,g_uix/2-300,g_uiy-100,PI/2.0,140);
		SpawnGBlaster(16,g_uix/2+300,g_uiy-100,PI/2.0,140);
		api_sleep(70);
		SpawnGBlaster(18,150,g_uiy/2,0.0f,180);
		api_sleep(1700);
		//纵向平行线 
		SpawnGBlaster(18,g_uix * 0.45f,100,PI*3/2.0f,120);
		api_sleep(10);
		SpawnGBlaster(18,g_uix * 0.55f,100,PI*3/2.0f,120);
		api_sleep(10);
		SpawnGBlaster(16,g_uix * 0.25f,g_uiy-100,PI/2.0f,140);
		api_sleep(50);
		SpawnGBlaster(18,g_uix * 0.35f,100,PI*3/2.0f,120);
		api_sleep(10);
		SpawnGBlaster(18,g_uix * 0.65f,100,PI*3/2.0f,120);
		api_sleep(10);
		SpawnGBlaster(16,g_uix * 0.75f,g_uiy-100,PI/2.0f,140);
		api_sleep(1900);
		//横向平行线 
		SpawnGBlaster(20,100,g_uiy * 0.45,0.0f, 120);
		api_sleep(10);
		SpawnGBlaster(20,100,g_uiy * 0.55,0.0f, 120);
		api_sleep(20);
		SpawnGBlaster(18,g_uiy-100,g_uiy * 0.5, PI, 70);
		api_sleep(5);
		SpawnGBlaster(20,100,g_uiy * 0.25,0.0f, 140);
		api_sleep(10);
		SpawnGBlaster(20,100,g_uiy * 0.75,0.0f, 140);
		api_sleep(60);
		SpawnGBlaster(20,g_uix-100,p.y-pic p_h,PI,50);
		SpawnGBlaster(20,g_uix-100,p.y+pic p_h,PI,50);
	}
	else if(chain == "wools_h")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _left + i * getwidth(GetEntityImage(12)) * 3.0;
			et.y = 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _right - i * getwidth(GetEntityImage(12)) * 3.0;
			et.y = g_uiy - 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_v")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = 100;
			et.y = _top + i * getheight(GetEntityImage(12)) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = g_uix - 100;
			et.y = _bottom - i * getheight(GetEntityImage(12)) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_leftdown")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = 100;
			et.y = _bottom - i * getheight(GetEntityImage(12)) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _left + i * getwidth(GetEntityImage(12)) * 3.0;
			et.y = g_uiy - 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_leftup")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = 100;
			et.y = _top + i * getheight(GetEntityImage(et.GetID())) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _left + i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.y = 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_rightup")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = g_uix - 100;
			et.y = _top + i * getheight(GetEntityImage(et.GetID())) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _right - i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.y = 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_rightdown")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = g_uix - 100;
			et.y = _bottom - i * getheight(GetEntityImage(et.GetID())) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _right - i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.y = g_uiy - 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_down")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _right - i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.y = g_uiy - 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_up")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = _left + i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.y = 100;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_left")
	{
		for(int i=0; i < cnt; i ++)
		{
			Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = 100;
			et.y = _top + i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wools_right")
	{
		for(int i=0; i < cnt; i ++)
		{
		    Entity et(12);
			et.v = GetEntitySpeed(et.GetID()) * (cnt - abs(i - cnt/2)) / cnt * 1.5f;
			et.x = g_uix - 100;
			et.y = _bottom - i * getwidth(GetEntityImage(et.GetID())) * 3.0;
			et.theta = atan2(-p.y + et.y + Randint(-5,5), p.x - et.x + Randint(-10,10));
			et.AddAttrib(ETA_DELAY_3s);
			et.start = clock();
			entities.push_back(et);
			api_sleep(WOOL_CD);
		}
	}else if(chain == "wool_wave_right")
	{
		CreateIndicator(g_uix / 2 , 2, g_uix - 1, g_uiy - 2, WOOL_WAVE_WARNING_T);
		api_sleep(WOOL_WAVE_WARNING_T);
		Sound(SND_CRACK2);
		for(int i=0; i < wcnt; i ++)
		{
			Entity et(12);
			et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
			et.x = Randint(g_uix / 2 - 1, g_uix - 3);
			et.y = g_uiy;
			et.theta = PI / 2;	//up
			et.start = clock();
			entities.push_back(et);
			if(i % 4 == 0)
				api_sleep(WOOL_WAVE_INTERNAL);
		}
	}else if(chain == "wool_wave_left")
	{
		CreateIndicator(2 , 2, g_uix / 2 + 1, g_uiy - 2, WOOL_WAVE_WARNING_T);
		api_sleep(WOOL_WAVE_WARNING_T);
		Sound(SND_CRACK2);
		for(int i=0; i < wcnt; i ++)
		{
			Entity et(12);
			et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
			et.x = Randint(0, g_uix / 2 + 1);
			et.y = g_uiy;
			et.theta = PI / 2;	//up
			et.start = clock();
			entities.push_back(et);
			if(i % 4 == 0)
				api_sleep(WOOL_WAVE_INTERNAL);
		}
	}else if(strxtail(chain,1) == "wool_wave_h_")
	{
//		AddMessage(chain);
//		AddMessage(strtail(chain,2));
		short anum = ToShort(strtail(chain,2)[0]);
//		g_msg += "" + ToString(anum);
		//创建警告区域 
		CreateIndicator((anum-1) * (g_uix / 4), 2, (anum) * (g_uix / 4), g_uiy - 2, WOOL_WAVE_WARNING_T);
		api_sleep(WOOL_WAVE_WARNING_T);
		Sound(SND_CRACK2);
		for(int i=0; i < wcnt; i ++)
		{
			Entity et(12);
			et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
			et.x = Randint((anum-1) * (g_uix / 4) + 2, (anum) * (g_uix / 4) - 2);
			et.y = g_uiy;
			et.theta = PI / 2;	//up
			et.start = clock();
			entities.push_back(et);
			if(i % 4 == 0)
				api_sleep(WOOL_WAVE_INTERNAL);
		}
	}else if(strxtail(chain,1) == "wool_wave_v_")
	{
//		AddMessage(chain);
		short anum = ToShort(strtail(chain,2)[0]);
		//创建警告区域 
		CreateIndicator(2, (anum-1) * (g_uiy / 4), g_uix - 2, (anum) * (g_uiy / 4), WOOL_WAVE_WARNING_T);
		api_sleep(WOOL_WAVE_WARNING_T);
		Sound(SND_CRACK2);
		for(int i=0; i < wcnt; i ++)
		{
			Entity et(12);
			et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
			et.y = Randint((anum-1) * (g_uiy / 4) + 2, (anum) * (g_uiy / 4) - 2);
			et.x = 0;
			et.theta = 0.0f;	//Right
			et.start = clock();
			entities.push_back(et);
			if(i % 4 == 0)
				api_sleep(WOOL_WAVE_INTERNAL);
		}
	}else if(chain == "wool_wave_h_random")
	{
		long w = Randint(g_uix / 6 , g_uix / 4);
		long left1 = Randint(2,g_uix - w - 2);
		long right1 = left1 + w;
		//创建警告区域 
		CreateIndicator(left1, 2, right1, g_uiy - 2, WOOL_WAVE_WARNING_T);
		api_sleep(WOOL_WAVE_WARNING_T);
		Sound(SND_CRACK2);
		for(int i=0; i < wcnt / 2; i ++)
		{
			Entity et(12);
			et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
			et.x = Randint(left1 + 2, right1 - 2);
			et.y = g_uiy;
			et.theta = PI / 2;	//up
			et.start = clock();
			entities.push_back(et);
			if(i % 2 == 0)
				api_sleep(WOOL_WAVE_INTERNAL);
		}
	}else if(chain == "wool_wave_v_random")
	{
		long h = Randint(g_uiy / 6 , g_uiy / 4);
		long top1 = Randint(2,g_uiy - h - 2);
		long bottom1 = top1 + h;
		//创建警告区域 
		CreateIndicator(2, top1, g_uix - 2, bottom1, WOOL_WAVE_WARNING_T);
		api_sleep(WOOL_WAVE_WARNING_T);
		Sound(SND_CRACK2);
		for(int i=0; i < wcnt / 2; i ++)
		{
			Entity et(12);
			et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
			et.y = Randint(top1 + 2, bottom1 - 2);
			et.x = 0.0f;
			et.theta = 0.0f;	//Right
			et.start = clock();
			entities.push_back(et);
			if(i % 2 == 0)
				api_sleep(WOOL_WAVE_INTERNAL);
		}
	}else if(chain == "wool_wave_h_four")
	{
		for(short j = 0; j <= 4; ++j)
		{
			//创建警告区域 
			CreateIndicator((j-1) * (g_uix / 4), 2, (j) * (g_uix / 4), g_uiy - 2, WOOL_WAVE_WARNING_T);
			api_sleep(WOOL_WAVE_WARNING_T);
			Sound(SND_CRACK2);
			for(int i=0; i < wcnt; i ++)
			{
				Entity et(12);
				et.v = Varience2(GetEntitySpeed(et.GetID()) * 4.0f, GetEntitySpeed(et.GetID()) * 1.0f);
				et.x = Randint((j-1) * (g_uix / 4) + 2, (j) * (g_uix / 4) - 2);
				et.y = g_uiy;
				et.theta = PI / 2;	//up
				et.start = clock();
				entities.push_back(et);
				if(i % 4 == 0)
					api_sleep(WOOL_WAVE_INTERNAL);
			}
		}
	} 
	else if(chain == "spear_v_row")
	{
		bool up1 = Choice({true,false});
		short cnt = ((g_uix / _spw) );
		short except = Randint(cnt*0.3f,cnt*0.7f);
		for(short i = 0; i < cnt; ++i)
		{
			if(i == except)
			    continue;
			Entity et(1);
			et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			et.y = (up1?g_uiy-10:10);
			et.x = i * _spw + 10;
			et.theta = (up1?(PI/2.0f):(PI*3.0f/2.0f));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}
	else if(chain == "spear_h_row")
	{
		bool right1 = Choice({true,false});
		short cnt = (g_uiy / _spw );
		short except = Randint(cnt*0.3f,cnt*0.7f);
		for(short i = 0; i < cnt; ++i)
		{
			if(i == except)
			    continue;
			Entity et(1);
			et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			et.x = (right1?g_uix:0);
			et.y = i * _spw + 10;
			et.theta = (!right1?(0.0f):(PI));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_v_row2")
	{
		short cnt = (g_uix / _spw );
		short except = Randint(cnt*0.3f,cnt*0.7f);
		for(short i = 0; i < cnt; ++i)
		{
			if(i == except)
			    continue;
			Entity et(1);
			et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			et.y = ((i%2)?g_uiy-10:10);
			et.x = i * _spw + 10;
			et.theta = ((i%2)?(PI/2.0f):(PI*3.0f/2.0f));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_h_row2")
	{
		short cnt = (g_uiy / _spw );
		short except = Randint(cnt*0.3f,cnt*0.7f);
		for(short i = 0; i < cnt; ++i)
		{
			if(i == except)
			    continue;
			Entity et(1);
			et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			et.x = ((i%2)?g_uix:0);
			et.y = i * _spw + 10;
			et.theta = (!(i%2)?(0.0f):(PI));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_h_row_acc")
	{
		bool right1 = Choice({true,false});
		short cnt = (g_uiy / _spw );
		for(short i = 0; i < cnt; ++i)
		{
			Entity et(i % 5 ? 9 : 1);
			if(et.GetID() == 1)
				et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			else
				et.v = GetEntitySpeed(et.GetID()) * 1.1f;
			et.x = ((!right1)?g_uix:0);
			et.y = i * _spw + 10;
			et.theta = ((right1)?(0.0f):(PI));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_v_row_acc")
	{
		bool up1 = Choice({true,false});
		short cnt = (g_uix / _spw );
		for(short i = 0; i < cnt; ++i)
		{
			Entity et(i % 5 ? 9 : 1);
			if(et.GetID() == 1)
				et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			else
				et.v = GetEntitySpeed(et.GetID()) * 1.1f;
			et.y = ((up1)?g_uiy:0);
			et.x = i * _spw + 10;
			et.theta = ((!up1)?(PI * 3.0f / 2.0f):(PI / 2.0f));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_h_row2_acc")
	{
		short cnt = (g_uiy / _spw );
		for(short i = 0; i < cnt; ++i)
		{
			Entity et(i % 5 ? 9 : 1);
			if(et.GetID() == 1)
				et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			else
				et.v = GetEntitySpeed(et.GetID()) * 1.1f;
			et.x = ((i%2)?g_uix:0);
			et.y = i * _spw + 10;
			et.theta = (!(i%2)?(0.0f):(PI));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_v_row2_acc")
	{
		short cnt = (g_uix / _spw );
		for(short i = 0; i < cnt; ++i)
		{
			Entity et(i % 5 ? 9 : 1);
			if(et.GetID() == 1)
				et.v = GetEntitySpeed(et.GetID()) * 2.5f;
			else
				et.v = GetEntitySpeed(et.GetID()) * 1.1f;
			et.y = ((i%2)?g_uiy:0);
			et.x = i * _spw + 10;
			et.theta = (!(i%2)?(PI * 3.0f / 2.0f):(PI / 2.0f));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_h_row_vary")
	{
		bool right1 = Choice({true,false});
		short cnt = (g_uiy / _spw );
		for(short i = 0; i < cnt; ++i)
		{
			Entity et(1);
			et.v = Varience2(GetEntitySpeed(et.GetID())*10.0f, GetEntitySpeed(et.GetID())*20.0f)/10.0f;
			ClampA(et.v, GetEntitySpeed(et.GetID())*0.5f,GetEntitySpeed(et.GetID()) * 2.4f );	//limit
			et.x = ((!right1)?g_uix:0);
			et.y = i * _spw + 10;
			et.theta = (right1?(0.0f):(PI));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(chain == "spear_v_row_vary")
	{
		bool down1 = Choice({true,false});
		short cnt = (g_uix / _spw );
		for(short i = 0; i < cnt; ++i) 
		{
			Entity et(1);
			et.v = Varience2(GetEntitySpeed(et.GetID())*10.0f, GetEntitySpeed(et.GetID())*20.0f)/10.0f;
			ClampA(et.v, GetEntitySpeed(et.GetID())*0.5f,GetEntitySpeed(et.GetID()) * 2.4f );	//limit
//			et.x = ((!right1)?g_uix:0);  
//			et.y = i * _spw + 10;
			et.x = i * _spw + 10;
			et.y = ((!down1)?g_uiy:0);
			et.theta = (down1?(PI*1.5f):(PI*.5f));
			et.AddAttrib(ETA_DELAY_2s);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}
	/*else if(chain == "bluebone_up_swing")
	{
		const long _gap = 10;
		const long _step = 20;
		const long _w = 75;
		long _x = _gap;
		char k=0x01;
		while(_x >= _gap || k == 0x01)
		{
			for(int i=0; i < bscnt; i ++)
			{
				Entity et;
				et.GetID() = 10;
				et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.1f);
				et.x = _x + Randint(0, _w);
				et.y = g_uiy;
				et.theta = PI / 2;	//up
				et.start = clock();
				entities.push_back(et);
				if(i % 3 == 0)
					api_sleep(BONE_SWING_INTERNAL);
			}
			_x += k * _step;
			if(_x >= g_uix - _gap)
				k = -k;
		}
	}else if(chain == "orangebone_right_swing")
	{
		const long _gap = 10;
		const long _step = 100;
		const long _h = 75;
		long _y = _gap;
		char k=0x01;
		while(_y >= _gap || k == 0x01)
		{
			for(int i=0; i < bscnt; i ++)
			{
				Entity et;
				et.GetID() = 11;
				et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.1f);
				et.x = 0;
				et.y = _y + Randint(0, _h);
				et.theta = 0.0f;	//right
				et.start = clock();
				entities.push_back(et);
				if(i % 3 == 0)
					api_sleep(BONE_SWING_INTERNAL);
			}
			_y += k * _step;
			if(_y >= g_uiy - _gap)
				k = -k;
		}
	}*/
	else if(strxhead(chain,1) == "spear_center")
	{	//N矛穿心一并处理 
		short spcnt = atoi(chain.substr(0,1).c_str());
		float theta = Randint(0, 20 * PI) / 10.0f;
		float ox = p.x;
		float oy = p.y;
		float _spr1 = _spr + Randint(0, _spr_delta * 10) / 10.0f;
		for(long i = 0; i < spcnt; ++i)
		{
			Entity et(1);
			et.theta = OppoRadian(theta + i * (2 * PI / spcnt) );
			et.x = ox + cos(OppoRadian(et.theta)) * _spr1;
			et.y = oy - sin(OppoRadian(et.theta)) * _spr1;
			et.v = GetEntitySpeed(et.GetID());
			et.AddAttrib(ETA_DELAY_1s);	//1s delay
			et.AddAttrib(ETA_LIFE_1_5s);
			et.AddAttrib(ETA_ROTATE);
			et.AddAttrib(ETA_FADE);
			et.start = clock();
			entities.push_back(et);
		}
		Sound(SND_LOW_WARN);
	}else if(strxhead(chain,1) == "spear_roundin")
	{	//N矛旋转穿心一并处理 
		short spcnt = atoi(chain.substr(0,1).c_str());
		float theta = Randint(0, 20 * PI) / 10.0f;
		float ox = p.x;
		float oy = p.y;
		float _spr2 = _spr*1.8f + Randint(0, _spr_delta * 10) / 10.0f;
		float k = Choice({-1,1});
		for(long i = 0; i < spcnt; ++i)
		{
			Entity et(1);
			et.theta_r = OppoRadian(theta + i * (2 * PI / spcnt) );
			et.theta = OppoRadian(et.theta_r);
			et.x = ox + cos(OppoRadian(et.theta_r)) * _spr2;
			et.y = oy - sin(OppoRadian(et.theta_r)) * _spr2;
			et.ox = ox,	et.oy = oy;
			et.v = 0.2f;
			et.v_r = k * 1.1f;
			et.r = _spr2;
			et.a = k * -0.0008f;
			et.ai = AI_CIRCLE_ACC_IN;
			et.AddAttrib(ETA_LIFE_1_5s);
			et.AddAttrib(ETA_FADE);
			et.start = clock();
			entities.push_back(et);
		}
	}
	else{
		return 0L;
	}
	XCPT_R
	return 1L;
}
void PerformEntitiesAttack(void)
{
	if(entities.empty())	return;
	for(short i = 0; i < entities.size(); ++i)
		entities.at(i).Attack();
}
typedef short OPER_ID;
#define OPER_REMOVE 0x01
#define OPER_SET_ID 0x02
#define OPER_SET_V 0x03
#define OPER_SET_A 0x04
#define OPER_SET_X 0x05
#define OPER_SET_Y 0x06
#define OPER_REL_X 0x07
#define OPER_REL_Y 0x08
#define OPER_SET_AI 0x09
#define OPER_SET_V_R 0x0A
#define OPER_SET_THETA 0x0B
#define OPER_SET_THETA_R 0x0C
#define OPER_SET_THETA_0 0x0D
short OperateEntities(ENTITY_ID id=-1, OPER_ID oid = OPER_REMOVE, float arg=-1.0) 
{	//对指定ID实体操作  返回操作的数量 
	short ret=0;
	for(short i = 0; i < entities.size(); ++i)
	{
		if(id == -1 || entities.at(i).GetID() == id)
		{
			if(oid == OPER_REMOVE)
			{
				auto iter = entities.begin();
				short j=0;
				while(j < i)
					++iter,++j;
				iter = entities.erase(iter);	//Delete it in the vector
				++ret;
			}else if(oid == OPER_SET_AI)
			{
				entities.at(i).SetAI((AI_ID)arg);
				//...
			}else if(oid == OPER_SET_V)
			{
				entities.at(i).v = arg;
			}else if(oid == OPER_SET_V_R)
			{
				entities.at(i).v_r = arg;
			}else if(oid == OPER_SET_THETA)
			{
				entities.at(i).theta = arg;
			}else if(oid == OPER_SET_THETA_R)
			{
				entities.at(i).theta_r = arg;
			}else if(oid == OPER_SET_THETA_0)
			{
				entities.at(i).theta_0 = arg;
			}
			else{
				DebugLog("<!> OperateEntities: Invalid Operation ID="+ToString(oid));
				return 0;
			}
		}
	}
	return ret;
}
#define SPAWN_SPEAR_CD 1000
#define SPAWN_BONE_H_CD 300
#define SPAWN_BONE_V_CD 300
#define SPAWN_WOOL_S_CD 300
#define SPAWN_WOOL_WAVE_CD 4500
#define SPAWNCHAIN(sz) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(sz),0,NULL)
void CheckEntitiesSpawn(void)
{	//针对不同回合刷弹幕の函数 够长的 
	XCPT_L
        //Start!
		if(g_round <= 9)
		{
			if(clock() - lastSpawn1 >= SPAWN_SPEAR_CD * pow(0.9, g_round - 1))
			{
				SpawnEntity((g_round <= 5 ? 1 : 3), SPN_CENTER, ETA_ROTATE);
				lastSpawn1 = clock();
			}
		}
		else if(g_round == 10)
		{
			if(clock() - lastSpawn1 >= 200)
			{
				SpawnEntity(9,SPN_CENTER,ETA_ROTATE);
				lastSpawn1 = clock();
			}
		}
		else if(g_round >= 11 && g_round <= 13)
		{
			if(g_round == 11 && clock() - lastRound < 20)
			{
					spawnMark = p.y - 85;
					spawnMark2 = 190;
					spawnMark3 = 1;
			}else if(clock() - lastRound < 2)
			{
				if(g_round == 12)
					spawnMark3 = (spawnMark3>0?2:-2);
				else if(g_round == 13)
					spawnMark3 = (spawnMark3>0?3:-3);
				//spawnMark3 += (spawnMark3>0?3:-3);
			}else if(g_round == 13 && clock() - lastRound > ROUND_T/2 && clock() - lastRound < ROUND_T/2+300)
			{
				spawnMark3 = (spawnMark3>0?4:-4);
			}
			if(clock() - lastSpawn1 >= SPAWN_BONE_H_CD * pow(0.6, g_round - 10))
			{
				//SpawnEntity(Choice({3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4}), SPN_H_ATK);
				SpawnEntityEx(1,0,int(spawnMark)%(int)g_uiy,1.5f,0.0f);
				SpawnEntityEx(1,0,int(spawnMark + spawnMark2)%(int)g_uiy,1.5f,0.0f);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 5)
			{
				spawnMark+=spawnMark3;
				if(spawnMark + spawnMark2 >= g_uiy
				|| spawnMark <= 0)
					spawnMark3 = -spawnMark3;
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn3 >= 300)
			{
				spawnMark2-=spawnMark3;
				ClampA(spawnMark2, short(pic p_h * 2.0f), (short)g_uiy);
				lastSpawn3 = clock();
			}
		}else if(g_round >= 14 && g_round <= 15)
		{
			if(g_round == 14 && clock() - lastRound < 30)
			{
					spawnMark = p.x - 160;
					spawnMark2 = 320;
					spawnMark3 = 6;
			}
			if(clock() - lastSpawn1 >= 50)
			{
				SpawnEntityEx(1,int(spawnMark)%(int)g_uix,0,1.8f,PI*3/2);
				SpawnEntityEx(1,int(spawnMark + spawnMark2)%(int)g_uix,0,1.8f,PI*3/2);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 5)
			{
				spawnMark+=spawnMark3;
				if(spawnMark + spawnMark2 >= g_uix
				|| spawnMark <= 0)
					spawnMark3 = -spawnMark3;
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn3 >= 300)
			{
				spawnMark2-=spawnMark3;
				ClampA(spawnMark2, short(pic p_w * 2.0f), (short)g_uix);
				lastSpawn3 = clock();
			}
			//SpawnEntity(Choice({3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5}), SPN_4_CENTER);
			
		}else if(g_round == 16)
		{
			if(clock() - lastSpawn1 >= 150)
			{
				SpawnEntityEx(9,0,Clamp<float>(p.y+Choice({0.0,0.0,-200.0,200.0}),0.0,(float)g_uiy),
				              GetEntitySpeed(9),0.0f,0,0,0,ETA_DELAY_2s,true);
				lastSpawn1 = clock();
			}
		}else if(g_round == 17 || g_round == 19)
		{
			if(clock() - lastSpawn1 >= 220 * pow(0.93f,g_round-17))
			{
				SpawnEntityEx(9,0,Clamp<float>(p.y+Choice({0.0,0.0,-150.0,150.0}),0.0,(float)g_uiy),
				              GetEntitySpeed(9)*0.9f,0.0f,0,0,0,ETA_DELAY_3s,true);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 220 * pow(0.92f,g_round-17))
			{
				SpawnEntityEx(9,Clamp<float>(p.x+Choice({0.0,0.0,-200.0,200.0}),0.0,(float)g_uix),0,
				              GetEntitySpeed(9)*1.1f,PI*1.5f,0,0,0,ETA_DELAY_3s,true);
				lastSpawn2 = clock();
			}
		}else if(g_round == 18 || g_round == 20)
		{
			if(clock() - lastSpawn1 >= 1250 * pow(0.8,g_round-18))
			{
				float o = Choice({0.0,60.0,-60.0});
				SpawnEntityEx(9,g_uix,p.y+o-90,GetEntitySpeed(9)*1.5f,PI,0,0,0,ETA_DELAY_1s,true); 
				SpawnEntityEx(9,g_uix,p.y+o+90,GetEntitySpeed(9)*1.5f,PI,0,0,0,ETA_DELAY_1s,true); 
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 1550 * pow(0.8,g_round-18))
			{
				float o = Choice({0.0,60.0,-60.0});
				SpawnEntityEx(9,p.x+o-90,g_uiy,GetEntitySpeed(9)*1.5f,PI*0.5f,0,0,0,ETA_DELAY_1s,true); 
				SpawnEntityEx(9,p.x+o+90,g_uiy,GetEntitySpeed(9)*1.5f,PI*0.5f,0,0,0,ETA_DELAY_1s,true); 
				lastSpawn2 = clock();
			}
		}else if(g_round == 21 || g_round == 22)
		{
			if(clock() - lastRound < 40)
			     spawnMark = 900;
			if(clock() - lastSpawn1 >= spawnMark * pow(0.85, g_round-21))
			{
				bool h_1_v_0 = Choice({true,false});
				float   _x=(h_1_v_0?(g_uix/2+Choice({-1,1})*(g_uix/2-100)):(Randint(100,g_uix-100))),
						_y=(h_1_v_0?(Randint(100,g_uiy-100)):(g_uiy/2+Choice({-1,1})*(g_uiy/2-100))); 
//				SpawnGBlaster(16,g_uix/2,g_uiy/2, DirToRadian(Choice({LEFT,LEFTUP,RIGHTDOWN,LEFTDOWN,RIGHTUP,RIGHT,UP,DOWN})),70);
				SpawnGBlaster((g_round==21?16:Choice({16,16,18,20})),
							    _x,
								_y, 
								Towards(_x, _y, p.x+Randint(-60,60), p.y+Randint(-60,60)),70);
				lastSpawn1 = clock();
				spawnMark = (g_round==21?900:Choice({900,920,880,1000,1500,750,850}));
			}
		}else if(g_round == 23)
		{
			if(clock() - lastRound < 50)
			{
				if(spawnMark != 88)
				{
					SPAWNCHAIN("sans_gblaster");
					spawnMark = 88;
				}
			}
		}else if(g_round == 24 || g_round == 25)
		{	//blue bones with gblasters
			if(clock() - lastSpawn1 >= 900)
			{
				for(short j = Randint(0,100) ; j < g_uix; j += (g_round==24?180:100))
					SpawnEntityEx(10, j, (g_round==24?g_uiy:0), 1.0f, (g_round==24?PI/2.0f:PI*3/2.0f));
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 3500 * pow(0.8f,g_round-24))
			{
				bool _right = Choice({true,false});
				SpawnGBlaster((g_round==24?20:Choice({16,18,20})),_right?0.0f:g_uix,Choice({p.y,p.y,p.y,p.y+10.0f,p.y-10.0f,(float)Randint(200,g_uiy-200)}),(_right?0.0f:PI),100);
				lastSpawn2 = clock();
			}
		}else if(g_round >= 26 && g_round <= 27)
		{	//spear rows with gblasters 
			if(clock() - lastSpawn1 >= 1500)
			{
				SPAWNCHAIN("spear_h_row");
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 3500 * pow(0.8f,g_round-26))
			{
				bool _down = Choice({true,false});
				SpawnGBlaster((g_round==26?20:Choice({16,18,20})),Choice({p.x,p.x,p.x,p.x+10.0f,p.x-10.0f,(float)Randint(200,g_uix-200)}),_down?0.0f:g_uiy,(_down?PI*3/2.0f:PI/2.0f),100);
				lastSpawn2 = clock();
			}
		}else if(g_round == 28 || g_round == 30)
		{
			const short _gap = 120;
			if(clock() - lastRound < 30)
				p.HeartMode() = HEART_BLUE;
			if(clock() - lastSpawn1 >= 1600)
			{
				short len = Randint(150,350); 
				if(g_round == 30)
					len = 90;
				SpawnEntityEx(2,0,BLUE_BOTTOM-len/2,GetEntitySpeed(3),0.0f,len/2); 
				SpawnEntityEx(2,g_uix,BLUE_BOTTOM-len/2,GetEntitySpeed(3),PI,len/2); 
				SpawnEntityEx(2,-5,(BLUE_BOTTOM-len-_gap)/2,GetEntitySpeed(3),0.0f,(BLUE_BOTTOM-len-_gap)/2); 
				SpawnEntityEx(2,g_uix+5,(BLUE_BOTTOM-len-_gap)/2,GetEntitySpeed(3),PI,(BLUE_BOTTOM-len-_gap)/2); 
				lastSpawn1 = clock();
			}
		}else if(g_round == 29)
		{
			if(clock() - lastRound < 30)
				p.HeartMode() = HEART_BLUE;
			if(clock() - lastSpawn1 >= 999)
			{
				SpawnEntityEx(2,0,BLUE_BOTTOM-50/2,GetEntitySpeed(3),0.0f,50/2); 
				SpawnEntityEx(10,-180,BLUE_BOTTOM-420/2,GetEntitySpeed(3),0.0f,420/2); 
				lastSpawn1 = clock();
			}
		}else if(g_round == 31)
		{
			if(clock() - lastRound < 30 && spawnMark != 99)
			{	//orange bone quick
				SpawnEntityEx(11,0,BLUE_BOTTOM/2,GetEntitySpeed(11)*2.0f,0.0f,BLUE_BOTTOM/2);
				spawnMark = 99;
			}
			if(clock() - lastRound >= 900 && clock() - lastSpawn1 >= spawnMark)
			{
				short len = Choice({50,100,120,140,180,200,240,260,300,500});
				SpawnEntityEx(10,-10,BLUE_BOTTOM-len/2,GetEntitySpeed(3) * Randint(10,30)/10.0f,0.0f,len/2); 
				lastSpawn1 = clock();
				spawnMark = Randint(90,700);
			}
			if(clock() - lastRound >= ROUND_T * 0.4f && (clock() - lastSpawn2 >= 2789))
			{
				SpawnGBlaster(20, p.x, 0, PI*3/2, pic p_w * 1.6f);
				lastSpawn2 = clock();
			}
		}else if(g_round == 32 || g_round == 33)
		{
			if(clock() - lastSpawn1 >= 1800 * pow(0.96f, g_round - 32))
			{	//orange bones
				SpawnEntityEx(11,0,BLUE_BOTTOM/2,GetEntitySpeed(11)*1.4f,0.0f,BLUE_BOTTOM/2);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 1300 * pow(0.8f, g_round - 32))
			{	//blue bones
				short len = Choice({20,110,450});
				if(g_round == 32)
					SpawnEntityEx(10,0,BLUE_BOTTOM - len/2,GetEntitySpeed(10)*0.9f,0.0f,len/2);
				else
					SpawnEntityEx(10,g_uix,BLUE_BOTTOM - len/2,GetEntitySpeed(10)*0.9f,PI,len/2);
				lastSpawn2 = clock();
			}
		} 
		else if(g_round == 34)
		{
			if(clock() - lastRound < 30 && p.HeartMode() != HEART_RED)
			{
				p.HeartMode() = HEART_RED;
				Sound(SND_DING);
			}
//			const vector<float> _rl {0.5,0.5,0.5,0.6,0.65,0.7,0.75,0.8,0.8,0.77,0.72,0.65,0.63,0.6,0.55,0.5,0.5,0.45,0.4,0.4,0.35,0.32,0.28,0.23,0.2,0.2,0.2};
			if(clock() - lastRound < ROUND_T / 2.0f - 350
			&& clock() - lastSpawn1 >= 185 * pow(0.95f, g_round - 34))
			{
//				BonePair(2,RIGHT,2.5f,_rl[spawnMark]);
				BonePair(2,RIGHT,2.5f,
						0.5 * sin(PI * 3 / 4 + spawnMark/13.0) + 0.5
					);
				spawnMark = (spawnMark + 1) % SHRT_MAX;
				lastSpawn1 = clock();
			}else if(clock() - lastRound >= ROUND_T / 2.0f - 50
			&& clock() - lastRound <= ROUND_T / 2.0f)
			{
				lastSpawn1 = clock() + 750;	//滞后 
				if(p.HeartMode() != HEART_BLUE)
				{
					for(short i = 0; i < entities.size(); ++i)
						if(entities.at(i).GetID() == 3)
							entities.at(i).SetID(11);	//orange
					Sound(SND_DING);
					p.HeartMode() = HEART_BLUE;
				}
			}
			else if(clock() - lastRound >= ROUND_T / 2.0f 
			&& clock() - lastSpawn1 >= 400 * pow(0.95f, g_round - 34))
			{
				BonePair(2,DOWN,0.7f,
						(0.5 * sin(PI / 4 + spawnMark/18.0) + 0.5)
					);
				spawnMark = (spawnMark + 1) % SHRT_MAX;
				lastSpawn1 = clock();
			}
		}else if(g_round == 35)
		{
			if(clock() - lastRound >= 950
			&& clock() - lastRound <= ROUND_T - 1000
			&& clock() - lastSpawn1 >= 400 * pow(0.95f, g_round - 34))
			{
				BonePair(2,DOWN,1.0f,
						(0.1 * sin(spawnMark/3.0) + 0.5),
						(clock() - lastRound <= ROUND_T * 0.5f ? pic p_w * 3 : pic p_w * 2)
					);
				if(clock() - lastRound <= ROUND_T * 0.5f)
						   spawnMark = (spawnMark + 1) % SHRT_MAX;
				lastSpawn1 = clock();
			}
			if(clock() - lastRound >= ROUND_T * 0.5f + 1500
			&& clock() - lastSpawn2 >= 2500)
			{
				short len = Choice({90,100,120,300,500});
				short len2 = Choice({50,60,80});
				SpawnEntityEx(11,0,BLUE_BOTTOM-len/2-100,1.2f,0.0f,len/2);
				SpawnEntityEx(11,g_uix,BLUE_BOTTOM-len/2-100,1.2f,PI,len/2);
				SpawnEntityEx(10,-30,BLUE_BOTTOM-len2/2,1.1f,0.0f,len2/2);
				SpawnEntityEx(10,g_uix+30,BLUE_BOTTOM-len2/2,1.1f,PI,len2/2);
				lastSpawn2 = clock();
			}
		}
		else if(g_round == 36)
		{
			if(clock() - lastRound < 30 && spawnMark != 44)
			{
				SpawnEntityEx(10,g_uix,BLUE_BOTTOM - 400 / 2 + 180, GetEntitySpeed(3) * 3.0f, 0.85 * PI,400);
				SpawnEntityEx(10,0,400 / 2 + 220, GetEntitySpeed(3) * 3.0f, 1.85 * PI ,400);
				spawnMark = 44;
			}
			if(clock() - lastSpawn1 >= 2500)
			{
				BonePair(2,RIGHT,1.2f,0.81f, 100, false);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 3300)
			{
				SpawnGBlaster(Choice({16,18,20}),p.x, 10, PI*3/2,pic p_w * 1.4f);
				lastSpawn2 = clock();
			}
		}else if(g_round == 37 || g_round == 38 || g_round == 39)
		{
			if(clock() - lastSpawn1 >= 4500 * pow(0.77, g_round - 37))
			{
				BonePair(2, Choice({LEFT,RIGHT}), Choice({0.7f,0.9f,1.1f}), 0.78f, Choice({80,92,95,96,99}), false);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 2240)
			{	//orange
				SpawnEntityEx(11,0,BLUE_BOTTOM - 720 / 2, GetEntitySpeed(11) * 2.5f, 0.0f, 360);
				lastSpawn2 = clock();
			}
			if(g_round >= 38 && clock() - lastSpawn3 >= 3000)
			{	//gblaster
				SpawnGBlaster(Choice({16,18,20}),p.x + Randint(-5,5), 10, PI*3/2, pic p_w);
				lastSpawn3 = clock();
			}
		}else if(g_round == 40 || g_round == 41)
		{
			if(clock() - lastSpawn1 >= 3000 * pow(0.77, g_round - 40))
			{
				float vratio = Choice({0.7f,0.9f,1.1f});
				BonePair(2, RIGHT, vratio, 0.78f, Choice({95,105}), false);
				BonePair(2, LEFT, vratio, 0.78f, Choice({95,105}), false);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 2300)
			{	//orange
				short len = Choice({50,90,140,190,288,444});
				SpawnEntityEx(11,g_uix,BLUE_BOTTOM - len / 2, GetEntitySpeed(11) * 2.5f, PI, len/2);
				lastSpawn2 = clock();
			}
		}else if(g_round >= 42 && g_round <= 45)
		{
			if(clock() - lastSpawn1 >= 3800 * pow(0.77, g_round - 42))
			{
				short r = Choice({0,0,1,1,2,2,3,4});
				if(r == 0)
				{
					float vratio = Choice({0.7f,0.9f,1.1f});
					BonePair(2, RIGHT, vratio, 0.78f, Choice({95,104}), false);
					BonePair(2, LEFT, vratio, 0.78f, Choice({96,105}), false);
				}else if(r == 1)
				{	//blue
					float vratio = Choice({0.7f,0.9f,1.1f});
					float gratio = Choice({0.82f,0.5f,0.2f});
					BonePair(10, RIGHT, vratio, gratio, Choice({95,105}), false);
					BonePair(10, LEFT, vratio, gratio, Choice({95,105}), false);
				}else if(r == 2){	//orange
					float vratio = Choice({0.7f,0.9f,1.1f});
					float gratio = Choice({0.84f,0.5f,0.2f});
					BonePair(11, RIGHT, vratio, gratio, Choice({97,100}), false);
					BonePair(11, LEFT, vratio, gratio, Choice({97,103}), false);
				}else if(r == 3)
				{	//blue and orange
					float vratio = Choice({0.7f,0.9f,1.1f});
					float gratio = Choice({0.6f,0.5f,0.2f});
					BonePair(10, RIGHT, vratio, gratio, Choice({95,109}), false);
					BonePair(11, LEFT, vratio, gratio, Choice({94,105}), false);
				}else if(r == 4)
				{	//orange and blue
					float vratio = Choice({0.7f,0.9f,1.1f});
					float gratio = Choice({0.6f,0.5f,0.2f});
					BonePair(11, RIGHT, vratio, gratio, Choice({97,105}), false);
					BonePair(10, LEFT, vratio, gratio, Choice({95,103}), false);
				}
				lastSpawn1 = clock();
			}
		}
		else if(g_round >= 46 && g_round <= 48)
		{	//redheart
			if(clock() - lastRound < 30)
			{
				if(p.HeartMode() != HEART_RED)
				{
					Sound(SND_DING);
					p.HeartMode() = HEART_RED;
				}
			}
			if(clock() - lastSpawn1 >= 1000 * pow(0.992f, g_round - 46))
			{
				BonePair(2,(g_round%2?LEFT:RIGHT),Choice({1.0f,1.1f}),Choice({0.3f,0.4f,0.5f,0.6f,0.7f}),90);
				lastSpawn1 = clock();
			}
			if(g_round > 46 && clock() - lastSpawn2 >= (g_round == 48?spawnMark:2888))
			{	//orange
				short len = Choice({50,90,140,190,288,444});
				SpawnEntityEx(11,0, p.y, GetEntitySpeed(11) * 2.5f, Choice<float>({0.1f, PI*2-0.1f}), len/2);
				lastSpawn2 = clock();
				spawnMark = Choice({2888,2444,1111,3000,888});
			}
			if(g_round > 46 && clock() - lastSpawn3 >= 2900)
			{	//gblaster
				SpawnGBlaster(18,p.x + Choice({-100,100}),0,PI*3/2,pic p_w * 1.2f);
				lastSpawn3 = clock();
			}
		}else if(g_round == 49)
		{	//羊爸招式之一  羊毛两排进行攻击 
			if((clock() - lastRound <= 1000 && clock() - lastSpawn1 > 1000) 
			|| clock() - lastSpawn1 >= ROUND_T / 2.2f)
			{
				if(spawnMark == 0)
				{	//上下两排攻击 
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wools_h",0,NULL);
					spawnMark = 1;
				}else {	//以下是左右方的两排 
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wools_v",0,NULL);
					spawnMark = 0;
				}//end of if spawnMark 
				lastSpawn1 = clock();
			}
		}else if(g_round == 50)
		{
			if((clock() - lastRound <= 1000 && clock() - lastSpawn1 > 1000) 
			|| clock() - lastSpawn1 >= ROUND_T / 5.0f)
			{	//羊妈招式——直角羊毛攻击 
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wools_leftup",0,NULL);
					spawnMark++;
				}else if(spawnMark == 1){	
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wools_leftdown",0,NULL);
					spawnMark++;
				}else if(spawnMark == 2){	
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wools_rightdown",0,NULL);
					spawnMark++;
				}else if(spawnMark == 3){	
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wools_rightup",0,NULL);
					spawnMark=0;
				}//end of if spawnMark 
				lastSpawn1 = clock();
			}
		}else if(g_round == 51)
		{
			if(clock() - lastRound <= 30)
			{
				if(p.HeartMode() != HEART_BLUE)
				{
					Sound(SND_DING);
					p.HeartMode() = HEART_BLUE;
					spawnMark = 0;
					lastSpawn2 = clock();
				}
			 } 
			 if(clock() - lastSpawn1 >= 50 && clock() - lastSpawn2 >= 200)
			 {
			 	SpawnEntityEx(2,(spawnMark==0?g_uix/4:g_uix*3/4),0,GetEntitySpeed(2)*2.5f,PI*3/2,g_uix/4);
			 	lastSpawn1 = clock();
			 }
			 if(clock() - lastSpawn2 >= 1000)
			 {
			 	spawnMark = (spawnMark == 0 ? 1 : 0);
			 	lastSpawn2 = clock();
			 }
			 if(clock() - lastSpawn3 >= 1000)
			 {
			 	SpawnEntityEx(10,(spawnMark==1?0:g_uix),BLUE_BOTTOM-100/2,GetEntitySpeed(10)*4.4f,(spawnMark==1?0.0:PI),50);
			 	lastSpawn3 = clock();
			 }
		}else if(g_round == 52)
		{
			if(clock() - lastSpawn1 >= 330)
			{	//矛穿心 
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)("3spear_center"),0,NULL);
				lastSpawn1 = clock();
			}
		}else if(g_round == 53 || g_round == 54)
		{
			if(clock() - lastSpawn1 >= 300 * pow(0.36f,g_round-53))
			{
				float r=Choice({90,90,90,90,100,120,180});
				float _x=Randint(50,g_uix-50);
				float _y=Randint(50,g_uix-50);
				SpawnEntityEx(9,_x+r,_y,GetEntitySpeed(9),PI,0,0,0,ETA_DELAY_1s,true);
				SpawnEntityEx(9,_x,_y+r,GetEntitySpeed(9),PI*.5f,0,0,0,ETA_DELAY_1s,true);
				SpawnEntityEx(9,_x-r,_y,GetEntitySpeed(9),0.0f,0,0,0,ETA_DELAY_1s,true);
				SpawnEntityEx(9,_x,_y-r,GetEntitySpeed(9),PI*1.5f,0,0,0,ETA_DELAY_1s,true);
				lastSpawn1 = clock();
			}
		}else if(g_round >= 55 && g_round <= 57)
		{	//.,.
			if(clock() - lastRound >= (g_round==55?1000:400)
			&& clock() - lastSpawn1 >= 320*pow(1.08f,g_round-55))
			{
				short r=g_uiy/4+Choice({0,0,0,0,0,0,0,0,10,15,50,100,150,-100,-200,-300});
				SpawnEntityEx(2,0,g_uiy/2+(spawnMark==0?1:-1)*r,GetEntitySpeed(2)*pow(1.1f,g_round-55),0.0,r,0,0);
				spawnMark = Choice({0,1});
				lastSpawn1 = clock();
			}
		}else if(g_round >= 58 && g_round <= 59)
		{	//.,.
			if(clock() - lastRound >= (g_round==58?600:500)
			&& clock() - lastSpawn1 >= 310*pow(1.12f,g_round-55))
			{
				short r=g_uiy/4+Choice({0,0,10,15,50,100,150,250,-5,-100,-200});
				SpawnEntityEx(2,0,g_uiy/2+(spawnMark==0?1:-1)*r,GetEntitySpeed(2)*1.09f*pow(1.1f,g_round-55),0.0,r,0,0);
				if(g_round == 58)
					spawnMark = Choice({0,1});
				else{
					spawnMark = (Choice({0,1})==0?
								Choice({0,1}):(spawnMark==0?1:0));
				}
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 310*pow(1.12f,g_round-55))
			{
				short r=Choice({34,50,60,90,120});
				SpawnEntityEx(2,0,Choice({g_uiy/2+r+110, g_uiy/2-r-110}),GetEntitySpeed(2)*1.27f*pow(1.3f,g_round-55),0.0,r,0,0);
				lastSpawn2 = clock();
			}
		}else if(g_round == 60 || g_round == 61)
		{
			if(clock() - lastRound <= 30)
			{
				lastSpawn1=clock();
				lastSpawn2=clock();
				if(g_round==60)
					spawnMark=0;
			}
			 if(clock() - lastSpawn1 >= 50 && clock() - lastSpawn2 >= 400)
			 {
			 	short r = (g_uix>g_uiy?g_uix/2:g_uiy/2);	//正方形而不是长方形 
			 	if(spawnMark==0)
			 		SpawnEntityEx(2,g_uix/4,g_uiy/2-r,GetEntitySpeed(2)*2.5f,PI*3/2,g_uix/4);
			 	else if(spawnMark==1)
			 		SpawnEntityEx(2,g_uix*3/4,g_uiy/2+r,GetEntitySpeed(2)*2.5f,PI/2,g_uix/4);
			 	else if(spawnMark==2)
			 		SpawnEntityEx(2,g_uix/2-r,g_uiy/4,GetEntitySpeed(2)*2.5f,0,g_uiy/4);
			 	else if(spawnMark==3)
			 		SpawnEntityEx(2,g_uix/2+r,g_uiy*3/4,GetEntitySpeed(2)*2.5f,PI,g_uiy/4);
			 	else if(spawnMark==4)
			 		SpawnEntityEx(2,g_uix/4,g_uiy/2+r,GetEntitySpeed(2)*2.5f,PI/2,g_uix/4);
			 	else if(spawnMark==5)
			 		SpawnEntityEx(2,g_uix*3/4,g_uiy/2-r,GetEntitySpeed(2)*2.5f,PI*3/2,g_uix/4);
			 	else if(spawnMark==6)
			 		SpawnEntityEx(2,g_uix/2+r,g_uiy/4,GetEntitySpeed(2)*2.5f,PI,g_uiy/4);
			 	else if(spawnMark==7)
			 		SpawnEntityEx(2,g_uix/2-r,g_uiy*3/4,GetEntitySpeed(2)*2.5f,0,g_uiy/4);
			 	lastSpawn1 = clock();
			 }
			 if(clock() - lastSpawn2 >= (g_round==60?1500:401))
			 {	//double-L-shaped moving is the key to success
			 	//「」 
			 	spawnMark = (spawnMark + 1) % 8;
			 	lastSpawn2 = clock();
			 }
		}else if(g_round >= 62 && g_round <= 63)
		{
			if(clock() - lastSpawn1 >= 1500 * pow(0.9f,g_round-62))
			{
//				short r1 = Choice({90,120,220,500})/2;
				short r1 = Choice({420,450,475,500,520,550,600,650})/2;
				r1 = Clamp<short>(r1,50,g_uiy*0.88f/2);
				short r2 = (g_uiy - r1*2 + Choice({0,0,0,4,10}))/2; 
				byte r = Choice({0,1,2,3});
				const float _klist[] {0.8f,0.9f,1.0f,1.1f,1.3f};
				byte ri=Choice({0,1,2});
				float k1 = _klist[ri];
				float k2 = _klist[2-ri];
				if(r==0||r==2)
					SpawnEntityEx(2,0,r1,GetEntitySpeed(2)*k1,0.0f,r1);
				else
					SpawnEntityEx(2,g_uix,r1,GetEntitySpeed(2)*k1,PI,r1);
				if(r==0||r==2)
					SpawnEntityEx(2,g_uix,g_uiy-r2,GetEntitySpeed(2)*k2,PI,r2);
				else
					SpawnEntityEx(2,0,g_uiy-r2,GetEntitySpeed(2)*k2,0,r2);
				lastSpawn1 = clock();
			}
			if(g_round != 62 && clock() - lastSpawn2 >= 2500)
			{
				SpawnEntityEx(11,0,g_uiy/2,GetEntitySpeed(11)*2.0f,0.0f,g_uiy/2,0,0);
				lastSpawn2 = clock();
			}
			if(g_round == 63 && clock() - lastRound >= ROUND_T * 0.6f && clock() - lastSpawn3 >= 1000)
			{
				short r = 120;
				SpawnEntityEx(10,0,g_uiy-r,GetEntitySpeed(10)*1.6f,0.0f,r,0,0);
				lastSpawn3 = clock();
			}
		}else if(g_round == 64 || g_round == 65)
		{
			if(clock() - lastRound <= 30)
				spawnMark = 0;
			if(clock() - lastSpawn1 >= 45)
			{
				SpawnEntityEx(9,(g_round==64?0:g_uix),spawnMark * 200 + Randint(10,190),GetEntitySpeed(9)*1.5f,(g_round==64?0.0f:PI),0,0,0,ETA_DELAY_1s,true);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 400)
			{
				spawnMark = Randint(0,g_uiy/200);
				lastSpawn2 = clock();
			}
			if(clock() - lastRound >= ROUND_T/2.0 && clock() - lastSpawn3 >= 3000 * pow(0.8,g_round-64))
			{
				SpawnGBlaster(18,Clamp<float>(p.x+Choice({0.0f,5.0f,-5.0f,15.0f,-15.0f,0.0f,0.0f,0.0f,0.0f,0.0f}),5.0,g_uix-5.0),1,PI*1.5f,50);
				lastSpawn3 = clock();
			}
		}else if(g_round == 66)
		{	//小羊毛一排排攻击 
			const short cx = pic p_w * 2.4f, cy = pic p_h * 2.4f;
			if(clock() - lastRound <= 30)
			{
				spawnMark = spawnMark2 = 0;
//				spawnMark3 = Choice({0,1,2,3});
//				spawnMark4 = Choice({0,1,2,3});
				spawnMark3 = spawnMark4 = 0;
			}
			/*if(clock() - lastRound >= 440 && clock() - lastSpawn1 >= 50)
			{	//加上的话太难了 
				if(spawnMark / (g_uix/4.0) != spawnMark3 && spawnMark/cx%4!=spawnMark3
				)
					SpawnEntityEx(13,spawnMark,g_uiy,GetEntitySpeed(13),PI*.5f,0,0,0,ETA_DELAY_1s);
				lastSpawn1 = clock();
				spawnMark += cx;
				if(spawnMark >= g_uix)
				{
					spawnMark -= g_uix;
					spawnMark3 = (spawnMark3 + 1) % 4;
				}
			}*/
			if(clock() - lastRound >= 440 && clock() - lastSpawn2 >= 50)
			{
				if(spawnMark2 / (g_uiy/4.0) != spawnMark4 && spawnMark2/cy%4!=spawnMark4
				)
					SpawnEntityEx(13,0,spawnMark2,GetEntitySpeed(13),0.0f,0,0,0,ETA_DELAY_1s);
				lastSpawn2 = clock();
				spawnMark2 += cy;
				if(spawnMark2 >= g_uiy)
				{
					spawnMark2 -= g_uiy;
					spawnMark4 = (spawnMark4 + 1) % 4;
				}
			}
		} 
		else if(g_round >= 67 && g_round <= 68)
		{	//羊爸招式之二 
			if(clock() - lastSpawn1 >= SPAWN_WOOL_S_CD * pow(0.90, g_round - 67))
			{	//小羊毛 
				SpawnEntity(13, SPN_V_ATK);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn3 >= 900)
			{
				float r = Choice({150,250,280,390,450,498})/2;
				ClampA<float>(r,10,(g_uix/2-150)/2);
				if(spawnMark==0)
					SpawnEntityEx(2,g_uix-r,g_uiy,GetEntitySpeed(2)*1.5f,PI/2,r);
				else
					SpawnEntityEx(2,r,g_uiy,GetEntitySpeed(2)*1.5f,PI/2,r);
				lastSpawn3 = clock();
			} 
			if(clock() - lastSpawn2 >= SPAWN_WOOL_WAVE_CD)
			{	//羊毛潮水 
				if(spawnMark == 0)
				{	//左半边 
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_left",0,NULL);
					spawnMark = 1;
				}else{	//右半边 
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_right",0,NULL);
					spawnMark = 0;
				}
				lastSpawn2 = clock();
			}
		}else if(g_round >= 69 && g_round <= 70)
		{
			if(clock() - lastSpawn1 >= SPAWN_WOOL_S_CD * pow(0.90f, g_round - 69))
			{	//小羊毛 
				SpawnEntity(13, SPN_V_ATK);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 320 * pow(0.9f, g_round - 69))
			{	//长矛偷袭 
				SpawnEntityEx(1,p.x+Choice({-20,0,0,20}),0,GetEntitySpeed(1)*1.5f,PI*1.5f+Choice({0.0f,0.0f,0.0f,0.1f,-0.1f,0.14f,-0.14f,0.3f,-0.4f}),0,0,0,ETA_ROTATE);
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn4 >= SPAWN_WOOL_WAVE_CD * 1.2f)
			{	//羊毛潮水 
				if(spawnMark == 0)
				{	//左半边 
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_left",0,NULL);
					spawnMark = 1;
				}else{	//右半边 
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_right",0,NULL);
					spawnMark = 0;
				}
				lastSpawn4 = clock();
			}
		}else if(g_round >= 71 && g_round <= 80)
		{
			static const short bscnt = 3;
			static const long _step = 35;
			static const long _w = 75;
			static const long _h = 75;
			if(clock() - lastSpawn1 >= SPAWN_BONE_V_CD * 0.3f)
			{	//blue bones
				for(int i = 0; i < bscnt * (g_round >= 77 ? 0.02f : 1.0f); i++)
				{
					Entity et(4);
					et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.1f);
					et.x = blue_left + Randint(0,_w);
					et.y = g_uiy;
					et.theta = PI / 2;	//up
					et.start = clock();
					entities.push_back(et);
				}
				BlueLeft(_step);
				if(g_temp[0] >= g_uix - 10
				|| g_temp[0] < 0 && g_temp[0] >= -_step)
					g_temp[0] = -g_temp[0];	//dir changing
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= SPAWN_BONE_V_CD * 0.35f)
			{	//orange bones
				for(int i = 0; i < bscnt * (g_round >= 77 ? 0.01f : 1.0f); i++)
				{
					Entity et(5);
					et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.1f);
					et.x = 0;
					et.y = orange_top + Randint(0, _h);
					et.theta = 0.0f;	//right
					et.start = clock();
					entities.push_back(et);
				}
				OrangeTop(_step);
				if(g_temp[1] >= g_uiy - 10
				|| g_temp[1] < 0 && g_temp[1] >= -_step)
					g_temp[1] = -g_temp[1];	//dir changing
				lastSpawn2 = clock();
			}
			if((g_round == 73 || g_round == 80) && clock() - lastSpawn3 >= SPAWN_SPEAR_CD * 0.9f)
			{	//spear
				SpawnEntity(1,SPN_H_ATK,ETA_FADE);
				lastSpawn3 = clock();
			}
			if(g_round >= 74 && g_round <= 76 && clock() - lastSpawn3 >= 1000 * pow(0.83f,g_round-74))
			{	
				SpawnEntityEx(9,0,Choice({10.0f,g_uiy-10.0f,g_uiy*0.2f,g_uiy*0.8f,g_uiy*0.5f}),GetEntitySpeed(9)*0.5f,0.0f,0,0,0,ETA_DELAY_1s,false);
				lastSpawn3 = clock();
			}
			if(g_round >= 77 && g_round <= 78 && clock() - lastSpawn3 >= 800 * pow(0.65f,g_round-77))
			{	
				bool _1=Choice({true,false});
				SpawnEntityEx(9,p.x+Choice({-50,50}),_1?0:g_uiy,GetEntitySpeed(9)*0.8f,_1?PI*1.5f:PI*.5f,0,0,0,ETA_DELAY_3s,false);
				lastSpawn3 = clock();
			}
			if(g_round >= 79 && g_round <= 80 && clock() - lastSpawn3 >= 1200 * pow(0.7f,g_round-79))
			{	
				SpawnEntityEx(12,g_uix,p.y+100,GetEntitySpeed(12)*1.5f,PI,0,0,0,ETA_DELAY_1s);
				SpawnEntityEx(12,g_uix,p.y-100,GetEntitySpeed(12)*1.5f,PI,0,0,0,ETA_DELAY_1s);
				lastSpawn3 = clock();
			}
			if(g_round == 80 && clock() - lastSpawn4 >= 2498)
			{	
				SpawnGBlaster(16,p.x + Choice({100.0f,50.0f,0.0f,-50.0f,-100.0f}),g_uiy,PI*.5f,98);
				lastSpawn4 = clock();
			}
		}else if(g_round >= 81 && g_round <= 82)
		{
			if(clock() - lastSpawn1 >= 1500 * pow(0.7f, g_round - 81))
			{	//N矛穿心 
				short cnt = Choice({3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,6,6,7});
				string tmp = ToString(cnt)+"spear_center";
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(tmp.c_str()),0,NULL);
				lastSpawn1 = clock();
			}
		}else if(g_round >= 83 && g_round <= 84)
		{
			if(clock() - lastRound <= 15 && clock() - lastSpawn1 > 16
			|| clock() - lastSpawn1 >= ROUND_T / 3.0)
			{	//羊毛潮水 
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_left",0,NULL);
					spawnMark = 1;
				}else{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_right",0,NULL);
					spawnMark = 0;
				}
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 2555 * pow(0.9f,g_round-83))
			{	//N矛穿心 
				short cnt = Choice({3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,6,6,6,6,7});
				string tmp = ToString(cnt)+"spear_center";
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(tmp.c_str()),0,NULL);
				lastSpawn2 = clock();
			}
		}else if(g_round >= 85 && g_round <= 88)
		{
			if(clock() - lastRound <= 15 && clock() - lastSpawn1 > 16
			|| clock() - lastSpawn1 >= ROUND_T / 16 * pow(0.83f, g_round - 85))
			{	//85 两个1/4区域的羊毛潮水，横纵都有 
				//86-88 随机区域快速羊毛潮水，横纵都有 
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_h_1" : "wool_wave_h_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 1){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_h_3" : "wool_wave_h_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 2){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_h_2" : "wool_wave_h_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 3){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_h_4" : "wool_wave_h_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 4){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_v_2" : "wool_wave_v_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 5){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_v_4" : "wool_wave_v_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 6){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_v_1" : "wool_wave_v_random"),0,NULL);
					(g_round == 85 ? ++spawnMark : (spawnMark = Randint(0,7)));
				}else if(spawnMark == 7){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(g_round == 85 ? "wool_wave_v_3" : "wool_wave_v_random"),0,NULL);
					(g_round == 85 ? spawnMark=0 : (spawnMark = Randint(0,7)));
				}
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 > SPAWN_BONE_V_CD * 2.4f * pow(0.85f, g_round - 85))
			{	//orange bones
				SpawnEntity(5, (spawnMark <= 3 ? SPN_V_ATK : SPN_H_ATK));
				lastSpawn2 = clock();
			}
			if(g_round >= 87 && clock() - lastSpawn3 > SPAWN_SPEAR_CD * 2.0f * pow(0.65f,g_round - 87))
			{	//spear
				SpawnEntity(1, (spawnMark <= 3 ? SPN_V_ATK : SPN_H_ATK));
				lastSpawn3 = clock();
			}
		}
		else if(g_round >= 89 && g_round <= 90)
		{
			static const short bscnt = 3;
			static const short _h = 75;
			static const long _step = 45;
			if(clock() - lastSpawn1 >= 1999 * pow(0.5f, g_round - 89))
			{	//N矛穿心 
				short cnt = Choice({4,4,4,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,8,8});
				string tmp = ToString(cnt)+"spear_center";
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(tmp.c_str()),0,NULL);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 3500 * pow(0.75f, g_round - 89))
			{	//羊毛潮水 
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_v_random",0,NULL);
					spawnMark = 1; 
				}else{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_h_random",0,NULL);
					spawnMark = 0; 
				}
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn2 >= SPAWN_BONE_V_CD * 0.35f)
			{	//orange bones
				for(int i = 0; i < bscnt * 0.1f; i++)
				{
					Entity et(5);
					et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.4f);
					et.x = g_uix;
					et.y = orange_top + Randint(0, _h);
					et.theta = - PI;	//right
					et.start = clock();
					entities.push_back(et);
				}
				OrangeTop(_step);
				if(g_temp[1] >= g_uiy - 10
				|| g_temp[1] < 0 && g_temp[1] >= -_step)
					g_temp[1] = -g_temp[1];	//dir changing
				lastSpawn2 = clock();
			}
		}
		else if(g_round >= 91 && g_round <= 93)
		{
			if(clock() - lastSpawn1 >= 2555 * pow(0.85f, g_round - 91))
			{	//spear rows
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_v_row",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 1){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 2){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_v_row2",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 3){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row2",0,NULL);
					spawnMark = 0;
				}
				lastSpawn1 = clock(); 
			}
		}else if(g_round >= 94 && g_round <= 95)
		{
			if(clock() - lastSpawn1 >= 3567 * pow(0.81f, g_round - 94))
			{	//spear rows && wool waves
				short _area = Randint(1,4);
				string _tmp = "wool_wave_h_"+ToString(_area);
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row",0,NULL);
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(_tmp.c_str()),0,NULL);
					spawnMark ++;
				}/*else if(spawnMark == 1){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row",0,NULL);
					spawnMark ++;
				}*/else if(spawnMark == 1){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row2",0,NULL);
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(_tmp.c_str()),0,NULL);
					spawnMark = 0;
				}
				lastSpawn1 = clock(); 
			}
		}else if(g_round >= 96 && g_round <= 97) 
		{
			if(clock() - lastSpawn1 >= 2555 * pow(0.85f, g_round - 96))
			{	//spear rows with accele 
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_v_row_acc",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 1){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row_acc",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 2){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_v_row2_acc",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 3){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row2_acc",0,NULL);
					spawnMark = 0;
				}
				lastSpawn1 = clock(); 
			}
		}else if(g_round == 98)
		{
			if(clock() - lastSpawn1 >= 2222)
			{	//spear rows whose speeds vary
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row_vary",0,NULL);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 3500)
			{
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_h_random",0,NULL);
				lastSpawn2 = clock();
			}
		}else if(g_round == 99)
		{
			static const short bscnt = 3;
			static const short _h = 75;
			static const long _step = 45;
			if(clock() - lastSpawn1 >= 2222)
			{	//spear rows whose speeds vary
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row_vary",0,NULL);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= SPAWN_BONE_V_CD * 0.35f)
			{	//orange bones
				Entity et(5);
				for(int i = 0; i < bscnt * 0.1f; i++)
				{
					et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.4f);
					et.x = g_uix;
					et.y = orange_top + Randint(0, _h);
					et.theta = - PI;	//right
					et.start = clock();
					entities.push_back(et);
				}
				OrangeTop(_step);
				if(g_temp[1] >= g_uiy - 10
				|| g_temp[1] < 0 && g_temp[1] >= -_step)
					g_temp[1] = -g_temp[1];	//dir changing
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn3 >= 1000)
			{
				SpawnEntity(12, SPN_V_ATK);
				lastSpawn3 = clock();
			}
		}else if(g_round == 100)
		{
			if(clock() - lastSpawn1 >= 2555 * pow(0.85f, g_round - 96))
			{	//spear rows with accele 
				if(spawnMark == 0)
				{
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_v_row_acc",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 1){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row_acc",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 2){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_v_row2_acc",0,NULL);
					spawnMark ++;
				}else if(spawnMark == 3){
					CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"spear_h_row2_acc",0,NULL);
					spawnMark = 0;
				}
				lastSpawn1 = clock(); 
			}
			if(clock() - lastSpawn2 >= 4444)
			{
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_h_four",0,NULL);
				lastSpawn2 = clock();
			}
		}else if(g_round == 101)
		{
			static const long _step = 45;
			static const float _diff = pic p_w;
			if(clock() - lastRound < 5)
			{
				for(short j=0;j<entities.size();++j)
					entities.at(j).v *= 3.0f;
			} 
			if(clock() - lastSpawn1 >= SPAWN_BONE_V_CD)
			{	//蓝骨头夹击 让你不敢动 
				SpawnEntityEx(4,0,orange_top,GetEntitySpeed(4),atan2(-p.y+orange_top - _diff, p.x - _diff));
				SpawnEntityEx(4,0,orange_top-_diff,GetEntitySpeed(4),atan2(-p.y+orange_top + _diff, p.x + _diff));
				OrangeTop(_step);
				if(g_temp[1] >= g_uiy - _diff
				|| g_temp[1] < 1 && g_temp[1] >= -_step)
					g_temp[1] = -g_temp[1];	//dir changing
				lastSpawn1 = clock();
			 }
			 if(clock() - lastSpawn2 >= SPAWN_BONE_V_CD)
			 {
			 	SpawnEntityEx(5,p.x - _diff, g_uiy,GetEntitySpeed(5),PI/2);
			 	SpawnEntityEx(5,p.x + _diff, g_uiy,GetEntitySpeed(5),PI/2);
			 	lastSpawn2 = clock();
			 } 
			 if(clock() - lastSpawn3 >= 3000)
			 {
			 	if(spawnMark == 0)
			 		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)Choice({"wool_wave_h_2","wool_wave_v_1"}),0,NULL);
			 	else
			 		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)Choice({"wool_wave_h_4","wool_wave_v_3"}),0,NULL);
			 	spawnMark = (spawnMark + 1) % 2;
			 	lastSpawn3 = clock();
			 }
		}else if(g_round == 102)
		{
			if(clock() - lastRound <= 10)
			{	//吓你一下 
				SpawnEntityEx(4,g_uix,p.y,GetEntitySpeed(4)*2, PI);
				spawnMark = 15;
			}
			if(clock() - lastRound > 1000 && clock() - lastSpawn1 > 50)
			{
				float _x = spawnMark * 25.0f;
				SpawnEntityEx(9, _x, g_uiy, GetEntitySpeed(9),
						Towards(_x , g_uiy, p.x + Choice({1.5f,-1.5f}) * Clamp(abs(_x - g_uix/2) * 2.0f, 60.0f, 250.0f), p.y));
//						Towards(_x , g_uiy, p.x + 0, p.y+0));
				spawnMark += 1;
				if(spawnMark > 60)
					spawnMark = 0;
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= SPAWN_BONE_V_CD * 0.6f)
			{	//blue quick bones (v dir)
				static const short bscnt = 3;
				Entity et(4);
				for(int i = 0; i < bscnt; i++)
				{
					et.v = Varience2(GetEntitySpeed(et.GetID()), GetEntitySpeed(et.GetID()) * 0.95f);
					et.x = g_uix;
					et.y = orange_top + Randint(0, 245.0f);
					et.theta = - PI;	//right
					et.start = clock();
					entities.push_back(et);
				}
				OrangeTop(100);
				if(g_temp[1] >= g_uiy - 50
				|| g_temp[1] < 0 && g_temp[1] >= -100)
					g_temp[1] = -g_temp[1];	//dir changing
				lastSpawn2 = clock();
			}
		}else if(g_round == 103)
		{
			if(clock() - lastRound <= 10)
			{	//orange bones 吓你一下 
				SpawnEntityEx(5, 0, g_uiy * 0.3f, GetEntitySpeed(5),Towards(0,g_uiy * 0.3f, p.x + Randint(-45,45), p.y + Randint(-68,68)));
			}
			if(clock() - lastSpawn1 >= SPAWN_SPEAR_CD * 0.25f)
			{	//V字形长矛移动向上攻击 
				static float _x = 20;
				SpawnEntityEx(1, _x, g_uiy, GetEntitySpeed(1) * Clamp(abs(_x - g_uix/2)/g_uix*10.0f, 0.05f,5.0f), ToRadian(60));
				SpawnEntityEx(1, _x, g_uiy, GetEntitySpeed(1) * Clamp(abs(_x - g_uix/2)/g_uix*10.0f, 0.05f,5.0f), ToRadian(120));
				_x += 25;
				//比较难躲 
				if(_x >= g_uix - 20)
					  _x = 20;
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= SPAWN_BONE_V_CD * 2.0f)
			{
				SpawnEntityEx(4,0,g_uiy - 60, GetEntitySpeed(4)* 0.7f, 0.0f);
				SpawnEntityEx(5,g_uix,60, GetEntitySpeed(5)* 0.7f, PI);
				lastSpawn2 = clock();
			}
		}else if(g_round == 104)
		{	//绿心 
			if(clock() - lastRound < 50)
			{
				Sound(SND_DING);
				p.HeartMode() = HEART_GREEN;
				p.dir = UP;
			}
			if(clock() - lastSpawn1 >= 800)
			{
				SpawnSA(14, Choice({UP,DOWN}), 1.0f);
				lastSpawn1 = clock();
			}
		}else if(g_round == 105)
		{ 
			if(clock() - lastSpawn1 >= 700)
			{
				SpawnSA(14, Choice({UP,LEFT,RIGHT,DOWN}), 1.0f);
				lastSpawn1 = clock();
			}
		}else if(g_round == 106 || g_round == 107)
		{ 
			if(clock() - lastSpawn1 >= 660 * pow(0.8f,g_round-106))
			{
				SpawnSA(14, Choice({UP,LEFT,RIGHT,DOWN}), Randint(5,18)/10.0f);
				lastSpawn1 = clock();
			}
		}else if(g_round == 108 || g_round == 109)
		{ 
			if(clock() - lastSpawn1 >= 620 * pow(0.8f,g_round-108))
			{
				SpawnSA(14, Choice({UP,LEFT,RIGHT,DOWN}), Choice({0.5f,0.9f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.6f}));
				lastSpawn1 = clock();
			}
		}else if(g_round == 110 || g_round == 111)
		{ 
			if(clock() - lastSpawn1 >= 580 * pow(0.8f,g_round-110))
			{
				SpawnSA(14, Choice({LEFT,RIGHT}), Choice({0.1f,0.98f,1.0f,2.0f,2.0f,2.0f}));
				lastSpawn1 = clock();
			}
		}else if(g_round == 112 || g_round == 113)
		{ 
			if(clock() - lastSpawn1 >= 700 * pow(0.9f,g_round-112))
			{
				SpawnSA(14, Choice({LEFTUP,RIGHTDOWN,UP,DOWN,RIGHTUP,LEFTDOWN,RIGHT,DOWN}), Choice({1.0f}));
				lastSpawn1 = clock();
			}
		}else if(g_round == 114 || g_round == 115)
		{ 
			if(clock() - lastSpawn1 >= 660 * pow(0.9f,g_round-114))
			{
				SpawnSA(14, NthDir4(spawnMark), 1.3f);
				lastSpawn1 = clock();
				spawnMark  = (spawnMark + 1) % 4;
			}
		}else if(g_round == 116)
		{ 
			if(clock() - lastSpawn1 >= 620)
			{
				SpawnSA(14, NthDir4_2(spawnMark), 1.4f);
				lastSpawn1 = clock();
				spawnMark  = (spawnMark + 1) % 4;
			}
		}else if(g_round == 117)
		{ 
			if(clock() - lastSpawn1 >= 618)
			{
				SpawnSA(14, NthDir8_Sery(spawnMark), 1.5f);
				lastSpawn1 = clock();
				spawnMark  = (spawnMark + 1) % 8;
			}
		}else if(g_round == 118)
		{
			if(clock() - lastSpawn1 >= 610 * pow(0.7f, g_round - 118))
			{
				SpawnSA(14, NthDir8(spawnMark), 1.8f);
				lastSpawn1 = clock();
				spawnMark  = (spawnMark + 1) % 8;
			}
		}else if(g_round == 119)
		{
			if(clock() - lastSpawn1 >= 120)
			{
				/*initializer_list<DIR> seq = {UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,UP,RIGHT};
				SpawnSA(14, Nth(seq, spawnMark), 5.0f);
				lastSpawn1 = clock();
				spawnMark = (spawnMark + 1) % vector<DIR>(seq).size();*/
				if(Randint(0,10)>8)
				{
					lastSpawn1 += Randint(40,100);
					return;
				}
				float _v=5.0f;
				DIR _dir=UP;
				if(spawnMark == 19)
				{	//出其不意 
					_dir = Choice({LEFT,RIGHT});
					_v = 4.0f;
					lastSpawn1 = clock() + 500;
				}else{
					lastSpawn1 = clock();
				}
				SpawnSA(14,_dir,_v);
				spawnMark = (spawnMark + 1) % 20;
				spawnMark2 = 0;
			}
		}else if(g_round == 120)
		{
			if(clock() - lastRound < 10 && spawnMark2 == 0)
			{
				spawnMark2 = short(Choice({LEFT,RIGHT}));
			}
			if(clock() - lastSpawn1 >= 120)
			{
				float _v=5.0f;
				DIR _dir=(DIR)spawnMark2;
				if(spawnMark != 4)
				{
					if(spawnMark == 14)
					{	//出其不意 
						_dir = Choice({UP,DOWN});
						_v = 4.0f;
						lastSpawn1 = clock() + 440;
					}else{
						lastSpawn1 = clock();
					}
					SpawnSA(14,_dir,_v);
				}else{
					_dir = Choice({UP,DOWN});
					_v = Choice({2.0f, 3.0f, 4.0f, 4.0f, 4.0f, 6.0f, 20.0f});
					SpawnSAEx(14,
						g_uix/2 + pic p_w * 1.95f * Choice({-1,1}),0,
							_dir,_v);
					lastSpawn1 = clock() + Randint(50,400);
				}
				spawnMark = (spawnMark + 1) % 15;
			}
			if(clock() - lastSpawn2 >= SPAWN_SPEAR_CD * 2.0f)
			{
				if(spawnMark < 3)
				{
					SpawnEntityEx(1,0,g_uiy/2-100,GetEntitySpeed(1)*1.5f,0.0f);
					SpawnEntityEx(1,0,g_uiy/2+100,GetEntitySpeed(1)*1.5f,0.0f);
				}else if(spawnMark < 6)
				{
					SpawnEntityEx(1,g_uix/2 - 100,0,GetEntitySpeed(1)*1.5f,PI * 3 / 2);
					SpawnEntityEx(1,g_uix/2 + 100,0,GetEntitySpeed(1)*1.5f,PI * 3 / 2);
				}else if(spawnMark < 9)
				{
					SpawnEntityEx(1,g_uix,g_uiy/2-100,GetEntitySpeed(1)*1.5f,PI);
					SpawnEntityEx(1,g_uix,g_uiy/2+100,GetEntitySpeed(1)*1.5f,PI);
				}else if(spawnMark < 12)
				{
					SpawnEntityEx(1,g_uix/2 - 100,g_uiy,GetEntitySpeed(1)*1.5f,PI / 2);
					SpawnEntityEx(1,g_uix/2 + 100,g_uiy,GetEntitySpeed(1)*1.5f,PI / 2);
				}else{
					short r = Randint(0,3);
					if(r == 0)
						CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_h_1",0,NULL);
					else if(r == 1)
						CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_v_1",0,NULL);
					else if(r == 2)
						CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_h_4",0,NULL);
					else if(r == 3)
						CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)"wool_wave_v_4",0,NULL);
				}
				lastSpawn2 = clock();
			}
		}else if(g_round == 121 || g_round == 122)
		{
			static const clock_t _cd = 400;
			static const clock_t _cd_cd = 5000;
			static const initializer_list<float> _vlist =  {1.0f};
			if(g_round == 121 && clock() - lastRound < 3)
			{	//initialize
				spawnMark = 0;
				lastSpawn1 = clock() + _cd;
				lastSpawn2 = clock() + _cd_cd;
			}
			static const array<initializer_list<DIR>,4> _dirs={initializer_list<DIR>({LEFT,RIGHT}),initializer_list<DIR>({LEFTUP,RIGHTDOWN}),initializer_list<DIR>({UP,DOWN}),initializer_list<DIR>({RIGHTUP,LEFTDOWN})};
			if(clock() - lastSpawn1 >= Varience2(_cd * pow(0.95f, g_round - 121), 70.0f))
			{	//spawn spear arrows
				SpawnSA(14, Choice(_dirs[spawnMark]), 
					Choice(_vlist));
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= Varience2(_cd_cd * pow(0.6f, g_round - 121), 800.0f))
			{	//change direction
				spawnMark = (spawnMark + 1) % 4;
				lastSpawn2 = clock();
			}
		}else if(g_round == 123)
		{
			if(clock() - lastRound < 3)
				spawnMark = spawnMark2 = 0;
			if(clock() - lastSpawn1 >= (spawnMark?110:1000))
			{
				SpawnSA((spawnMark?15:14),NthDir4(spawnMark2), 1.5f);
				spawnMark = (spawnMark + 1) % 2; 
				if(!spawnMark)
					spawnMark2 = (spawnMark2 + 1) % 4;
				lastSpawn1 = clock();
			}
		}else if(g_round == 124)
		{
			if(clock() - lastRound < 3)
				spawnMark = spawnMark2 = 0;
			if(clock() - lastSpawn1 >= (spawnMark?120:900))
			{	//oblique!
				SpawnSA((spawnMark?14:15),NthDir4_2(spawnMark2), 1.8f);
				spawnMark = (spawnMark + 1) % 2; 
				if(!spawnMark)
					spawnMark2 = (spawnMark2 + 1) % 4;
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 1500)
			{	//pose a threat
				string _name;
				if(spawnMark2 == 0)
					_name = "wool_wave_h_1";
				else if(spawnMark2 == 1)
					_name = "wool_wave_h_4";
				else if(spawnMark2 == 2)
					_name = "wool_wave_v_1";
				else
					_name = "wool_wave_v_4";
				CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSpawnChain,(LPVOID)(_name.c_str()),0,NULL);
				lastSpawn2 = clock();
			}
		}else if(g_round == 125 || g_round == 126) 
		{
			if(clock() - lastRound < 3)
				spawnMark2 = 0;
			if(clock() - lastRound > 1000 
			&& clock() - lastSpawn2 >= 600
			&& clock() - lastSpawn1 >= 90)
			{
				DIR _dir = NthDir8_Sery(spawnMark);
				SpawnSA((spawnMark%2?14:15), _dir,2.5f);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 2000)
			{	//dir changing
				spawnMark = (spawnMark + 1) % 8;
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn3 > 150)
			{	//顶/底部长矛朝着你旁边攻击
				float _x = spawnMark2 * 25.0f;
				float _y = Choice({0.0f,(float)g_uiy});
				SpawnEntityEx(9, _x, _y, GetEntitySpeed(9) * 2.0f,
						Towards(_x , _y, p.x + Choice({1.5f,-1.5f}) * Clamp(abs(_x - g_uix/2) * 2.0f, 60.0f, 250.0f), p.y));
				spawnMark2 += 1;
				if(spawnMark2 > 60)
					spawnMark2 = 0;
				lastSpawn3 = clock();
			}
		}else if(g_round == 127 || g_round == 128)
		{
			if(clock() - lastRound < 3)
				spawnMark2 = 0;
			if(clock() - lastRound > 500
			&& clock() - lastSpawn2 >= 550	//否则一定会受伤 
			&& clock() - lastSpawn1 >= 150)
			{
				DIR _dir = NthDir8_Sery(spawnMark);
				SpawnSA((spawnMark%2?14:15), _dir,2.5f);
				SpawnSA((spawnMark%2?15:14),OppoDir(_dir),3.0f);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 1500)
			{	//dir changing
				spawnMark = (spawnMark + 1) % 8;
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn3 >= 200)
			{	//横向加速长矛干扰 
				SpawnEntityEx(9,(spawnMark % 2 ? 0 : g_uix),g_uiy/2 + Choice({-1,1}) * Randint(pic p_h * 1.5f, g_uiy/2-50),GetEntitySpeed(9) * 0.3f, (spawnMark % 2 ? 0.0f:PI));
				lastSpawn3 = clock();
			}
		}else if(g_round == 129)
		{	//,,,
			if(clock() - lastRound < 3)
				spawnMark = 1, spawnMark2 = 0;
			if(clock() - lastRound >= 1000
			&& clock() - lastSpawn2 >= 500
			&& clock() - lastSpawn1 >= (spawnMark%2?600:350))
			{
				DIR _dir = NthDir4(spawnMark2);
				SpawnSA((spawnMark%2?14:15),_dir,2.4f);
				spawnMark = (spawnMark + 1) % 2; 
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 1000
			&& clock() - lastRound >= 2000)
			{	//dir changing
				spawnMark2 = (spawnMark2 + 1) % 4;
				lastSpawn2 = clock();
			}
		}else if(g_round == 130)
		{
			if(clock() - lastRound > 1000 
			&& clock() - lastSpawn1 >= 100
			&& clock() - lastRound < 5000)
			{
				SpawnSA(14,UP,4.0f);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn1 >= 150
			&& clock() - lastRound > 5800)
			{
				SpawnSA(15,DOWN,4.0f);
				lastSpawn1 = clock();
			}
		}else if(g_round == 131 || g_round == 132)
		{
			if(clock() - lastRound <= 50 && p.HeartMode() != HEART_RED)
			{
				Sound(SND_DING); 
				p.HeartMode() = HEART_RED;
			}
			if(clock() - lastSpawn1 >= 1200 * pow(0.75f,g_round-131))
			{
				SPAWNCHAIN("6spear_center");
				lastSpawn1 = clock();
			}
		}else if(g_round == 133)
		{
			if(clock() - lastRound <= ROUND_T/2
			&& clock() - lastSpawn1 >= 450)
			{
				SPAWNCHAIN("5spear_center");
				lastSpawn1 = clock();
			}else if(clock() - lastRound >= ROUND_T/2
			&& clock() - lastSpawn1 >= 350)
			{
				SPAWNCHAIN("3spear_center");
				lastSpawn1 = clock();
			}
		}else if(g_round == 134)
		{
			if(clock() - lastSpawn1 >= 1400)
			{
				SPAWNCHAIN("9spear_center");
				lastSpawn1 = clock();
			}
		}else if(g_round == 135)
		{
			if(clock() - lastSpawn1 >= 150)
			{
				SPAWNCHAIN("2spear_center");
				lastSpawn1 = clock();
			}
		}else if(g_round == 136)
		{
			if(clock() - lastSpawn1 >= 50)
			{
				SPAWNCHAIN("1spear_center");
				lastSpawn1 = clock();
			}
		}else if(g_round == 137)
		{
			if(clock() - lastSpawn1 >= 500)
			{
				SpawnEntityEx(1,0,p.y,GetEntitySpeed(1)*2.5f,0.0f,0,0,0,ETA_DELAY_2s);
				SpawnEntityEx(1,g_uix,p.y,GetEntitySpeed(1)*2.5f,PI,0,0,0,ETA_DELAY_2s);
				SpawnEntityEx(1,p.x,0,GetEntitySpeed(1)*2.5f,PI*1.5f,0,0,0,ETA_DELAY_2s);
				SpawnEntityEx(1,p.x,g_uiy,GetEntitySpeed(1)*2.5f,PI*0.5f,0,0,0,ETA_DELAY_2s);
				lastSpawn1 = clock();
			}
		}else if(g_round == 138)
		{
			if(clock() - lastRound <= 40 && spawnMark != 56)
			{
				SpawnEntityEx(1,0,g_uiy * 0.2f,GetEntitySpeed(1)*2.5f,0.0f,0,0,0,ETA_DELAY_2s);
				SpawnEntityEx(1,0,g_uiy * 0.4f,GetEntitySpeed(1)*2.4f,0.0f,0,0,0,ETA_DELAY_1s);
				SpawnEntityEx(1,0,g_uiy * 0.5f,GetEntitySpeed(1)*3.7f,0.0f,0,0,0,ETA_DELAY_2s);
				SpawnEntityEx(1,0,g_uiy * 0.6f,GetEntitySpeed(1)*2.4f,0.0f,0,0,0,ETA_DELAY_1s);
				SpawnEntityEx(1,0,g_uiy * 0.8f,GetEntitySpeed(1)*2.5f,0.0f,0,0,0,ETA_DELAY_2s);
				SpawnEntityEx(9,0,p.y,GetEntitySpeed(9)*1.5f,0.0f,0,0,0,ETA_DELAY_3s);
				spawnMark = 56;
				CreateIndicator(g_uix/2-60,100,g_uix-250,g_uiy-100,500);
			}
			if(clock() - lastRound >= 740 && spawnMark == 56)
			{
				SpawnEntityEx(9,g_uix/2-40,0,GetEntitySpeed(9)*4.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2,0,GetEntitySpeed(9)*3.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+40,0,GetEntitySpeed(9)*4.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+80,0,GetEntitySpeed(9)*3.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+140,0,GetEntitySpeed(9)*1.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+170,0,GetEntitySpeed(9)*4.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+220,0,GetEntitySpeed(9)*3.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+320,0,GetEntitySpeed(9)*2.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix/2+420,0,GetEntitySpeed(9)*3.0f,PI*1.5f);
				SpawnEntityEx(9,g_uix-40,0,GetEntitySpeed(9)*4.0f,PI*1.5f);
				spawnMark = 57;
			}
			if(clock() - lastRound >= 1200
			&& clock() - lastSpawn1 >= 300)
			{
				SpawnEntityEx(9,p.x+Choice({-6,6,-2,2,0,0,0}),0,GetEntitySpeed(9)*0.8f,PI*1.5f);
				lastSpawn1 = clock();
			}
			if(clock() - lastRound >= 4200
			&& clock() - lastSpawn2 >= 400)
			{
				SpawnEntityEx(9,g_uix,p.y+Choice({-120,120}),GetEntitySpeed(9)*1.3f,PI);
				lastSpawn2 = clock();
			}
		}else if(g_round == 139)
		{
			if(clock() - lastSpawn1 >= 650)
			{ 
				SpawnEntityEx(Choice({2,10,11}),0,g_uiy/4,PL_QUICK_SPEED,0,g_uiy/4-2);
				SpawnEntityEx(Choice({2,10,11}),g_uix,g_uiy*3/4,PL_QUICK_SPEED,PI,g_uiy/4-2);
				lastSpawn1 = clock();
			}
		}else if(g_round == 140)
		{
			if(clock() - lastRound < 40 && p.HeartMode() != HEART_BLUE)
			{
				Sound(SND_DING);
				p.HeartMode() = HEART_BLUE;
			}
			if(clock() - lastRound > 600 && clock() - lastSpawn1 >= 1000)
			{
				BonePair(2,RIGHT,1.0f,0.89f,65,false);
				BonePair(2,LEFT,1.0f,0.89f,65,false);
				lastSpawn1=clock();
			}
		}else if(g_round == 141)
		{
			if(clock() - lastRound <= 50 && p.HeartMode() != HEART_RED)
			{
				p.HeartMode() = HEART_RED;
			} 
			if(clock() - lastSpawn1 >= 900)
			{
				SPAWNCHAIN("7spear_roundin");
				lastSpawn1 = clock();
			}
		}else if(g_round == 142)
		{
			if(clock() - lastRound <= 25 && spawnMark != 32)
			{
				SpawnEntityEx(7,-10.0,g_uiy-70,0,0,0,0,0,ETA_FADE,true);
				SpawnEntityEx(7,g_uix+10.0,70,0,PI,0,0,0,ETA_FADE,true);
				spawnMark = 32;	//summon the lying dog
				spawnMark2 = 100;
				OperateEntities(7, OPER_SET_AI, AI_LINE);
			}
			if(clock() - lastSpawn1 >= spawnMark2)
			{
				PerformEntitiesAttack();
				lastSpawn1 = clock();
				spawnMark2 = (spawnMark2 == 100 ? 450 : 100);
			}
			if(clock() - lastRound >= 5000)
			{
				if(clock() - lastSpawn1 >= 190)
				{
					SpawnEntityEx(1,g_uix,p.y+Choice({0.0,20.0,-20.0}),GetEntitySpeed(1)*Choice({1.0,0.6,1.4,1.8,0.3}),PI,0,0,0,ETA_DELAY_1s);
					lastSpawn1 = clock();
				}
			}
		}else if(g_round >= 143 && g_round <= 144)
		{
			if(clock() - lastSpawn1 >= 55 * pow(0.88f,g_round-143))
			{
				SpawnEntityEx(22,Randint(0,g_uix),0,GetEntitySpeed(22),PI*1.5f+Choice({0.1f,0.05f,-0.1f,-0.05f}),0,0,0,ETA_DELAY_1s);
				lastSpawn1 = clock();
			}
			if(clock() - lastRound <= 20 && g_round == 144 && spawnMark != 3000)
				spawnMark = 3000;
			if(g_round == 144 && clock() - lastSpawn2 >= spawnMark)
			{
				short r=Choice({80,120,135,150,175,200,250,275,350});
				SpawnEntityEx(Choice({10,11}),0,g_uiy-r,GetEntitySpeed(10)*1.2f,0.0f,r,0,0,ETA_DELAY_1s);
				lastSpawn2 = clock();
				spawnMark = Choice({50,300,900,1200,1400,1750});
			}
		}else if(g_round == 145)
		{
			if(clock() - lastRound <= 20 && spawnMark != 44)
			{	//summon a lying dog
				SpawnEntityEx(7,45,g_uiy*0.9f,GetEntitySpeed(7)*0.5f,PI*.5f,0,0,0,ETA_DELAY_2s);
				OperateEntities(7, OPER_SET_AI, AI_LINE);
				spawnMark = 44;
			}
			if(clock() - lastSpawn2 >= spawnMark2)
			{
				PerformEntitiesAttack();
				lastSpawn2 = clock();
				spawnMark2 = (spawnMark2 == 120 ? 420 : 120);
			}
			if(clock() - lastSpawn1 >= 250)
			{
				SpawnEntityEx(22,g_uix,Randint(0,g_uiy),GetEntitySpeed(22)*0.35f,PI+Choice({-0.24f,0.1f,0.0f,0.0f,0.0f,0.0f,-0.1f,0.24f}),0,0,0,ETA_DELAY_1s);
				lastSpawn1 = clock();
			}
			if(clock() - lastRound >= ROUND_T * 0.9f && spawnMark3 != 33)
			{
				CreateIndicator(g_uix*0.3f,80,g_uix*0.45f,g_uiy-80,200);
				spawnMark3 = 33;
			}
			if(clock() - lastRound >= ROUND_T * 0.9f + 220 && spawnMark4 != 34)
			{
				SpawnEntityEx(1,g_uix*0.305f,0,GetEntitySpeed(1)*15.0f,PI*1.5f);
				SpawnEntityEx(1,g_uix*0.325f,g_uiy,GetEntitySpeed(1)*15.0f,PI*.5f);
				SpawnEntityEx(1,g_uix*0.35f,0,GetEntitySpeed(1)*15.0f,PI*1.5f);
				SpawnEntityEx(1,g_uix*0.365f,g_uiy,GetEntitySpeed(1)*15.0f,PI*.5f);
				SpawnEntityEx(1,g_uix*0.38f,0,GetEntitySpeed(1)*15.0f,PI*1.5f);
				SpawnEntityEx(1,g_uix*0.395f,g_uiy,GetEntitySpeed(1)*15.0f,PI*.5f);
				SpawnEntityEx(1,g_uix*0.41f,0,GetEntitySpeed(1)*15.0f,PI*1.5f);
				SpawnEntityEx(1,g_uix*0.426f,g_uiy,GetEntitySpeed(1)*15.0f,PI*.5f);
				SpawnEntityEx(1,g_uix*0.445f,0,GetEntitySpeed(1)*15.0f,PI*1.5f);
				spawnMark4 = 34;
			}
		}else if(g_round == 146)
		{
			if(clock() - lastRound <= 20 && spawnMark != 38)
			{
				SPAWNCHAIN("spear_h_row_vary");
				spawnMark = 38;
			}
			if(clock() - lastRound >= 70 && clock() - lastRound <= 100 && spawnMark2 != 38)
			{
				SPAWNCHAIN("spear_v_row_vary");
				spawnMark2 = 38;
				spawnMark3 = 500;
			}
			if(clock() - lastRound >= 4500 && clock() - lastSpawn1 >= spawnMark3)
			{
				SpawnGBlaster(16,p.x,g_uiy,PI*.5f,15);
				SpawnGBlaster(16,g_uix,p.y,PI,15);
				lastSpawn1 = clock();
				spawnMark -= 5;
			}
		}else if(g_round == 147)
		{
			if(clock() - lastRound <= 20 && spawnMark != 22)
			{
				SPAWNCHAIN("spear_h_row_vary");
				spawnMark = 22;
			}
			if(clock() - lastRound >= 3000 && clock() - lastSpawn1 >= 2200)
			{
				for(short i = (spawnMark?0:45); i < g_uix; i += 90)
					SpawnGBlaster(16,i,10,PI*1.5f,15);
				spawnMark = (spawnMark == 0 ? 1 : 0);
				lastSpawn1 = clock();
			}
		}else if(g_round >= 148 && g_round <= 150)
		{
			if(clock() - lastSpawn1 >= spawnMark2)
			{
				SpawnEntityEx(23,g_uix/2,-5,GetEntitySpeed(23)*1.5f,PI*1.5f,g_uix*0.3f,g_uix/2,50);
//				OperateEntities(23, OPER_SET_V_R, 0.9f);
				OperateEntities(23, OPER_SET_THETA_0, PI * 2 * (spawnMark / 1000.0));	//修改初相 
				lastSpawn1 = clock();
				spawnMark  = int(spawnMark + 25 * pow(1.01f,g_round-148)) % 1000;
				spawnMark2 = Choice({50,80,80,90,120,150,160,170,185,200,400});
//				spawnMark2 = 65;
			}
			if((abs(p.x - g_uix/2) > g_uix*0.32f) && clock() - lastSpawn2 >= spawnMark3)
			{	//区间打击 
				SpawnEntityEx(1,p.x+Choice({-10.0,0.0,10.0}),g_uiy-10,GetEntitySpeed(1)*1.6f,PI*.5f,0,0,0,ETA_DELAY_2s);
				lastSpawn2 = clock(); 
				spawnMark3 = Choice({1000,1400,100,400,750,2500});
			}
			if(clock() - lastSpawn3 >= 650)
			{
				SpawnEntityEx(11,g_uix*0.15f,g_uiy,GetEntitySpeed(2)*2.0f,PI*.5f,g_uix*0.2f,0,0);
				SpawnEntityEx(11,g_uix*0.75f,g_uiy,GetEntitySpeed(2)*2.0f,PI*.5f,g_uix*0.2f,0,0);
				lastSpawn3 = clock();
			}
			if((abs(p.x - g_uix/2) > g_uix*0.37f) && clock() - lastSpawn4 >= 1400)
			{	//区间打击 
				SpawnGBlaster(16,p.x+Choice<float>({30.0,-30.0,5.0,-5.0,100.0,-100.0}),0,PI*1.5f,20);
				lastSpawn4 = clock();
			}
		}else if(g_round == 151)
		{
			if(clock() - lastSpawn1 >= 1500)
			{
				SPAWNCHAIN("6spear_roundin");
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 350)
			{
				SpawnEntityEx(9,p.x+Choice({0.0,-5.0,5.0,1.0}),g_uiy,GetEntitySpeed(9)*1.5f,PI*.5f,0,0,0,ETA_ROTATE,true);
				lastSpawn2 = clock();
			}
			if(clock() - lastSpawn3 >= spawnMark)
			{
				short r = Choice({80,150,185,233,265,388});
				SpawnEntityEx(Choice({10,11,11,11,10}),0,r,GetEntitySpeed(10),0.0,r,0,0,ETA_FADE);
				lastSpawn3 = clock();
				spawnMark = Choice({1520,1001,805});
			}
		}else if(g_round == 152)
		{
			short r1=BLUE_BOTTOM*0.9f/2, r2=(BLUE_BOTTOM-r1*2-15)/2;
			if(clock() - lastRound <= 25 && p.HeartMode() != HEART_BLUE)
			{
				p.HeartMode() = HEART_BLUE;
				Sound(SND_DING);
			}
			if(clock() - lastSpawn1 >= 750)
			{	//bottom bones
				SpawnEntityEx(2,0,BLUE_BOTTOM-r2,PL_DEF_SPEED*PL_BLUE_RATIO,0.0,r2,0,0,ETA_FADE);
				lastSpawn1 = clock();
			}
			if(clock() - lastSpawn2 >= 640)
			{	//top bones
				SpawnEntityEx(2,g_uix,r1,PL_DEF_SPEED*PL_BLUE_RATIO,PI,r1,0,0,ETA_FADE);
				lastSpawn2 = clock();
			}
		}else if(g_round == 153)
		{	//模拟 Asgore 另一个回合 
			if(clock() - lastRound <= 20 && p.HeartMode()!=HEART_RED)
				spawnMark = 80,spawnMark2=0,spawnMark3 = 0,lastSpawn2=clock(),p.HeartMode()=HEART_RED;
			if(clock() - lastSpawn1 >= spawnMark)
			{
				float _Amp = Choice({95,65,48,35,30,101});
				float _offset = Choice({0.0,0.0,30.0,-30.0,80.0,-80.0});
				float k1 = spawnMark4 / 100.0;	if(fequ(k1,0.0))	k1=1.0f;
				float k2 = spawnMark5 / 100.0;	if(fequ(k2,0.0))	k2=1.0f;
				SpawnEntityEx(23,g_uix * 0.25f+_offset,0,GetEntitySpeed(23)*0.9f*k1,PI*1.5f,_Amp);
				SpawnEntityEx(23,g_uix * 0.5f+_offset,0,GetEntitySpeed(23)*0.76f*k2,PI*1.5f,_Amp);
				SpawnEntityEx(23,g_uix * 0.75f+_offset,0,GetEntitySpeed(23)*0.9f*k1,PI*1.5f,_Amp);
				lastSpawn1 = clock();
				spawnMark = (spawnMark2 == 2 ? 2500 : 90);
				if(spawnMark2 == 2)
				{
					k1 = Choice({1.0,1.1,0.9,1.8});
					k2 = Choice({0.95,1.0,1.0,1.5,4.0});
					spawnMark4 = short(k1 * 100);
					spawnMark5 = short(k2 * 100);
				}
				spawnMark2 = (spawnMark2 + 1) % 3;
			}
			if(clock() - lastSpawn2 >= 1500)
			{
				if(spawnMark3 == 0)
					SPAWNCHAIN("wool_wave_left");
				else
					SPAWNCHAIN("wool_wave_right");
				spawnMark3 = (spawnMark3 == 0 ? 1 : 0);
				lastSpawn2 = clock();
			}
		}else if(g_round == 154)
		{
			if(clock() - lastRound <= 20)
				spawnMark = 900,spawnMark2 = 0,spawnMark3 = 0;
			if(clock() - lastSpawn1 >= spawnMark)
			{
				SpawnEntityEx(24,g_uix,g_uiy/2,GetEntitySpeed(24)*1.3f,PI,g_uiy * 0.27f);
				lastSpawn1 = clock();
				spawnMark = (spawnMark2 == 5 ? 900 : 90);
				spawnMark2 = (spawnMark2 + 1) % 6;
			}
			if(clock() - lastSpawn3 >= 666)
			{
				SpawnEntityEx(5,g_uix,p.y+Choice({-10.0,10.0,0.0,0.0}),GetEntitySpeed(5)*1.5f,PI,0,0,0,ETA_DELAY_1s);
				lastSpawn3 = clock();
			}
			if(abs(p.y - g_uiy/2.0)/g_uiy > 0.27f)
			{	//区间打击 
				if(clock() - lastSpawn2 >= 188)
				{
			        float _y = g_uiy * 0.18f;
					_y = g_uiy * 0.09f * (spawnMark3);
					if(p.y < g_uiy/2.0)	//Up
						SpawnEntityEx(9,0,_y,GetEntitySpeed(9)*Choice({2.0,1.0,1.0,0.9}),0.0,0,0,0,ETA_ROTATE);
					else	//Down
						SpawnEntityEx(9,0,g_uiy-_y,GetEntitySpeed(9)*Choice({2.0,1.0,1.0,0.9}),0.0,0,0,0,ETA_ROTATE);
					lastSpawn2 = clock();
					spawnMark3 = (spawnMark3 + 1) % 4;
				}
			}
			else if(clock() - lastSpawn2 >= 900)
			{	//幌子 
				SpawnEntityEx(9,0,g_uiy*0.18f,GetEntitySpeed(9)*Choice({2.0,1.0,1.0,0.9}),0.0,0,0,0,ETA_ROTATE);
				SpawnEntityEx(9,0,g_uiy*0.82f,GetEntitySpeed(9)*Choice({2.0,1.0,1.0,0.9}),0.0,0,0,0,ETA_ROTATE);
				lastSpawn2 = clock();
			}
		}
		//========================================================
		else{	//others
		    if(clock() - lastRound <= 20 && spawnMark != 98)
		    {
		    	Sound(SND_ASGORE_VOICE);
		    	spawnMark = 98;
			}
			if(clock() - lastSpawn1 >= SPAWN_SPEAR_CD * pow(0.75, g_round - 145))
			{
				SpawnEntity(1, SPN_4_CENTER);
				lastSpawn1 = clock();
			}
		}
	XCPT_R
}
void CheckEntitiesMove(void)
{	XCPT_L
	if(entities.empty())	return;
	for(long i = 0; i < entities.size(); ++i)
	{
		if(i >= entities.size())
			return;	//防止崩溃 
		entities.at(i).Move();
		entities.at(i).ExtraAction(i);
	}
	/*for(auto iter = entities.begin(); iter != entities.end(); ++iter)	//迭代器写法 
		iter->Move();*/
	XCPT_R
}
void CheckEntitiesLife(void)
{
	XCPT_L
	if(entities.empty())	return;
	for(long i = 0; i < entities.size(); ++i)
	{
		if(!entities.at(i).IsAlive())
		{
			auto iter = entities.begin();
			short j=0;
			while(j < i)
				++iter,++j;
			iter = entities.erase(iter);	//Delete it in the vector
		}
	}
	XCPT_R
}
void KeyboardControl()
{
	XCPT_L
	//Move Keys
		if(LEFT_KEYS)
		{
			p.isMoving=true;
			if(UP_KEYS)
				p.Move(LEFTUP);
			else if(DOWN_KEYS)
				p.Move(LEFTDOWN);
			else
				p.Move(LEFT); 
		}else if(RIGHT_KEYS)
		{
			p.isMoving=true;
			if(UP_KEYS)
				p.Move(RIGHTUP);
			else if(DOWN_KEYS)
				p.Move(RIGHTDOWN);
			else
				p.Move(RIGHT); 
		}else if(UP_KEYS)
			p.Move(UP),p.isMoving=true;
		else if(DOWN_KEYS)
			p.Move(DOWN),p.isMoving=true;
		else if(
		p.HeartMode() != HEART_BLUE
		|| p.HeartMode() == HEART_BLUE && fequ(p.v_g, 0.0f)){
			p.isMoving=false;
		}
		if(QUICK_KEYS)
		{
			p.v = PL_QUICK_SPEED;
		}else if(SLOW_KEYS)
		    p.v = PL_SLOW_SPEED;
		else
	        p.v = PL_DEF_SPEED;
		//Coordination Limitations are inside Player::Move()
		
		//Cheat keys
		if(K(VK_SPACE))
		{
			while(K(VK_SPACE));
			
			g_round++;
			AddMessage("Round "+ToString(g_round));
			lastRound = clock();
		}else if(K('Z'))
		{
			while(K('Z'));
			g_round = SKIP_TO_ROUND;
			AddMessage("Round "+ToString(g_round));
			//Quick changing heart mode
			//<!>111
			/*Sound(SND_DING);
			p.HeartMode() = HEART_GREEN;
			p.dir = UP;*/
			
			lastRound = clock(); 
		}
		//Exit
		if(ESCAPE_KEYS)
		{
			g_playing = false;
			closegraph();
			exit(1);
		}
	XCPT_R
}
void Determination()
{
	XCPT_L
//	srand(time(0));
#ifndef NOSOUND
	soundBank.Music("mus_gameover.mp3");
#endif
	randomize();
	DebugLog("Determination: 玩家死亡");
	/*setfont(pic p_h + 5,0,"微软雅黑");
	setcolor(BLACK);
	xyprintf(p.x - pic p_w / 2 - 10, p.y - pic p_h / 2 - 10,"   ");	//把心清除 */
	DrawScene();
	imagefilter_blurring(NULL,0xff,0x10);	//模糊+变暗效果 
	
	putimage_withalpha(NULL, pic img_gameover, g_uix / 2 - getwidth(pic img_gameover) / 2, 50);
	Sleep(1000);
	Sound(SND_CRACK2);
	setfont(60,0,_T("Comic Sans MS"));
	setbkmode(OPAQUE);
	setcolor(EGERGB(254,254,254));
	const int _left = g_uix/2 - 370;
	const int _top = g_uiy/2;
	Sound(SND_ASGORE_VOICE);
	delayprint(_left, _top, _T("Now isn't the time to end! "),150);
	Sleep(3000);
	xyprintf(_left, _top, _T("                                                  "));	//fill spaces
	Sound(SND_ASGORE_VOICE);
	delayprint(_left, _top, _T("Stay your DETERMINATION!   "),170);
	Sleep(2200);
	getch();
#ifndef NOSOUND
	soundBank.StopMusic(); 
#endif
	XCPT_R
}

/*DWORD ThrEntityMove(LPVOID none)
{
	DebugLog("ThrEntityMove: 线程启动");
	while(g_playing)
	{
		CheckEntitiesSpawn();
		CheckEntitiesMove();
		CheckEntitiesLife();
	}
	DebugLog("ThrEntityMove: 线程关闭");
}*/
void CheckRound()
{	//检查进入下一回合 
	XCPT_L
	if(clock() - lastRound >= ROUND_T)
	{
		g_round++;
		AddMessage("Round " + ToString(g_round));
		p.Heal(Choice({0,1,2,2,2,3,3,3,3,3,3,3,3,4,4,5,10,25}));
		lastRound = clock();
		randomize();
	}
	XCPT_R
}
#define SHUFFLE_T 50
void CheckEntitiesShuffle()
{	//打乱 
	static clock_t lastShuffle = clock();
	if(clock() - lastShuffle >= SHUFFLE_T)
	{
		random_shuffle(entities.begin(),entities.end());
		lastShuffle = clock();
	}
}
DWORD Checks(LPVOID none)
{
	for(;is_run() && g_playing; api_sleep(1))	//Game Loop
	{
		CheckPlayerMove();
		CheckEntitiesSpawn();
	}
	return 1;
}
#define DRAW_CD 4
void InGame()
{
	XCPT_L
//	srand(time(0));
	randomize();
	g_playing = true;
	if(!entities.empty())	entities.clear();
	lastSpawn1 = clock();
	lastDraw = clock() + DRAW_CD;
	lastRound = clock();
	lastFrame = clock();
	g_temp[0] = 1;
	g_temp[1] = 1;
	
	g_round = 1;
	p.Init();	//Player Initialization
	//CreateThread((LPTHREAD_START_ROUTINE)ThrEntityMove);
	DebugLog("InGame: 进入游戏");
	AddMessage("Round 1");
	setbkmode(TRANSPARENT);
	CreateThread((LPTHREAD_START_ROUTINE)Checks);
	DrawScene();
	for(;is_run() && g_playing;)	//Game Loop
	{
		clock_t cur = clock();	//current clock
		
		if((cur - lastDraw) >= DRAW_CD)
		{	//Scene Drawing Regularly
			DrawScene();
			lastDraw = clock();
		}
		//Checks
//		Checks(nullptr);
		CheckRound();
		CheckPlayerCollision();
		#ifndef NOSHUFFLE
		CheckEntitiesShuffle();
		#endif
//		CheckEntitiesSpawn();
		CheckEntitiesMove();
		CheckEntitiesLife();
		CheckIndicatorsLife();
		//Control
		KeyboardControl();	//键盘控制 
		//Death
		if(!p.IsAlive())
		{
			Sound(SND_CRACK1);
			g_playing = false;
			Sleep(500);
			Determination();
		}
		lastFrame = clock();
	}
	
	XCPT_R
	
} 
void FinalTerminate()
{
	MessageBoxA(NULL,"程序由于未知问题崩溃！","Underspace EXCEPTION",MB_ICONERROR|MB_OK);
	return;
}
//-----------------------------------------------------------------------------------
#define BOTTOM_GAP 40
int WinMain()	//Main
{
	CPlayer g_music; 
	XCPT_L
	if(ExistFile(LOG_NAME))
	{
		DeleteFile(LOG_NAME);
	}
	DebugLog("main: 程序启动");
	
	/*BOOL ret = g_music.Open("snd_spear.wav");
	if(ret)
	{
		g_music.Play();
	}else{
		DebugLog("<!> Open Music Failed!");
	}*/
	#ifndef NOSOUND 
	PrepareSounds();
	#endif
	set_terminate(FinalTerminate);
	//Attain Window Size
	g_uix = GetScreenWidth();
	g_uiy = GetSystemWorkAreaHeight() - BOTTOM_GAP;
	//Initialize Window
	setinitmode(0);
	initgraph(g_uix, g_uiy);
	hwnd = getHWnd();
	setcaption(("Underspace " + string(CRT_VER) + " ★Designed By wormwaker").c_str());
	setrendermode(RENDER_MANUAL);
	setbkcolor(BLACK);
	SetWindowPos(hwnd,NULL,0,0,0,0,SWP_NOSIZE|SWP_NOZORDER);
	//Other Initializations
	pic LoadImages();
	ReadEntityData();
	lastDraw = clock();
	//Game
	while(1)
		InGame();
	//Program Ending
//	getch();
	XCPT_R
	pic ClearLengthBones();
	soundBank.Dispose();
	closegraph();
	return 0;
}
//------------------------------------------------------------------------
