#include "settingsdialog.h"
#include <QFormLayout>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("设置");
    setupUI();
    setupStyle();
}

void SettingsDialog::setupUI() {
    auto *layout = new QFormLayout(this);
    
    workDurationSpinBox = new QSpinBox(this);
    workDurationSpinBox->setRange(1, 120);
    workDurationSpinBox->setValue(25);
    
    shortBreakSpinBox = new QSpinBox(this);
    shortBreakSpinBox->setRange(1, 30);
    shortBreakSpinBox->setValue(5);
    
    longBreakSpinBox = new QSpinBox(this);
    longBreakSpinBox->setRange(1, 60);
    longBreakSpinBox->setValue(15);
    
    totalRoundsSpinBox = new QSpinBox(this);
    totalRoundsSpinBox->setRange(1, 10);
    totalRoundsSpinBox->setValue(4);
    
    layout->addRow("工作时长（分钟）:", workDurationSpinBox);
    layout->addRow("短休息时长（分钟）:", shortBreakSpinBox);
    layout->addRow("长休息时长（分钟）:", longBreakSpinBox);
    layout->addRow("工作轮次:", totalRoundsSpinBox);
    
    auto *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    
    layout->addRow(buttonBox);
}

void SettingsDialog::setupStyle() {
    setStyleSheet(R"(
        QDialog {
            background-color: white;
            border-radius: 10px;
        }
        QSpinBox {
            padding: 5px;
            border: 2px solid #e9ecef;
            border-radius: 5px;
            min-width: 80px;
        }
        QPushButton {
            padding: 8px 16px;
            border: none;
            border-radius: 5px;
            background-color: #4a9eff;
            color: white;
        }
        QPushButton:hover {
            background-color: #3a8eef;
        }
    )");
}

// Getters
int SettingsDialog::getWorkDuration() const {
    return workDurationSpinBox->value();
}

int SettingsDialog::getShortBreakDuration() const {
    return shortBreakSpinBox->value();
}

int SettingsDialog::getLongBreakDuration() const {
    return longBreakSpinBox->value();
}

int SettingsDialog::getTotalRounds() const {
    return totalRoundsSpinBox->value();
}

// Setters
void SettingsDialog::setWorkDuration(int minutes) {
    workDurationSpinBox->setValue(minutes);
}

void SettingsDialog::setShortBreakDuration(int minutes) {
    shortBreakSpinBox->setValue(minutes);
}

void SettingsDialog::setLongBreakDuration(int minutes) {
    longBreakSpinBox->setValue(minutes);
}

void SettingsDialog::setTotalRounds(int rounds) {
    totalRoundsSpinBox->setValue(rounds);
} 