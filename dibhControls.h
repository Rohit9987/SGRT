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

};



#endif  // DIBHCONTROLS
