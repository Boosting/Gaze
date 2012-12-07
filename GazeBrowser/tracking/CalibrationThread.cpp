
#include <iostream>

#include "CalibrationThread.hpp"
#include "calibration/Calibration.hpp"
#include "detection/GazeTracker.hpp"
#include "video/LiveSource.hpp"

using namespace std;

CalibrationThread::CalibrationThread(int width, int height, LiveSource *camera) 
       : width(width), height(height), camera(camera){
}

void CalibrationThread::run()
{
     int x_offset = 40;
     int y_offset = 40;
     
     int height = this->height - 2 * y_offset;
     int width =  this->width - 2 * x_offset;
     
     Calibration calibration;
     
     GazeTracker tracker(*camera);
     
     for(unsigned short i=0;i<3;i++){
         for(unsigned short j=0;j<3;j++){
            int point_x = width / 2 * j + x_offset; 
            int point_y = height / 2 * i + y_offset;
             
            cout << "Calibration Point: " << point_x << "/" << point_y << endl;
            
            QString code = QString("calibrationCircle.move(%1,%2);")
                    .arg(point_x).arg(point_y);
            emit jsCommand(code);
            
            Point2f p(point_x, point_y);
            CalibrationData data = tracker.measurePoint(p, 5);
            calibration.addCalibrationData(data);
            cout << "Point: " << p << "Vector: " << data.getAverageVector() << endl;
         }
     }
     
     calibration.calcCoefficients();
     //calibration.calculateCoordinates()
}