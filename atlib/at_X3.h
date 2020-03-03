

//--------------------------------------------------------------------------------
// 
#define AT_REGBUFLEN        256            //pula adresowa przypadaj?ca na jedno urz?dzenie
#define AT_REGADDRES_MODBUS 0x00
//-----------------------------------------silniki (max 8)
#define AT_REGADRES_MOTOR1  0x01
#define AT_REGADRES_MOTOR2  0x02
#define AT_REGADRES_MOTOR3  0x03
#define AT_REGADRES_MOTOR4  0x04
//-------------------------------------PWMy (max 7, po cztery kanaly w kazdym = 28)
#define AT_REGADRES_PWM1    0x09
#define AT_REGADRES_PWM2    0x0a
#define AT_REGADRES_PWM3    0x0b
//------------------------------------- TWI/SPI/inne
#define AT_REGADRES_EPROM   0x10
#define AT_REGADRES_DISPLAY 0x11
#define AT_REGADRES_TIMER   0x12
#define AT_REGADRES_OSC     0x13
#define AT_REGADRES_TIMER1  0x14
//..
#define AT_REGADRES_TWI1    0x1b
#define AT_REGADRES_TWI2    0x1c
#define AT_REGADRES_SPI1    0x1d
#define AT_REGADRES_SPI2    0x1e
#define AT_REGADRES_SPI3    0x1f
//-------------------------------------moduly odpalane na TWI/SPI
#define AT_REGADRES_TWI_MPU6050_1 0x20
#define AT_REGADRES_TWI_MCP3424_1 0x21
//------------------------------------
#define AT_REGADRES_DI1 0x30
#define AT_REGADRES_DI2 0x31
#define AT_REGADRES_DI3 0x32
#define AT_REGADRES_DI4 0x33
#define AT_REGADRES_DI5 0x34
#define AT_REGADRES_DI6 0x35
//------------------------------------moduly logiczne: PIDy
#define AT_REGADRES_PID0 0x40       //Roll
#define AT_REGADRES_PID1 0x41       //Pitch
#define AT_REGADRES_PID2 0x42       //Yaw
#define AT_REGADRES_PID3 0x43       //Zet
//-------------------------------------moduly logiczne: Tracery
#define AT_REGADRES_TRACE0 0x50
#define AT_REGADRES_TRACE1 0x51
#define AT_REGADRES_TRACE2 0x52
//-----------------------------------....
#define AT_REGADRES_FS    0x60
//-----------------------------------....

//-------------------------------------moduly logiczne programu.
#define AT_REGADRES_SMM        0x7b     //saMMuruj
#define AT_REGADRES_H0         0x7c     //H0
#define AT_REGADRES_HELI       0x7d     //Heli
#define AT_REGADRES_POMIAR     0x7e     //dyfuzja
#define AT_REGADRES_PROGRAM    0x7f
#define AT_REGADRES_SMM_KAPPA  0x81     //IdKappa
#define AT_REGADRES_SMM_BETA   0x82     //IdBeta
#define AT_REGADRES_SMM_ALFA   0x83     //IdAlfa
#define AT_REGADRES_SMM_OMEGA  0x84     //IdOmega
#define AT_REGADRES_SMM_RHO    0x85     //IdRho
#define AT_REGADRES_TERMO      0x86     //Id termomodułu
#define AT_REGADRES_WAVE       0x87     //Id generatora fali
#define AT_REGADRES_LAMBDA     0x88     //Lambda
#define AT_REGADRES_SD         0x89     //SD

//-----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
#define AT_MODBUS_RESPNAG       4    //dlugosc naglowka ramki Receive (Response) - nie zawiera informacji o ilo?ci danych. Ilo?c danych okre?lona na podstawie funkcji.
#define AT_MODBUS_REQUNAG       6    //dlugosc naglowka ramki Send (Request)
#define AT_MODBUS_FRAME_LEN     32   //dlugosc bufora ramki
#define AT_MODBUS_REGOFFSET     3    //bajt na którym zapisany jest OFFSET rejestrów  (X3 - zamienić na AT_MODBUS_FRAME_REG)
#define AT_MODBUS_FRAME_ID      0    //adres w ramce ID
#define AT_MODBUS_FRAME_FUN     1    //adres w ramce Fun
#define AT_MODBUS_FRAME_DEV     2    //adres w ramce Dev
#define AT_MODBUS_FRAME_REG     3    //adres w ramce Reg.Start (pierwszy rejestr danych - offset)
#define AT_MODBUS_FRAME_DATALEN 4    //adres w ramce dlugosc danych (ilosc rejestrow) - bajty 4-5

//-------------------------------------------------------------------------
//funkcje standardowe MODBUS'a
#define AT_MODBUS_WRITE  0x10
#define AT_MODBUS_SET    0x13
#define AT_MODBUS_READ   0x03

#define AT_MODBUS_ID_X3        1
#define AT_MODBUS_ID_ADS1256   2
#define AT_MODBUS_ID_PC_USB    3
#define AT_MODBUS_ID_PI        4 //nie uzywać wartości 0 (zero) ze względu na to że ten parametr słuzy takze do idNad w X3

//-------------------------------------------------------------------------------------------------------------------------------------------
#define AT_MOTOR_DIR_UP         0				//praca w gore
#define AT_MOTOR_DIR_DN         1				//praca w dol
//-----------------------------------
#define AT_HELI_PID_KAT_MIN        -32767	    	//minimalny dostepny kat wychylenia - parametr pida (niefizyczny)
#define AT_HELI_PID_KAT_MAX        32767	    	//minimalny dostepny kat wychylenia - parametr pida
#define AT_HELI_PID_THR_MIN      0	        	//minimalny dostepny ciag - parametr pida
#define AT_HELI_PID_THR_MAX      65535	       	//maksymalny dostepny ciag - parametr pida


//------------------------------------------------------------------------------------------------------------------------------------------
#define AT_X3ERROR_FRAMEOUT_CRC  0x06     //ramki błędów zwróconych z poziomu X3 mają status oddzielnego urządzenia
#define AT_X3ERROR_OUTBUFLEN      8
//-------------------------------------------------------------------------------------------------------------------------------------------
//   Funkcje,
//   adresy w bajtach (a nie w rejestrach jak w modbusie)
//-----------------------------------
#define AT_MOTOR_DO            0x00     //0
    #define AT_MOTOR_DO_GO        0	    //   efektywny sygnal wyjscia
    #define AT_MOTOR_DO_DIR       1	    //   efektywny kierunek
    #define AT_MOTOR_DO_EN        2	    //   efektywny EN
    #define AT_MOTOR_DO_DEST      3		//   efektywny DEST
    #define AT_MOTOR_DO_UPSTOP    4		//   krancowka
    #define AT_MOTOR_DO_DNSTOP    5		//   krancowka
    #define AT_MOTOR_DO_UP        6		//   ruch w gore (przycisk)
    #define AT_MOTOR_DO_DN        7		//   ruch w dol (przycisk)
    #define AT_MOTOR_DO_FUPSTOP   8		//   krancowka UP (przeciazenie)
    #define AT_MOTOR_DO_FDNSTOP   9		//   krancowka DN (przeciazenie)
    #define AT_MOTOR_DO_EMG      10		//   emg      (czerwony klawisz)
    #define AT_MOTOR_DO_ONOFF    11		//   on/off      (funkcja dodatkowa)
#define AT_MOTOR_AO_POS        0x02		//2  aktualna pozycja (podwojny rejestr)
#define AT_MOTOR_AO_FREQ       0x06		//6  aktualna predkosc
#define AT_MOTOR_AO_DEST       0x0a		//10 destynacja
#define AT_MOTOR_FRAMEOUT_CRC  0x0e     //14
#define AT_MOTOR_OUTBUFLEN        20    //16   + 4
#define AT_MOTOR_DI           0x00      //0
    #define AT_MOTOR_DI_GO        0     //   uruchomienie zegara CLK
    #define AT_MOTOR_DI_DIR       1     //   kierunek obrotow
    #define AT_MOTOR_DI_EN        2     //   sygnal "Enabled" dla zablokowania/odblokowania silnika
    #define AT_MOTOR_DI_DEST      3     //   wskaznik ze silnik pracuje do osiagniecia pozycji docelowej ustawionej w rejestrach DEST
    #define AT_MOTOR_DI_ZERO      4     //   impuls zerujacy dla licznika drogi
    #define AT_MOTOR_DI_ONOFF     5     //   sterowanie załączeniem zasilaczy. funkcjonalnośc dodatkowa, nie związana z silnikiem
#define AT_MOTOR_AI_DEST      0x02		//2  aktualna pozycja (podwojny rejestr)
#define AT_MOTOR_AI_FREQ      0x06		//6  aktualna predkosc
#define AT_MOTOR_FRAMEIN_CRC  0x0a      //10
#define AT_MOTOR_INBUFLEN            18 //12  +  6


//--------------------------------------------------------------------------
//PID  zestaw parametrow dla PID'a
#define AT_PID_WRITE  0x10
#define AT_PID_SET    0x13
#define AT_PID_READ   0x03

#define AT_PID_AI_TGT    0x00
#define AT_PID_AI_KP     0x02
#define AT_PID_AI_TI     0x04
#define AT_PID_AI_TD     0x06
#define AT_PID_AI_PMIN   0x08
#define AT_PID_AI_PMAX   0x0a

#define AT_PID_AO_N      0x00
#define AT_PID_AO_IN     0x04
#define AT_PID_AO_PP     0x06
#define AT_PID_AO_PI     0x08
#define AT_PID_AO_PD     0x0a
#define AT_PID_AO_P      0x0c
#define AT_PID_AO_Y      0x0e
#define AT_PID_AO_C      0x10
#define AT_PID_AO_A      0x14
#define AT_PID_AO_TGT    0x16
#define AT_PID_AO_KP     0x18
#define AT_PID_AO_TI     0x1a
#define AT_PID_AO_TD     0x1c

//-----------------------------------
#define AT_PWM_AI_FREQ       0x00    //0
#define AT_PWM_AI_RCMIN      0x04    //4
#define AT_PWM_AI_RCMAX      0x06    //6
#define AT_PWM_AI_WIDTHA     0x08    //8
#define AT_PWM_AI_WIDTHB     0x0a    //10
#define AT_PWM_AI_WIDTHC     0x0c    //12
#define AT_PWM_AI_WIDTHD     0x0e    //14
#define AT_PWM_FRAMEIN_CRC   0x10    //16
#define AT_PWM_INBUFLEN         24   //18 + 6

#define AT_PWM_AO_TC         0x00    //0
#define AT_PWM_AO_PER        0x02    //2
#define AT_PWM_AO_RCMIN      0x04    //4
#define AT_PWM_AO_RCMAX      0x06    //6
#define AT_PWM_AO_CHA        0x08    //8
#define AT_PWM_AO_CHB        0x0a    //10
#define AT_PWM_AO_CHC        0x0c    //12
#define AT_PWM_AO_CHD        0x0e    //14
#define AT_PWM_AO_FREQ       0x10    //16
#define AT_PWM_FRAMEOUT_CRC  0x14    //20
#define AT_PWM_OUTBUFLEN        26   //22 + 4

//-----------------------------------
#define AT_H0_WRITE  0x10
#define AT_H0_SET    0x13
#define AT_H0_READ   0x03

#define AT_H0_AO_ROLL        0x00  //act (na podst czujnikow)
#define AT_H0_AO_PITCH       0x02  //
#define AT_H0_AO_YAW         0x04  //
#define AT_H0_AO_ZET         0x06  //
#define AT_H0_AO_PITCH_M     0x08  //

//-----------------------------------

#define AT_SD_AI_SD           0x00  //
#define AT_SD_AI_FD           0x02  //2
#define AT_SD_AI_SMIN         0x04  //4
#define AT_SD_AI_SMAX         0x06  //6
#define AT_SD_AI_FMIN         0x08  //8
#define AT_SD_AI_FMAX         0x0a  //10
#define AT_SD_DI              0x0c  //12
#define AT_SD_DI_POMIAR             0
#define AT_SD_DI_UP                 1
#define AT_SD_DI_DN                 2
#define AT_SD_FRAMEIN_CRC     0x0e  //14
#define AT_SD_INBUFLEN          22  //16  + 6

#define AT_SD_DO             0x00
#define AT_SD_DO_POMIAR             0 //proc
#define AT_SD_DO_UP                 1 //proc
#define AT_SD_DO_DN                 2 //proc
#define AT_SD_DO_UPSTOP             3 //krańcówka
#define AT_SD_DO_DNSTOP             4 //krańcówka
#define AT_SD_AO_N           0x02  //2
#define AT_SD_AO_0           0x04  //4
#define AT_SD_AO_1           0x08  //8
#define AT_SD_AO_2           0x0c  //12
#define AT_SD_AO_3           0x10  //16
#define AT_SD_AO_4           0x14  //20
#define AT_SD_AO_5           0x18  //24
#define AT_SD_AO_6           0x1c  //28
#define AT_SD_AO_7           0x20  //32
#define AT_SD_AO_AMPL        0x24  //36
#define AT_SD_AO_FREQ        0x26  //38
#define AT_SD_AO_TIME        0x28  //40
#define AT_SD_AO_POS         0x2a  //42
#define AT_SD_FRAMEOUT_CRC   0x2e  //46
#define AT_SD_OUTBUFLEN        52  //48  + 4

//-----------------------------------
#define AT_HELI_WRITE          0x10
#define AT_HELI_SET            0x13
#define AT_HELI_READ           0x03

#define AT_HELI_AI_ROLL        0x00     //0
#define AT_HELI_AI_PITCH       0x02     //2
#define AT_HELI_AI_YAW         0x04     //4
#define AT_HELI_AI_ZET         0x06     //6
#define AT_HELI_DI             0x08     //8
#define AT_HELI_DI_START             0
#define AT_HELI_DI_ROTORS            1
#define AT_HELI_DI_MOTORS            2
#define AT_HELI_FRAMEIN_CRC    0x0a     //10
#define AT_HELI_INBUFLEN          18    //12 + 6

#define AT_HELI_AO_TIME         0x00    //0
#define AT_HELI_AO_ALFAL        0x04    //4
#define AT_HELI_AO_ALFAR        0x06    //6
#define AT_HELI_AO_THRUSTL      0x08    //8
#define AT_HELI_AO_THRUSTR      0x0a    //10
#define AT_HELI_AO_ROLL         0x0c    //12
#define AT_HELI_AO_PITCH        0x0e    //14
#define AT_HELI_AO_YAW          0x10    //16
#define AT_HELI_AO_ZET          0x12    //18
#define AT_HELI_DO              0x14    //20
#define AT_HELI_DO_START             0
#define AT_HELI_DO_ROTORS            1
#define AT_HELI_DO_MOTORS            2
#define AT_HELI_FRAMEOUT_CRC    0x16     //22
#define AT_HELI_OUTBUFLEN          28    //24 + 4

//-----------------------------------
#define AT_MPU6050_WRITE  0x10
#define AT_MPU6050_SET    0x13
#define AT_MPU6050_READ   0x03

#define AT_MPU6050_AO_ACCX        0x00
#define AT_MPU6050_AO_ACCY        0x02
#define AT_MPU6050_AO_ACCZ        0x04
#define AT_MPU6050_AO_GYROX       0x06
#define AT_MPU6050_AO_GYROY       0x08
#define AT_MPU6050_AO_GYROZ       0x0a
#define AT_MPU6050_AO_TEMP        0x0c
#define AT_MPU6050_AO_N           0x0e

//-----------------------------------
#define AT_SMM_WRITE  0x10
#define AT_SMM_SET    0x13
#define AT_SMM_READ   0x03

#define AT_SMM_AI_KAPPA        0x00
#define AT_SMM_AI_BETA         0x02
#define AT_SMM_AI_ALFA         0x04
#define AT_SMM_AI_OMEGA        0x06
#define AT_SMM_AI_RHO          0x08

#define AT_SMM_AO_KAPPA        0x00
#define AT_SMM_AO_BETA         0x02
#define AT_SMM_AO_ALFA         0x04
#define AT_SMM_AO_OMEGA        0x06
#define AT_SMM_AO_RHO          0x08
#define AT_SMM_DO              0x10
#define AT_SMM_DO_KUP                0
#define AT_SMM_DO_KDN                1
#define AT_SMM_DO_BUP                2
#define AT_SMM_DO_BDN                3
#define AT_SMM_DO_AUP                4
#define AT_SMM_DO_ADN                5
#define AT_SMM_DO_OUP                6
#define AT_SMM_DO_ODN                7
#define AT_SMM_DO_RUP                8
#define AT_SMM_DO_RDN                9


//-----------------------------------
#define AT_TIMER_DI                      0x00    //0
#define AT_TIMER_DI_STOPER_ON              0     //      stoper
#define AT_TIMER_DI_STOPER_RESET           1     //      reset stoper
#define AT_TIMER_DI_STOPER_PAUSE           2     //      pauza stoper
#define AT_TIMER_AI_FREQ                 0x02    //2 podwójny
#define AT_TIMER_AI_SHAPEFREQ            0x06    //6 podwójny
#define AT_TIMER_AI_STOPER_TIMEOUT       0x0a    //10 podwójny
#define AT_TIMER_FRAMEIN_CRC             0x0e    //14
#define AT_TIMER_INBUFLEN          22            //16  + 6

#define AT_TIMER_AO_TC         0x00   //0
#define AT_TIMER_AO_PER        0x02   //2
#define AT_TIMER_AO_FREQ       0x04   //4  podwójny
#define AT_TIMER_AO_CZAS       0x08   //8  podwójny
#define AT_TIMER_AO_STOPER     0x0c   //12 podwójny
#define AT_TIMER_FRAMEOUT_CRC  0x10   //16 CRC
#define AT_TIMER_OUTBUFLEN         22 //18 + 4

//-----------------------------------
#define AT_WAVE_AI_SHAPEAMPL            0x00   //0 amplituda sygnalu wy
#define AT_WAVE_AI_SHAPEFREQ            0x02   //2
#define AT_WAVE_FRAMEIN_CRC             0x04   //4
#define AT_WAVE_INBUFLEN                    12 //6 +6

#define AT_WAVE_AO_TC           0x00    //0
#define AT_WAVE_AO_PER          0x02    //2
#define AT_WAVE_AO_FREQ         0x04    //4 (bufor zmiejszony)
#define AT_WAVE_AO_SHAPEAMPL    0x06    //4
#define AT_WAVE_AO_SHAPEFREQ    0x08    //8
#define AT_WAVE_AO_VALUE        0x0a    //10
#define AT_WAVE_FRAMEOUT_CRC    0x0c    //12
#define AT_WAVE_OUTBUFLEN          18   //14 + 4


//---------------------------------------------------
#define AT_SERIAL_TIMEOUT 10000000              //ustawiac eksperymenalnie, zaleznie od dlugosci danych
#define AT_SERIAL_TTY     "/dev/ttyAMA0"       //nazwa urzadzenia w raspi
#define AT_SERIAL_BAUD    115200
//---------------------------------------------------
#define AT_SOCKET_IP      "192.168.2.201"
#define AT_SOCKET_PORTNO  5000

//-----------------------------------
#define AT_DO_DI                 0x00   //0
#define AT_DO_FRAMEIN_CRC        0x01   //1
#define AT_DO_INBUFLEN               9  //3 +6
//-----------------------------------
#define AT_DO_DO                  0x00   //0
#define AT_DO_FRAMEOUT_CRC        0x01   //1
#define AT_DO_OUTBUFLEN               7  //3 +4

