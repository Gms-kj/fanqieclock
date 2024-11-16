#include "floatingwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>

FloatingWidget::FloatingWidget(QWidget *parent)
    : QWidget(parent), isDragging(false), isLocked(false) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupUI();
    setupStyle();
    resize(120, 50);  // 增大尺寸
    
    // 添加阴影效果
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 2);
    setGraphicsEffect(shadow);
}

void FloatingWidget::setupUI() {
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(15, 8, 15, 8);
    layout->setSpacing(8);
    
    timeLabel = new QLabel("25:00", this);
    timeLabel->setAlignment(Qt::AlignCenter);
    
    lockButton = new QPushButton(this);
    lockButton->setFixedSize(24, 24);
    lockButton->setCursor(Qt::PointingHandCursor);
    lockButton->setIcon(QIcon(":/images/unlock.png"));  // 需要添加图标
    lockButton->hide();  // 初始隐藏
    
    connect(lockButton, &QPushButton::clicked, this, &FloatingWidget::toggleLock);
    
    layout->addWidget(timeLabel);
    layout->addWidget(lockButton);
}

void FloatingWidget::setupStyle() {
    setStyleSheet(R"(
        FloatingWidget {
            background-color: rgba(255, 255, 255, 0.95);
            border: 1px solid #e1e1e1;
            border-radius: 8px;
        }
        QLabel {
            color: #333333;
            font-size: 20px;  /* 增大字体 */
            font-weight: bold;
        }
        QPushButton {
            background-color: transparent;
            border: none;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: rgba(0, 0, 0, 0.05);
        }
    )");
}

void FloatingWidget::toggleLock() {
    isLocked = !isLocked;
    lockButton->setIcon(QIcon(isLocked ? ":/images/lock.png" : ":/images/unlock.png"));
    setCursor(isLocked ? Qt::ArrowCursor : Qt::SizeAllCursor);
}

void FloatingWidget::updateTime(int minutes, int seconds) {
    timeLabel->setText(QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void FloatingWidget::mousePressEvent(QMouseEvent *event) {
    if (!isLocked && event->button() == Qt::LeftButton) {
        isDragging = true;
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }
}

void FloatingWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!isLocked && isDragging) {
        move(event->globalPosition().toPoint() - dragPosition);
    }
}

void FloatingWidget::mouseReleaseEvent(QMouseEvent *) {
    isDragging = false;
}

void FloatingWidget::enterEvent(QEnterEvent *) {
    lockButton->show();
}

void FloatingWidget::leaveEvent(QEvent *) {
    if (!lockButton->underMouse()) {
        lockButton->hide();
    }
}

void FloatingWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 设置透明背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 242));
    
    // 使用 QPainterPath 创建圆角矩形路径
    QPainterPath path;
    path.addRoundedRect(rect(), 8, 8);
    
    // 设置组合模式，使背景外的区域完全透明
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillPath(path, QColor(255, 255, 255, 242));
}

void FloatingWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked();
    }
} 