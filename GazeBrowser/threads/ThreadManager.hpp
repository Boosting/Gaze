/* 
 * File:   ThreadManager.hpp
 * Author: fri
 *
 * Created on January 2, 2013, 11:45 AM
 */

#ifndef THREADMANAGER_HPP
#define	THREADMANAGER_HPP

#include <QtCore>
#include <calibration/Calibration.hpp>

class ThreadManager;

enum PROGRAM_STATES {
    ST_STARTED_UP,
    ST_IDLE,
    ST_CALIBRATING,
    ST_TRACKING
};

enum PROGRAM_EVENTS {
    EV_GO_IDLE,
    EV_CALIBRATE, 
    EV_TRACKING
};

typedef struct {
    PROGRAM_STATES state;
    PROGRAM_EVENTS event;
    PROGRAM_STATES next_state;
    void (ThreadManager::*func)(void);
} state_transitions;

class CalibrationThread;
class TrackingThread;
class BrowserWindow;
class IdleThread;

class ThreadManager : public QObject {
    
    Q_OBJECT
    
public:
    ThreadManager(BrowserWindow *parent);
    virtual ~ThreadManager();
    void calibrate();
    void goIdle();
    
    signals: 
    void runCalibration(void);
    void runTracker(Calibration);
    void runIdleThread(void);
    
public slots:
    void error(QString message);
    void calibrationFinished(Calibration calib);
    void threadStopped(PROGRAM_EVENTS nextEvent);
    
private:
    
    BrowserWindow *parent;
    PROGRAM_STATES state;
    
    // our threads "application logic"
    CalibrationThread *calibrator;
    TrackingThread *tracker;
    IdleThread *idle;
    
    // the threads
    QThread *trackingThread;
    QThread *calibrationThread;
    QThread *idleThread;
    
    // the Camera-Lock
    QMutex *cameraLock;
    
    state_transitions *transitions;
    int num_of_transitions;
    
    void setUpSignalHandling();
    
    void fsmSetupStateMachine();
    bool fsmProcessEvent(PROGRAM_EVENTS event);
    void fsmGoIdle();
    void fsmCalibrate();
    void fsmStopIdle();
    void fsmStopCalibration();
    void fsmStopTracking();
    
    // copying the ThreadManager makes absolutely no sense, 
    // let's therefore disable it
    ThreadManager(const ThreadManager& orig);
};

#endif	/* THREADMANAGER_HPP */

