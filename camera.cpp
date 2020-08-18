#include "camera.h"

#include <QDebug>
#include <QApplication>

Camera::Camera(QMutex *lock): data_lock(lock) 
{
	camera = 0;
    lbpClassifier = new cv::CascadeClassifier(OPENCV_LBP_DIR "lbpcascade_frontalface.xml");
    haarClassifier = new cv::CascadeClassifier(OPENCV_DATA_DIR "haarcascade_frontalface_default.xml");
    mark_detector = cv::face::createFacemarkLBF();
    QString model_data = QApplication::instance()->applicationDirPath() +"/data/lbfmodel.yaml";
    mark_detector->loadModel(model_data.toStdString());
}

void Camera::read_camera()
{
	running = true;
	cv::VideoCapture cap(camera);
	cv::Mat frame, frame_send;
	
	//load face detection here
	
	while(running)
	{
		cap >> frame;
		if(frame.empty())
			running = false;

		cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        detectFaces(frame);
        data_lock->lock();
        frame_send = frame;
        data_lock->unlock();
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

    vector<vector<cv::Point2f>> shapes;
    if(mark_detector->fit(frame, faces, shapes))
    {
        for(size_t i =0; i < faces.size(); i++)
        {
            for(size_t k = 0; k< shapes[i].size(); k++)
                cv::circle(frame, shapes[i][k], 2, color, cv::FILLED);
        poseEstimation(frame, shapes[i]);
        }
    }
}

void Camera::poseEstimation(cv::Mat& frame, vector<cv::Point2f>& shapes)
{
    std::vector<cv::Point2f> image_points;
    //nose tip
    image_points.push_back(shapes[30]);
    //left eye left corner
    image_points.push_back(shapes[8]);
    //chin
    image_points.push_back(shapes[36]);
    //right eye roght corner
    image_points.push_back(shapes[45]);
    //mouth left corner
    image_points.push_back(shapes[48]);
    //mouth right corner
    image_points.push_back(shapes[54]);

    std::vector<cv::Point3d> model_points;
    model_points.push_back(cv::Point3d( 0.0f, 0.0f, 0.0f));
    model_points.push_back(cv::Point3d( 0.0f, -330.0f, -65.0f));
    model_points.push_back(cv::Point3d( -225.0f, 170.0f, -135.0f));
    model_points.push_back(cv::Point3d( 225.0f, 170.0f, -135.0f));
    model_points.push_back(cv::Point3d( -150.0f, -150.0f, -125.0f));
    model_points.push_back(cv::Point3d( 150.0f, -150.0f, -125.0f));

    double focal_length = frame.cols;
    cv::Point2d center = cv::Point2d(frame.cols/2, frame.rows/2);
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<focal_length, 0,
        center.x, 0, focal_length, center.y, 0, 0, 1);
    //Assuming no lens distortion
    cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

    //solve for pose
    cv::Mat rotation_vector, translation_vector;
    cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs,
            rotation_vector, translation_vector);

    //drae a line sticking out of the nose
    vector<cv::Point3d> nose_end_point3D;
    vector<cv::Point2d> nose_end_point2D;
    nose_end_point3D.push_back(cv::Point3d(0, 0, 1000.0));

    cv::projectPoints(nose_end_point3D, rotation_vector, translation_vector,
            camera_matrix, dist_coeffs, nose_end_point2D);
    for(size_t i = 0; i<image_points.size(); i++)
        cv::circle(frame, image_points[i], 3, cv::Scalar( 0, 0, 255), -1);
    cv::line(frame, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);
}
