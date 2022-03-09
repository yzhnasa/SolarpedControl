#include "trainer_controller.h"

TrainerController::TrainerController(QObject *parent)
    : QObject{parent}
{
    this->is_controlling = false;
    this->is_training = false;
    this->timer = new QTimer(this);
    this->timer->setTimerType(Qt::PreciseTimer);
    this->serial_port_writer = new SerialPortWriter(this);
    seed = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
    engine = std::default_random_engine(seed);
    generator = std::uniform_real_distribution<float>(0, 1);
}

void TrainerController::setSerialPortName(QString serial_port_name) {
    serial_port_writer->setSerialPortName(serial_port_name);
}

QString TrainerController::getSerialPortName() {
    return serial_port_writer->getSerialPortName();
}

void TrainerController::setBaudRate(int baud_rate) {
    serial_port_writer->setBaudRate(baud_rate);
}

int TrainerController::getBaudRate() {
    return serial_port_writer->getBaudRate();
}

void TrainerController::setCurrentState(State &current_state) {
    state_data_queue.enqueue(current_state);
    this->current_state = current_state;
//    qDebug() << "enqueue state data";
//    qDebug() << QString::number(current_state.x) << "," << QString::number(current_state.y) << "," << QString::number(current_state.angle);
}

State TrainerController::getCurrentState() {
    return this->current_state;
}

void TrainerController::setGoalState(State &goal_state) {
    this->goal_state = goal_state;
}

State TrainerController::getGoalState() {
    return this->goal_state;
}

void TrainerController::setGoalStateBound(State &goal_state_bound) {
    this->goal_state_bound = goal_state_bound;
}

State TrainerController::getGoalStateBound() {
    return this->goal_state_bound;
}

void TrainerController::setActuatorsNumber(int actuator_number) {
    this->actuators_number = actuator_number;
}

int TrainerController::getActuatorNumber() {
    return this->actuators_number;
}

void TrainerController::setStateNumber(int state_number) {
    this->state_number = state_number;
}

int TrainerController::getStateNumber() {
    return this->state_number;
}

void TrainerController::setControlSequency(QString control_sequency) {
    this->control_sequency = control_sequency;
}

QString TrainerController::getControlSequency() {
    return this->control_sequency;
}

void TrainerController::setEpisodeNumber(int episode_number) {
    this->episode_number = episode_number;
}

int TrainerController::getEpisodeNumber() {
    return this->episode_number;
}

//void TrainerController::sendActionCommand(QString command) {

//}

void TrainerController::initTrainerController() {
    if(!state_data_queue.isEmpty()) {
        current_state = state_data_queue.dequeue();
    } else {
        current_state.x = 0;
        current_state.y = 0;
        current_state.angle = 0;
        qDebug() << "not get current state";
    }
    qDebug() << actuators_number;
    qDebug() << state_number;
    solarped = QSharedPointer<Solarped>::create(actuators_number, state_number);
    this->episode_counter = 0;
    this->steps_counter = 0;
}

State TrainerController::generateRandomGoalState() {
    goal_state.x = goal_state_bound.x * generator(engine);
    goal_state.y = goal_state_bound.y * generator(engine);
    goal_state.angle = goal_state_bound.angle * generator(engine);
    Q_EMIT goalStateChanged(goal_state);
    return goal_state;
}

//void TrainerController::updateWindowGoalState() {

//}

//void TrainerController::updateWindowControlSequency() {

//}

void TrainerController::startTraining() {
    is_training = true;
//    initTrainerController();
    qDebug() << "start test thread";
//    serial_port_writer->openPort();
//    connect(timer, SIGNAL(timeout()), this, SLOT(test()));
    connect(timer, SIGNAL(timeout()), this, SLOT(training()));
    connect(this, SIGNAL(startTrainerControllerTimer()), timer, SLOT(start()));
    connect(this, SIGNAL(stopTrainerControllerTimer()), timer, SLOT(stop()));
    Q_EMIT startTrainerControllerTimer();
    initTrainerController();
}

void TrainerController::stopTraining() {
    is_training = false;
    serial_port_writer->closePort();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(training()));
    Q_EMIT stopTrainerControllerTimer();
    disconnect(this, SIGNAL(startTrainerControllerTimer()), timer, SLOT(start()));
    disconnect(this, SIGNAL(stopTrainerControllerTimer()), timer, SLOT(stop()));
    state_data_queue.clear();
}

void TrainerController::training() {
    while(!state_data_queue.isEmpty() && is_training) {
        current_state = state_data_queue.dequeue();
        if(!solarped->isDone(current_state) && steps_counter < 10240) {
            action = solarped->learn(current_state);
            qDebug() << "m"+QString::number(action)+control_sequency;
            serial_port_writer->writeMessage("m"+QString::number(action)+control_sequency);
            steps_counter++;
        } else {
            goal_state.x = goal_state_bound.x * generator(engine);
            goal_state.y = goal_state_bound.y * generator(engine);
            goal_state.angle = goal_state_bound.angle * generator(engine);
            solarped->setGoalState(goal_state);
            steps_counter = 0;
            episode_counter++;
        }
        if(episode_counter > episode_number) {
            stopTraining();
            break;
        }
    }
}

/*void TrainerController::training() {
    is_training = true;
    int steps_counter = 0;
    for(int i = 0; i < episode_number; i++) {
        goal_state.x = goal_state_bound.x * generator(engine);
        goal_state.y = goal_state_bound.y * generator(engine);
        goal_state.angle = goal_state_bound.angle * generator(engine);
        solarped->setGoalState(goal_state);
        steps_counter = 0;
        while(!solarped->isDone(current_state) && steps_counter < 10240) {
            action = solarped->learn(current_state);
            qDebug() << "m"+QString::number(action)+control_sequency;
            serial_port_writer->writeMessage("m"+QString::number(action)+control_sequency);
            if(!state_data_queue.isEmpty()) {
                current_state = state_data_queue.dequeue();
            }
            steps_counter++;
        }
    }
}*/

void TrainerController::startControl() {
    is_controlling = true;
//    initTrainerController();
    solarped->setGoalState(goal_state);
    serial_port_writer->openPort();
    connect(timer, SIGNAL(timeout()), this, SLOT(control()));
    connect(this, SIGNAL(startTrainerControllerTimer()), timer, SLOT(start()));
    connect(this, SIGNAL(stopTrainerControllerTimer()), timer, SLOT(stop()));
    Q_EMIT startTrainerControllerTimer();
}

void TrainerController::stopControl() {
    is_controlling = false;
    serial_port_writer->closePort();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(control()));
    Q_EMIT stopTrainerControllerTimer();
    disconnect(this, SIGNAL(startTrainerControllerTimer()), timer, SLOT(start()));
    disconnect(this, SIGNAL(stopTrainerControllerTimer()), timer, SLOT(stop()));
    state_data_queue.clear();
}

void TrainerController::control() {
    while(!state_data_queue.isEmpty() && is_controlling) {
        current_state = state_data_queue.dequeue();
        if(!solarped->isDone(current_state)) {
            action = solarped->selectAction(current_state);
            qDebug() << "m"+QString::number(action)+control_sequency;
            serial_port_writer->writeMessage("m"+QString::number(action)+control_sequency);
        } else {
            stopControl();
            break;
        }
    }
}

void TrainerController::test() {
    while (!state_data_queue.isEmpty()) {
        current_state = state_data_queue.dequeue();

        if(test_counter == 0)
            break;
        test_counter--;
    }
}

TrainerController::~TrainerController() {
    serial_port_writer->closePort();
}
