#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	initUI();
	createAction();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI()
{
	showMaximized();
	
	QWidget *widget = new QWidget(this);

	QGridLayout *main_layout = new QGridLayout(widget);
	
	imageScene = new QGraphicsScene(widget);
	imageView = new QGraphicsView(imageScene);
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




}
