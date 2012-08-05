class BufferedReader
{
public:
    BufferedReader& operator >> (int& number)
    {
        number = getInt();
        return *this;
    }
private:
    int getInt()
    {
        char ch;
        while((ch = getchar()) && !isdigit(ch));
        int ret = ch - '0';
        while((ch = getchar()) && isdigit(ch))
        {
            ret *= 10;
            ret += ch -'0';
        }
        return ret;
    }
} buf;
