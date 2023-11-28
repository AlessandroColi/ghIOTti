#include "CarWasher.h"
#include "config.h"
#include "devices/ButtonImpl.h"
#include "devices/Led.h"
#include "devices/Lcd.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/ServoMotorImpl.h"
#include "devices/TempSensorLM35.h"

CarWasher::CarWasher(){
}

void CarWasher::init(){
    pButton = new ButtonImpl(START_BTN);
    led01 = new Led(LED01_PIN);
    led02 = new Led(LED02_PIN);
    led03 = new Led(LED03_PIN);
    lcd = new Lcd(); //task?
    pPir = new Pir(PIR_PIN);
    pSonar = new Sonar(DIST_ECHO_PIN, DIST_TRIG_PIN, MAXTIME);
    pServoMotor = new ServoMotorImpl(MOTOR_PIN);
    pTempSensor = new TempSensorLM35(TEMP_PIN);
    detPresence = false;
    setWaitingForCarState();
}
    
void wake(){
    detachInterrupt (digitalPinToInterrupt(PIR_PIN));
    pCarWasher->setCarDetectedForCheckInState();
    setEnteringWashingAreaState();
}

void goToSleep(){
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), wake, RISING); //qualsiasi pin va bene meno 1/2 credo
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
    sleep_enable();
    sleep_mode();  
    sleep_disable();  
}

double CarWasher::getCurrentDistance(){
    return distance;
}
double CarWasher::getCurrentTemperature(){
    return temperature;
}

bool CarWasher::detectedPresence(){
    return this->detPresence;
}

bool CarWasher::isWaitingForCarState(){
    return state == WAITING_FOR_CAR;
}
bool CarWasher::isCarDetectedForCheckInState(){
    return state == CAR_DETECTED_FOR_CHECK_IN;
}
bool CarWasher::isEnteringWashingAreaState(){
    return state == ENTERING_WASHING_AREA;
}
bool CarWasher::isReadyToWashState(){
    return state == READY_TO_WASH;
}
bool CarWasher::isWashingState(){
    return state == WASHING;
}
bool CarWasher::isLeavingWashingAreaState(){
    return state == LEAVING_WASHING_AREA;
}
bool CarWasher::isCheckOutState(){
    return state == CHECK_OUT;
}
bool CarWasher::isMaintenaceState(){
    return state == MAINTENANCE;
}

void CarWasher::setWaitingForCarState(){
    state = WAITING_FOR_CAR;
    goToSleep();
}
void CarWasher::setCarDetectedForCheckInState(){
    state = CAR_DETECTED_FOR_CHECK_IN;
    led01->switchOn();
    this->printOnLcd("Welcome");
}
void CarWasher::setEnteringWashingAreaState(){
    state = ENTERING_WASHING_AREA;
    led01->switchOff();
    led02->switchOn();
    this->printOnLcd("Proceed to the washing area");
}
void CarWasher::setReadyToWashState(){
    state = READY_TO_WASH;
    led02->switchOff();
    this->printOnLcd("Ready to wash"); 
}
void CarWasher::setWashingState(){
    state = WASHING;
    this->displayProgressBar(N3);
}
void CarWasher::setLeavingWashingAreaState(){
    state = LEAVING_WASHING_AREA;
    led02->switchOff();
    led03->switchOn();
    this->printOnLcd("Washing complete, you can leave the area");
}
void CarWasher::setCheckOutState(){
    state = CHECK_OUT;
    led03->switchOff();
}
void CarWasher::setMaintenaceState(){
    state = MAINTENANCE;
    this->printOnLcd("Detected a problem - please wait");
}

void CarWasher::samplePresence(){
    this->pPir->sync();
    this->detPresence = pPir->isDetected();
}
void CarWasher::sampleDistance(){
    //mapping?
    double d = pSonar->getDistance();
    if (d == NO_OBJ_DETECTED){
        this->distance = 0;
    } else {
        double dist = MAXDIST - d*10; //?
        if (dist < 0){
            dist = 0;
        }
        this->distance = dist;
    }
}
void CarWasher::sampleTemperature(){
    this->temperature = pTempSensor->getTemperature();
}

void CarWasher::printOnLcd(String text){
    lcd->clearDisplay();
    lcd->setCursor(0, 0);
    lcd->printText(text);
}

void CarWasher::displayProgressBar(int seconds){
    lcd->clearDisplay();
    lcd->setCursor(0, 0);
    lcd->printText("Time left:");
    lcd->progressBar(seconds);
}

void CarWasher::servoOn(){
    pServoMotor->on();
}

void CarWasher::servoOff(){
    pServoMotor->off();
}
        
void CarWasher::setServoPosition(int angle){
    pServoMotor->setPosition(angle);
}

bool CarWasher::isButtonClicked(){
    return pButton->isClicked();
}

void CarWasher::test(){
    
}