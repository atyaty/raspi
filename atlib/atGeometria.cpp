#include "atGeometria.h"
#include "math.h"

//=====================================================================
//---------------------------------------------------------------------
atPoint::atPoint(atWektor w){ X=w.x();Y=w.y();Z=w.z();}
//---------------------------------------------------------------------
void atPoint::przesun(atWektor w){ X+=w.x(); Y+=w.y(); Z+=w.z();}
//---------------------------------------------------------------------
bool atPoint::rowny( atPoint p, double d){  return fabs(x()-p.x())<=d && fabs(y()-p.y())<=d && fabs(z()-p.z())<=d;}



//=====================================================================
//---------------------------------------------------------------------
void atWektor::normalizuj(){
    double mod=modul();
    if(mod!=0) pomnoz(1/mod);
}


//=====================================================================
//---------------------------------------------------------------------
double atTrojkat::powierzchnia(){
    atWektor *AB=new atWektor(A,B);
    atWektor *AC=new atWektor(A,C);
    atWektor *a=new atWektor(*AB,*AC);
    return a->modul()/2;
}
//---------------------------------------------------------------------
atWektor atTrojkat::normalny(){
    atWektor *AB=new atWektor(A,B);
    atWektor *AC=new atWektor(A,C);
    atWektor *a=new atWektor(*AB,*AC);
    a->normalizuj();
    return *a;
}
//---------------------------------------------------------------------
double atTrojkat::polozenie(atPoint P){ //położenie punktu względem płaszczyzny, jest to doległość ze znakiem "+" jak jest po stronie zgodnej z wektorem normalnym, "-" jak po przeciwnej
    atWektor *AB=new atWektor(A,B);
    atWektor *AC=new atWektor(A,C);
    atWektor *a=new atWektor(*AB,*AC); //ten wektor wyznaca parametry A,B,C równania parametrycznego płaszczyzny tych punktów a=[A,B,C]
                                       //(..nie mylić ze zmiennymi A,B,C tu występującymi)
                                       //parametr D równania parametrycznego płaszczyzny: D=-a->odleglosc(A) ..tu akurat A oznacza zmienną czyli punkt A
                                       //
                                       //jest to doległośc w sensie
    return (a->iloczyn_skalarny(P)-a->iloczyn_skalarny(A))/a->modul();
}
//---------------------------------------------------------------------
bool atTrojkat::rowny(atTrojkat T){
    return (A.rowny(T.p1()) && B.rowny(T.p2()) && C.rowny(T.p3())) ||
           (A.rowny(T.p2()) && B.rowny(T.p3()) && C.rowny(T.p1())) ||
           (A.rowny(T.p3()) && B.rowny(T.p1()) && C.rowny(T.p2())) ;
}


//=====================================================================
//---------------------------------------------------------------------
double atCzworoscian::przestrzen(){
    atWektor *DA=new atWektor(D,A);
    atWektor *DB=new atWektor(D,B);
    atWektor *DC=new atWektor(D,C);
    atWektor *a= new atWektor(*DA,*DB);
    return a->iloczyn_skalarny(*DC) / 6;
}

