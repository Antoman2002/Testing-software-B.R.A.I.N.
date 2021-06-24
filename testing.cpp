#include "mbed.h"
DigitalOut trigger(D9);
DigitalIn echo(D8);
DigitalOut green(D4);
DigitalOut red(D2);

int correction = 0;
Timer myTime;

//Calculating the distance in cm using a small correction found in the beginning
int distance(int correction)
{
    trigger = 1;
    int distance = 0;

    myTime.reset();
    ThisThread::sleep_for(1ms);
    trigger = 0;
    while (echo == 0){};
    myTime.start();
    while (echo == 1){};
    myTime.stop();
    return distance = (myTime.elapsed_time().count() - correction)/58.0;
};

//Calculating the mean of multiple values determined by samples
int mean(int samples, int correction)
{
    int mean = 0;
    for (int i = 0; i < samples; i++){
        mean+=distance(correction);
    }
    return mean/samples;
}
// main() runs in its own thread in the OS
int main()
{
    // a small setup
    myTime.reset();
    myTime.start();
    while(echo == 2) {};
    green = 0;
    red = 0;
    correction = myTime.elapsed_time().count();
    
    while(true){
        int afstand = distance(correction);
        int gemiddelde = mean(20, correction);
        printf("distance: %d cm // mean: %d\n\r", afstand, gemiddelde);
        
        //when the distance is below 5cm the claws need to close for 20 seconds. 
        if(gemiddelde <= 5){
            green = 1;
            ThisThread::sleep_for(1000ms);
            green = 0;
            ThisThread::sleep_for(20000ms);
            red = 1;
            ThisThread::sleep_for(1000ms);
            red = 0;
        }        

        ThisThread::sleep_for(1000ms);
    }

}
