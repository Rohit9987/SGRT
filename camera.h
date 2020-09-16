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

    void receiveAreaPoints(QPointF p1, QPointF p2);
    
    void mouseReleased();

    void contourAreaSelected();
signals:
	void send_videoSignal(cv::Mat *);
    void send_maxMinHSV(int Hmin, int Hmax, int Smin, int Smax, int Vmin, int Vmax);

public slots:

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
    void objectDetection(cv::Mat& frame, cv::Mat& processedFrame);
    void drawContours(cv::Mat& frame);
    
    //TODO modify
    bool contour_area_selection;
    void writeFile(cv::Point point);
    QFile mFile;    
    QString *filename;
    int n;
    void getMaxMinHSV(cv::Mat& croppedFrame);
    
    //draw cv::Rect
    cv::Point point1, point2;
    bool draw_area_rect, mouse_released;
    cv::Rect roi;
};
#endif
