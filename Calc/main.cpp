#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>

#define pb push_back

using namespace std;

vector <char> vec;
vector <int> vec0;
vector<int>::iterator pos;
int q;

int ch_num(char c) ///// check num
{
    return (c >= '0' && c <= '9');
}


int ch_sign(char c) ///// check sign
{
    return (int(c) == 42) || (int(c) == 43) || (int(c) == 45) || (int(c) == 47) || (int(c) == 37);
}

int main()
{
    //freopen ("in.txt", "r", stdin);freopen ("out.txt", "w", stdout);
    string s, t = "";
    int p = 0;
    getline(cin, s);
    if (s == "")
    {
        cout << "Incorrect input" << endl;
        return 0;
    }
    for (int i = 0; i < s.size(); i++)//delete all spaces
        if (s[i] != ' ')
            t += s[i];
    if (ch_sign(t[0]) || ch_sign(t[t.size() - 1]))
    {
        cout << "Incorrect input" << endl;
        return 0;
    }
    for (int i = 0; i < t.size(); i++)// save all signs in vector - vec, save of numbers in vector - vec0
    {
        if (ch_sign(t[i]))
        {
            vec0.pb (p);
            vec.pb (t[i]);
            if (ch_sign(t[i + 1]) || i + 1 == t.size())
            {
                cout << "Incorrect input" << endl;
                return 0;
            }
            p = 0;
        }
        else
            if (ch_num(t[i]))
                p = p * 10 + t[i] - '0';
            else
            {
                cout << "Incorrect input" << endl;
                return 0;
            }
    }
    vec0.pb (p);

    for (int x = 0; x < vec.size(); x++)// do '/' and '*' and '%'
    {
        if (vec[x] == '*')
        {
            vec0[q] *= vec0[q + 1];
            vector<int>::iterator pos = vec0.begin() + q + 1;
            vec0.erase(pos);
            q--;
        }
        else
            if (vec[x] == '/')
            {
                if (vec0[q + 1] == 0)
                {
                    cout << "Division by zero" << endl;
                    return 0;
                }
                vec0[q] /= vec0[q + 1];
                vector<int>::iterator pos = vec0.begin() + q + 1;
                vec0.erase(pos);
                q--;
            }
        else
        if (vec[x] == '%')
        {
            vec0[q] %= vec0[q + 1];
            vector<int>::iterator pos = vec0.begin() + q + 1;
            vec0.erase(pos);
            q--;
        }
        q++;
    }
    int ans = vec0[0], l = 1;
    for (int i = 0; i < vec.size(); i++)// do plus and minus
    {
        if (vec[i] != '-' && vec[i] != '+')
            continue;
        if (vec[i] == '+')
            ans += vec0[l];
        else
            ans -= vec0[l];
        l++;
    }
    cout << ans;//answer
    return 0;
}
