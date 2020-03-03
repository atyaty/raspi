#ifndef ATFILESTL_H
#define ATFILESTL_H

#include <QVector>
#include <QString>
#include <QStringList>
#include <QMessageBox>

#include "atFile.h"
#include "atGeometria.h"

class atVertex;
typedef  atVertex  atVertex;

class atFacet;
typedef  atFacet  atFacet;

class atBulk;
typedef  atBulk  atBulk;

//-------------------------------------------------------------------------------
class atVertex : public atPoint {
protected:
    QVector <atFacet *> facet;  //ściany wychodzące z tego punktu
    QVector <atFacet *> bfacet; //ściany wychodzące z tego punktu przynależne do bulk'a
    QVector <atBulk *> bulk;    //przestrzenie wychodzące z tego punktu

    QString smsg;
public:
    atVertex(double x,double y,double z):atPoint(x,y,z){}

    void addFacet(atFacet *f);
    void removeFacet(atFacet *f);
    void moveFacet(atFacet *f);
    void moveFacet(int i);

    int facets(){return facet.size();}
    atFacet *Facet(int i){return facet.at(i);}
    bool getFacet(atVertex *v1,atVertex *v2,atFacet **f);
    bool findFacet(atVertex *v1,atVertex *v2,atFacet **f);
    bool maFacet(atFacet *f);
    bool orientacja(atFacet *f0,atFacet *f1);
    int indexFacet(atFacet *f);

    void addBulk(atBulk *b);
    void removeBulk(atBulk *b);

    int bulks(){return bulk.size();}
    atBulk *Bulk(int i){return bulk.at(i);}
    bool getBulk(atVertex *v1,atVertex *v2,atVertex *v3,atBulk **b);
    bool getBulk(atFacet *f,atBulk **b);
    bool findBulk(atVertex *v1,atVertex *v2,atVertex *v3,atBulk **b);
    bool findBulk(atFacet *f,atBulk **b);
    bool maBulk(atBulk *b);
    int indexBulk(atBulk *b);

    bool wypukly();
    bool zamkniety();
    atVertex *outerVertex(atFacet *f);
    atFacet podstawa();
    atFacet podstawa(atFacet *f0, atFacet *f1);
    atFacet trzecia_sciana(atFacet *f0, atFacet *f1);
    int commonIndexFacet( atVertex *v );
    int commonIndexFacet( atVertex *v, atFacet *f );

    QString facetsToString(){ zamkniety(); return smsg; }
    QString Message(){return smsg;}

};



//---------------------------------------------------------------------------------
class atFacet : public atTrojkat {
protected:
    atVertex *vertex1;
    atVertex *vertex2;
    atVertex *vertex3;
public:
    atFacet(atVertex *v1,atVertex *v2,atVertex *v3);
    ~atFacet();

    atVertex *Vertex1(){return vertex1;}
    atVertex *Vertex2(){return vertex2;}
    atVertex *Vertex3(){return vertex3;}
    atVertex *Vertex(int i){if(i==3) return vertex3; else if(i==2) return vertex2; else return vertex1;}
    bool maVertex(atVertex *v){return v==vertex1 || v==vertex2 || v==vertex3;} //sprawdza czy facet ma taki vertex
    atVertex *outerVertex(atFacet *f); //zwraca vjeden z verteksów faceta f, ten który nie należy do naszego faceta
    int commonIndexVertex(atFacet *f,atVertex *v); //wspólny vertex inny niż v (zwracamy nasz index)
    int commonVertexes(atFacet *f); //zlicza ile wspólnych Verteksów mają dwa facety
    int indexVertex(atVertex *v); //podaje indev verteksa vf
    bool wypukly(atFacet *f);
    bool orientacja(atFacet *f); //sprawdza czy facet f ma tę samą orientację co my. sprawdzenie ma sens gdy may przynajmniej jedną krawędź wspólną
    void odwroc(){atVertex *v=vertex1; vertex1=vertex2; vertex2=v; atPoint D=A; A=B; B=D; }
    bool jednaki(atFacet *f){return jednaki(f->Vertex1(),f->Vertex2(),f->Vertex3());}
    bool jednaki(atVertex *v1,atVertex *v2,atVertex *v3){ return (vertex1==v1 && vertex2==v2 && vertex3==v3) || (vertex1==v2 && vertex2==v3 && vertex3==v1) ||  (vertex1==v3 && vertex2==v1 && vertex3==v2) ; }
    bool prawiejednaki(atVertex *v1,atVertex *v2,atVertex *v3){return maVertex(v1) && maVertex(v2) && maVertex(v3);}
    bool prawiejednaki(atFacet *f){return prawiejednaki(f->Vertex1(),f->Vertex2(),f->Vertex3());}


    QString toString(atVertex *v0);
    QString toString(){return atTrojkat::toString();}

};


//---------------------------------------------------------------------------------
class atBulk : public atCzworoscian {
protected:
    atVertex *vertex1;
    atVertex *vertex2;
    atVertex *vertex3;
    atVertex *vertex4;
public:
    atBulk(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4);
    atBulk(atFacet *f,atVertex *v);
    ~atBulk();


    atVertex *Vertex1(){return vertex1;}
    atVertex *Vertex2(){return vertex2;}
    atVertex *Vertex3(){return vertex3;}
    atVertex *Vertex4(){return vertex4;}
    bool maVertex(atVertex *v){return v==vertex1 || v==vertex2 || v==vertex3 || v==vertex4;} //sprawdza czymamy vertex v

    bool jednaki(atBulk *b){return jednaki(b->Vertex1(),b->Vertex2(),b->Vertex3(),b->Vertex4());}
    bool jednaki(atFacet *f,atVertex *v){return jednaki(f->Vertex1(),f->Vertex2(),f->Vertex3(),v);}
    bool jednaki(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4);
    bool _jednaki(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4);

    bool prawiejednaki(atBulk *b){return prawiejednaki(b->Vertex1(),b->Vertex2(),b->Vertex3(),b->Vertex4());}
    bool prawiejednaki(atFacet *f,atVertex *v){return prawiejednaki(f->Vertex1(),f->Vertex2(),f->Vertex3(),v);}
    bool prawiejednaki(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4){return maVertex(v1) && maVertex(v2) && maVertex(v3) && maVertex(v4);}

};



//---------------------------------------------------------------------------------
class atFileSTL : public atFile{
protected:

    QStringList Elements;
    QVector <atVertex *> v;
    QVector <atFacet *> f;
    QVector <atBulk *> b;
    QVector <atVertex *> vb; //verteksy bulka. przeniesione z v

    QString smsg;
    bool serr;
    double normalny_chk; //wskaźnik poprawności wektora normalnego przy odczycie z pliku STL

    atVertex * getVertex(QString s);
    atWektor getWektor(QString s);

    bool Process1_1(atVertex *v0);
    bool Process1_2(atVertex *v0, atFacet *f0, atFacet *f1);
    void addVertex( atVertex *v0);
    void addFacet( atFacet *f0 );
    void addBulk( atBulk *b0 );
//    bool addBulk( atVertex *v0, atFacet *f0, atFacet *f1 );
//    void moveVertex(int i);
//    void moveVertex(atVertex *vv);
    void moveFacet(atFacet *ff);
    void insertVertexByFacets(atVertex *v0);
    void insertVertexByBulks(atVertex *v0);
    void removeVertex(atVertex *v0);

public:
    atFileSTL();
    ~atFileSTL();
    void Ini();
    void Set_normalny_chk(double n){normalny_chk=n;}

    void Process0();
    void Process1();
    void Scale(double f); //skalowanie objektu, funkcja mboży współrzędne każdego z punktów przez faktor f
    bool Czy_zamkniete_vertexy();
    int ElementsSize(){return Elements.size();}
    int vSize(){return v.size();}
    int fSize(){return f.size();}
    int vbSize(){return vb.size();}
    int bSize(){return b.size();}
    bool Error(){return serr;}
    QString Message(){return smsg;}

    double Volume();
    QString vToString();
    void Save(QString savefile);
};

#endif // ATFILESTL_H
