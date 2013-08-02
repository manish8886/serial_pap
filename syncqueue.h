#include <QtCore/QQueue>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
template<class T>
class QSynchQueue{
 public:
  QSynchQueue();
 public:
  void enqueue(const T& data);
  T dequeue();
  /*  T& head();
      const T& head()const;*/
 private:
  QQueue<T>container;
  static const int MAX_SIZE; 
  QWaitCondition cv;
  QMutex mutex;
};
template<class T>
int QSynchQueue<T>::MAX_SIZE = 256;

template<class T>
QSynchQueue<T>::QSynchQueue(){
  
}

template<class T>
void QSynchQueue<T>::enqueue(const T& data){
  mutext.lock();
  while(container.size()==MAX_SIZE){
    cv.wait(&mutex);
  }
  contaniner.enqueue(data);
  if(contanier.size()==1)//if contanier has atleast one item
    container.wakeall();
  mutext.unlock();
}

template<class T>
T QSynchQueue<T>::dequeue(){
  T data;
  mutext.lock();
  while(container.size()==0){
    cv.wait(&mutex);
  }
  data = contaniner.dequeue();
  if(contanier.size()==MAX_SIZE-1)//if contanier has atleast one item
    container.wakeall();
  mutext.unlock();
  return data;
}
