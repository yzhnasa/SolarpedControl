#ifndef LEARNERCONTROLLER_H
#define LEARNERCONTROLLER_H

#include <math.h>
#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QSharedPointer>
#include "serial_port_writer.h"
#include "utilities.h"
#include "environment.h"


class TrainerController : public QObject
{
    Q_OBJECT
public:
    explicit TrainerController(QObject *parent = nullptr);
    ~TrainerController();
    void setSerialPortName(QString serial_port_name);
    QString getSerialPortName();
    void setBaudRate(int baud_rate);
    int getBaudRate();
    void setCurrentState(State &current_state);
    State getCurrentState();
    void setGoalState(State &goal_state);
    State getGoalState();
    void setGoalStateBound(State &goal_state_bound);
    State getGoalStateBound();
    void setActuatorsNumber(int actuator_number);
    int getActuatorNumber();
    void setStateNumber(int state_number);
    int getStateNumber();
    void setControlSequency(QString control_sequency);
    QString getControlSequency();
    void setEpisodeNumber(int episode_number);
    int getEpisodeNumber();

private:
    AsyncQueue<State> state_data_queue;
    QTimer *timer;
    bool is_controlling;
    bool is_training;
    State current_state;
    State goal_state;
    State random_goal_state;
    State goal_state_bound;
    int actuators_number;
    int state_number;
    QString control_sequency;
    int episode_number;
    int episode_counter;
    int steps_counter;
    bool is_done_episode;
    int action;
    int test_counter;

    SerialPortWriter *serial_port_writer;
    std::mt19937 seed;
    std::default_random_engine engine;
    std::uniform_real_distribution<float> generator;
    QSharedPointer<Solarped> solarped;


    void initTrainerController();
    State generateRandomGoalState();

public Q_SLOTS:
    void startTraining();
    void stopTraining();
    void training();

    void startControl();
    void stopControl();
    void control();

    void test();

Q_SIGNALS:
    void startTrainerControllerTimer();
    void stopTrainerControllerTimer();
    void goalStateChanged(State &goal_state);
    void controlSequencyChanged(QString control_sequency);
};

#endif // LEARNERCONTROLLER_H
