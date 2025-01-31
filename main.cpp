#include <QCoreApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QTime>

#include "arraysort.h"

void fillArrayWithRand(int *pArray, unsigned int size);
void printArray(int *pArray, unsigned int size);
bool isSortedArray(int *pArray, unsigned int size);
void printArrayCheck(bool isSorted);

typedef void (*sortfunc)(int *pArray, unsigned int size);

void testSortFunc(int *pArray, unsigned int size, QString name, sortfunc func);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    typedef QMap<QString, sortfunc> vFunc;
    vFunc vFunctions;

    vFunctions["Bubble"] = bubbleSort;
    vFunctions["Shaker"] = shakerSort;
    vFunctions["Gnome"] = gnomeSort;
    vFunctions["Quick"] = quickSort;

    const unsigned int iSize = 10000;
    int *pOriginal = nullptr; // Исходный массив

    pOriginal = new int[iSize];
    fillArrayWithRand(pOriginal, iSize);

    qDebug() << "Testing sorting functions on " << iSize << " elements";\
    if(iSize <= 10)
    {
        qDebug() << "Original array:";
        printArray(pOriginal, iSize);
    }

    for(vFunc::iterator it = vFunctions.begin(); it != vFunctions.end(); ++it)
        testSortFunc(pOriginal,iSize, it.key(), it.value());

    delete [] pOriginal;

    return a.exec();
}

void testSortFunc(int *pArray, unsigned int size, QString name, sortfunc func)
{
    int *pTest = new int[size];
    memcpy(pTest, pArray, sizeof(int) * size);

    int start = QTime::currentTime().msecsSinceStartOfDay();
    (*func)(pTest, size);
    int finish = QTime::currentTime().msecsSinceStartOfDay();

    qDebug() << name << (isSortedArray(pTest, size) ? " success" : "failure") << ", took" << finish - start << "ms";

    if(size <= 10)
    {
        qDebug() << "Sorted array:";
        printArray(pTest, size);
    }

    delete [] pTest;
}

// Заполнение массива  случайными величинами
void fillArrayWithRand(int *pArray, unsigned int size)
{
    for(unsigned int i = 0; i < size; ++i)
        pArray[i] = QRandomGenerator::global()->bounded(100);
}

void printArray(int *pArray, unsigned int size)
{
    QDebug dbg = qDebug();

    dbg << "{";
    for(unsigned int i = 0; i< size; ++i)
    {
        dbg << pArray[i];
        if(i < size - 1)
            dbg << ", ";
    }
    dbg << "}";
}

void printArrayCheck(bool isSorted)
{
    QDebug dbg = qDebug();

    if(isSorted)
        dbg << "Array is sorted";
    else
        dbg << "Array is not sorted";
}

bool isSortedArray(int *pArray, unsigned int size)
{
    for(unsigned int i = 0; i < size - 1; ++i)
        if(pArray[i] > pArray[i + 1])
            return false;

    return true;
}
