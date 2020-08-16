#include "camera.h"

#include <QDebug>
#include <QApplication>

Camera::Camera(QObject *parent) : QObject(parent)
{
	camera = 0;
    lbpClassifier = new cv::CascadeClassifier(OPENCV_LBP_DIR "lbpcascade_frontalface.xml");
    haarClassifier = new cv::CascadeClassifier(OPENCV_DATA_DIR "haarcascade_frontalface_default.xml");

}

void Camera::read_camera()
{
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
        detectFaces(frame);
		emit send_videoSignal(&frame);
	}
	cap.release();
	running = false;
}

void Camera::detectFaces(cv::Mat& frame)
{
    vector<cv::Rect> faces;
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);


    lbpClassifier->detectMultiScale(gray_frame, faces, 1.3, 5);
    //haarClassifier->detectMultiScale(frame, faces, 1.3, 5);
    
    cv::Scalar color = cv::Scalar(0, 0, 255);
    for(size_t i = 0; i<faces.size(); i++)
       cv::rectangle(frame, faces[i], color, 1);
}
