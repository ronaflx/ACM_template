int Minimum_Representation(char *s, int len)
{
    int i = 0, j = 1, k = 0, t;
    while(i < len && j < len && k < len)
    {
        t = s[(i + k) % len] - s[(j + k) % len];
        if(t == 0) k++;
        else
        {
            if(t < 0) j += k + 1;
            else i += k + 1;
            if(i == j) j++;
            k = 0;
        }
    }
    return min(i, j);
}
 
