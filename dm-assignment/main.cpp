#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <climits>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include "bellman.h"
#include "tsm.h"
using namespace std;
int main()
{
    int G[20][20];
    int BFValue[20];
    int BFPrev[20];
    int n;
    cout<<"Number of vertices: ";
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        BFValue[i] = -1;
        BFPrev[i] = -1;
        for (int j = 0; j < n; j++)
        {
            cin >> G[i][j];
        }
    }
    int continueValue = 1;
    bool run = true;
    do
    {
        cout << "0. Quit" << endl;
        cout << "1. BF" << endl;
        cout << "2. BF_Path" << endl;
        cout << "3. Traveling" << endl;
        cout << "Your Choice: ";
        cin >> continueValue;
        switch (continueValue)
        {
        case 1:
        {
            char startVertex;
            cout << "Start Vertex: ";
            cin >> startVertex;
            for (int i = 0; i < n; i++)
            {
                BF(G, n, startVertex, BFValue, BFPrev);
                cout << "Step " << i + 1 << ":" << endl;
                for (int j = 0; j < n; j++)
                {
                    cout << BFValue[j] << " ";
                }
                cout << endl;
                for (int j = 0; j < n; j++)
                {
                    cout << BFPrev[j] << " ";
                }
                cout << endl;
            }
            break;
        }
        case 2:
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    cout << BF_Path(G, n, char(i + 'A'), char(j + 'A')) << endl;
                }
            }
            break;
        }
        case 3:
        {
            char startVertex;
            cout << "Start Vertex: ";
            cin >> startVertex;
            cout << Traveling(G, n, startVertex) << endl;
            break;
        }
        default:
        {
            run = false;
            break;
        }
        }
    } while (run == true);
    return 0;
}