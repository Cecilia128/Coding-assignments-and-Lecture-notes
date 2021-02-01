//
//  AS1_PrefixExp_118020046.cpp
//  Demo
//
//  Created by Cecilia on 2020/11/5.
//  Copyright © 2020 Cecilia. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

long long ADD(long long a, long long b)
{
    return a+b;
}

long long MINUS(long long a, long long b)
{
    return a-b;
}

long long TIMES(long long a, long long b)
{
    return a*b;

}

int main()
{
    int N;
    cout << "";
    cin >> N;
    string expression[N];
    stringstream ss;
    for (int i = 0; i < N; i++) cin >> expression[i];
    long long num[(N+1)/2];
    int index = -1;
    
    for (int j = N-1; j>=0; --j)
    {
        if (expression[j] == "+")
        {
            if (index < 1)
            {
                cout << "Invalid" << endl;
                return 0;
            }
            index = index - 1;
            num[index] = ADD(num[index+1], num[index]);
            num[index] = num[index] % 1000000007;
        }
        else if (expression[j] == "-")
        {
            if (index < 1)
            {
                cout << "Invalid" << endl;
                return 0;
            }
            index = index - 1;
            num[index] = MINUS(num[index+1], num[index]);
        }
        else if (expression[j] == "*")
        {
            if (index < 1)
            {
                cout << "Invalid" << endl;
                return 0;
            }
            index = index - 1;
            num[index] = TIMES(num[index+1], num[index]);
            num[index] = num[index] % 1000000007;
        }
        else {
            index = index + 1;
            ss << expression[j];
            ss >> num[index];
            num[index] = num[index] % 1000000007;
            ss.clear();
        }
    }
    if (index != 0) cout << "Invalid";
    else cout << ((num[index] % 1000000007) + 1000000007) % 1000000007;
    return 0;
}
