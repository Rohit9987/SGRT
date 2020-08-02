#include "camera.h"

#include <QDebug>
#include <QApplication>

Camera::Camera(QObject *parent) : QObject(parent)
{
	camera = 0;
}

void Camera::read_camera()
{
	qDebug() << "Hello from read_camera";
	running = true;
	cv::VideoCapture cap(camera);
	cv::Mat frame;
	
	//load face detection here
	
	while(running)
	{
		cap >> frame;
		if(frame.empty())
			running = false;

		cvtColor(frame, frame, cv::COLOR_BGR2RGB);
		emit send_videoSignal(&frame);
	}
	cap.release();
	running = false;
}
