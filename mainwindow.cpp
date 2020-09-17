#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	initUI();
	createAction();
    data_lock = new QMutex();
    dibhWindow = nullptr;
    camera = nullptr;
}

MainWindow::~MainWindow()
{
    disconnect(camera, &Camera::send_videoSignal, this, &MainWindow::display_Video);
	cameraThread->exit();
}

void MainWindow::initUI()
{
	showMaximized();

	QWidget *widget = new QWidget(this);

	QGridLayout *main_layout = new QGridLayout(widget);

	imageScene = new QGraphicsScene(widget);
	imageView = new GraphicsView(imageScene);
	main_layout->addWidget(imageView, 0, 0, 12, 3);

	//____OPTIONS____LAYOUT____
	QGridLayout *options_layout = new QGridLayout();
	main_layout->addLayout(options_layout, 0 , 3, 12, 1);
	options_layout->setAlignment(Qt::AlignTop);
	QLabel *options = new QLabel(this);
	options->setText("_________OPTIONS__________");
	options_layout->addWidget(options, 0, 0, 1, 2, Qt::AlignTop | Qt::AlignCenter);
	//buttons to start camera and use CV
	cameraButton = new QPushButton("Start Camera", this);
	options_layout->addWidget(cameraButton, 1, 0, 1, 2, Qt::AlignCenter);

	detectButton = new QPushButton("Detect", this);
	options_layout->addWidget(detectButton, 2, 0, Qt::AlignCenter);

	modeComboBox = new QComboBox(this);
	modeComboBox->addItem(tr("Face Orientation"));
	modeComboBox->addItem(tr("DIBH"));
	modeComboBox->addItem(tr("Body Pose"));
	options_layout->addWidget(modeComboBox, 2, 1, Qt::AlignLeft);

	//____OUTPUT____LAYOUT____
	QGridLayout *output_layout = new QGridLayout();
	main_layout->addLayout(output_layout, 13, 0, 4, 1);


	QLabel *output = new QLabel(this);
	output->setText("OUTPUT:");
	output_layout->addWidget(output, 0, 0, Qt::AlignTop | Qt::AlignLeft);

	setCentralWidget(widget);
}


void MainWindow::createAction()
{
	fileMenu = menuBar()->addMenu("&File");
	cameraInfoAction = new QAction("Camera &Information", this);
	connect(cameraInfoAction, &QAction::triggered, this, &MainWindow::showCameraInfo);
	fileMenu->addAction(cameraInfoAction);

	openCameraAction = new QAction("Open &Camera", this);
	fileMenu->addAction(openCameraAction);
	connect(openCameraAction, &QAction::triggered, this, &MainWindow::openCamera);
	connect(cameraButton, &QPushButton::clicked, this, &MainWindow::openCamera);

	exitAction = new QAction("&Exit", this);
	fileMenu->addAction(exitAction);
	//connect(exitAction, &QAction::triggered, QApplication::instance(), &QCoreApplication::quit);
	connect(exitAction, &QAction::triggered, QApplication::instance(), &QCoreApplication::quit);

    connect(detectButton, &QPushButton::clicked, this, &MainWindow::showdibhWindow);
    connect(imageView, &GraphicsView::areaSetSignal, this, &MainWindow::mouseReleased);
    connect(imageView, &GraphicsView::contourAreaSignal, this, &MainWindow::receiveContourSlot);
}


void MainWindow::showCameraInfo()
{
	QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	QString cameraInfo = QString("Available camera: \n");

	foreach(const QCameraInfo& camera, cameras)
	{
		cameraInfo += "-" + camera.deviceName() + ":" + camera.description() + "\n";
	}
	QMessageBox::information(this, "Available Camera", cameraInfo);
}

void MainWindow::showdibhWindow()
{
    if(dibhWindow == nullptr)
    {
        dibhWindow = new dibhControls(this);
        connect(dibhWindow, &dibhControls::hsvChanged, this, &MainWindow::hsvChanged);
        connect(dibhWindow, &dibhControls::sendSelectRegion, imageView, &GraphicsView::setAreaCapture);
        connect(dibhWindow, &dibhControls::sendContourRegion, imageView, &GraphicsView::setContourAreaCapture);
        connect(imageView, &GraphicsView::areaSetSignal, dibhWindow, &dibhControls::areaSet);
        connect(imageView, &GraphicsView::contourAreaSignal, dibhWindow, &dibhControls::contourAreaSet);
    }
    dibhWindow->show();
}

void MainWindow::hsvChanged(int lowH, int lowS, int lowV,
            int highH, int highS, int highV)
{
    if(lowH < highH || lowS < highS || lowV < highV)
        camera->hsvChanged(lowH, lowS, lowV, highH, highS, highV);
}
void MainWindow::openCamera()
{
    if(camera != nullptr)
        return;
    camera = new Camera(data_lock);	//TODO destroy these objects
	cameraThread = new QThread;
    showdibhWindow();

	connect(camera, &Camera::send_videoSignal, this, &MainWindow::display_Video);
	connect(cameraThread, &QThread::started, camera, &Camera::read_camera);
    connect(cameraThread, &QThread::finished, cameraThread, &QThread::deleteLater);
    connect(imageView, &GraphicsView::sendAreapoints, this, &MainWindow::receiveAreaPoints);
    connect(camera, &Camera::send_maxMinHSV, dibhWindow, &dibhControls::recthsvChanged);

	camera->moveToThread(cameraThread);
	cameraThread->start();
}

void MainWindow::display_Video(cv::Mat *frame)
{
    data_lock->lock();
	cv::Mat displayFrame = *frame;
    data_lock->unlock();
	QImage image(displayFrame.data,
			displayFrame.cols,
			displayFrame.rows,
			displayFrame.step,
			QImage::Format_RGB888);

	QPixmap pixmap = QPixmap::fromImage(image);

	imageScene->clear();
	imageView->resetMatrix();
	imageScene->addPixmap(pixmap);
	imageScene->update();
	imageView->setSceneRect(pixmap.rect());
}

void MainWindow::receiveAreaPoints(QPointF p1, QPointF p2)
{
    if(camera != nullptr)
        camera->receiveAreaPoints(p1, p2);
}
void MainWindow::receiveContourSlot()
{
    if(camera != nullptr)
        camera->contourAreaSelected();
}

void MainWindow::mouseReleased()
{
    if(camera != nullptr)
        camera->mouseReleased();
}
