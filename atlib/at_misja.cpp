#include "at_misja.h"


//-------------------------------------------------------------------
// główna funkcja misji
// uruchamiana jest w pętli zdarzeń
// jej zadaniem jest wyznaczanie trajektorii po której mamy sie poruszać
// (rozwojowo: wykonywanie innych zadań wchodzących w skład misji, narazie realizujemy jedno zadanie dotrzec do miejsca tgt)
//
// Parametr p to nowe nasze polozenie przesyłany jest z modułów określających położenie
// 		dane z: ojos, GPS, accel/gyro, magneto, czujniki ultradźwiękowe, itp służą do wyliczenia naszego położenia
//
//
void  at_misja::Go( at_statek* p ){
	act=p;
	Koryguj_schemat_z mapy();            // korygujemy schemat przestrzenny ze względu na nasze przemieszczenie
	Koryguj_schemat_z_czujikow();        // dopisujemy dane do schematu (obiekty statyczne) na podst czujników
	Tworz_uklad();                       // sprawdzamy ruchy statków i tworzymy układ przestrzeni zabronionych
	if( path.size() <=0 ) Planuj_path(); // jesli nie mamy ścieżki podróży to planujemy ją ( to jest propozycja ścieżki bez uwzględnienia kolizji)
	if( !On_path() ) Koryguj_path();     // jeśli nie jesteśmy na ścieżce korygujemy ścieżkę (np dodatkowe punkty dojścia dościeżki, albo przeliczenie od nowa)
										 // to ciągle nie uwzględnia kolizji
	Usun_kolizje();                      // sprawdza czy nasza sciezka nie przecina przestrzei zabronionych, jesli tak to koryguje sciezke
	Aktualizacja_mapy();
}
//-------------------------------------------------------------------------------------
//
//jakaś sprytna pętla po starym schemacie
//porównje nasze nowe położenie act z każdym z punktów schematu
//- szuka nowego punktu najbliższego na mapie
//- przypisuje poszczególne punkty do stref (daleki, bliski, ..)
//- w zależności od strefy do której przynależy punkt:
//   a) pobieramy odpowiednio uszczegółowienie z mapy   - DodajSzczegóły(atVertex* v)
//   b) ewetualnie kasujemy szczgóły -UsunSzczegóły( atVertex v) - uwaga na wypukłość, nie usuwać szczegółów wypukłych w naszą stronę
//
//
//
void at_misja::Koryguj_schemat_z_mapy(){
 for(unsigned i;i<schemat.vSize(); i++){
	if( !schemat.getVertex(i).Strefa_0() && schemat.getVertex(i).odleglosc( act.Punkt() )<streda_0 ){
		schemat.DodajSzczegóły();
	}
	if( ... ){
		schemat.UsunSzczegóły();
	}

 }
}
//----------------------------------------------------------------------------------------------
//
// petal po statkach
// dla każdego statku dodawana jest przestrzen zajmowana przez statek w określonym czasie który wynika z jego prędkości i naszej prędkosci
// przestrzen "za statkiem" traktujemy jok otwartą
//
void at_misja::Tworz_uklad(){
 uklad.Przepisz_ze_shcematu(schemat);
 for(unsigned i;i<statki.size(); i++){
	uklad.Dodaj_strefa_statku( statki.getStatek(i) , ... );
 }
}

//----------------------------------------------------------------------------------------------------
//
// najpierw liczymy różnice pomiedzy act a tgt
// ..we wszystkich wymiarach
// ??
//
// jakieś równanaia ruchu trzeba rozwiązac..
//
void at_misja::Planuj_path(){

}
