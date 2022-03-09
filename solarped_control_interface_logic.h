#ifndef SOLARPED_CONTROL_INTERFACE_LOGIC_H
#define SOLARPED_CONTROL_INTERFACE_LOGIC_H

#include <QString>
#include <QTextStream>
#include <QQueue>
#include <QLabel>
#include <QSemaphore>
#include <QDebug>


class SolarpedControlInterfaceLogic : public QObject
{
    Q_OBJECT
public:
    const unsigned int WINDOW_HIGHT = 720;
    const unsigned int WINDOW_WIDTH = 1080;

    const unsigned int CONTROL_INTERFACE_BOX_WIDTH = 150;
    const unsigned int CONTROL_INTERFACE_BOX_HIGHT = 700;

    const unsigned int BAUD_RATE_NUMBERS = 14;
    const unsigned int BAUD_RATE_VALUES[14] = {110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000, 256000};

    const unsigned int STATE_NUMBER = 3;

    QLabel *camera_display;

    explicit SolarpedControlInterfaceLogic(QObject *parent = nullptr);
    void turnOnCamera();
    void turnOffCamera();
    bool isCameraOn();
    void startLearning();
    void stopLearning();
    bool isLearning();
    void startControl();
    void stopControl();
    bool isControl();
    void switchToControlState();
    void switchToLearningState();
    bool isControlState();
    bool isLearningState();

    void enableWriteDataFile();
    void disableWriteDataFile();
    bool isWriteDataFile();
    void setSerialPortName(QString serial_port_name);
    QString getSerialPortName();
    void setBaudRate(int baud_rate);
    int getBaudRate();
    void setCameraNumber(int camera_number);
    int getCameraNumber();
    void setActuatorsNumber(int actuator_number);
    int getActuatorNumber();
    void setControlSequency(QString control_sequency);
    QString getControlSequency();
    void setEpisodeNumber(int episode_number);
    int getEpisodeNumber();
    void setCurrentX(double current_x);
    double getCurrentX();
    void setCurrentY(double current_y);
    double getCurrentY();
    void setCurrentAngle(double current_angle);
    double getCurrentAngle();
    void setGoalX(double goal_x);
    double getGoalX();
    void setGoalY(double goal_y);
    double getGoalY();
    void setGoalAngle(double goal_angle);
    double getGoalAngle();

private:
    bool is_write_data_file;
    bool is_camera_on;
    bool is_learning;
    bool is_control;
    bool learning_control_state; // False is learning state, True is control state.

    QString file_name;
    QString serial_port_name;
    int baud_rate;
    int camera_number;
    int actuators_number;
    QString control_sequency;
    int episode_number;

    double current_x;
    double current_y;
    double current_angle;

    double goal_x;
    double goal_y;
    double goal_angle;

};

#endif // SOLARPED_CONTROL_INTERFACE_LOGIC_H
