string CJZAPI strtail(string s,int cnt=1) 
{
	if(cnt > s.size())
		return s;
	return s.substr(s.size()-cnt+1,cnt);
}
string CJZAPI strhead(string s,int cnt=1) 
{
	if(cnt > s.size())
		return s;
	return s.substr(0,cnt);
}
string CJZAPI strxtail(string s,int cnt=1) 
{
	if(cnt > s.size())
		return "";
	return s.substr(0,s.size()-cnt);
}
string CJZAPI strxhead(string s,int cnt=1) 
{
	if(cnt > s.size())
		return "";
	return s.substr(cnt,s.size()-cnt);
}
