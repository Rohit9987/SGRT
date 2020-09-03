#include "graphicsview.h"
#include <QMouseEvent>

GraphicsView::GraphicsView(QGraphicsScene *w): QGraphicsView(w)
{
    mouseDown = false;
}

GraphicsView::~GraphicsView()
{}

void GraphicsView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    if(area_capture)
        viewport()->setCursor(Qt::CrossCursor);
    else
        viewport()->setCursor(Qt::ArrowCursor);
}

void GraphicsView::setAreaCapture()
{
    area_capture = !area_capture;
}
void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if(area_capture)
    {
        p2 = event->pos();
        qDebug() << p1.x() <<", " << p1.y() << "; "
            <<p2.x() <<", " << p2.y(); 
        QPointF p3, p4;
        qDebug() << p1.x() <<", " << p1.y() << "; "
            <<p2.x() <<", " << p2.y(); 

        p3 = mapToScene(p1);
        p4 = mapToScene(p2);
        qDebug() << p3.x() <<", " << p3.y() << "; "
            <<p4.x() <<", " << p4.y(); 
        emit sendAreapoints(p3, p4);
        //TODO convert points p1 and p2 from view to scene points
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if(area_capture)
    {
        p1 = event->pos();
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if(area_capture)
    {
        p2 = event->pos();
        area_capture = false;
        viewport()->setCursor(Qt::ArrowCursor);
        emit areaSetSignal(); 
    
    }
}
