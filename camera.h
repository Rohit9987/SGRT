#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QImage>
#include <QMutex>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"

using namespace std;

class Camera : public QObject
{
	Q_OBJECT
public:
	explicit Camera(QMutex *lock);

	void read_camera();
	
signals:
	void send_videoSignal(cv::Mat *);

private:
	bool running;
	int camera;
	cv::CascadeClassifier *haarClassifier;
	cv::CascadeClassifier *lbpClassifier;
	cv::Ptr<cv::face::Facemark> mark_detector;

    QMutex *data_lock;
	void detectFaces(cv::Mat& frame);
    void poseEstimation(cv::Mat &frame, vector<cv::Point2f> &shapes);

};
#endif
