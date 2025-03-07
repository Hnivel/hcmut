#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <climits>
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
// BF
void BF(int graphs[][20], int numberOfVertices, char startVertex, int value[], int previous[]);
// BF Path
string BF_Path(int graphs[][20], int numberOfVertices, char startVertex, char goalVertex);