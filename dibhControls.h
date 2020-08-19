#ifndef DIBHCONTROLS_H
#define DIBHCONTROLS_H

#include <QObject>
#include<QDialog>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>

class dibhControls: public QDialog
{
    Q_OBJECT

public:
        explicit dibhControls(QWidget *parent = nullptr);

private:
    QSlider *lowH, *lowS, *lowV, *highH, *highS, *highV;
    QLabel *lowHueLabel, *lowSaturationLabel, *lowValueLabel,
           *highHueLabel, *highSaturationLabel, *highValueLabel;
    int lowHValue, lowSValue, lowVValue, highHValue, highSValue, highVValue;

private slots:
    void hsv_changed();

signals:
    void hsvChanged(int lowH, int lowS, int lowV,
            int highH, int highS, int highV);
};



#endif  // DIBHCONTROLS
