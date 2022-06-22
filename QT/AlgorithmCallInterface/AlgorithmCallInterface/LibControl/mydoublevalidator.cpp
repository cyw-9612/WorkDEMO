#include "MyDoubleValidator.h"

MyDoubleValidator::MyDoubleValidator(QObject *parent)
    : QDoubleValidator(parent)
{

}

MyDoubleValidator::~MyDoubleValidator()
{

}

QValidator:: State MyDoubleValidator::validate(QString & input, int & pos) const
{
    if (input.isEmpty())
    {
        return QValidator::Intermediate;
    }
    bool OK = false;
    double val = input.toDouble(&OK);

    if (!OK)
    {
        return QValidator::Invalid;
    }

    int dotPos = input.indexOf(".");
    if (dotPos > 0)
    {
        if (input.right(input.length() - dotPos - 1).length() > decimals())
        {
            return QValidator::Invalid;
        }
    }
    if(val<bottom()|| val>top())
        return QValidator::Invalid;
    return QValidator::Acceptable;
}
