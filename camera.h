#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QImage>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"

using namespace std;

class Camera : public QObject
{
	Q_OBJECT
public:
	explicit Camera(QObject *parent = nullptr);

	void read_camera();
	
signals:
	void send_videoSignal(cv::Mat *);

private:
	bool running;
	int camera;
	cv::CascadeClassifier *haarClassifier;
	cv::CascadeClassifier *lbpClassfier;
	cv::Ptr<cv::face::Facemark> mark_detector;

	//void detectFaces();
	//void poseEstimation(cv::Mat &frame, vector<cv::Point2f> &shapes);

};
#endif
