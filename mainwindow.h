//
// Created by gms on 24-11-16.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPoint>
#include "circularprogressbar.h"
#include <QScreen>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include "floatingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onStartButtonClicked();
    void onResetButtonClicked();
    void updateTimer();
    void onModeChanged(int index);
    void onMinimizeClicked();
    void onCloseClicked();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMainWindow();
    void onFloatingWidgetDoubleClicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int remainingTime;  // 剩余时间（秒）
    int totalTime;      // 总时间（秒）
    bool isRunning;     // 计时器是否在运行
    QPixmap backgroundImage;
    bool isDragging;
    QPoint dragPosition;
    CircularProgressBar *circularProgress;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    FloatingWidget *floatingWidget;
    
    void updateDisplay();
    void setMode(int minutes);
    void setupStyle();
    void setupTrayIcon();
    void createTrayMenu();
    void updateTrayIcon();
};

#endif //MAINWINDOW_H
