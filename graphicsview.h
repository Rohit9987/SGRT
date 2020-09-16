#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H 

#include <QGraphicsView>
#include <QObject>
#include <QGraphicsScene>
#include <QDebug>
#include <QEvent>

class GraphicsView:public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsView(QGraphicsScene *w);
    ~GraphicsView();

protected:
    void enterEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    //void closeMe;
    //void confirmCapture;

public slots:
    void setAreaCapture();    
    void setContourAreaCapture();

signals:
    void areaSetSignal();
    void sendAreapoints(QPointF p1, QPointF p2); 
    void contourAreaSignal();

private:
    QPoint p1, p2;
    bool mouseDown, area_capture, contour_capture;
};
 #endif  //GRAPHICSVIEW_H 
