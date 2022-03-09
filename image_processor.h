#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QLabel>
#include <QQueue>
#include <QTimer>
#include <QImage>
#include <QDebug>
#include <QVector>
#include <QtMath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/matx.hpp>
#include "file_writer.h"
#include "utilities.h"

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(QObject *parent = nullptr, int camera_number = 0);
    explicit ImageProcessor(QLabel *camera_display = nullptr, int camera_number = 0,  FileWriter *file_writer = nullptr, State *display_current_state = nullptr, QObject *parent = nullptr);
    void setCameraDisplay(QLabel *camera_display);
    QLabel *getCameraDisplay();
    void setCameraNumber(int camera_number);
    int getCameraNumber();
    void setFileWriter(FileWriter *file_writer);
    FileWriter *getFileWriter();
    void setDisplayCurrentState(State *display_current_state);
    State *getDisplayCurrentState();
    bool isWriteData();
    void startWriteData();
    void stopWriteData();

private:
    QLabel *camera_display;
    QTimer *timer;
    int camera_number;
    cv::VideoCapture cap;
    cv::Mat frame;
    cv::Mat box_frame;
    cv::Mat gray_frame;
    cv::Mat binary_frame;
    cv::Mat_<double> measurement;
    cv::Mat prediction;
    cv::Mat estimated;
    cv::Rect2i roi_box;
    cv::KalmanFilter filter;
    QImage qt_image;
    cv::Ptr<cv::Tracker> tracker;
    bool is_tracker_empty;
    State current_state;
    State *display_current_state;
    FileWriter *file_writer;
    bool is_write_data;

    void initialTracker();
    void initialFilter();
    void filterState(State &current_state);

    void drawBox(cv::Mat &frame, cv::Rect2i &roi_box);
    void drawCenterPoint(cv::Mat &frame, cv::Rect2i &roi_box);
    void drawAxis(cv::Mat& img, cv::Point p, cv::Point q, cv::Scalar colour, const float scale = 0.2);
    unsigned int findMaxContour(std::vector<std::vector<cv::Point>> &contours);
    double findOrientation(cv::Mat &frame, cv::Rect2i &roi_box);

    const unsigned int image_hight = 650;
    const unsigned int image_width = 900;

public Q_SLOTS:
    void turnOnCamera();
    void turnOffCamera();
    void updateWindowDisplay();

Q_SIGNALS:
    void startCameraTimer();
    void stopCameraTimer();
    void finishOneFrameProcessing(QImage *image, State *current_state);

};

#endif // IMAGEPROCESSOR_H
