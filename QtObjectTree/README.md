# Qt Object Tree

Лабораторная работа №2. Дерево объектов QObject: владелец и потомки.

## Описание

GUI-приложение на Qt, демонстрирующее механизм **дерева объектов QObject**.
Каждый `QObject` может иметь одного родителя и произвольное число детей.
При уничтожении родителя все его потомки удаляются автоматически.

Программа показывает:
- создание виджетов с явным родителем;
- отслеживание жизненного цикла объектов через `qDebug()` в конструкторах и деструкторах;
- автоматическое удаление дочерних виджетов при удалении родителя;
- смену владельца виджета через `setParent()`;
- отвязку объекта через `setParent(nullptr)` и ручное удаление через `deleteLater()`.

## Возможности

- Главное окно с 5+ виджетами: `QLabel`, `QLineEdit`, `QPushButton`, `QFrame`, layouts
- Логирование создания и уничтожения через производные классы:
  - `LogLabel`
  - `LogLineEdit`
  - `LogButton`
  - `LogPanel`
- Кнопка **«Переместить виджет: панель <-> окно»** — демонстрация `setParent()`
- Кнопка **«Удалить панель (deleteLater)»** — демонстрация автоматического удаления детей
- Кнопка **«Демо setParent(nullptr)»** — демонстрация отвязки и ручного удаления

## Требования к окружению

- Компилятор: MinGW-w64 GCC 16.2.0 (MSYS2 UCRT64)
- Qt: 6.11.2
- CMake: 4.4.4
- Ninja: 1.13.2

## Сборка

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

## Запуск

```bash
# Windows
.\build\QtObjectTree.exe

# Linux/macOS
./build/QtObjectTree
```

## Управление

| Элемент | Действие |
|---------|----------|
| Поле ввода | Виджет-ребёнок главного окна |
| Кнопка «Переместить виджет: панель <-> окно» | Меняет родителя перемещаемого виджета через `setParent()` |
| Кнопка «Удалить панель (deleteLater)» | Удаляет панель; её дети (кроме перемещаемого) уничтожаются автоматически |
| Кнопка «Демо setParent(nullptr)» | Создаёт временную панель, отвязывает одного ребёнка через `setParent(nullptr)` и удаляет его вручную |

## Структура проекта

| Файл | Назначение |
|------|-----------|
| `main.cpp` | Точка входа, `QApplication`, UTF-8 message handler для консоли |
| `widgets.h` | Производные виджеты `LogLabel`, `LogLineEdit`, `LogButton`, `LogPanel` с логированием в `qDebug()` |
| `mainwindow.h` | Объявление главного окна `MainWindow` |
| `mainwindow.cpp` | Создание виджетов и слоты `moveMovableWidget()`, `deletePanel()`, `demoDetaching()` |
| `CMakeLists.txt` | Система сборки (`AUTOMOC ON`, `Qt6::Core` + `Qt6::Widgets`) |

## Пример работы

### Стартовый вывод в консоль

```
[CREATE] MainWindow
[CREATE] LogLabel "Дерево объектов Qt: владелец и потомки"
[CREATE] LogLineEdit
[CREATE] LogLabel "Статус: окно создано"
[CREATE] LogPanel "PanelOwner"
[CREATE] LogButton "Статическая кнопка внутри панели"
[CREATE] LogButton "Перемещаемый виджет (в панели)"
[CREATE] LogButton "Переместить виджет: панель <-> окно"
[CREATE] LogButton "Удалить панель (deleteLater)"
[CREATE] LogButton "Демо setParent(nullptr)"
```

### Нажатие «Удалить панель (deleteLater)»

Дети панели уничтожаются автоматически:

```
[DESTROY] LogPanel "PanelOwner"
[DESTROY] LogButton "Статическая кнопка внутри панели"
[DESTROY] LogButton "Перемещаемый виджет (в панели)"
```

### Нажатие «Переместить виджет: панель <-> окно»

Виджет меняет владельца:

```
[SET_PARENT] Новый владелец виджета: ""
```

### Нажатие «Демо setParent(nullptr)»

```
[CREATE] LogPanel "TempOwner"
[CREATE] LogLabel "Ребёнок TempOwner"
[CREATE] LogLabel "Отвязанный ребёнок TempOwner"
[DEMO] Количество детей tempOwner до detach: 2
[DEMO] Количество детей tempOwner после detach: 1
[DESTROY] LogLabel "Отвязанный ребёнок TempOwner"
[DESTROY] LogPanel "TempOwner"
[DESTROY] LogLabel "Ребёнок TempOwner"
```

### Закрытие главного окна

Каскадное удаление всех виджетов:

```
[DESTROY] MainWindow
[DESTROY] LogLabel "Дерево объектов Qt: владелец и потомки"
[DESTROY] LogLineEdit
[DESTROY] LogLabel "Статус: ..."
[DESTROY] LogButton "Переместить виджет: панель <-> окно"
[DESTROY] LogButton "Удалить панель (deleteLater)"
[DESTROY] LogButton "Демо setParent(nullptr)"
```

## Особенности реализации

- **Собственный обработчик сообщений Qt** (`qInstallMessageHandler`) пишет UTF-8-байты напрямую в `stdout` через WinAPI (`WriteConsoleA`), чтобы русский текст в `qDebug()` не превращался в кракозябры. Это чисто программное решение — не требует ручного `chcp` в терминале.
- **`SetConsoleOutputCP(CP_UTF8)`** вызывается в начале `main()` для корректной работы `stdin`/`stdout`.
- **`#ifndef NOMINMAX`** перед `#include <windows.h>` — MSYS2-заголовки GCC уже определяют этот макрос, повторное определение вызывало warning.
- **`QPointer<...>`** для всех полей-указателей — безопасная работа с объектами, которые могут быть удалены через `deleteLater()`.

## Лицензия

Учебный проект. Лабораторная работа №2 по дисциплине «Программирование на Qt».