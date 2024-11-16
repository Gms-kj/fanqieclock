#ifndef CIRCULARPROGRESSBAR_H
#define CIRCULARPROGRESSBAR_H

#include <QWidget>
#include <QColor>
#include <QPropertyAnimation>

class CircularProgressBar : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal currentValue READ currentValue WRITE setCurrentValue)

public:
    explicit CircularProgressBar(QWidget *parent = nullptr);
    void setValue(int value);
    void setColor(const QColor& color);
    void setTime(int minutes, int seconds);
    
    qreal currentValue() const { return m_currentValue; }
    void setCurrentValue(qreal value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int m_targetValue;
    qreal m_currentValue;
    QColor m_color;
    int m_size;
    int m_minutes;
    int m_seconds;
    QPropertyAnimation* m_animation;
    
    void updateSize();
    void startAnimation(int endValue);
};

#endif // CIRCULARPROGRESSBAR_H 