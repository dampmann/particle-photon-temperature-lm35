/*
    This is a little project to meassure my indoor temperature and post the 
    result to a slack channel once an hour. I used an LM35 temperaature sensor
    and had a power supply board attached to my breadboard to get 5V for the temperature
    sensor and 3.3V for the particle photon. I used a 1mOhm resistor between signal and
    ground to get stable temperature values. I created a slack channel and a web hook for
    that channel, used the webhook url in an integration on the particle.io website.
*/

int ts = A0;

double minvalue = 0.0;
double maxvalue = 0.0;
double voltage = 0.0;
double temperature = 0.0;
int analogValue = 0;
int counter = 0;

void setup() {
    pinMode(ts,INPUT);    
    Particle.variable("temperatur", &temperature, DOUBLE);
    Particle.variable("min", &minvalue, DOUBLE);
    Particle.variable("max", &maxvalue, DOUBLE);
}

void loop() {
    analogValue = analogRead(ts);
    voltage = analogValue*0.8; //mV
    temperature = voltage / 10.0;
    if(minvalue == 0.0 || temperature < minvalue) {
        minvalue = temperature;
    }
    
    if(maxvalue == 0.0 || temperature > maxvalue) {
        maxvalue = temperature;
    }
    
    if(counter == 3600 || counter == 0) {
        String data = String(temperature, 2) + " C (min " + String(minvalue, 2) + " C, max " + String(maxvalue,2) + " C)";
        Particle.publish("current", data, 60, PRIVATE);
        counter = 0;
    }
    delay(1000);
    counter++;
}

