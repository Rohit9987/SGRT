#ifndef DIBHCONTROLS_H
#define DIBHCONTROLS_H

#include <QObject>
#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>


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
    QPushButton *selectHSVRegion;

private slots:
    void hsv_changed();
    void selectRegion();

public slots:
    void areaSet();

signals:
    void hsvChanged(int lowH, int lowS, int lowV,
            int highH, int highS, int highV);
    void sendSelectRegion();
};



#endif  // DIBHCONTROLS
