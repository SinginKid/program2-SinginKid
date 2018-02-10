#ifndef QUEUEARRAY_H
#define QUEUEARRAY_H
#include <queue>
using namespace std;
template <typename T>
class QueueArray{
private:
        int size;
        queue<T>* myArray;
public:
        QueueArray(int size);
        ~QueueArray();
        int Enqueue(T item, int index);
        T Dequeue();
        int Qsize(int index);
        int Asize();
        int QAsize();
        T* Qstate(int index);
};
template <typename T>
QueueArray <T>::QueueArray(int num) {
        myArray = new queue<T>[num];
        size = num;
}
template <typename T>
QueueArray <T>::~QueueArray() {
        delete[] myArray;
}
template <typename T>
int QueueArray<T>::Enqueue(T item, int index) {
        if (index >= size) {
                return -1;
        }
        myArray[index].push(item);
        return 1;
}
template <typename T>
T QueueArray<T>::Dequeue() {
        int count = 0;
        T hold;
        while (myArray[count].size() == 0) {
                if (count >= size) {
                        return 0;
                }
                count++;
        }
        hold = myArray[count].front();
        myArray[count].pop();
        return hold;
}
template <typename T>
int QueueArray<T>::Qsize(int index) {
        return myArray[index].size();
}
template <typename T>
int QueueArray<T>::Asize() {
        return size;
}
template <typename T>
int QueueArray<T>::QAsize() {
        int count = 0;
        for (int i = 0; i < size; i++) {
                count += myArray[i].size();
        }
        return count;
}
template <typename T>
T* QueueArray<T>::Qstate(int index) {
        if (index >= size) {
                return 0;
        }
        int vecSize = myArray[index].size();
        T* temp = new T[vecSize];
        for (int i = 0; i < vecSize; i++) {
                temp[i] = myArray[index].front();
                myArray[index].pop();
        }
        for (int i = 0; i < vecSize; i++) {
                myArray[index].push(temp[i]);
        }
        return temp;
}
#endif