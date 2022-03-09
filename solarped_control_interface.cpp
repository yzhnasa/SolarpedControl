#include "solarped_control_interface.h"

SolarpedControlInterface::SolarpedControlInterface(QWidget *parent)
    : QMainWindow{parent}
{
    backend = new SolarpedControlInterfaceLogic(this);

    setFixedSize(backend->WINDOW_WIDTH, backend->WINDOW_HIGHT);

    initialSetupsGroup();
    initialSetupsGroupBox();
    initialControlLearningGroup();
    initialControlLearningGroupBox();
    initialSetupsControlInterfaceGroup();
    initialSetupsControlInterfaceBox();
    initialCameraDisplay();
    initialMainLayout();
    initialMainLayoutBox();

    image_processor = new ImageProcessor(camera_display);

    file_writer = new FileWriter();
    trainer_controller = new TrainerController(this);
    State state_bound;
    state_bound.x = (double) backend->WINDOW_WIDTH;
    state_bound.y = (double) backend->WINDOW_HIGHT;
    state_bound.angle = (double) 180;
    trainer_controller->setGoalStateBound(state_bound);
}

void SolarpedControlInterface::initialSetupsGroup() {
    select_serial_port = new QComboBox(this);
    for(const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        select_serial_port->addItem(info.portName());
    }
    connect(select_serial_port, SIGNAL(currentTextChanged(QString)), this, SLOT(configureSerialPort(QString)));

    select_baud_rate = new QComboBox(this);
    for(unsigned int i = 0; i < backend->BAUD_RATE_NUMBERS; i++) {
        select_baud_rate->addItem(QString::number(backend->BAUD_RATE_VALUES[i]));
    }

    select_baud_rate->setEnabled(true);
    connect(select_baud_rate, SIGNAL(currentTextChanged(QString)), this, SLOT(configureBaudRate(QString)));

    input_camera_id = new QLineEdit(this);
    input_camera_id->setPlaceholderText("Camera Number");
    input_camera_id->setFocus();
    input_camera_id->setEnabled(false);
    connect(input_camera_id, SIGNAL(textEdited(QString)), this, SLOT(configureCameraPort(QString)));

    input_actuators_number = new QLineEdit(this);
    input_actuators_number->setPlaceholderText("Number of Actuators");
    input_actuators_number->setFocus();
    input_actuators_number->setEnabled(false);
    connect(input_actuators_number, SIGNAL(textEdited(QString)), this, SLOT(configureActuatorsNumber(QString)));

    input_control_sequency = new QLineEdit(this);
    input_control_sequency->setPlaceholderText("Control Sequency");
    input_control_sequency->setFocus();
    input_control_sequency->setEnabled(false);
    connect(input_control_sequency, SIGNAL(textEdited(QString)), this, SLOT(configureControlSequency(QString)));

    input_episode_number = new QLineEdit(this);
    input_episode_number->setPlaceholderText("Number of Episode");
    input_episode_number->setFocus();
    input_episode_number->setEnabled(false);
    connect(input_episode_number, SIGNAL(textEdited(QString)), this, SLOT(configureEpisodeNumber(QString)));

    camera_button = new QPushButton("Camera On", this);
    camera_button->setEnabled(false);
    connect(camera_button, SIGNAL(clicked(bool)), this, SLOT(cameraOnOff()));

    write_data_file_button = new QPushButton("Enable Write Data", this);
    write_data_file_button->setEnabled(false);
    connect(write_data_file_button, SIGNAL(clicked(bool)), this, SLOT(enableDisableWriteData()));

    setups_group = new QVBoxLayout();
    setups_group->addWidget(new QLabel("Serial Port"));
    setups_group->addWidget(select_serial_port);
    setups_group->addWidget(new QLabel("Select Baud Rate"));
    setups_group->addWidget(select_baud_rate);
    setups_group->addWidget(new QLabel("Input Camera ID"));
    setups_group->addWidget(input_camera_id);
    setups_group->addWidget(input_actuators_number);
    setups_group->addWidget(input_control_sequency);
    setups_group->addWidget(input_episode_number);
    setups_group->addWidget(camera_button);
    setups_group->addWidget(write_data_file_button);

}

void SolarpedControlInterface::initialSetupsGroupBox() {
    setups_group_box = new QGroupBox(this);
    setups_group_box->setLayout(setups_group);
}

void SolarpedControlInterface::initialControlLearningGroup() {
    learning_button = new QPushButton("Start Learning", this);
    learning_button->setEnabled(false);
    connect(learning_button, SIGNAL(clicked(bool)), this, SLOT(startStopLearning()));

    control_learning_switch = new QRadioButton("Learning/Control", this);
    control_learning_switch->setEnabled(false);
    connect(control_learning_switch, SIGNAL(clicked(bool)), this, SLOT(learningControlSwitching()));

    current_x = new QLineEdit(this);
    current_x->setReadOnly(true);

    current_y = new QLineEdit(this);
    current_y->setReadOnly(true);

    current_angle = new QLineEdit(this);
    current_angle->setReadOnly(true);

    goal_x = new QLineEdit(this);
    goal_x->setPlaceholderText("Goal X");
    goal_x->setFocus();
    goal_x->setEnabled(false);
    connect(goal_x, SIGNAL(textEdited(QString)), this, SLOT(movingSolarpedX(QString)));

    goal_y = new QLineEdit(this);
    goal_y->setPlaceholderText("Goal Y");
    goal_y->setFocus();
    goal_y->setEnabled(false);
    connect(goal_y, SIGNAL(textEdited(QString)), this, SLOT(movingSolarpedY(QString)));

    goal_angle = new QLineEdit(this);
    goal_angle->setPlaceholderText("Goal Angle");
    goal_angle->setFocus();
    goal_angle->setEnabled(false);
    connect(goal_angle, SIGNAL(textEdited(QString)), this, SLOT(movingSolarpedAngle(QString)));

    control_button = new QPushButton("Start Moving", this);
    control_button->setEnabled(false);
    connect(control_button, SIGNAL(clicked(bool)), this, SLOT(startStopControl()));

    quit_button = new QPushButton("Quit", this);
    connect(quit_button, SIGNAL(clicked()), this, SLOT(close()));

    control_learning_group = new QVBoxLayout(this);
    control_learning_group->addWidget(new QLabel("Current X"));
    control_learning_group->addWidget(current_x);
    control_learning_group->addWidget(new QLabel("Current Y"));
    control_learning_group->addWidget(current_y);
    control_learning_group->addWidget(new QLabel("Current Angle"));
    control_learning_group->addWidget(current_angle);
    control_learning_group->addWidget(learning_button);
    control_learning_group->addWidget(control_learning_switch);
    control_learning_group->addWidget(new QLabel("Goal X"));
    control_learning_group->addWidget(goal_x);
    control_learning_group->addWidget(new QLabel("Goal Y"));
    control_learning_group->addWidget(goal_y);
    control_learning_group->addWidget(new QLabel("Goal Angle"));
    control_learning_group->addWidget(goal_angle);
    control_learning_group->addWidget(control_button);
    control_learning_group->addWidget(quit_button);
}


void SolarpedControlInterface::initialControlLearningGroupBox() {
    control_learning_box = new QGroupBox(this);
    control_learning_box->setLayout(control_learning_group);
}

void SolarpedControlInterface::initialSetupsControlInterfaceGroup() {
    setups_control_interface_group = new QVBoxLayout(this);
    setups_control_interface_group->addWidget(setups_group_box);
    setups_control_interface_group->addWidget(control_learning_box);
    setups_control_interface_group->addStretch(1);
}

void SolarpedControlInterface::initialSetupsControlInterfaceBox() {
    setups_control_interface_box = new QGroupBox(this);
    setups_control_interface_box->setLayout(setups_control_interface_group);
    setups_control_interface_box->setFixedWidth(backend->CONTROL_INTERFACE_BOX_WIDTH);
    setups_control_interface_box->setFixedHeight(backend->CONTROL_INTERFACE_BOX_HIGHT);
}

void SolarpedControlInterface::initialCameraDisplay() {
    camera_display = new QLabel(this);

    // Todo: set size of resolution.
}

void SolarpedControlInterface::initialMainLayout() {
    main_layout = new QHBoxLayout(this);
//    main_layout = new QHBoxLayout();
    main_layout->addWidget(camera_display);
    main_layout->addWidget(setups_control_interface_box);
}

void SolarpedControlInterface::initialMainLayoutBox() {
    main_layout_box = new QGroupBox(this);
    main_layout_box->setLayout(main_layout);
    setCentralWidget(main_layout_box);
}

void SolarpedControlInterface::configureSerialPort(QString serial_port_name) {
    backend->setSerialPortName(serial_port_name);
    select_baud_rate->setEditable(true);
}

void SolarpedControlInterface::configureBaudRate(QString baud_rate) {
    backend->setBaudRate(baud_rate.toInt());
    input_camera_id->setEnabled(true);
}

void SolarpedControlInterface::configureCameraPort(QString camera_number) {
    backend->setCameraNumber(camera_number.toInt());
    input_actuators_number->setEnabled(true);
    image_processing_thread = new QThread(this);
    write_file_thread = new QThread(this);
    learning_control_thread = new QThread(this);
    backend->stopControl();
    backend->stopLearning();
}

void SolarpedControlInterface::configureActuatorsNumber(QString actuators_number) {
    backend->setActuatorsNumber(actuators_number.toInt());
    input_control_sequency->setEnabled(true);
}

void SolarpedControlInterface::configureControlSequency(QString control_sequency) {
    backend->setControlSequency(control_sequency);
    input_episode_number->setEnabled(true);
}

void SolarpedControlInterface::configureEpisodeNumber(QString episode_number) {
    backend->setEpisodeNumber(episode_number.toInt());
    camera_button->setEnabled(true);
}

void SolarpedControlInterface::enableDisableWriteData() {
    if(backend->isWriteDataFile()) {
        backend->disableWriteDataFile();
        file_writer->stopWriteFile();
        write_data_file_button->setText("Enable Write Data");
        disconnect(file_writer->thread(), SIGNAL(started()), file_writer, SLOT(startWriteFile()));
    } else {
        backend->enableWriteDataFile();
        write_data_file_button->setText("Disable Write Data");
        image_processor->setFileWriter(file_writer);
        image_processor->startWriteData();
        file_writer->moveToThread(write_file_thread);
        connect(file_writer->thread(), SIGNAL(started()), file_writer, SLOT(startWriteFile()));
        connect(this, SIGNAL(destroyed()), file_writer, SLOT(deleteLater()));
        file_writer->startWriteFile();
        file_writer->thread()->start();
    }
}

void SolarpedControlInterface::cameraOnOff() {
    if(backend->isCameraOn()) {
        backend->disableWriteDataFile();
        file_writer->stopWriteFile();
        write_data_file_button->setEnabled(false);
        write_data_file_button->setText("Enable Write Data");

        backend->switchToLearningState();
        backend->stopLearning();
        backend->stopControl();
        trainer_controller->stopTraining();
        trainer_controller->stopControl();
        control_learning_switch->setEnabled(false);
        control_learning_switch->setChecked(false);
        learning_button->setEnabled(false);
        learning_button->setText("Start Learning");
        goal_x->setEnabled(false);
        goal_y->setEnabled(false);
        goal_angle->setEnabled(false);
        control_button->setEnabled(false);
        control_button->setText("Start Moving");

        backend->turnOffCamera();
        image_processor->turnOffCamera();
        camera_button->setText("Camera On");
        disconnect(image_processor, SIGNAL(finishOneFrameProcessing(QImage *, State *)), this, SLOT(updateWindowDisplay(QImage *, State *)));
        disconnect(file_writer->thread(), SIGNAL(started()), file_writer, SLOT(startWriteFile()));
        disconnect(trainer_controller->thread(), SIGNAL(started()), trainer_controller, SLOT(startTraining()));
        disconnect(trainer_controller->thread(), SIGNAL(started()), trainer_controller, SLOT(startControl()));
    } else {
        write_data_file_button->setEnabled(true);

        backend->switchToControlState();
        control_learning_switch->setEnabled(true);
        learning_button->setEnabled(true);
        backend->turnOnCamera();
        camera_button->setText("Camera Off");
        image_processor->setCameraNumber(backend->getCameraNumber());
        image_processor->moveToThread(image_processing_thread);
        connect(image_processor->thread(), SIGNAL(started()), image_processor, SLOT(turnOnCamera()));
        connect(image_processor, SIGNAL(finishOneFrameProcessing(QImage *, State *)), this, SLOT(updateWindowDisplay(QImage *, State *)));
        connect(this, SIGNAL(destroyed()), image_processor, SLOT(deleteLater()));
        image_processor->turnOnCamera();
        image_processor->thread()->start();
    }
}
void SolarpedControlInterface::learningControlSwitching() {
    if(backend->isLearningState()) {
        backend->switchToControlState();
        backend->stopControl();
        learning_button->setEnabled(true);
        goal_x->setEnabled(false);
        goal_y->setEnabled(false);
        goal_angle->setEnabled(false);
        control_button->setEnabled(false);
        control_button->setText("Start Moving");
        qDebug() << "control state";
    } else {
        backend->switchToLearningState();
        backend->stopLearning();
        learning_button->setEnabled(false);
        learning_button->setText("Start Learning");

        goal_x->setEnabled(true);
        goal_y->setEnabled(true);
        goal_angle->setEnabled(true);
        control_button->setEnabled(true);
        qDebug() << "learning state";
    }
}

void SolarpedControlInterface::movingSolarpedX(QString x) {
    backend->setGoalX(x.toDouble());
}

void SolarpedControlInterface::movingSolarpedY(QString y) {
    backend->setGoalY(y.toDouble());
}
void SolarpedControlInterface::movingSolarpedAngle(QString angle) {
    backend->setGoalAngle(angle.toDouble());
}

void SolarpedControlInterface::startStopLearning() {
    if(backend->isLearning()) {
        backend->stopLearning();
        learning_button->setText("Start Learning");
        trainer_controller->stopTraining();
        disconnect(trainer_controller->thread(), SIGNAL(started()), trainer_controller, SLOT(startTraining()));
    } else {
        backend->startLearning();
        qDebug() << "Start learning";
        learning_button->setText("Stop Learning");
        trainer_controller->setSerialPortName(backend->getSerialPortName());
        trainer_controller->setBaudRate(backend->getBaudRate());
        trainer_controller->setActuatorsNumber(backend->getActuatorNumber());
        trainer_controller->setStateNumber(backend->STATE_NUMBER);
        trainer_controller->setControlSequency(backend->getControlSequency());
        trainer_controller->setEpisodeNumber(backend->getEpisodeNumber());
        qDebug() << "learning_control_thread";
        qDebug() << learning_control_thread;
        trainer_controller->moveToThread(learning_control_thread);
        connect(trainer_controller->thread(), SIGNAL(started()), trainer_controller, SLOT(startTraining()));
        connect(this, SIGNAL(destroyed()), trainer_controller, SLOT(deleteLater()));
        trainer_controller->startTraining();
        trainer_controller->thread()->start();
    }
}

void SolarpedControlInterface::startStopControl() {
    if(backend->isControl()) {
        backend->stopControl();
        control_button->setText("Start Moving");
        trainer_controller->stopControl();
        disconnect(trainer_controller->thread(), SIGNAL(started()), trainer_controller, SLOT(startControl()));
    } else {
        backend->startControl();
        control_button->setText("Stop Moving");
        State goal_state;
        goal_state.x = backend->getGoalX();
        goal_state.y = backend->getGoalY();
        goal_state.angle = backend->getGoalAngle();
        trainer_controller->setGoalState(goal_state);
        trainer_controller->setActuatorsNumber(backend->getActuatorNumber());
        trainer_controller->setStateNumber(backend->STATE_NUMBER);
        trainer_controller->setControlSequency(backend->getControlSequency());
        trainer_controller->setEpisodeNumber(backend->getEpisodeNumber());
        trainer_controller->moveToThread(learning_control_thread);
        connect(trainer_controller->thread(), SIGNAL(started()), trainer_controller, SLOT(startControl()));
        connect(this, SIGNAL(destroyed()), trainer_controller, SLOT(deleteLater()));
        trainer_controller->startControl();
        trainer_controller->thread()->start();
    }
}

void SolarpedControlInterface::updateWindowDisplay(QImage *image, State *current_state) {
    camera_display->setPixmap(QPixmap::fromImage(*image));
    current_x->setText(QString::number(current_state->x));
    current_y->setText(QString::number(current_state->y));
    current_angle->setText(QString::number(current_state->angle));
    if(backend->isWriteDataFile()) {
        file_writer->writeData(*current_state);
        qDebug() << "write data";
        qDebug() << QString::number(current_state->x) << "," << QString::number(current_state->y) << "," << QString::number(current_state->angle);
    }

    if(backend->isLearning() || backend->isControl()) {
//        qDebug() << "Learning or control";
        trainer_controller->setCurrentState(*current_state);
    }
}

void SolarpedControlInterface::updateWindowGoalState(State &goal_state) {

}

void SolarpedControlInterface::updateWindowControlSequency(QString control_sequency) {

}

SolarpedControlInterface::~SolarpedControlInterface() {
    image_processor->thread()->quit();
    image_processor->thread()->wait();
//    image_processing_thread->quit();
//    image_processing_thread->wait();
//    write_file_thread->quit();
//    write_file_thread->wait();
    file_writer->thread()->quit();
    file_writer->thread()->wait();
    trainer_controller->thread()->quit();
    trainer_controller->thread()->wait();
}
