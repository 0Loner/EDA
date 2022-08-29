#include <iostream>
#include <stdlib.h> 
using namespace std;


class Vector
{
public:
    int* arr, 
        m_nCount,  
        m_nMax, 
        m_nDelta; 
    void Resize(int*& rpVect, int& rnMax)
    {
        const int delta = 10;
        int* pTemp, i;
        pTemp = new int[rnMax + delta]; 
        for (i = 0; i < rnMax; i++) 
            pTemp[i] = rpVect[i]; 
        delete[] rpVect; 
        rpVect = pTemp; 
        rnMax += delta; 
    }

    void Insert(int*& rpVect, int& rnCount, int& rnMax, int elem)
    {
        if (rnCount == rnMax)
        {
            Resize(rpVect, rnMax);
        }
        rpVect[rnCount++] = elem;
    }


    void print(int*& rpVect, int& rnMax)
    {
        int* pTemp, i;
        for (i = 0; i < rnMax; i++)
        {
            cout << rpVect[i] << " - ";
        }
    }
};
int main()
{
    Vector v;
    v.m_nCount = 0;
    v.m_nMax = 10;
    v.arr = new int[v.m_nMax];
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 1);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 2);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 3);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 4);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 5);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 6);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 7);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 8);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 9);
    v.Insert(v.arr, v.m_nCount, v.m_nMax, 10);
    v.print(v.arr, v.m_nMax);
}