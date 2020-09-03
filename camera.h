#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QFile>
#include <QTextStream>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;

class Camera : public QObject
{
	Q_OBJECT
public:
	explicit Camera(QMutex *lock);

	void read_camera();
	
    void hsvChanged(int lowHValue, int lowSValue, int lowVValue,
            int highHValue, int highSValue, int highVValue);
signals:
	void send_videoSignal(cv::Mat *);

public slots:
    void receiveAreaPoints(QPointF p1, QPointF p2);

private:
	bool running;
	int camera;
	cv::CascadeClassifier *haarClassifier;
	cv::CascadeClassifier *lbpClassifier;
	cv::Ptr<cv::face::Facemark> mark_detector;

    QMutex *data_lock;
	void detectFaces(cv::Mat& frame);
    void poseEstimation(cv::Mat &frame, vector<cv::Point2f> &shapes);

    //object detection
    int lowH, lowS, lowV, highH, highS, highV;
    void objectDetection(cv::Mat& frame);
    void drawContours(cv::Mat& frame);
    
    //TODO modify
    void writeFile(cv::Point point);
    QFile mFile;    
    QString *filename;
    int n;
};
#endif
