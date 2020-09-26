#include "camera.h"

#include <QDebug>
#include <QApplication>

//TODO
//the contours take up memory and slows down the process
//use a smaller roi to set the region for contour detection

Camera::Camera(QMutex *lock): data_lock(lock)
{
	camera = 0;
//    lbpClassifier = new cv::CascadeClassifier(OPENCV_LBP_DIR "lbpcascade_frontalface.xml");
//    haarClassifier = new cv::CascadeClassifier(OPENCV_DATA_DIR "haarcascade_frontalface_default.xml");
//    mark_detector = cv::face::createFacemarkLBF();
//    QString model_data = QApplication::instance()->applicationDirPath() +"/data/lbfmodel.yaml";
//    mark_detector->loadModel(model_data.toStdString());

    lowH = 50; lowS = 40; lowV = 70;
    highH = 255; highS = 255; highV = 255;

    filename = nullptr;
    n = 0;

    draw_area_rect = false;
    mouse_released = false;
    contour_area_selection = false;

	color = false;
}

void Camera::read_camera()
{
	running = true;
	cv::VideoCapture cap(camera);
	cv::Mat frame, frame_send;

    int width, height;
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    point1.x = 0;
    point1.y = 0;
    point2.x = width;
    point2.y = height;
    roi = cv::Rect(point1, point2);
	//load face detection here

	while(running)
	{
		cap >> frame;
		if(frame.empty())
			running = false;

//		cvtColor(frame, frame, cv::COLOR_BGR2RGB);
//        detectFaces(frame);
        if(draw_area_rect)
        {
            calculateHSV(frame);
        }

		data_lock->lock();
			objectDetection(frame, frame_send);
		data_lock->unlock();
		
        emit send_videoSignal(&frame_send);
    }

    cap.release();
	running = false;
}

void Camera::calculateHSV(cv::Mat& frame)
{
            cv::Rect hsv_area= cv::Rect(point1, point2);
            cv::rectangle(frame, hsv_area, cv::Scalar(0, 150, 20));

            if(mouse_released && !contour_area_selection)
            {
                cv::Mat croppedFrame = frame(hsv_area);
                cv::Mat hsvImage;
                cv::cvtColor(croppedFrame, hsvImage, cv::COLOR_BGR2HSV);
                cv::GaussianBlur(hsvImage, hsvImage, cv::Size(5, 5), 0);
                cv::dilate(hsvImage, hsvImage, 0);
                cv::erode(hsvImage, hsvImage, 0);
                getMaxMinHSV(hsvImage);
                mouse_released = false;
                draw_area_rect= false;
            }
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

void Camera::objectDetection(cv::Mat& frame, cv::Mat& processedFrame)
{
   	cv::Mat hsvImage, threshImage;
    //these number have to be input from the mainwindow options

    cv::cvtColor(frame, hsvImage, cv::COLOR_BGR2HSV);
    cv::GaussianBlur(hsvImage, hsvImage, cv::Size(5, 5), 0);
    cv::inRange(hsvImage, cv::Scalar(lowH, lowS, lowV),
            cv::Scalar(highH, highS, highV), threshImage);

    //blur, dilate and erode
    cv::GaussianBlur(threshImage, threshImage, cv::Size(5, 5), 0);
    cv::dilate(threshImage, threshImage, 0);
    cv::erode(threshImage, threshImage, 0);
    drawContours(frame, threshImage);
    if(color)
    {
        cvtColor(frame, processedFrame, cv::COLOR_BGR2RGB);
        return;
    }
    cv::cvtColor(threshImage, threshImage, cv::COLOR_GRAY2RGB);
    processedFrame = threshImage;
}

void Camera::hsvChanged(int lowHValue, int lowSValue, int lowVValue, int highHValue, int highSValue, int highVValue)
{
    lowH = lowHValue;
    lowS = lowSValue;
    lowV = lowVValue;
    highH = highHValue;
    highS = highSValue;
    highV = highVValue;
}

// TODO Need to insert a cv::Rect
void Camera::drawContours(cv::Mat& frame, cv::Mat& bwframe)
{
    cv::Mat contour_frame;
    if(contour_area_selection)
    {
        roi = cv::Rect(point1, point2);
        contour_area_selection = false;
    }

    contour_frame = bwframe(roi);

    vector<vector<cv::Point>> contours;
    vector<cv::Vec4i> heirarchy;

    cv::findContours(contour_frame, contours, heirarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    //RETR_EXTERNAL
    //RETR_TREE
    //CHAIN_APPROX_TC89_L1
    //CHAIN_APPROX_TC89_KCOS
    //CHAIN_APPROX_SIMPLE
    //CHAIN_APPROX_NONE

    for(size_t i=0; i < contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(120, 0, 0);
        if(this->color)
            cv::drawContours(frame(roi), contours, (int) i, color, 2, cv::LINE_8, heirarchy, 0);
        else
            cv::drawContours(bwframe(roi), contours, (int) i, color, 2, cv::LINE_8, heirarchy, 0);
    }

    //TODO modify
   // if(n < 100000)
    //{
    //    for(size_t i =0; i < contours.size();i++)
    //    {
    //        for(size_t j = 0; j <contours[i].size(); j++)
    //        {
    //            //qDebug() << contours[i][j].x << ", " <<contours[i][j].y;
               // writeFile(contours[i][j]);

    //        }
    //    }
 //   }
 //   else
 //   {
 //       mFile.close();
 //       delete filename;
 //       filename = nullptr;
 //   }
}

    //TODO modify this code to detect contours
    //Let RTT select an area to detect the motion initially(simulation)
    //The same area can be retraced
void Camera::writeFile(cv::Point point)
{
    if(!filename)
        filename = new QString("/home/rohit/Desktop/SGRT/Surface guidance/outputfile");

    if(!mFile.isOpen())
    {
        mFile.setFileName(*filename);
        if(!mFile.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug() << "Could not open the file";
            return;
        }
    }

    QTextStream mStream(&mFile);
    mStream << QString::number(point.x) <<" " << QString::number(point.y) <<"\n";
    mFile.flush();
    n++;
}

void Camera::receiveAreaPoints(QPointF p1, QPointF p2)
{
    point1.x = p1.x();
    point1.y = p1.y();
    point2.x = p2.x();
    point2.y = p2.y();
    draw_area_rect = true;
}

void Camera::mouseReleased()
{
    mouse_released = true;
}

void Camera::contourAreaSelected()
{
    contour_area_selection = true;
}

void Camera::getMaxMinHSV(cv::Mat& croppedFrame)
{
    std::vector<int> H_ROI, S_ROI, V_ROI;
    int Hmin = 0,
        Smin = 0,
        Vmin = 0,
        Hmax = 255,
        Smax = 255,
        Vmax = 255;

    if(croppedFrame.rows >0 && croppedFrame.cols >0)
    {
        for(int i =0; i< croppedFrame.rows; i++)
        {
            for(int j =0; j <croppedFrame.cols; j++)
            {
                H_ROI.push_back((int) croppedFrame.at<cv::Vec3b>(j,i)[0]);
                S_ROI.push_back((int) croppedFrame.at<cv::Vec3b>(j,i)[1]);
                V_ROI.push_back((int) croppedFrame.at<cv::Vec3b>(j,i)[2]);
            }
        }

        Hmin = *std::min_element(H_ROI.begin(), H_ROI.end());
        Hmax = *std::max_element(H_ROI.begin(), H_ROI.end());

        Smin = *std::min_element(S_ROI.begin(), S_ROI.end());
        Smax = *std::max_element(S_ROI.begin(), S_ROI.end());

        Vmin = *std::min_element(V_ROI.begin(), V_ROI.end());
        Vmax = *std::max_element(V_ROI.begin(), V_ROI.end());

        emit send_maxMinHSV(Hmin, Hmax, Smin, Smax, Vmin, Vmax);
    }
}

void Camera::colorScheme(bool colorScheme)
{
	color = colorScheme;
}
