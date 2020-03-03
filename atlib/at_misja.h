#ifndef AT_MISJAH
#define AT_MISJAH

#include "at_omam.h"
#include "at_statek.h"
#include "at_statki.h"
#include "atFileSTL.h"
#include <vector>

//--------------------------------------------------------------------------------------------
// obiekt opisuje zadanie do wykonania
// na razie jest to droga która jest do przebycia
// rozwojowo droga powinna wynikac z misji sformułowanej bardziej ogólnie
//
// tgt - miejscae do którego mamy dotrzeć (13-wymiarów) -siebie opisujemy tym samym obiektem co i inne statki
// act - aktualne nasze położenie
// path - punkty pośrednie pomiędzy act a tgt (anie tgt ani act nie należą do path)
// trace - punkty minione - historia, zamiana act powoduje zapisanie starego act do trace
//
//
class at_misja  {

private:

	at_statek tgt;    //cel misji - punkt docelowy w (13-wymiarowej) przestrzeni - czas, położenie, nachylenie, prędkość liniowa, prędkość obrotowa
	at_statek* act;   //aktualne położenie - (13-wymiarów) - wskaźnik do położenia wyliczonego na wyższym poziomie
	vector  <at_statek> trace; //trajektoria przebyta - historia (ewentualnie zostawić tylko jeden (poprzedni) punkt
	vector  <at_statek> path;  //trajektoria przyszła

						// można sobie wyobrazić różne tryby pracy (ewentualnie różne moduły, które odpalamy w razie potrzeby)
						// - strategia - sami określamy sobie tgt, w oparciu o zadania formułowane bardziej ogólnie - to jest wyższa szkoła, na razie się ną nie zajmuję
						//                 tu może być także opcja działania w trybie nadzorcy, gdzie wyznaczamy tgt dla innych zasobów którymi dysponujemy..
						//
						// - taktyka - dostaję z zewnątrz tgt i realizuję misję dotarcia do tgt, wyznaczam path
						// - egzekucja - dostaję path i to realizuję nie martwiąc sie ogólnie (chyba, że podejmuję decyzje na zasadzie odruchów...)



	at_STL* mapa;     //mapa terenu - jest to stl opisujący przestrzeń w której się poruszamy -
					  //stl wyznacza powierzchnię (zamkniętą i ciągłą)
					  //my znajdujemy się na zewnątrz tej powierzchni - wektor normalny skierowany na nasza stronę
					  // tu mamy dostęp do mapy wspólny z innymi modułami.. a nawet szerzej systemami takimi jak my
					  // mapa generalnie jest aktualizowana na zewnątrz modułu,
					  // ale możemy działać w trybie budowniczego mapy, czyli możemy ją też aktualizować


	//-------------------------
	at_STL schemat;  //czyli plan sytuacyjny
					 // - jest to mapa w powiększeniu +
					 //   dane z obserwacji statycznych przeszkód terenowych +
					 //   dane z obserwacji dynamicznych przeszkód na kursie kolizyjnym
					 //
					 //ten stl opisuje najbliższą przestrzeń w której się poruszamy
					 //obiekt jest dynamiczny - dane na dalszym planie są mniej szczegółowe niż dane bliskie.
					 //dane zbyt odległe są usuwane żeby nie opóźniać procedur
					 // propozycja stref:
					 // strefa_0 - obiekty w zasięgu aktualnej prędkości poruszania się naszego
					 // strefa_1 - obiekty w zasięgu maksymalnej naszej prędkości
					 // strefa_2 - obiekty w zasięgu pięciu maksumalnych prędkości (dobrać eksperymentalnie, ewentualnie uzależnić od innych zdolności manewrowych)
					 //
					 // obiekt będzie zbudowany jakoś na kształt stFileSTL - na razie wykorzystuję niektóre jego metody


	atVertex* v0;    //zerowy punkt na schemacie - najbliższy nam vertex należący do schematu
					 //punkt alarmowy,
					 //do podejmowania szybkich decyzji..
					 // na razie tylko intuicja -może do wywalenia


	//---------------------------
	at_statki  *statki; //obiekty ruchome zidentyfikowane w przestrzeni, na przykład przez ojos
						//nie mogą to byc omamy, bo omamy sa we współrzędnych ojos
						//statki sa natomiast we współrzędnych mapy
						//każdy statek reprezentowany jest przez zamkniętą bryłę STL
						//tu mamy tylko dostęp przez wskaźnik, aktualizacja następuje na innym poziomie

	at_params *params; //parametry statyczne (nie zmieniające się) naszego statku (wymiary, masa, moment bezwładności,
					   //  moce silników, max prędkości,
					   //  strefy bezpieczeństwa, itp..)

	double strefa_0;  //paramretr schematu - jesli punkty schematu są bliższe niż strefa_0 schemat ma najwyższą szczegółowość


public:
	at_misja(at_statki*o, at_params* p, at_STL* m){ statki=o; params=p; mapa=m;};
	void Go( at_statek* p );    //główna funkcja misji - uruchamiana jest cyklicznie w pętli zdarzeń
						   //jako parametr dostaje nowe nasze położenie wyliczone przez inne moduły

	void Planuj_path();         //przelicza od nowa trajektorię dla misji
	void Koryguj_schemat();     //funkcja koryguje plan sytuacyjny wraz ze zmianą naszego położenia
	void Tworz_uklad();        //tworzenie układu przestrzeni zabronionych ze względu na zderzenia ze statkami
	bool On_path();             //sprawdzenie czy jestesmy na sciezce
	void Koryguj_path();       //korekta sciezki ze względu na nasze złe połozenie
    void Usun_kolizje();       //korekta sciezki ze względu na zderzenia

};

#endif // AT_OMAMY_H
