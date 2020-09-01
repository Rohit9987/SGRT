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
    viewport()->setCursor(Qt::CrossCursor);

}
void GraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}


void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    qDebug() << QString::number(event->pos().x()) 
            << " "
            << QString::number(event->pos().y());
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    qDebug() << QString::number(event->pos().x()) 
            << " "
            << QString::number(event->pos().y());
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    qDebug() << QString::number(event->pos().x()) 
            << " "
            << QString::number(event->pos().y());
}
