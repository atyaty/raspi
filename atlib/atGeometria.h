#ifndef ATGeometria_H
#define ATGeometria_H


#include <QString>
#include "math.h"

class atProsta;
typedef atProsta atProsta;

class atWektor;
typedef atWektor atWektor;

class atPoint;
typedef atPoint atPoint;

//-------------------------------------------
class atPoint{
 protected:
	double X;
	double Y;
	double Z;

 public:
	atPoint(){X=0.;Y=0.;Z=0.;}
	atPoint(double xx,double yy){X=xx;Y=yy;Z=0.;}
	atPoint(double xx,double yy,double zz){X=xx;Y=yy;Z=zz;}
    atPoint(atWektor w);
	double x(){return X;}
	double y(){return Y;}
	double z(){return Z;}
	void setXY(double xx,double yy){X=xx;Y=yy;}
	void setXYZ(double xx,double yy,double zz){X=xx;Y=yy;Z=zz;}
    void przesun(atWektor w);
    void pomnoz(double f){X*=f;Y*=f;Z*=f;}
    double odleglosc(atPoint P){return sqrt((P.x()-X)*(P.x()-X)+(P.y()-Y)*(P.y()-Y) +(P.z()-Z)*(P.z()-Z) );}
    bool rowny(atPoint P){return (X==P.x() && Y==P.y() && Z==P.z()); }
    bool rowny(atPoint p, double d); //d-dopuszczalne odchylenie (przybliżenie)

    QString toString(){  return "["+QString::number( X ) +", "+QString::number( Y ) +", "+QString::number( Z ) +"]";}
    QString toString(QString b,QString p,QString e){  return b+QString::number( X ) +p+QString::number( Y ) +p+QString::number( Z ) +e;}

};

//-------------------------------------------
class atTrojkat{
 protected:
	atPoint A;
	atPoint B;
	atPoint C;

 public:
	atTrojkat(atPoint a,atPoint b,atPoint c){  A=a;B=b;C=c;  }
	double powierzchnia();
    atWektor normalny();
    double polozenie(atPoint P);
    double odleglosc(atPoint P){return fabs(polozenie(P)); }
    atPoint p1(){return A;}
    atPoint p2(){return B;}
    atPoint p3(){return C;}
    bool rowny(atTrojkat T);

    QString toString(){     return A.toString()+"\n"+B.toString()+"\n"+C.toString()+"\n"; }
    QString toString(QString b,QString p,QString e){     return b+ A.toString()+p+B.toString()+p+C.toString()+e; }

};

//-------------------------------------------
class atWektor: public atPoint{
 private:
 public:
	atWektor(double xx,double yy,double zz):atPoint(xx,yy,zz) {}
	atWektor(atPoint a, atPoint b):atPoint(b.x()-a.x(),b.y()-a.y(), b.z()-a.z() ){}
	atWektor(atPoint a):atPoint(a.x(),a.y(), a.z() ){}
	atWektor(atWektor a, atWektor b):atPoint(a.y()*b.z()-a.z()*b.y(), a.z()*b.x()-b.z()*a.x(), a.x()*b.y()-a.y()*b.x()) {} //iloczyn wektorowy
	void dodaj(atWektor w){X+=w.x();Y+=w.y();Z+=w.z();}
	void odejmij(atWektor w){X-=w.x();Y-=w.y();Z-=w.z();}
	double iloczyn_skalarny(atWektor w){ return X*w.x()+Y*w.y()+Z*w.z(); }
	double modul(){return sqrt(X*X+Y*Y +Z*Z);}
    void normalizuj();
    bool rowny(  atWektor w, double d ){ return atPoint::rowny(w,d) && zgodny(w); }
    bool zgodny(atWektor w){return iloczyn_skalarny(w)>0;}

};


//-------------------------------------------
class atCzworoscian{
 private:
	atPoint A;
	atPoint B;
	atPoint C;
	atPoint D;


 public:
	atCzworoscian(atPoint a,atPoint b,atPoint c,atPoint d){A=a;B=b;C=c;D=d;}
	atCzworoscian(atWektor a,atWektor b,atWektor c){A=a;B=b;C=c;} //wektory zrzutowane do punktów, a D zerowy
	bool Error(){return A.rowny(B)||A.rowny(C)||A.rowny(D)||B.rowny(C)||B.rowny(D)||C.rowny(D);}
    double przestrzen();
    double objetosc(){return fabs(przestrzen());}
    atPoint p1(){return A;}
    atPoint p2(){return B;}
    atPoint p3(){return C;}
    atPoint p4(){return D;}

    QString toString(){     return A.toString()+"\n"+B.toString()+"\n"+C.toString()+"\n"+D.toString()+"\n"; }

};

#endif // ATGeometria_H
