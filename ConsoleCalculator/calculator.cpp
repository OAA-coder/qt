#include "calculator.h"
#include <QDebug>
#include <cmath>

Calculator::Calculator(QObject *parent)
    : QObject(parent)
    , m_result(0.0)
    , m_hasError(false)
{
    qDebug() << "Calculator created";
}

void Calculator::setResult(double value)
{
    m_result = value;
    m_hasError = false;
    m_errorMessage.clear();
    emit resultReady(m_result);
}

void Calculator::setError(const QString &msg)
{
    m_hasError = true;
    m_errorMessage = msg;
    emit errorOccurred(m_errorMessage);
}

void Calculator::add(double a, double b)
{
    qDebug() << "add(" << a << ", " << b << ")";
    setResult(a + b);
}

void Calculator::subtract(double a, double b)
{
    qDebug() << "subtract(" << a << ", " << b << ")";
    setResult(a - b);
}

void Calculator::multiply(double a, double b)
{
    qDebug() << "multiply(" << a << ", " << b << ")";
    setResult(a * b);
}

void Calculator::divide(double a, double b)
{
    qDebug() << "divide(" << a << ", " << b << ")";
    if (qFuzzyIsNull(b)) {
        setError(QStringLiteral("Деление на ноль невозможно!"));
        return;
    }
    setResult(a / b);
}

void Calculator::sqrtOp(double a)
{
    qDebug() << "sqrt(" << a << ")";
    if (a < 0.0) {
        setError(QStringLiteral("Квадратный корень из отрицательного числа не определён!"));
        return;
    }
    setResult(std::sqrt(a));
}

void Calculator::sumList(const QList<double> &values)
{
    qDebug() << "sumList(size =" << values.size() << ")";
    if (values.isEmpty()) {
        setError(QStringLiteral("Список пуст — нечего суммировать!"));
        return;
    }
    double sum = 0.0;
    for (double v : values) sum += v;
    setResult(sum);
}

void Calculator::reset()
{
    qDebug() << "reset()";
    m_result = 0.0;
    m_hasError = false;
    m_errorMessage.clear();
    emit resultReady(m_result);
}