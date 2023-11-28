#include "WashControlTask.h";

#include "config.h"

WashControlTask::WashControlTask(CarWasher* pCarWasher, BlinkingTask* pBlinkingTask, SerialMonitor* pSerialMonitor): 
        pCarWasher(pCarWasher), pBlinkingTask(pBlinkingTask),pSerialMonitor(pSerialMonitor) {
    state = WAITING;
}
  
void WashControlTask::tick(){
    switch(state) {
        case WAITING:
            if (pCarWasher->isReadyToWashState() && pCarWasher->isButtonClicked()) {
                StartWashing();
                washingTimeElapsed = 0;
                pCarWasher->setWashingState();
                pBlinkingTask->setPeriod(BLINK_INT2);
                pBlinkingTask->setActive(true);
            }
            break;
        case WASHING:
            pCarWasher->sampleTemperature();
            if (pCarWasher->getCurrentTemperature() >= MAXTEMP) {
                state = TEMP_HIGH;
                tempHighStartTime = millis();
            }
            if (washingTimeElapsed >= N3) {
                state = WAITING;
                pCarWasher->setLeavingWashingAreaState();
                pBlinkingTask->setActive(false);
            }
            break;
    
        case TEMP_HIGH:
            pCarWasher->sampleTemperature();
            if (pCarWasher->getCurrentTemperature() < MAXTEMP) {
                state = WASHING;
            }
            else if ((millis() - tempHighStartTime) >= N4) {
                state = MAINTENACE;
                pCarWasher->setMaintenaceState();
                StopWashing();
            }
            break;
        
        case MAINTENACE:
            if(pSerialMonitor->isMsgAvailable()){
                String msg = pSerialMonitor->getMsg();
                if(msg == "Maintenence done"){
                    pCarWasher->setWashingState();
                    StartWashing();
                }
            }
            break;
    }
}

void WashControlTask::StopWashing() {
    washingTimeElapsed += (washingStartTime - millis());
}

void WashControlTask::StartWashing() {
    state = WASHING;
    washingStartTime = millis();
}