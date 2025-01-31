#include "arraysort.h"

#include <QDebug>

void swapElements(int *pArray, unsigned int first, unsigned int second)
{
    int tmp = pArray[first];
    pArray[first] = pArray[second];
    pArray[second] = tmp;
}

void swap(int *first, int *second)
{
    int tmp = *first;
    *first = *second;
    *second = tmp;
}

int partition(int *pArray, int begin, int end)
{
    int pivot = begin;
    for(int i = begin; i <= end; ++i)
    {
        if(pArray[i] < pArray[pivot])
        {
            int tmp = pArray[i];
            memcpy(pArray + pivot + 1, pArray + pivot, (i - pivot) * sizeof(int));
            pArray[pivot] = tmp;
            ++pivot;
        }
    }
    return pivot;
}

void printArray(int *pArray, unsigned int size);

void sort(int *pArray, int begin, int end)
{
    if(end - begin < 1)
        return;

    int pivotIdx = partition(pArray, begin, end);
    sort(pArray, begin, pivotIdx - 1);
    sort(pArray, pivotIdx + 1, end);
}

void bubbleSort(int *pArray, unsigned int size)
{
    bool finish;
    do
    {
        finish = true;
        for(unsigned int i = 0; i < size - 1; ++i)
        {
            if(pArray[i] > pArray[i + 1])
            {
                swapElements(pArray, i, i + 1);
                finish = false;
            }
        }
    }while(!finish);
}

void shakerSort(int *pArray, unsigned int size)
{
    unsigned int begin = 0;
    unsigned int end = size - 1;
    bool finish;
    do
    {
        finish = true;
        for(unsigned int i = begin; i < end; ++i)
        {
            if(pArray[i] > pArray[i + 1])
            {
                swapElements(pArray, i, i + 1);
                finish = false;
            }
        }
        if (finish)
            break;
        end -= 1;
        for(unsigned int i = end; i > begin; --i)
        {
            if(pArray[i] < pArray[i - 1])
            {
                swapElements(pArray, i, i - 1);
                finish = false;
            }
        }
        begin += 1;
    }while(!finish);
}

void gnomeSort(int *pArray, unsigned int size)
{
    unsigned int i = 1;
    // Запоминаем место, где появилась ошибка, чтобы после исправления вернуться
    unsigned int idx2Return = 1;
    while (i < size)
    {
        if(i == 0)
            ++i;
        if(pArray[i] < pArray[i - 1])
        {
            swapElements(pArray, i, i - 1);
            if(i > idx2Return)
                idx2Return = i;
            --i;
        }
        else {
            if(i < idx2Return)
                i = idx2Return;
            ++i;
        }

    }
}

void quickSort(int *pArray, unsigned int size)
{
    sort(pArray, 0, size - 1);
}

