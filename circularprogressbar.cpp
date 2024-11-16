#include "circularprogressbar.h"
#include <QPainter>
#include <QResizeEvent>
#include <QFont>

CircularProgressBar::CircularProgressBar(QWidget *parent)
    : QWidget(parent), m_targetValue(100), m_currentValue(100),
      m_color(QColor(74, 158, 255)), m_size(200), 
      m_minutes(25), m_seconds(0) {
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumSize(m_size, m_size);
    setMaximumSize(m_size, m_size);

    // 初始化动画
    m_animation = new QPropertyAnimation(this, "currentValue", this);
    m_animation->setDuration(300);  // 改回300ms的动画时长
    m_animation->setEasingCurve(QEasingCurve::OutCubic);  // 改回缓出曲线
}

void CircularProgressBar::setValue(int value) {
    if (m_targetValue != value) {
        m_targetValue = qBound(0, value, 100);
        startAnimation(m_targetValue);  // 使用动画过渡
    }
}

void CircularProgressBar::setCurrentValue(qreal value) {
    m_currentValue = value;
    update();
}

void CircularProgressBar::startAnimation(int endValue) {
    m_animation->stop();
    m_animation->setStartValue(m_currentValue);
    m_animation->setEndValue(endValue);
    m_animation->start();
}

void CircularProgressBar::setColor(const QColor& color) {
    if (m_color != color) {
        m_color = color;
        update();
    }
}

void CircularProgressBar::setTime(int minutes, int seconds) {
    m_minutes = minutes;
    m_seconds = seconds;
    update();
}

void CircularProgressBar::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateSize();
}

void CircularProgressBar::updateSize() {
    m_size = qMin(width(), height());
}

void CircularProgressBar::paintEvent(QPaintEvent *) {
    if (!isVisible()) return;
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 计算绘制区域
    const int margin = 10;
    const int ringWidth = 8;
    QRectF rect(margin, margin, width() - 2*margin, height() - 2*margin);

    // 绘制背景圆环
    QPen pen(QColor(233, 236, 239));
    pen.setWidth(ringWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawEllipse(rect);

    // 绘制进度圆环
    pen.setColor(m_color);
    painter.setPen(pen);
    
    int startAngle = 90 * 16;
    int spanAngle = -m_currentValue * 360 * 16 / 100;  // 使用当前动画值
    painter.drawArc(rect, startAngle, spanAngle);

    // 绘制时间文本
    QString timeText = QString("%1:%2")
        .arg(m_minutes, 2, 10, QChar('0'))
        .arg(m_seconds, 2, 10, QChar('0'));

    // 设置字体
    QFont font = painter.font();
    font.setPointSize(40);
    font.setBold(true);
    painter.setFont(font);

    // 计算文本矩形
    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(timeText);
    
    // 居中绘制时间
    painter.setPen(QColor(33, 37, 41));
    painter.drawText(rect, Qt::AlignCenter, timeText);
} 