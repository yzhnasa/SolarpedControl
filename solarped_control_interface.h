#ifndef SOLARPED_CONTROL_INTERFACE_H
#define SOLARPED_CONTROL_INTERFACE_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QRect>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QThread>
#include <QImage>
#include <QString>
#include <QtMultimedia/QCameraInfo>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "solarped_control_interface_logic.h"
#include "image_processor.h"
#include "file_writer.h"
#include "trainer_controller.h"
#include "serial_port_writer.h"
#include "utilities.h"

class SolarpedControlInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit SolarpedControlInterface(QWidget *parent = nullptr);
    ~SolarpedControlInterface();
private:
    QPushButton *camera_button;
//    QPushButton *camera_gray_button;
//    QPushButton *camera_binary_button;
    QPushButton *learning_button;
    QPushButton *write_data_file_button;
    QPushButton *control_button;
    QPushButton *quit_button;
    QRadioButton *control_learning_switch;

    QComboBox *select_serial_port;
    QComboBox *select_baud_rate;
//    QComboBox *select_camera;
    QLineEdit *input_camera_id;
    QLineEdit *input_actuators_number;
    QLineEdit *input_control_sequency;
    QLineEdit *input_episode_number;
    QLineEdit *current_x;
    QLineEdit *current_y;
    QLineEdit *current_angle;
    QLineEdit *goal_x;
    QLineEdit *goal_y;
    QLineEdit *goal_angle;

    QLabel *camera_display;

    QVBoxLayout *setups_group;
    QVBoxLayout *control_learning_group;
    QVBoxLayout *setups_control_interface_group;
    QGroupBox *setups_group_box;
    QGroupBox *control_learning_box;
    QGroupBox *setups_control_interface_box;
    QHBoxLayout *main_layout;
    QGroupBox *main_layout_box;

//    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    SolarpedControlInterfaceLogic *backend;
    ImageProcessor *image_processor;
    FileWriter *file_writer;
    TrainerController *trainer_controller;

    QThread *image_processing_thread;
    QThread *write_file_thread;
    QThread *learning_control_thread;

    void initialSetupsGroup();
    void initialControlLearningGroup();
    void initialSetupsControlInterfaceGroup();
    void initialSetupsGroupBox();
    void initialControlLearningGroupBox();
    void initialSetupsControlInterfaceBox();
    void initialCameraDisplay();
    void initialMainLayout();
    void initialMainLayoutBox();

private Q_SLOTS:
    void configureSerialPort(QString serial_port_number);
    void configureBaudRate(QString baud_rate);
    void configureCameraPort(QString camera_number);
    void configureActuatorsNumber(QString actuators_number);
    void configureControlSequency(QString control_sequency);
    void configureEpisodeNumber(QString episode_number);
    void enableDisableWriteData();
    void cameraOnOff();
    void learningControlSwitching();
    void movingSolarpedX(QString x);
    void movingSolarpedY(QString y);
    void movingSolarpedAngle(QString angle);
    void startStopLearning();
    void startStopControl();
    void updateWindowDisplay(QImage *image, State *current_state);
    void updateWindowGoalState(State &goal_state);
    void updateWindowControlSequency(QString control_sequency);

Q_SIGNALS:

};

#endif // SOLARPED_CONTROL_INTERFACE_H
