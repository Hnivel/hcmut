#include "tsm.h"
// TSP
string Traveling(int graphs[20][20], int numberOfVertices, char startVertex)
{
    const int infinity = 1000000000;
    int startIndex = startVertex - 'A';
    int **value = new int *[1 << numberOfVertices];
    int **previous = new int *[1 << numberOfVertices];
    for (int i = 0; i < (1 << numberOfVertices); i++)
    {
        value[i] = new int[numberOfVertices];
        previous[i] = new int[numberOfVertices];
        for (int j = 0; j < numberOfVertices; j++)
        {
            value[i][j] = infinity;
            previous[i][j] = -1;
        }
    }
    value[1 << startIndex][startIndex] = 0;
    for (int i = 0; i < (1 << numberOfVertices); i++)
    {
        for (int u = 0; u < numberOfVertices; u++)
        {
            if (i & (1 << u))
            {
                for (int v = 0; v < numberOfVertices; v++)
                {
                    if (!(i & (1 << v)) && graphs[u][v] != 0)
                    {
                        int nextIndex = i | (1 << v);
                        if (value[i][u] + graphs[u][v] < value[nextIndex][v])
                        {
                            value[nextIndex][v] = value[i][u] + graphs[u][v];
                            previous[nextIndex][v] = u;
                        }
                    }
                }
            }
        }
    }
    int minimumCost = infinity;
    int lastVertex = -1;
    for (int i = 0; i < numberOfVertices; i++)
    {
        if (value[(1 << numberOfVertices) - 1][i] + graphs[i][startIndex] < minimumCost && graphs[i][startIndex] != 0)
        {
            minimumCost = value[(1 << numberOfVertices) - 1][i] + graphs[i][startIndex];
            lastVertex = i;
        }
    }
    string path = "";
    int mask = (1 << numberOfVertices) - 1;
    int currentIndex = lastVertex;
    while (currentIndex != -1)
    {
        path = char('A' + currentIndex) + (path.empty() ? "" : " " + path);
        int temp = currentIndex;
        currentIndex = previous[mask][currentIndex];
        mask = mask ^ (1 << temp);
    }
    path = path + " " + startVertex;
    for (int i = 0; i < (1 << numberOfVertices); i++)
    {
        delete[] value[i];
        delete[] previous[i];
    }
    delete[] value;
    delete[] previous;
    return path;
}
