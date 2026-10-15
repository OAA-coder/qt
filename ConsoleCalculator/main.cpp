#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include <QStringConverter>

#ifdef Q_OS_WIN
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#endif

#include "calculator.h"

void printHelp(QTextStream &out)
{
    out << "Доступные команды:\n";
    out << "  add <a> <b>        - сложение\n";
    out << "  sub <a> <b>        - вычитание\n";
    out << "  mul <a> <b>        - умножение\n";
    out << "  div <a> <b>        - деление\n";
    out << "  sqrt <a>           - квадратный корень (вариант 9)\n";
    out << "  sum <a> <b> ...    - сумма списка чисел (вариант 15)\n";
    out << "  reset              - сброс\n";
    out << "  help               - эта справка\n";
    out << "  quit               - выход\n";
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    // Переключаем консоль Windows в UTF-8, чтобы русский текст
    // в QTextStream не превращался в кракозябры.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QCoreApplication app(argc, argv);

    QTextStream in(stdin);
    QTextStream out(stdout);
    in.setEncoding(QStringConverter::Utf8);
    out.setEncoding(QStringConverter::Utf8);

    Calculator calc;

    QObject::connect(&calc, &Calculator::resultReady,
                     [&out](double result) {
                         out << "Результат: " << result << "\n";
                         out.flush();
                     });
    QObject::connect(&calc, &Calculator::errorOccurred,
                     [&out](const QString &msg) {
                         out << "Ошибка: " << msg << "\n";
                         out.flush();
                     });

    out << "=== Консольный калькулятор на Qt ===\n";
    printHelp(out);
    out << "\n> ";
    out.flush();

    QString line;
    while (in.readLineInto(&line)) {
        line = line.trimmed();
        if (line.isEmpty()) {
            out << "> ";
            out.flush();
            continue;
        }

        const QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        const QString command = parts.value(0).toLower();

        if (command == "quit" || command == "exit") {
            out << "До свидания!\n";
            break;
        }
        if (command == "help") {
            printHelp(out);
            out << "> ";
            out.flush();
            continue;
        }
        if (command == "reset") {
            calc.reset();
            out << "> ";
            out.flush();
            continue;
        }

        // --- ВАРИАНТ 9: sqrt <a> — ровно 2 токена ---
        if (command == "sqrt") {
            if (parts.size() != 2) {
                out << "Ошибка: неверный формат. Используйте: sqrt <a>\n> ";
                out.flush();
                continue;
            }
            bool ok = false;
            double a = parts[1].toDouble(&ok);
            if (!ok) {
                out << "Ошибка: не удалось преобразовать аргумент в число\n> ";
                out.flush();
                continue;
            }
            calc.sqrtOp(a);
            out << "> ";
            out.flush();
            continue;
        }

        // --- ВАРИАНТ 15: sum <a> <b> ... — минимум 3 токена ---
        if (command == "sum") {
            if (parts.size() < 3) {
                out << "Ошибка: неверный формат. Используйте: sum <a> <b> ...\n> ";
                out.flush();
                continue;
            }
            QList<double> values;
            bool allOk = true;
            for (int i = 1; i < parts.size(); ++i) {
                bool ok = false;
                double v = parts[i].toDouble(&ok);
                if (!ok) { allOk = false; break; }
                values.append(v);
            }
            if (!allOk) {
                out << "Ошибка: не удалось преобразовать одно из чисел\n> ";
                out.flush();
                continue;
            }
            calc.sumList(values);
            out << "> ";
            out.flush();
            continue;
        }

        // --- Базовые арифметические команды: ровно 3 токена ---
        if (parts.size() != 3) {
            out << "Ошибка: неверный формат. Используйте: <команда> <a> <b>\n> ";
            out.flush();
            continue;
        }

        bool ok1 = false, ok2 = false;
        const double a = parts[1].toDouble(&ok1);
        const double b = parts[2].toDouble(&ok2);
        if (!ok1 || !ok2) {
            out << "Ошибка: не удалось преобразовать операнды в числа\n> ";
            out.flush();
            continue;
        }

        if (command == "add")      calc.add(a, b);
        else if (command == "sub") calc.subtract(a, b);
        else if (command == "mul") calc.multiply(a, b);
        else if (command == "div") calc.divide(a, b);
        else out << "Неизвестная команда: " << command << "\n";

        out << "> ";
        out.flush();
    }

    return 0;
}