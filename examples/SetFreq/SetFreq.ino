#include <uniFT857D.h>
#include <EEPROM.h>

uniFT857D radio = uniFT857D();

// variables
volatile long freq = 7110000;
volatile int band = 0;
boolean ptt = false;
boolean splitActive = false;
boolean vfoAActive = true;
byte mode = 0;
byte eeprom_adr = 0;
byte eeprom_flag = 73;
byte eeprom_freqadr = 2;
byte flag_button = 0;
#define PAUSE_EEPROM 300
#define BUTTON 3
#define LED 13

// radio modes
#define MODE_LSB 00
#define MODE_USB 01
#define MODE_CW 02

// DEBUG flag, uncomment it if you want to test it by hand
//#define DEBUG true

// function to run when we must put radio on TX/RX
void catGoPtt(boolean pttf) {
    // the var ptt follows the value passed, but you can do a few more thing here
    ptt = pttf;

    #if defined (DEBUG)
    // debug
    Serial.print("PTT Status is: ");
    Serial.println(ptt);
    #endif
}

// function to run when VFOs A/B are toggled
void catGoToggleVFOs() {
    // here we simply toggle the value
    vfoAActive = !vfoAActive;

    #if defined (DEBUG)
    // debug
    Serial.print("VFO A active?: ");
    Serial.println(vfoAActive);
    #endif
}

// function to set a freq from CAT
void catSetFreq(long f) {
    // the var freq follows the value passed, but you can do a few more thing here
    freq = f;
    EEPROM.put(eeprom_freqadr,freq);   // Запомним в EEPROM 
    delay(PAUSE_EEPROM);

    #if defined (DEBUG)
    // debug
    Serial.print("Active VFO freq is: ");
    Serial.println(freq);
    #endif
}

// function to set the mode from the cat command
void catSetMode(byte m) {
    // the var mode follows the value passed, but you can do a few more thing here
    mode = m;

    #if defined (DEBUG)
    // debug
    Serial.print("Active VFO mode is: ");
    Serial.println(mode);
    #endif
}

// function to pass the freq to the cat library
long catGetFreq() {
    // this must return the freq as an unsigned long in Hz, you must prepare it before

    #if defined (DEBUG)
    // debug
    Serial.println("Asked for freq");
    #endif

    // pass it away
    return freq;
}

// function to pass the mode to the cat library
byte catGetMode() {
    // this must return the mode in the wat the CAT protocol expect it

    #if defined (DEBUG)
    // debug
    Serial.println("Asked for mode");
    #endif

    // pass it away
    return mode;
}

// function to pass the smeter reading in RX mode
byte catGetSMeter() {
    // this must return a byte in with the 4 LSB are the S meter data
    // so this procedure must take care of convert your S meter and scale it
    // up to just 4 bits

    #if defined (DEBUG)
    // debug
    Serial.println("Asked for S meter");
    Serial.print("S meter = ");
    Serial.println(byte(4));
    #endif

    // pass it away (fixed here just for testing)
    return byte(4);
}

// function to pass the TX status
byte catGetTXStatus() {
    /*
     * this must return a byte in wich the different bits means this:
     * 0b abcdefgh
     *  a = 0 = PTT off
     *  a = 1 = PTT on
     *  b = 0 = HI SWR off
     *  b = 1 = HI SWR on
     *  c = 0 = split on
     *  c = 1 = split off
     *  d = dummy data
     *  efgh = PO meter data
     */

    #if defined (DEBUG)
    // debug
    Serial.println("Asked for TX status");
    #endif

    // you have to craft the byte from your data, we will built it from
    // our data
    byte r = 0;
    // we fix the TX power to half scale (8)
    r = ptt<<7 + splitActive<<5 + 8;

    return r;
}

void pressButton(){

    if (flag_button == 1 && digitalRead(BUTTON) == 1){ // button OFF (кнопка отжата)
        flag_button = 0;
        if(band == 1) freq = 1948500;
        if(band == 3) freq = 3550000;
        if(band == 7) freq = 7080000;
        if(band == 10) freq = 10244000;
        if(band == 14) freq = 14150000;
        if(band == 18) freq = 18081000;
        if(band == 21) freq = 21023000;
        if(band == 24) freq = 24900900;
        if(band == 28) freq = 28028000;
        if(band == 29) freq = 29500000;
        if(band == 50) freq = 50070000;
        if(band == 145) freq = 145500000;
        if(band == 434) freq = 434154200;
                delay(20);
        EEPROM.put(eeprom_freqadr,freq); 
    }
    
    if (flag_button == 0 && digitalRead(BUTTON) == 0){ //button ON (кнопка нажата)

        flag_button = 1;
        digitalWrite(LED,!digitalRead(LED));
        band = freq/1000000;
        if(band == 434)band = 0;
        if(band == 145)band = 434;
        if(band == 50) band = 145;
        if(band == 29) band = 50;
        if(band == 28) band = 29;
        if(band == 24) band = 28;
        if(band == 21) band = 24;
        if(band == 18) band = 21;
        if(band == 14) band = 18;
        if(band == 10) band = 14;
        if(band == 7)  band = 10;
        if(band == 3)  band = 7;
        if(band == 1) band = 3;
        if(band == 0) band = 1;
                delay(20);
    }
}
  


void setup() {
    // preload the vars in the cat library
    radio.addCATPtt(catGoPtt);
    radio.addCATAB(catGoToggleVFOs);
    radio.addCATFSet(catSetFreq);
    radio.addCATMSet(catSetMode);
    radio.addCATGetFreq(catGetFreq);
    radio.addCATGetMode(catGetMode);
    radio.addCATSMeter(catGetSMeter);
    radio.addCATTXStatus(catGetTXStatus);

    // now we activate the library
    radio.begin(4800, SERIAL_8N1);

    #if defined (DEBUG)
    // serial welcome
    Serial.println("CAT Serial Test Ready");
    #endif


    // init EEPROM
    byte eeprom = 0;
    if( EEPROM.get(eeprom_adr,eeprom)!= 73){
    delay(PAUSE_EEPROM);
    EEPROM.put(eeprom_adr,73);
    delay(PAUSE_EEPROM);
    EEPROM.put(eeprom_freqadr,freq);   // Первично выбранный диапазон 
    }
    EEPROM.get(eeprom_freqadr,freq);
    
    #if defined (DEBUG)  
    Serial.print("Freq = ");
    Serial.println(freq);
    #endif  

    // init BUTTON & LED
    pinMode(BUTTON,INPUT_PULLUP);
    pinMode(LED,OUTPUT);
    band = freq/1000000;
}

void loop() {
    radio.check();
    pressButton();
}
