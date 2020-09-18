#include <QDebug>
#include <QGroupBox>

#include "dibhControls.h"

dibhControls::dibhControls(QWidget *parent): QDialog(parent)
{
    resize(250, 400);

    lowHValue = 0;
    lowSValue = 0;
    lowVValue = 0;
    highHValue = 255;
    highSValue = 255;
    highVValue = 255;

    QGridLayout *main_layout = new QGridLayout(this);

    lowHueLabel = new QLabel("Low Hue", this);
    main_layout->addWidget(lowHueLabel, 0, 0, 1, 1, Qt::AlignLeft);
    lowH = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(lowH, 0, 1, 1, 4);
    lowH->setMaximum(255);
    connect(lowH, &QSlider::valueChanged, this, &dibhControls::hsv_changed);

    lowSaturationLabel = new QLabel("Low Saturation", this);
    main_layout->addWidget(lowSaturationLabel, 1, 0, 1, 1, Qt::AlignLeft);
    lowS = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(lowS, 1, 1, 1, 4);
    lowS->setMaximum(255);
    connect(lowS, &QSlider::valueChanged, this, &dibhControls::hsv_changed);

    lowValueLabel = new QLabel("low Value", this);
    main_layout->addWidget(lowValueLabel, 2, 0, 1, 1, Qt::AlignLeft);
    lowV = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(lowV, 2, 1, 1, 4);
    lowV->setMaximum(255);
    connect(lowV, &QSlider::valueChanged, this, &dibhControls::hsv_changed);

    highHueLabel= new QLabel("high Hue", this);
    main_layout->addWidget(highHueLabel, 3, 0, 1, 1, Qt::AlignLeft);
    highH = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(highH, 3, 1, 1, 4);
    highH->setMaximum(255);
    highH->setValue(255);
    connect(highH, &QSlider::valueChanged, this, &dibhControls::hsv_changed);

    highSaturationLabel = new QLabel("high Saturation", this);
    main_layout->addWidget(highSaturationLabel, 4, 0, 1, 1, Qt::AlignLeft);
    highS = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(highS, 4, 1, 1, 4);
    highS->setMaximum(255);
    highS->setValue(255);
    connect(highS, &QSlider::valueChanged, this, &dibhControls::hsv_changed);

    highValueLabel = new QLabel("high Value", this);
    main_layout->addWidget(highValueLabel, 5, 0, 1, 1, Qt::AlignLeft);
    highV= new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(highV, 5, 1, 1, 4);
    highV->setMaximum(255);
    highV->setValue(255);
    connect(highV, &QSlider::valueChanged, this, &dibhControls::hsv_changed);

    selectHSVRegion = new QPushButton("HSV Region",this);
    main_layout->addWidget(selectHSVRegion, 6, 0 , 1, 1, Qt::AlignLeft);
    connect(selectHSVRegion, &QPushButton::clicked, this, &dibhControls::selectRegion);

    selectContourRegion = new QPushButton("Contour Region", this);
    main_layout->addWidget(selectContourRegion, 6, 1, 1, 1, Qt::AlignLeft);
    connect(selectContourRegion, &QPushButton::clicked, this, &dibhControls::contourRegion);

    //TODO put a groupbox with color/black and white mode
    QGroupBox *videoModeBox = new QGroupBox("Video Mode", this);
    colorRadioButton = new QRadioButton("Color", this);
    bwRadioButton = new QRadioButton("B&W", this);
    bwRadioButton->setChecked(true);
    QGridLayout *video_mode_layout = new QGridLayout(videoModeBox);
    video_mode_layout->addWidget(colorRadioButton, 0, 0);
    video_mode_layout->addWidget(bwRadioButton, 0, 1);
    main_layout->addWidget(videoModeBox, 7, 0, 1, 5);
    connect(colorRadioButton, &QRadioButton::toggled, this, &dibhControls::colorScheme);
}

void dibhControls::hsv_changed()
{
    lowHValue = lowH->value();
    lowSValue = lowS->value();
    lowVValue = lowV->value();
    highHValue = highH->value();
    highSValue = highS->value();
    highVValue = highV->value();

    emit hsvChanged(lowHValue, lowSValue, lowVValue, highHValue, highSValue, highVValue);
}

void dibhControls::recthsvChanged(int Hmin, int Hmax, int Smin, int Smax, int Vmin, int Vmax)
{
    lowH->setValue(Hmin);
    highH->setValue(Hmax);
    lowS->setValue(Smin);
    highS->setValue(Smax);
    lowV->setValue(Vmin);
    highV->setValue(Vmax);

    emit hsv_changed();
        qDebug() << "Hue: " <<Hmin <<", " << Hmax
            << "Saturation: " <<Smin << ", " << Smax
            << "Value: " << Vmin << ", " << Vmax;
}

void dibhControls::selectRegion()
{
    if(selectHSVRegion->text() == "HSV Region")
    {
        if(selectContourRegion->text() == "Cancel")
        {
            emit sendContourRegion();
            selectContourRegion->setText("Contour Region");
        }
        selectHSVRegion->setText("Cancel");

    }
    else
        selectHSVRegion->setText("HSV Region");
    emit sendSelectRegion();
}

void dibhControls::areaSet()
{
    selectHSVRegion->setText("HSV Region");
}

void dibhControls::contourAreaSet()
{
    selectContourRegion->setText("Contour Region");
}

void dibhControls::contourRegion()
{
    if(selectContourRegion->text() == "Contour Region")
    {
        if(selectHSVRegion->text() == "Cancel")
        {
            emit sendSelectRegion();
            selectHSVRegion->setText("HSV Region");
        }
        selectContourRegion->setText("Cancel");
    }
    else
        selectContourRegion->setText("Contour Region");
    emit sendContourRegion();
}

void dibhControls::colorScheme(bool color)
{
    //Color = true --> video display in color
//    qDebug() << color;
    emit sendColorScheme(color);
}
