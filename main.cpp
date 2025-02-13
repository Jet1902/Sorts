#include <QCoreApplication>
#include <QDebug>
#include <QRandomGenerator>
#include <QTime>
//#include <QThread>
#include <thread>
#include <mutex>
#include <iostream>

#include "arraysort.h"

void fillArrayWithRand(int *pArray, unsigned int size);
void printArray(int *pArray, unsigned int size);
bool isSortedArray(int *pArray, unsigned int size);
void printArrayCheck(bool isSorted);

typedef void (*sortfunc)(int *pArray, unsigned int size);

int testSortFunc(int *pArray, unsigned int size, QString name, sortfunc func);

static std::mutex mtx;                  // Объявление мьютекса для защиты разделяемых данных

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::vector<std::thread> vTh;       // Вектор потоков
    int execTime = 0;                   // Переменная для хранения суммы времени выполнения потоков

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

    int execStart = QTime::currentTime().msecsSinceStartOfDay();

    for(vFunc::iterator it = vFunctions.begin(); it != vFunctions.end(); ++it)
    {
        auto f = [&execTime](int * pOriginal, unsigned int iSize, QString name, sortfunc func)      // Лямбда-функция, добавляющая время выполнения потока в execTime
        {
            int time = testSortFunc(pOriginal, iSize, name, func);                                  // и вызывающая функцию сортировки
            mtx.lock();                                                                             // Лочим использование ресурса execTime
            execTime += time;
            mtx.unlock();                                                                           // Ресурс разблокирован

//            {
//                std::lock_guard<std::mutex> locker(mtx);                                          // Реализация того же самого с помощью lock_guard
//                execTime += time;                                                                 // Ограничение области видимости диктует область работы lock_guard
//            }

//            std::unique_lock<std::mutex> ul(mtx);                                                 // Опять то же самое
//            execTime += time;
//            ul.unlock();                                                                          // Если бы не освободили,
                                                                                                    // он бы осводил сам при выходе из области видимости
        };
        vTh.push_back(std::thread(f, pOriginal, iSize, it.key(), it.value()));                      // Создание потока, запихивание туда лямбда-функции
                                                                                                    // и запихивание самого потока в вектор потоков
                                                                                                    // Лямбда-функция: [передача ресурсов] (параметры){тело}
    }


    delete [] pOriginal;

    for(auto &th : vTh)                                                                             // Ждем каждый поток в векторе
        th.join();

    int execFinish = QTime::currentTime().msecsSinceStartOfDay();

    std::cout << "Execution Time: " << execFinish - execStart << " ms\n";
    std::cout << "Summ of threads time: " << execTime << " ms\n";
    std::cout << "Time saved: " << execTime - (execFinish - execStart) << " ms\n";
    return a.exec();
}

int testSortFunc(int *pArray, unsigned int size, QString name, sortfunc func)
{

    int *pTest = new int[size];
    memcpy(pTest, pArray, sizeof(int) * size);

    int start = QTime::currentTime().msecsSinceStartOfDay();
    (*func)(pTest, size);
    int finish = QTime::currentTime().msecsSinceStartOfDay();

    std::cout << "Thread number: " << std::this_thread::get_id() << "\n";           // Вывод номера потока
    qDebug() << name << (isSortedArray(pTest, size) ? " success" : "failure") << ", took" << finish - start << "ms";

    if(size <= 10)
    {
        qDebug() << "Sorted array:";
        printArray(pTest, size);
    }

    delete [] pTest;

    return finish - start;
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
