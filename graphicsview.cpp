#include "graphicsview.h"
#include <QMouseEvent>

GraphicsView::GraphicsView(QGraphicsScene *w): QGraphicsView(w)
{
    mouseDown = false;
    contour_capture = false;
    area_capture = false;
}

GraphicsView::~GraphicsView()
{}

void GraphicsView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    if(area_capture || contour_capture)
        viewport()->setCursor(Qt::CrossCursor);
    else
        viewport()->setCursor(Qt::ArrowCursor);
}

void GraphicsView::setAreaCapture()
{
    area_capture = !area_capture;
}

void GraphicsView::setContourAreaCapture()
{
    contour_capture = !contour_capture;     
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if(area_capture || contour_capture)
    {
        p2 = event->pos();
        if(p1.x() != p2.x() || p1.y() != p2.y())
        {
            QPointF p3, p4;
            p3 = mapToScene(p1);
            p4 = mapToScene(p2);
            //TODO check if p2 or p1 exceeds scene width to avoid crash
            emit sendAreapoints(p3, p4);
        }
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if(area_capture || contour_capture)
    {
        p1 = event->pos();
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if(area_capture || contour_capture)
    {
        p2 = event->pos();
        viewport()->setCursor(Qt::ArrowCursor);
        if(area_capture)
            emit areaSetSignal(); 
        else
            emit contourAreaSignal();
        area_capture = false;
        contour_capture = false;
    }
}

