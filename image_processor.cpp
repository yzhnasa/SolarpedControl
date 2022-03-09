#include "image_processor.h"

ImageProcessor::ImageProcessor(QObject *parent, int camera_number)
    : QObject{parent}
{
    this->camera_number = camera_number;
    this->timer = new QTimer(this);
    this->timer->setTimerType(Qt::PreciseTimer);
    this->is_tracker_empty = true;
    this->current_state = State(0, 0, 0);
    this->is_write_data = false;
}

ImageProcessor::ImageProcessor(QLabel *camera_display, int camera_number, FileWriter *file_writer, State *display_current_state, QObject *parent)
    : QObject{parent}
{
    this->camera_display = camera_display;
    this->camera_number = camera_number;
    this->timer = new QTimer(this);
    this->timer->setTimerType(Qt::PreciseTimer);
    this->is_tracker_empty = true;
    this->current_state = State(0, 0, 0);

    if(file_writer == nullptr) {
        this->is_write_data = false;
    } else {
        this->is_write_data = true;
    }
}

void ImageProcessor::initialTracker() {
    tracker = cv::TrackerCSRT::create();
    roi_box = cv::selectROI("tracker", frame);
    tracker->init(frame, roi_box);
}

void ImageProcessor::initialFilter() {
    filter = cv::KalmanFilter(9, 3, 0, CV_64F);
    filter.transitionMatrix = (cv::Mat_<double>(9, 9) << 1, 1,0.5, 0, 0,  0, 0, 0,  0,
                                                         0, 1,  1, 0, 0,  0, 0, 0,  0,
                                                         0, 0,  1, 0, 0,  0, 0, 0,  0,
                                                         0, 0,  0, 1, 1,0.5, 0, 0,  0,
                                                         0, 0,  0, 0, 1,  1, 0, 0,  0,
                                                         0, 0,  0, 0, 0,  1, 0, 0,  0,
                                                         0, 0,  0, 0, 0,  0, 1, 1,0.5,
                                                         0, 0,  0, 0, 0,  0, 0, 1,  1,
                                                         0, 0,  0, 0, 0,  0, 0, 0,  1);
    filter.measurementMatrix = (cv::Mat_<double>(3, 9) << 1,0,0,0,0,0,0,0,0,
                                                          0,0,0,1,0,0,0,0,0,
                                                          0,0,0,0,0,0,1,0,0);

    measurement = cv::Mat_<double>(3, 1);
    measurement.setTo(cv::Scalar(0));
    filter.statePost.at<double>(0) = (double)roi_box.x+roi_box.width/2;
    filter.statePost.at<double>(1) = 0;
    filter.statePost.at<double>(2) = 0;
    filter.statePost.at<double>(3) = (double)roi_box.x+roi_box.height/2;;
    filter.statePost.at<double>(4) = 0;
    filter.statePost.at<double>(5) = 0;
    filter.statePost.at<double>(6) = (double)findOrientation(frame, roi_box);
    filter.statePost.at<double>(7) = 0;
    filter.statePost.at<double>(8) = 0;

    cv::setIdentity(filter.processNoiseCov, cv::Scalar::all(1e-4));
    cv::setIdentity(filter.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(filter.errorCovPost, cv::Scalar::all(.1));
}

void ImageProcessor::filterState(State &current_state) {
    prediction = filter.predict();
    measurement(0) = current_state.x;
    measurement(1) = current_state.y;
    measurement(2) = current_state.angle;
    estimated = filter.correct(measurement);
    current_state.x = estimated.at<double>(0);
    current_state.y = estimated.at<double>(3);
    current_state.angle = estimated.at<double>(6);
}

void ImageProcessor::turnOnCamera() {
    cap.open(camera_number);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateWindowDisplay()));
    connect(this, SIGNAL(startCameraTimer()), timer, SLOT(start()));
    connect(this, SIGNAL(stopCameraTimer()), timer, SLOT(stop()));

    if(is_tracker_empty) {
        cap.read(frame);
        cv::resize(frame, frame, cv::Size(image_width, image_hight), cv::INTER_LINEAR);
        initialTracker();
        initialFilter();

        is_tracker_empty = false;
    }

    Q_EMIT startCameraTimer();
}

void ImageProcessor::turnOffCamera() {
    disconnect(timer, SIGNAL(timeout()), this, SLOT(updateWindowDisplay()));

    Q_EMIT stopCameraTimer();
    disconnect(this, SIGNAL(startCameraTimer()), timer, SLOT(start()));
    disconnect(this, SIGNAL(stopCameraTimer()), timer, SLOT(stop()));

    cap.release();
}

void ImageProcessor::setCameraDisplay(QLabel *camera_display) {
    this->camera_display = camera_display;
}

QLabel *ImageProcessor::getCameraDisplay() {
    return this->camera_display;
}

void ImageProcessor::setCameraNumber(int camera_number) {
    this->camera_number = camera_number;
}

int ImageProcessor::getCameraNumber() {
    return this->camera_number;
}

void ImageProcessor::setFileWriter(FileWriter *file_writer) {
    this->file_writer = file_writer;
    this->is_write_data = true;
}

FileWriter *ImageProcessor::getFileWriter() {
    return this->file_writer;
}

void ImageProcessor::setDisplayCurrentState(State *display_current_state) {
    this->display_current_state = display_current_state;
}

State *ImageProcessor::getDisplayCurrentState() {
    return this->display_current_state;
}

bool ImageProcessor::isWriteData() {
    return this->is_write_data;
}
void ImageProcessor::startWriteData() {
    this->is_write_data = true;
}

void ImageProcessor::stopWriteData() {
    this->is_write_data = false;
}

void ImageProcessor::drawBox(cv::Mat &frame, cv::Rect2i &roi_box) {
    int x = roi_box.x;
    int y = roi_box.y;
    int width = roi_box.width;
    int height = roi_box.height;
    cv::rectangle(frame, cv::Point(x, y), cv::Point((x+width), (y+height)), cv::Scalar(255, 0, 255), 3, cv::LINE_4);
}

void ImageProcessor::drawCenterPoint(cv::Mat &frame, cv::Rect2i &roi_box) {
    int x = roi_box.x;
    int y = roi_box.y;
    int width = roi_box.width;
    int height = roi_box.height;
    cv::circle(frame, cv::Point((x+width/2), (y+height/2)), 1, cv::Scalar(255, 0, 255));
}

void ImageProcessor::drawAxis(cv::Mat& img, cv::Point p, cv::Point q, cv::Scalar colour, const float scale) {
    double angle = atan2( (double) p.y - q.y, (double) p.x - q.x ); // angle in radians
    double hypotenuse = sqrt( (double) (p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));
    // Here we lengthen the arrow by a factor of scale
    q.x = (int) (p.x - scale * hypotenuse * cos(angle));
    q.y = (int) (p.y - scale * hypotenuse * sin(angle));
    line(img, p, q, colour, 1, cv::LINE_AA);
    // create the arrow hooks
    p.x = (int) (q.x + 9 * cos(angle + CV_PI / 4));
    p.y = (int) (q.y + 9 * sin(angle + CV_PI / 4));
    line(img, p, q, colour, 1, cv::LINE_AA);
    p.x = (int) (q.x + 9 * cos(angle - CV_PI / 4));
    p.y = (int) (q.y + 9 * sin(angle - CV_PI / 4));
    line(img, p, q, colour, 1, cv::LINE_AA);
}

unsigned int ImageProcessor::findMaxContour(std::vector<std::vector<cv::Point>> &contours) {
    unsigned int max_contour_index = 0;
    unsigned int object_max_contour_index = 0;
    unsigned int max_contour_length = 0;
    unsigned int temp_contour_length = 0;
    for(unsigned int i = 0; i < contours.size(); i++) {
        temp_contour_length = contours.at(i).size();
        if(temp_contour_length > max_contour_length){
            object_max_contour_index = max_contour_index;
            max_contour_index = i;
            max_contour_length = temp_contour_length;
        }
    }
    return object_max_contour_index;
}

double ImageProcessor::findOrientation(cv::Mat &frame, cv::Rect2i &roi_box) {
    box_frame = frame(cv::Range(roi_box.y, roi_box.y+roi_box.height), cv::Range(roi_box.x, roi_box.x+roi_box.width));
    cv::cvtColor(box_frame, gray_frame, cv::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(gray_frame, binary_frame, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary_frame, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    //Construct a buffer used by the pca analysis
    unsigned int max_contour_index = findMaxContour(contours);
    cv::drawContours(box_frame, contours, max_contour_index, cv::Scalar(0, 0, 255), 2);

    int sz = static_cast<int>(contours.at(max_contour_index).size());
    cv::Mat data_pts = cv::Mat(sz, 2, CV_64F);
    for (int i = 0; i < data_pts.rows; i++) {
        data_pts.at<double>(i, 0) = contours[max_contour_index][i].x;
        data_pts.at<double>(i, 1) =  contours[max_contour_index][i].y;
    }
    //Perform PCA analysis
    cv::PCA pca_analysis(data_pts, cv::Mat(), cv::PCA::DATA_AS_ROW);
    //Store the center of the object
    cv::Point cntr = cv::Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
                          static_cast<int>(pca_analysis.mean.at<double>(0, 1)));
    //Store the eigenvalues and eigenvectors
    std::vector<cv::Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);
    for (int i = 0; i < 2; i++) {
        eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                    pca_analysis.eigenvectors.at<double>(i, 1));
        eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
    }
    // Draw the principal components
    cv::circle(box_frame, cntr, 3, cv::Scalar(255, 0, 255), 2);
    cv::Point p1 = cntr + 0.02 * cv::Point(static_cast<int>(eigen_vecs[0].x * eigen_val[0]), static_cast<int>(eigen_vecs[0].y * eigen_val[0]));
    cv::Point p2 = cntr - 0.02 * cv::Point(static_cast<int>(eigen_vecs[1].x * eigen_val[1]), static_cast<int>(eigen_vecs[1].y * eigen_val[1]));
    drawAxis(box_frame, cntr, p1, cv::Scalar(0, 255, 0), 1);
    drawAxis(box_frame, cntr, p2, cv::Scalar(255, 255, 0), 5);
    double angle = qRadiansToDegrees(atan2(eigen_vecs[0].y, eigen_vecs[0].x)); // orientation in radians
    return angle;
}

void ImageProcessor::updateWindowDisplay() {
    cap.read(frame);
    cv::resize(frame, frame, cv::Size(image_width, image_hight), cv::INTER_LINEAR);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    tracker->update(frame, roi_box);
    drawBox(frame, roi_box);
    drawCenterPoint(frame, roi_box);
    double angle = findOrientation(frame, roi_box);
    current_state = State(roi_box.x+roi_box.width/2, roi_box.y+roi_box.height/2, angle);
    filterState(current_state);
    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
    Q_EMIT finishOneFrameProcessing(&qt_image, &current_state);
}

