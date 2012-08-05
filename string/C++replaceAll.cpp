string& replace_all(string& str, const string& src, const string& dest)
{
	string::size_type pos(0);
	while((pos = str.find(src)) != string::npos)
		str.replace(pos,src.length(), dest);
	return str;
}
string& replace_all_distinct(string& str, const string& src, const string& dest)
{
	for(string::size_type pos(0);(pos = str.find(src, pos)) != string::npos;pos += dest.length())
		str.replace(pos, src.length(), dest);
	return str;
}
