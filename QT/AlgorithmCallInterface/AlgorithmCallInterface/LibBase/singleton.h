/*************************************************
  Copyright (C), 2020-2021, AIHua Tech. Co., Ltd.
  File name:    singleton.h
  Author:       yangchun
  Version:      0.0.01
  Date:         2020-6-1
  Description:  通用单例类
  History:
*************************************************/
#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

template <typename T>
class Singleton
{
public:
    static T &getInstance();
    Singleton(const Singleton &other) = default;
    Singleton<T> &operator=(const Singleton &other) = default;

private:
    static QMutex mutex;
    static QScopedPointer<T> instance;
};

template <typename T> QMutex Singleton<T>::mutex;
template <typename T> QScopedPointer<T> Singleton<T>::instance;

template<typename T>
T &Singleton<T>::getInstance()
{
    if (instance.isNull()) {
        mutex.lock();
        if (instance.isNull()) {
            instance.reset(new T());
        }
        mutex.unlock();
    }
    return *instance.data();
}

#define SINGLETON(Class) \
    private: \
    Class(); \
    ~Class(); \
    Class(const Class &other); \
    Class &operator =(const Class &other); \
    friend class Singleton<Class>; \
    friend struct QScopedPointerDeleter<Class>;


#endif // SINGLETON_H
