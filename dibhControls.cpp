#include <QDebug>

#include "dibhControls.h"

dibhControls::dibhControls(QWidget *parent): QDialog(parent)
{
    resize(250, 400);

    QGridLayout *main_layout = new QGridLayout(this);

    lowHueLabel = new QLabel("Low Hue", this);
    main_layout->addWidget(lowHueLabel, 0, 0, 1, 1, Qt::AlignLeft);  
    lowH = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(lowH, 0, 1, 1, 4, Qt::AlignLeft);  

    lowSaturationLabel = new QLabel("Low Saturation", this);
    main_layout->addWidget(lowSaturationLabel, 1, 0, 1, 1, Qt::AlignLeft);  
    lowS = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(lowS, 1, 1, 1, 4, Qt::AlignLeft);  

    lowValueLabel = new QLabel("low Value", this);
    main_layout->addWidget(lowValueLabel, 2, 0, 1, 1, Qt::AlignLeft);  
    lowV = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(lowV, 2, 1, 1, 4, Qt::AlignLeft);  

    highHueLabel= new QLabel("high Hue", this);
    main_layout->addWidget(highHueLabel, 3, 0, 1, 1, Qt::AlignLeft);  
    highH = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(highH, 3, 1, 1, 4, Qt::AlignLeft);  

    highSaturationLabel = new QLabel("high Saturation", this);
    main_layout->addWidget(highSaturationLabel, 4, 0, 1, 1, Qt::AlignLeft);  
    highS = new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(highS, 4, 1, 1, 4, Qt::AlignLeft);  

    highValueLabel = new QLabel("high Value", this);
    main_layout->addWidget(highValueLabel, 5, 0, 1, 1, Qt::AlignLeft);  
    highV= new QSlider(Qt::Horizontal, this);
    main_layout->addWidget(highV, 5, 1, 1, 4, Qt::AlignLeft);  
}
