#include "solarped_control_interface_logic.h"


SolarpedControlInterfaceLogic::SolarpedControlInterfaceLogic(QObject *parent)
    : QObject{parent}
{
    this->is_camera_on = false;
    this->is_write_data_file = false;
    this->is_control = false;
    this->is_learning = false;
    this->switchToControlState();
}

void SolarpedControlInterfaceLogic::turnOnCamera() {
    this->is_camera_on = true;
    // TODO: turn on camera
}

void SolarpedControlInterfaceLogic::turnOffCamera() {
    this->is_camera_on = false;
    // TODO: turn off camera
}

bool SolarpedControlInterfaceLogic::isCameraOn() {
    return this->is_camera_on;
}

void SolarpedControlInterfaceLogic::startControl() {
    this->is_control = true;
}

void SolarpedControlInterfaceLogic::stopControl() {
    this->is_control = false;
}

bool SolarpedControlInterfaceLogic::isControl() {
    return this->is_control;
}

void SolarpedControlInterfaceLogic::startLearning() {
    this->is_learning = true;
}

void SolarpedControlInterfaceLogic::stopLearning() {
    this->is_learning = false;
}

bool SolarpedControlInterfaceLogic::isLearning() {
    return this->is_learning;
}

// False is learning state, True is control state.
void SolarpedControlInterfaceLogic::switchToControlState() {
    learning_control_state = true;
}

void SolarpedControlInterfaceLogic::switchToLearningState() {
    learning_control_state = false;
}

bool SolarpedControlInterfaceLogic::isControlState() {
    if(learning_control_state == true)
        return true;
    return false;
}

bool SolarpedControlInterfaceLogic::isLearningState() {
    if(learning_control_state == false)
        return true;
    return false;
}

void SolarpedControlInterfaceLogic::enableWriteDataFile() {
    this->is_write_data_file = true;
    // TODO: write .csv
}

void SolarpedControlInterfaceLogic::disableWriteDataFile() {
    this->is_write_data_file = false;
}

bool SolarpedControlInterfaceLogic::isWriteDataFile() {
    return this->is_write_data_file;
}

void SolarpedControlInterfaceLogic::setSerialPortName(QString serial_port_name) {
    this->serial_port_name = serial_port_name;
}

QString SolarpedControlInterfaceLogic::getSerialPortName() {
    return this->serial_port_name;
}

void SolarpedControlInterfaceLogic::setBaudRate(int baud_rate) {
    this->baud_rate = baud_rate;
}

int SolarpedControlInterfaceLogic::getBaudRate() {
    return this->baud_rate;
}

void SolarpedControlInterfaceLogic::setCameraNumber(int camera_number) {
    this->camera_number = camera_number;
}

int SolarpedControlInterfaceLogic::getCameraNumber() {
    return this->camera_number;
}

void SolarpedControlInterfaceLogic::setActuatorsNumber(int actuator_number) {
    this->actuators_number = actuator_number;
}

int SolarpedControlInterfaceLogic::getActuatorNumber() {
    return this->actuators_number;
}

void SolarpedControlInterfaceLogic::setControlSequency(QString control_sequency) {
    this->control_sequency = control_sequency;
}

QString SolarpedControlInterfaceLogic::getControlSequency() {
    return this->control_sequency;
}

void SolarpedControlInterfaceLogic::setEpisodeNumber(int episode_number) {
    this->episode_number = episode_number;
}

int SolarpedControlInterfaceLogic::getEpisodeNumber() {
    return this->episode_number;
}

void SolarpedControlInterfaceLogic::setCurrentX(double current_x) {
    this->current_x = current_x;
}
double SolarpedControlInterfaceLogic::getCurrentX() {
    return this->current_x;
}

void SolarpedControlInterfaceLogic::setCurrentY(double current_y) {
    this->current_y = current_y;
}

double SolarpedControlInterfaceLogic::getCurrentY() {
    return this->current_y;
}

void SolarpedControlInterfaceLogic::setCurrentAngle(double current_angle) {
    this->current_angle = current_angle;
}

double SolarpedControlInterfaceLogic::getCurrentAngle() {
    return this->current_angle;
}

void SolarpedControlInterfaceLogic::setGoalX(double goal_x) {
    this->goal_x = goal_x;
}

double SolarpedControlInterfaceLogic::getGoalX() {
    return this->goal_x;
}

void SolarpedControlInterfaceLogic::setGoalY(double goal_y) {
    this->goal_y = goal_y;
}

double SolarpedControlInterfaceLogic::getGoalY() {
    return this->goal_y;
}

void SolarpedControlInterfaceLogic::setGoalAngle(double goal_angle) {
    this->goal_angle = goal_angle;
}

double SolarpedControlInterfaceLogic::getGoalAngle() {
    return this->goal_angle;
}
