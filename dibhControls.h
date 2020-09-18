#ifndef DIBHCONTROLS_H
#define DIBHCONTROLS_H

#include <QObject>
#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>

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
    QPushButton *selectHSVRegion, *selectContourRegion;

    //Color B&W modes
    QRadioButton *colorRadioButton, *bwRadioButton;

private slots:
    void hsv_changed();
    void selectRegion();
    void contourRegion();
    void colorScheme(bool);

public slots:
    void areaSet();
    void recthsvChanged(int Hmin, int Hmax, int Smin, int Smax, int Vmin, int Vmax);
    void contourAreaSet();

signals:
    void hsvChanged(int lowH, int lowS, int lowV,
            int highH, int highS, int highV);
    void sendSelectRegion();
    void sendContourRegion();
    void sendColorScheme(bool);
};

#endif  // DIBHCONTROLS
