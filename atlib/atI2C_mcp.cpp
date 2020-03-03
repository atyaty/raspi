#include "atI2C_mcp.h"

//-------------------------------------------
atI2C_mcp::atI2C_mcp(int d):atI2C_dev(d){
    kanal=0;
    kanal_il=0;
    zwloka=0;
    dbw=new atDataBuffer(d,AT_TWIMCP3424_SENDBUFSIZE);
    dbr=new atDataBuffer(d,AT_TWIMCP3424_RECBUFSIZE);

}
//------------------------------------------
atI2C_mcp::~atI2C_mcp(){ delete dbr; delete dbw;}
//------------------------------------------
void atI2C_mcp::Setup(){ }

//------------------------------------------
void atI2C_mcp::AddKanal(uint8_t n, uint8_t g, uint8_t r){
 Kanaly[kanal_il].config     = n | r | g | AT_TWIMCP3424_RDY | AT_TWIMCP3424_CONTINUOUS;
 Kanaly[kanal_il].read_bytes = (r==AT_TWIMCP3424_RES_18)?4:3; //ilość bajtów do odczytania zależy od RES
 Kanaly[kanal_il].mnoznik    = 1;
 Kanaly[kanal_il].mnoznik   /= pow(4,(r&AT_TWIMCP3424_CONFIGBIT_RES)>>2);
 Kanaly[kanal_il].mnoznik   /= pow(2,g&AT_TWIMCP3424_CONFIGBIT_GAIN);
 Kanaly[kanal_il].zwloka     = 0;
 Kanaly[kanal_il].bufor      = 0;
 Kanaly[kanal_il].value      = 0.;
 kanal_il++;
}
//---------------------------------------------------
void atI2C_mcp::go(){
   go_write();
   go_read();
}
//---------------------------------------------------
bool atI2C_mcp::go_read(){
    zwloka++;          //to tylko dla kontroli, zliczamy przebiegi funkcji read
    dbr->setDataLen( Kanaly[kanal].read_bytes );
    if( !atI2C_dev::go_read()) return false;

    if( !dbr->getBit(Kanaly[kanal].read_bytes-1,AT_TWIMCP3424_CONFIGBITNR_RDY) ){ //bajt konfiguracji zwrocony, zawira informacje o gotowosci danych (bit:RDY)
        Kanaly[kanal].bufor = dbr->getInt18(0); //odczyt danych (od zerowego bajtu bufora)
        Kanaly[kanal].value = Kanaly[kanal].bufor * Kanaly[kanal].mnoznik; //przeliczenie wartości
        Kanaly[kanal].zwloka=zwloka;            //historyczna zwloka
        kanalNext();
        return true;
    }
    return false;
}
//---------------------------------------------------
void atI2C_mcp::go_write(){
    zwloka=0; //to tylko dla kontroli, zliczamy przebiegi funkcji read
    dbw->Clear();
    dbw->buforAddByte( Kanaly[kanal].config  );
    atI2C_dev::go_write();
}
