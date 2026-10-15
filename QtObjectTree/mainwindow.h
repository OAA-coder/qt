#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBoxLayout>
#include <QPointer>
#include <QWidget>

#include "widgets.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void moveMovableWidget();     // демонстрация setParent()
    void deletePanel();           // deleteLater() на панели с детьми
    void demoDetaching();         // setParent(nullptr) + deleteLater

private:
    void setupUi();
    void setStatus(const QString &text);

    QPointer<QBoxLayout> m_rootLayout;    // главный layout окна
    QPointer<QBoxLayout> m_panelLayout;   // layout внутри панели

    QPointer<LogLabel>   m_title;
    QPointer<LogLineEdit> m_input;
    QPointer<LogLabel>   m_status;
    QPointer<LogPanel>   m_panel;
    QPointer<LogButton>  m_movable;
    QPointer<LogButton>  m_moveButton;
    QPointer<LogButton>  m_deleteButton;
};

#endif // MAINWINDOW_H