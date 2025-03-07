#include "bellman.h"
// BF
void BF(int graphs[][20], int numberOfVertices, char startVertex, int value[], int previous[])
{
    int indexOfStartVertex = startVertex - 'A';
    const int infinity = 1000000000;
    if (value[indexOfStartVertex] == -1)
    {
        for (int i = 0; i < numberOfVertices; i++)
        {
            value[i] = infinity;
            previous[i] = -1;
        }
        value[indexOfStartVertex] = 0;

        for (int v = 0; v < numberOfVertices; v++)
        {
            if (graphs[indexOfStartVertex][v] != 0)
            {
                value[v] = graphs[indexOfStartVertex][v];
                previous[v] = indexOfStartVertex;
            }
        }
        for (int v = 0; v < numberOfVertices; v++)
        {
            if (value[v] == infinity)
            {
                value[v] = -1;
            }
        }
        return;
    }
    int tempValue[numberOfVertices];
    for (int i = 0; i < numberOfVertices; i++)
    {
        if (value[i] == -1)
        {
            tempValue[i] = infinity;
        }
        else
        {
            tempValue[i] = value[i];
        }
    }
    for (int u = 0; u < numberOfVertices; u++)
    {
        for (int v = 0; v < numberOfVertices; v++)
        {
            if (graphs[u][v] != 0 && tempValue[u] < infinity)
            {
                if (value[v] > tempValue[u] + graphs[u][v] || value[v] == -1)
                {
                    value[v] = tempValue[u] + graphs[u][v];
                    previous[v] = u;
                }
            }
        }
    }
}
// BF Path
string BF_Path(int graphs[][20], int numberOfVertices, char startVertex, char goalVertex)
{
    string path;
    const int infinity = 1000000000;
    int value[20], previous[20];
    int startIndex = startVertex - 'A';
    int goalIndex = goalVertex - 'A';
    if (goalIndex == startIndex)
    {
        path.push_back(startVertex);
        return path;
    }
    for (int i = 0; i < numberOfVertices; i++)
    {
        value[i] = infinity;
        previous[i] = -1;
    }
    value[startIndex] = 0;
    for (int i = 0; i < numberOfVertices - 1; i++)
    {
        int tempValue[numberOfVertices];
        for (int i = 0; i < numberOfVertices; i++)
        {
            if (value[i] == -1)
            {
                tempValue[i] = infinity;
            }
            else
            {
                tempValue[i] = value[i];
            }
        }
        for (int u = 0; u < numberOfVertices; u++)
        {
            for (int v = 0; v < numberOfVertices; v++)
            {
                if (graphs[u][v] != 0 && tempValue[u] < infinity)
                {
                    if (value[v] > tempValue[u] + graphs[u][v] || value[v] == -1)
                    {
                        value[v] = tempValue[u] + graphs[u][v];
                        previous[v] = u;
                    }
                }
            }
        }
    }
    int current = goalIndex;
    while (current != startIndex)
    {
        path = char('A' + current) + path;
        path = " " + path;
        current = previous[current];
    }
    path = startVertex + path;

    return path;
}