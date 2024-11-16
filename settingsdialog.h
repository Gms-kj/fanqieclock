#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    
    int getWorkDuration() const;
    int getShortBreakDuration() const;
    int getLongBreakDuration() const;
    int getTotalRounds() const;
    
    void setWorkDuration(int minutes);
    void setShortBreakDuration(int minutes);
    void setLongBreakDuration(int minutes);
    void setTotalRounds(int rounds);

private:
    QSpinBox *workDurationSpinBox;
    QSpinBox *shortBreakSpinBox;
    QSpinBox *longBreakSpinBox;
    QSpinBox *totalRoundsSpinBox;
    
    void setupUI();
    void setupStyle();
};

#endif // SETTINGSDIALOG_H 