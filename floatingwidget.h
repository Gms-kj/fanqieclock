#ifndef FLOATINGWIDGET_H
#define FLOATINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class FloatingWidget : public QWidget {
    Q_OBJECT
public:
    explicit FloatingWidget(QWidget *parent = nullptr);
    void updateTime(int minutes, int seconds);

signals:
    void doubleClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *timeLabel;
    QPushButton *lockButton;
    bool isDragging;
    bool isLocked;
    QPoint dragPosition;
    void setupUI();
    void setupStyle();
    void toggleLock();
};

#endif // FLOATINGWIDGET_H 