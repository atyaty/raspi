#ifndef ATI2C_MCP_H
#define ATI2C_MCP_H
/*
układ mcp3424
*/

#include <atI2C_dev.h>
#include <math.h>

//------------------------------------------------------
// AT TWI-MCP3424
//------------------------------------------------------
#define AT_TWIMCP3424_CONFIGBITNR_RDY  7           //bit konfiguracji   7
#define AT_TWIMCP3424_CONFIGBIT_ADDR 0b01100000  //bity konfiguracji  6-5
#define AT_TWIMCP3424_CONFIGBIT_CONT 0b00010000  //bit konfiguracji   4
#define AT_TWIMCP3424_CONFIGBIT_RES  0b00001100  //bity konfiguracji  3-2
#define AT_TWIMCP3424_CONFIGBIT_GAIN 0b00000011  //bity konfiguracji  1-0

#define AT_TWIMCP3424_RDY        0b10000000
#define AT_TWIMCP3424_CONTINUOUS 0b00010000

#define AT_TWIMCP3424_ADDR_68    0x68
#define AT_TWIMCP3424_ADDR_6A    0x6a
#define AT_TWIMCP3424_ADDR_6C    0x6c
#define AT_TWIMCP3424_ADDR_6E    0x6e

#define AT_TWIMCP3424_CH1        0b00000000  //bajt konfiguracji 6-5: (kanal 0 - 00)
#define AT_TWIMCP3424_CH2        0b00100000  //bajt konfiguracji 6-5: (kanal 1 - 01)
#define AT_TWIMCP3424_CH3        0b01000000  //bajt konfiguracji 6-5: (kanal 2 - 10)
#define AT_TWIMCP3424_CH4        0b01100000  //bajt konfiguracji 6-5: (kanal 3 - 11)

#define AT_TWIMCP3424_RES_18     0b00001100  //bajt konfiguracji 3-2: (18 bitow rozdzielczosci - 11)
#define AT_TWIMCP3424_RES_16     0b00001000  //bajt konfiguracji 3-2: (18 bitow rozdzielczosci - 11)
#define AT_TWIMCP3424_RES_14     0b00000100  //bajt konfiguracji 3-2: (18 bitow rozdzielczosci - 11)
#define AT_TWIMCP3424_RES_12     0b00000000  //bajt konfiguracji 3-2: (18 bitow rozdzielczosci - 11)

#define AT_TWIMCP3424_GAIN_1     0b00000000  //bajt konfiguracji 1-0: (wzmocnienie 1 - 00)
#define AT_TWIMCP3424_GAIN_2     0b00000001  //bajt konfiguracji 1-0: (wzmocnienie 1 - 00)
#define AT_TWIMCP3424_GAIN_4     0b00000010  //bajt konfiguracji 1-0: (wzmocnienie 1 - 00)
#define AT_TWIMCP3424_GAIN_8     0b00000011  //bajt konfiguracji 1-0: (wzmocnienie 1 - 00)

#define AT_TWIMCP3424_RECBUFSIZE 4       //wielkosc bufora ..wywalic po sprawdzeniu
#define AT_TWIMCP3424_SENDBUFSIZE 1      //wielkosc bufora ..wywalic po sprawdzeniu
#define AT_TWIMCP3424_KANALY 4           //ilosc kanalow


//-------------------------------------------------
typedef struct {
    uint8_t config;	      //bajt konfiguracji, wysylany do MCP przed odczytem
    double mnoznik;       //mnożnik do obliczania wartości rzeczywistej
    uint8_t read_bytes;   //ilosc bajtow do odczytania w procedurze READ
    uint16_t zwloka;      //kontrola dzialania - tu zlicza sie ilość kroków oczekiwania na pobranie danych
    int32_t bufor;        //aktualne (ostatnio odczytane wskazanie)
    double value;         //wartość rzeczywista
} chann_MCP3424;

//-------------------------------------------------
// MCP ma 4 kanaly na pokladzie, nie wszystkie musimy uzywac
// uzywamy te, ktore sa dodane do tablicy Kanaly
// dodawanie kanalow jest metoda AddKanal
class atI2C_mcp : public atI2C_dev {
private:
    chann_MCP3424 Kanaly[AT_TWIMCP3424_KANALY];
    uint8_t kanal;             //aktualnie obslugiwany kanal (0,1,2,3)
    uint8_t kanal_il;          //ilość kanalow zdefiniowanych
    uint16_t zwloka;           //do kontroli działania
public:
    atI2C_mcp(int d);
    ~atI2C_mcp();
    void AddKanal(uint8_t n, uint8_t g, uint8_t r);
    void kanalNext(){  kanal++;  if(kanal>=kanal_il) kanal=0; }
    uint8_t getZwloka(uint8_t i){return Kanaly[i].zwloka;}
    int32_t getBufor(uint8_t i){return Kanaly[i].bufor;}
    double getValue(uint8_t i){return Kanaly[i].value;}

    void Setup();
    bool go();
    bool go_read();           //
    void go_write();          //

};

#endif // ATI2C_MCP_H
