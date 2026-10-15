#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>
#include <QList>

class Calculator : public QObject
{
    Q_OBJECT  // Обязательный макрос для работы МОС

public:
    explicit Calculator(QObject *parent = nullptr);

    // Геттеры текущего состояния
    double result() const { return m_result; }
    bool hasError() const { return m_hasError; }
    QString errorMessage() const { return m_errorMessage; }

public slots:
    // --- Базовые арифметические операции ---
    void add(double a, double b);
    void subtract(double a, double b);
    void multiply(double a, double b);
    void divide(double a, double b);

    // --- ВАРИАНТ 9: квадратный корень с проверкой отрицательных чисел ---
    void sqrtOp(double a);

    // --- ВАРИАНТ 15: сумма элементов QList ---
    void sumList(const QList<double> &values);

    // Сброс состояния
    void reset();

signals:
    void resultReady(double result);
    void errorOccurred(const QString &message);

private:
    double m_result;
    bool m_hasError;
    QString m_errorMessage;

    void setResult(double value);
    void setError(const QString &msg);
};

#endif // CALCULATOR_H