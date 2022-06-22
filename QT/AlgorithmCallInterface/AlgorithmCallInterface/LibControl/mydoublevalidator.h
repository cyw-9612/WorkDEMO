#ifndef MYDOUBLEVALIDATOR_H
#define MYDOUBLEVALIDATOR_H
#include <QDoubleValidator>
#include "LibControl_global.h"

/***********************************
 *
 * 功 能: 自定义QDoubleValidator类
 *
 * 编 制: 陈宇葳
 *
 * 日 期: 2020/05/12
 *
 **********************************/

class LIBCONTROL_EXPORT MyDoubleValidator : public QDoubleValidator
{
    Q_OBJECT
public:
    MyDoubleValidator(QObject *parent);
    ~MyDoubleValidator();
    virtual QValidator::State validate(QString &input, int &pos) const;
};

#endif // MYDOUBLEVALIDATOR_H
