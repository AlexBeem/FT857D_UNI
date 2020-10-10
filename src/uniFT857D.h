#ifndef uniFT857D_h
#define uniFT857D_h

#include "Arduino.h"

#include <SomeSerial.h>
#include <Stream.h>

/*
 * CAT Command definition from VE3BUX library and more
 * We will comment the ones we don't use for code efficiency & refference
 */

// >>> First single command ones (toggle)
//~ #define CAT_LOCK_ON             0x00
//~ #define CAT_LOCK_OFF            0x80
#define CAT_PTT_ON              0x08
#define CAT_PTT_OFF             0x88
//~ #define CAT_CLAR_ON             0x05
//~ #define CAT_CLAR_OFF            0x85
//~ #define CAT_SPLIT_ON            0x02
//~ #define CAT_SPLIT_OFF           0x82
#define CAT_VFO_AB              0x81
// >>> Now complex ones
#define CAT_FREQ_SET            0x01
#define CAT_MODE_SET            0x07
//~ #define CAT_CLAR_SET            0xF5
#define CAT_RX_DATA_CMD         0xE7
#define CAT_TX_DATA_CMD         0xF7
#define CAT_RX_FREQ_CMD         0x03
//~ #define CAT_RPTR_OFFSET_CMD     0x09
//~ #define CAT_RPTR_FREQ_SET       0xF9
//~ #define CAT_SQL_CMD             0x0A
// >>> Modes definition
//~ #define CAT_MODE_LSB            0x00
//~ #define CAT_MODE_USB            0x01
//~ #define CAT_MODE_CW             0x02
//~ #define CAT_MODE_CWR            0x03
//~ #define CAT_MODE_AM             0x04
//~ #define CAT_MODE_FM             0x08
//~ #define CAT_MODE_DIG            0x0A
//~ #define CAT_MODE_PKT            0x0C
//~ #define CAT_MODE_FMN            0x88
// >>> SQL modes
//~ #define CAT_SQL_DCS             0x0A
//~ #define CAT_SQL_DCS_DECD        0x0B
//~ #define CAT_SQL_DCS_ENCD        0x0C
//~ #define CAT_SQL_CTCSS           0x2A
//~ #define CAT_SQL_CTCSS_DECD      0x3A
//~ #define CAT_SQL_CTCSS_ENCD      0x4A
//~ #define CAT_SQL_OFF             0x8A
//~ #define CAT_SQL_CTCSS_SET       0x0B
//~ #define CAT_SQL_DCS_SET         0x0C
// >>> RPT related
//~ #define CAT_RPTR_OFFSET_N       0x09
//~ #define CAT_RPTR_OFFSET_P       0x49
//~ #define CAT_RPTR_OFFSET_S       0x89
// >>> HAMLIB specific ones
#define CAT_HAMLIB_EEPROM       0xBB


// defining the funtion type by params
typedef void (*FuncPtrVoid)(void);
typedef long (*FuncPtrVoidLong)(void);
typedef byte (*FuncPtrVoidByte)(void);
typedef void (*FuncPtrToggles)(boolean);
typedef void (*FuncPtrByte)(byte);
typedef void (*FuncPtrLong)(long);

/*
 * The class...
 */
class uniFT857D {
 public:
    // we have two kind of constructors here
    void begin(); // default for the radio 9600 @ 8N2
    void begin(long baudrate, int mode); // custom baudrate and mode
    void check(); // periodic check for serial commands
    // the functions that links the lib with your code
    void addCATPtt(void (*)(boolean));
    void addCATAB(void (*)(void));
    void addCATFSet(void (*)(long));
    void addCATMSet(void (*)(byte));
    void addCATGetFreq(long (*)(void));
    void addCATGetMode(byte (*)(void));
    void addCATSMeter(byte (*)(void));
    void addCATTXStatus(byte (*)(void));
    boolean enabled     = true;

 private:
    byte nullPad[5]     = {0,0,0,0,0};
    long freq           = 0;
    byte ACK            = 0;
    void setFreq(void);
    void from_bcd_be(void);
    void to_bcd_be(long);
    void sendFreqMode(void);
    void rxStatus(void);
    void sent(void);
    void readEeprom(void);
    void npadClear(void);
    void sendTxStatus(void);
    void sent(byte);
    void fset(void);
};

#endif
