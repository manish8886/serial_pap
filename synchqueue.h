#ifndef SYNCH_QUEUE_H
#define SYNCH_QUEUE_H
#include <QtCore/QQueue>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
template<class T>
class QSynchQueue{
  Q_OBJECT
 public:
  QSynchQueue();
 public:
  void enqueue(const T& data);
  T dequeue();
  /*  T& head();
      const T& head()const;*/

  public Q_SLOTS:
  void makequnonblocking();
 private:
  QQueue<T>container;
  static const int MAX_SIZE; 
  QWaitCondition cv;
  QMutex mutex;
  bool canBlock;
};
template<class T>
const int QSynchQueue<T>::MAX_SIZE = 256;

template<class T>
QSynchQueue<T>::QSynchQueue(){
  canBlock=true;
}

template<class T>
QSynchQueue<T>::~QSynchQueue(){
  
}

template<class T>
QSynchQueue<T>::makequeuenonblocking(){
  /*check if someone is already blocked on the queue. if
   that's the case wakep up all thread*/
  mutex.lock();
  canBlock=false;
  cv.wakeAll();
  mutex.unlock();

}



template<class T>
void QSynchQueue<T>::enqueue(const T& data){
  mutex.lock();
  while(container.size()==MAX_SIZE){
    cv.wait(&mutex);
  }
  container.enqueue(data);
  if(container.size()==1)//if contanier has atleast one item
    cv.wakeAll();
  mutex.unlock();
}

template<class T>
T QSynchQueue<T>::dequeue(){
  T data;
  mutex.lock();
  while(container.size()==0){
    cv.wait(&mutex);
  }
  data = container.dequeue();
  if(container.size()==MAX_SIZE-1)//if contanier has atleast one item
    cv.wakeAll();
  mutex.unlock();
  return data;
}
#endif
