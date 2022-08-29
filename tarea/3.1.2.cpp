#include <iostream>
using namespace std;

int* gpVect = NULL; 
int gnCount = 0; 
int gnMax = 0;

void Resize()
{
    const int delta = 10;
    int* pTemp, i;
    pTemp = new int[gnMax + delta]; 
    for (i = 0; i < gnMax; i++) 
        pTemp[i] = gpVect[i]; 
    delete[] gpVect; 
    gpVect = pTemp; 
    gnMax += delta; 
}
void Insert(int elem)
{

    if (gnCount == gnMax)
    {
        Resize();
    }
    gpVect[gnCount++] = elem;

}
void print()
{
    int* pTemp, i;
    for (i = 0; i < gnMax; i++)
    {
        cout << gpVect[i] << ",";
    }
}
int main()
{
    Insert(1);
    Insert(2);
    Insert(3);
    print();
}