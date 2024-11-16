//
// Created by gms on 24-11-16.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyle>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include "circularprogressbar.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include "floatingwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow), 
    remainingTime(25 * 60), totalTime(25 * 60), 
    isRunning(false), isDragging(false),
    trayIcon(nullptr), trayMenu(nullptr), floatingWidget(nullptr) {
    ui->setupUi(this);
    
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // 将窗口移动到屏幕中央
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
    // 初始化计时器
    timer = new QTimer(this);
    timer->setInterval(1000);
    
    // 创建并设置圆形��度条
    circularProgress = new CircularProgressBar(this);
    auto* progressLayout = new QVBoxLayout(ui->progressWidget);
    progressLayout->addWidget(circularProgress, 0, Qt::AlignCenter);
    progressLayout->setContentsMargins(0, 0, 0, 0);
    
    // 连接信号和槽
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
    connect(ui->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onModeChanged);
    connect(ui->minimizeButton, &QPushButton::clicked, this, &MainWindow::onMinimizeClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseClicked);
    
    setupStyle();
    
    // 初始化系统托盘
    setupTrayIcon();
    createTrayMenu();
    
    // 创建悬浮窗并连接双击信号
    floatingWidget = new FloatingWidget;
    connect(floatingWidget, &FloatingWidget::doubleClicked,
            this, &MainWindow::onFloatingWidgetDoubleClicked);
    
    updateDisplay();
}

void MainWindow::setupStyle() {
    // 添加窗口阴影
    auto* windowShadow = new QGraphicsDropShadowEffect(this);
    windowShadow->setBlurRadius(20);
    windowShadow->setColor(QColor(0, 0, 0, 50));
    windowShadow->setOffset(0, 0);
    setGraphicsEffect(windowShadow);
    
    // 更新样式表
    setStyleSheet(R"(
        QWidget#MainWindow {
            background-color: white;
            border-radius: 10px;
            border: 1px solid #e1e1e1;
        }
        QWidget#titleBar {
            background-color: #ffffff;
            border-top-left-radius: 10px;
            border-top-right-radius: 10px;
            border-bottom: 1px solid #e1e1e1;
        }
        QLabel#iconLabel {
            font-size: 16px;
        }
        QLabel#titleLabel {
            color: #333333;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton#minimizeButton, QPushButton#closeButton {
            background-color: transparent;
            border-radius: 0px;
            color: #666666;
            font-family: "Segoe UI", Arial;
            font-size: 16px;
            font-weight: bold;
            padding: 0;
            margin: 0;
            line-height: 30px;
        }
        QPushButton#minimizeButton {
            padding-bottom: 3px;
        }
        QPushButton#closeButton {
            padding-bottom: 1px;
            border-top-right-radius: 10px;
        }
        QPushButton#minimizeButton:hover {
            background-color: #f5f5f5;
            color: #333333;
        }
        QPushButton#closeButton:hover {
            background-color: #ff4a4a;
            color: white;
            border-top-right-radius: 10px;
        }
        QWidget#contentWidget {
            background-color: white;
            border-bottom-left-radius: 10px;
            border-bottom-right-radius: 10px;
        }
        QPushButton {
            background-color: #4a9eff;
            border: none;
            border-radius: 20px;
            padding: 10px;
            color: white;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3a8eef;
        }
        QPushButton:pressed {
            background-color: #2a7edf;
        }
        QProgressBar {
            border: 2px solid #e9ecef;
            border-radius: 5px;
            text-align: center;
            height: 10px;
            background-color: #f8f9fa;
        }
        QProgressBar::chunk {
            background-color: #4a9eff;
            border-radius: 3px;
        }
        QComboBox {
            padding: 5px 10px;
            border: 1px solid #e9ecef;
            border-radius: 5px;
            background-color: white;
            color: #495057;
            min-width: 180px;
            font-size: 12px;
        }
        QComboBox:hover {
            border-color: #4a9eff;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox::down-arrow {
            image: none;
        }
        QComboBox QAbstractItemView {
            border: 1px solid #e9ecef;
            background-color: white;
            selection-background-color: #f0f7ff;
            selection-color: #4a9eff;
        }
        QLabel {
            color: #495057;
        }
        QLabel#titleLabel {
            color: #495057;
            font-weight: bold;
        }
        QLabel#timeLabel {
            color: #212529;
            font-size: 48px;
            font-weight: bold;
        }
        QLabel#statusLabel {
            color: #495057;
            font-size: 16px;
            font-weight: bold;
        }
    )");
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制白色背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRoundedRect(rect(), 10, 10);
    
    QWidget::paintEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QRect titleBarRect = ui->titleBar->rect();
        titleBarRect.setWidth(width()); // 扩展标题栏的可拖动区域到整个窗口宽度
        if (titleBarRect.contains(event->pos())) {
            isDragging = true;
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        move(event->globalPosition().toPoint() - dragPosition);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    isDragging = false;
}

void MainWindow::onMinimizeClicked() {
    hide();
    if (!floatingWidget->isVisible()) {
        floatingWidget->show();
    }
}

void MainWindow::onCloseClicked() {
    close();
}

void MainWindow::onStartButtonClicked() {
    if (!isRunning) {
        timer->start();
        ui->startButton->setText("暂停");
        ui->startButton->setStyleSheet("background-color: #ff4a4a;");
    } else {
        timer->stop();
        ui->startButton->setText("继续");
        ui->startButton->setStyleSheet("background-color: #4a9eff;");
    }
    isRunning = !isRunning;
}

void MainWindow::onResetButtonClicked() {
    timer->stop();
    isRunning = false;
    remainingTime = totalTime;
    ui->startButton->setText("开始");
    ui->startButton->setStyleSheet("background-color: #4a9eff;");
    updateDisplay();
}

void MainWindow::updateTimer() {
    if (remainingTime > 0) {
        remainingTime--;
        updateDisplay();
    } else {
        timer->stop();
        isRunning = false;
        ui->startButton->setText("开始");
        ui->startButton->setStyleSheet("background-color: #4a9eff;");
    }
}

void MainWindow::updateDisplay() {
    if (!circularProgress) return;
    
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    
    // 更新圆形进度条
    int progress = (remainingTime * 100) / totalTime;
    circularProgress->setValue(progress);
    circularProgress->setTime(minutes, seconds);
    
    // 更新悬浮窗时间
    if (floatingWidget) {
        floatingWidget->updateTime(minutes, seconds);
    }
    
    // 更新托盘图标提示
    if (trayIcon) {
        QString status = isRunning ? "进行中" : "已暂停";
        QString mode;
        switch(ui->modeComboBox->currentIndex()) {
            case 0: mode = "工作时间"; break;
            case 1: mode = "短休息"; break;
            case 2: mode = "长休息"; break;
        }
        trayIcon->setToolTip(QString("专注时钟 - %1\n%2\n剩余时间：%3:%4")
            .arg(mode)
            .arg(status)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0')));
    }
}

void MainWindow::onModeChanged(int index) {
    switch(index) {
        case 0: // 工作时间
            setMode(25);
            break;
        case 1: // 短休息
            setMode(5);
            break;
        case 2: // 长休息
            setMode(15);
            break;
    }
}

void MainWindow::setMode(int minutes) {
    timer->stop();
    isRunning = false;
    totalTime = minutes * 60;
    remainingTime = totalTime;
    ui->startButton->setText("开始");
    ui->startButton->setStyleSheet("background-color: #4a9eff;");
    updateDisplay();
}

void MainWindow::setupTrayIcon() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        return;
    }
    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/clock.png"));  // 使用静态图标
    trayIcon->setToolTip("专注时钟");
    
    connect(trayIcon, &QSystemTrayIcon::activated, 
            this, &MainWindow::trayIconActivated);
            
    trayIcon->show();
}

void MainWindow::createTrayMenu() {
    trayMenu = new QMenu(this);
    
    QAction *showAction = new QAction("显示主窗口", this);
    QAction *toggleFloatingAction = new QAction("显示/隐藏悬浮窗", this);
    QAction *quitAction = new QAction("退出", this);
    
    connect(showAction, &QAction::triggered, this, &MainWindow::showMainWindow);
    connect(toggleFloatingAction, &QAction::triggered, [this]() {
        floatingWidget->setVisible(!floatingWidget->isVisible());
    });
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    trayMenu->addAction(showAction);
    trayMenu->addAction(toggleFloatingAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    
    trayIcon->setContextMenu(trayMenu);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        showMainWindow();
    }
}

void MainWindow::showMainWindow() {
    show();
    raise();
    activateWindow();
    if (floatingWidget) {
        floatingWidget->hide();  // 确保悬浮窗被隐藏
    }
}

void MainWindow::onFloatingWidgetDoubleClicked() {
    floatingWidget->hide();  // 隐藏悬浮窗
    showMainWindow();        // 显示主窗口
}

MainWindow::~MainWindow() {
    if (floatingWidget) {
        delete floatingWidget;
    }
    delete ui;
}
