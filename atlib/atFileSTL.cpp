#include "atFileSTL.h"


//====================================================================================
//
//     Vertex
//
//====================================================================================

//------------------------------------------------------------------------------------
void atVertex::addFacet(atFacet *f){    facet.push_back(f); }
//------------------------------------------------------------------------------------
void atVertex::removeFacet(atFacet *f){   for(int i=0;i<facet.size();i++){  if(facet.at(i)==f){ facet.remove(i);  break; }    }  }
//------------------------------------------------------------------------------------
void atVertex::moveFacet(atFacet *f){   for(int i=0;i<facet.size();i++){  if(facet.at(i)==f){  moveFacet(i);  break; }    }  }
//------------------------------------------------------------------------------------
void atVertex::moveFacet(int i){  bfacet.push_back(facet.at(i)); facet.remove(i); }
//------------------------------------------------------------------------------------
void atVertex::addBulk(atBulk *b){    bulk.push_back(b);  }
//------------------------------------------------------------------------------------
void atVertex::removeBulk(atBulk *b){   for(int i=0;i<bulk.size();i++){  if(bulk.at(i)==b){ bulk.remove(i);  break; }    }  }
//------------------------------------------------------------------------------------
bool atVertex::wypukly(  ){
    return facet.size()==3 &&
           facet.at(0)->wypukly( facet.at(1) ) &&
           facet.at(2)->maVertex(facet.at(0)->outerVertex(facet.at(1))) &&
           facet.at(2)->maVertex(facet.at(1)->outerVertex(facet.at(0))) ;
}
//------------------------------------------------------------------------------------
bool atVertex::zamkniety(  ){
    smsg="\n"+toString()+"      facets("+QString::number(facets())+"):\n";
    if(facets()<=0){  return false;  }
    QVector <atFacet *> fac;
    for(int i=0;i<facet.size();i++){ fac.push_back(facet.at(i)); }
    atFacet *f0=fac.at(0);
    while(fac.size()>0){
        int j;
        bool found=false;
        for(j=1;j<fac.size();j++){
            if( fac.at(0)->orientacja( fac.at(j) ) ) {
                smsg+=fac.at(0)->toString(this)+"\n";
                fac.replace(0,fac.at(j));
                fac.remove(j);
                found=true;
                break;
            }
        }
        if(!found) break;
    }
    if( fac.size()==1 && fac.at(0)->orientacja( f0 ) ){smsg+=fac.at(0)->toString(this)+"\n"; return true;}
    smsg+="---------------------\n";
    for(int i=0;i<fac.size();i++){ smsg+=fac.at(i)->toString(this)+"\n"; }
    return false;
}
//------------------------------------------------------------------------------------
//zwraca vertex z faceta f, który nie jest naszym vertexem
atVertex *atVertex::outerVertex( atFacet *f){
    if( f->Vertex1()!=this ) return f->Vertex1();
    if( f->Vertex2()!=this ) return f->Vertex2();
    if( f->Vertex3()!=this ) return f->Vertex3();
    return NULL;
}
//------------------------------------------------------------------------------------
bool atVertex::maFacet(atFacet *f ){
    for(int i=0; i<facet.size();i++){ if(facet.at(i)==f) return true; }
    return false;
}
//------------------------------------------------------------------------------------
bool atVertex::getFacet( atVertex *v1, atVertex *v2, atFacet **f ){
    for(int i=0; i<facet.size();i++){ if(facet.at(i)->jednaki(v1,v2,this)) { *f=facet.at(i);return true;} }
    return false;
}
//------------------------------------------------------------------------------------
bool atVertex::findFacet( atVertex *v1, atVertex *v2, atFacet **f ){
    for(int i=0; i<facet.size();i++){ if(facet.at(i)->prawiejednaki(v1,v2,this)) { *f=facet.at(i);return true;} }
    return false;
}
//------------------------------------------------------------------------------------
int atVertex::indexFacet(atFacet *f ){
    for(int i=0; i<facet.size();i++){ if(facet.at(i)==f) return i; }
    return 0;
}
//------------------------------------------------------------------------------------
//zwraca facet (index) z vertexa, który jest wspólny z naszym vertexem ale inny niź f
int atVertex::commonIndexFacet( atVertex *v,atFacet *f){
    for(int i=0;i<facet.size();i++){
        if( facet.at(i)!=f && facet.at(i)->maVertex(v)) return i;
     }
    return -1;
}
//------------------------------------------------------------------------------------
//zwraca facet (index) z vertexa, który jest wspólny z naszym vertexem
int atVertex::commonIndexFacet( atVertex *v){
    for(int i=0;i<facet.size();i++){        if( facet.at(i)->maVertex(v)) return i;     }
    return -1;
}

//------------------------------------------------------------------------------------
bool atVertex::maBulk(atBulk *b ){
    for(int i=0; i<bulk.size();i++){ if(bulk.at(i)==b) return true; }
    return false;
}
//------------------------------------------------------------------------------------
bool atVertex::getBulk( atVertex *v1, atVertex *v2, atVertex *v3, atBulk **b ){
    for(int i=0; i<bulk.size();i++){ if(bulk.at(i)->jednaki(v1,v2,v3,this)) { *b=bulk.at(i);return true;} }
    return false;
}
//------------------------------------------------------------------------------------
bool atVertex::getBulk(atFacet *f,atBulk **b){  return getBulk(f->Vertex1(), f->Vertex2(),f->Vertex3(),b); }
//------------------------------------------------------------------------------------
bool atVertex::findBulk( atVertex *v1, atVertex *v2, atVertex *v3, atBulk **b ){
    for(int i=0; i<bulk.size();i++){ if(bulk.at(i)->prawiejednaki(v1,v2,v3,this)) { *b=bulk.at(i);return true;} }
    return false;
}
//------------------------------------------------------------------------------------
bool atVertex::findBulk(atFacet *f,atBulk **b){return findBulk(f->Vertex1(),f->Vertex2(),f->Vertex3(),b);}
//------------------------------------------------------------------------------------
int atVertex::indexBulk(atBulk *b ){
    for(int i=0; i<bulk.size();i++){ if(bulk.at(i)==b) return i; }
    return 0;
}
//-----------------------------------------------------------------------------------
// orientacja facetów danego Vertexa
//..uwzględnia kierunek
// obydwa facety mają orientowane są według wierzchołka którym jest nasz Vertex
// drugi wspólny punkt krawędzi faceta ma kolejność: "za" albo "przed" wierzchołkiem
// tru zwracamy tylko wtedy gdy pierwszy parametr ma kolejność "za" czyli next a pierwszy ma kolejność "przed" czyli prev
//
bool atVertex::orientacja( atFacet *f0, atFacet *f1){
    int t0=f0->indexVertex(this);
    int t1=f1->indexVertex(this);
    if( t0<=0 || t1<=0) return false;
    atVertex *n0 = (t0==1?f0->Vertex2():(t0==2?f0->Vertex3():f0->Vertex1()));
    atVertex *p1 = (t1==3?f1->Vertex2():(t1==2?f1->Vertex1():f1->Vertex3()));
    if( n0==p1 ) return true;
    return false;
}








//====================================================================================
//
//           Facet
//
//====================================================================================

//------------------------------------------------------------------------------------
atFacet::atFacet(atVertex *v1,atVertex *v2,atVertex *v3):atTrojkat(*v1,*v2,*v3){
    if(v1!=NULL){ vertex1=v1; vertex1->addFacet(this); }
    if(v2!=NULL){ vertex2=v2; vertex2->addFacet(this); }
    if(v3!=NULL){ vertex3=v3; vertex3->addFacet(this); }
}
//------------------------------------------------------------------------------------
atFacet::~atFacet(){
    vertex1->removeFacet(this);
    vertex2->removeFacet(this);
    vertex3->removeFacet(this);
}
//------------------------------------------------------------------------------------
//zwraca vertex z faceta f, który nie jest wspólny z naszym facetem
atVertex *atFacet::outerVertex( atFacet *f){
    if( !maVertex(f->Vertex1()) ) return f->Vertex1();
    if( !maVertex(f->Vertex2()) ) return f->Vertex2();
    if( !maVertex(f->Vertex3()) ) return f->Vertex3();
    return NULL;
}
//-----------------------------------------------------------------------------------
//ilość wspólnych vertexów
int atFacet::commonVertexes( atFacet *f){
    int n=0;
    if( maVertex(f->Vertex1()) ) n++;
    if( maVertex(f->Vertex2()) ) n++;
    if( maVertex(f->Vertex3()) ) n++;
    return n;
}
//-----------------------------------------------------------------------------------
//zwraca index vertexa (1,2,3)
int atFacet::indexVertex( atVertex *v){
    if( vertex1==v ) return 1;
    if( vertex2==v ) return 2;
    if( vertex3==v ) return 3;
    return 0;
}
//------------------------------------------------------------------------------------
//zwraca vertex (index) z faceta f, który jest wspólny z naszym facetem ale inny niź v
int atFacet::commonIndexVertex( atFacet *f, atVertex *v){
    if( vertex1!=v && f->maVertex(vertex1) ) return 1;
    if( vertex2!=v && f->maVertex(vertex2) ) return 2;
    if( vertex3!=v && f->maVertex(vertex3) ) return 3;
    return 0;
}
//-----------------------------------------------------------------------------------
//sprwdzenie czy f ma tę samą orientację co nasza. (czy jego wektor normalny wychodzi na tę samą stronę przestrzeni co nasz)
//pytamie o orientację ma sens tylko wtedy gdy facety stykają się przynajmniej jedną krawędzią!!
//szukamy w facecie f dwóch vertexów naszych. jeśli f zawiera dwa nasze vertexy to mamy wspólną krawędź
//i mozemy określić określić orientację względem nas.
//(zakładamy że kolejność verteksów jest nieprzypadkowa i wynika z niej kierunek wektora normalnego!!)
bool atFacet::orientacja( atFacet *f){
    int i1=f->indexVertex(vertex1);                     //indeks pierwszego naszego verteksa w f
    int i2=f->indexVertex(vertex2);                     //index drugiego vertexa
    if( i1==0 && i2==0) return false;                   //brak dwóch oznacza brak wspólnej krawędzi
    else if(i1==0){i1=i2; i2=f->indexVertex(vertex3);}  //brak pierwszego - sprawdzamy 3 i ustawiamy wo odpowiedniej kolejności
    else if(i2==0){i2=i1; i1=f->indexVertex(vertex3);}  //brak drugiego - jw
    if( i1==0 || i2==0) return false;                   //brak wspólnej krawędzdi
    int i = i2-i1;                                      //wskaźnik i mówi nam w jakiej kolejności ułożone są vertexy faceta f
    return (i==-1 || i==2);                             //orientacja zgodna z naszą jest wtedy gdy verteksy f na wspólnej krawędzi ułożone są w odwrotnej kolejności niż nasze
}
//-----------------------------------------------------------------------------------
//dwa facety są wypukłe jeżeli:
//-mają zgosną orientację ( a co za tym idzie dwa wspólne vertexy)
//-przestrzeń którą "zamykają" znajduje się wewnątrz bryły, czli "położenie" trzeciego vertexa pierwszego faceta jest ujemne (lub zerowe) względem drugiego faceta
//powyższe zdnie ma sens taki:
//wektor normalny faceta skierowany jest na zewnątrz bryły
//połozenie punktu (vertexa) względem faceta oznacza "odległość ze znakiem" tego punktu od płaszczyzny zawierającej facet,
//przy czym odległość ma znak + gdy punkt leży po stronie zgodnej z wektorem normalnym, - gdy po przeciwnej
//innymi słowy:
//stykające się krawędzią facety w ogólności wyznaczją pewien czworościan, czyli zamykają przestrzeń jeśli wektory normalne skierowane są na zewnżtrz czworościanu
//jeśli wektory normalne skierowane są do wewnątrz czworościanu - facety "zamykają" nieskończoność
bool atFacet::wypukly( atFacet *f){ return  orientacja( f )  &&  polozenie(*(outerVertex(f)))<=0 ; }
//-----------------------------------------------------------------------------------
QString atFacet::toString(atVertex *v0){
    if(vertex1==v0) return vertex2->toString()+"    "+vertex3->toString();
    else if(vertex2==v0) return vertex3->toString()+"    "+vertex1->toString();
    else if(vertex3==v0) return vertex1->toString()+"    "+vertex2->toString();
    return vertex1->toString()+"    "+vertex2->toString()+"    "+vertex3->toString();
}






//====================================================================================
//
//             Bulk
//
//====================================================================================

//------------------------------------------------------------------------------------
atBulk::atBulk(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4):atCzworoscian(*v1,*v2,*v3,*v4){
    vertex1=v1; vertex1->addBulk(this);
    vertex2=v2; vertex2->addBulk(this);
    vertex3=v3; vertex3->addBulk(this);
    vertex4=v4; vertex4->addBulk(this);
}
//------------------------------------------------------------------------------------
atBulk::atBulk(atFacet *f,atVertex *v):atCzworoscian(*(f->Vertex1()),*(f->Vertex2()),*(f->Vertex3()),*v){
    vertex1=f->Vertex1(); vertex1->addBulk(this);
    vertex2=f->Vertex2(); vertex2->addBulk(this);
    vertex3=f->Vertex3(); vertex3->addBulk(this);
    vertex4=v; vertex4->addBulk(this);
}
//------------------------------------------------------------------------------------
atBulk::~atBulk(){
    vertex1->removeBulk(this);
    vertex2->removeBulk(this);
    vertex3->removeBulk(this);
    vertex4->removeBulk(this);
}
//------------------------------------------------------------------------------------
bool atBulk::jednaki(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4){
    return _jednaki(v1,v2,v3,v4) || _jednaki(v2,v1,v4,v3) || _jednaki(v3,v4,v1,v2) || _jednaki(v4,v3,v2,v1) ;
}
//------------------------------------------------------------------------------------
bool atBulk::_jednaki(atVertex *v1,atVertex *v2,atVertex *v3,atVertex *v4){
    return vertex4==v4 && ( (vertex1==v1 && vertex2==v2 && vertex3==v3) || (vertex1==v2 && vertex2==v3 && vertex3==v1) ||  (vertex1==v3 && vertex2==v1 && vertex3==v2) ) ;
}

//====================================================================================
//
//            STL
//
//====================================================================================

//------------------------------------------------------------------------------------
atFileSTL::atFileSTL():atFile(){normalny_chk=0.0001;}
//------------------------------------------------------------------------------------
atFileSTL::~atFileSTL(){ Ini(); }
//------------------------------------------------------------------------------------
void atFileSTL::Ini(){
    for(int i=b.size()-1;i>=0;i--){ delete b.at(i); b.remove(i);}
    for(int i=f.size()-1;i>=0;i--){ delete f.at(i); f.remove(i);}
    for(int i=v.size()-1;i>=0;i--){ delete v.at(i); v.remove(i);}
    for(int i=vb.size()-1;i>=0;i--){ delete vb.at(i); vb.remove(i);}
    serr=false;
}
//------------------------------------------------------------------------------------
//ta funkcja zamienia dane z pliku które wstępnie są na zmiennej Filetekst na objekty
void atFileSTL::Process0(){
   Ini();
   serr=false;
   Elements=Filetext.split("endfacet",QString::SkipEmptyParts); //najpierw zapisujemy objekty w postaci tekstowej do tablicy Elements
   for(int i=0;i<Elements.size();i++){ //w tej pętli zamieniamy elementy tekstowe z tablicy Elements reprezentujące trójkątne ściany na objekty atFacet
     Elements.replace(i,getAfter(Elements.at(i),"facet"));
     if(Elements.at(i)!=""){
       QString normalny=getPole(Elements.at(i),"normal","\n");
       QString v1=getAfter(Elements.at(i),"vertex");
       QString v2=getAfter(v1,"vertex");
       QString v3=getAfter(v2,"vertex");
       v1=getBefore(v1,"\n");
       v2=getBefore(v2,"\n");
       v3=getBefore(v3,"\n");
       addFacet( new atFacet(getVertex(v1),getVertex(v2),getVertex(v3)) ); //objekty atFacet zapisywane są do tablicy f
                                                                              //wołane tu funkcje getVertex generują objekty atVertex, czyli punkty z których konstruowana jest ściana
                                                                              //objekty atVertex zapisane są w tablicy v

       //----------------------------------------------------------------------------------
       //to poniżej jest tylko sprawdzeniem poprawności danych wygenerowanych na podst pliku
       //..sprawdzamy czy wektor normalny odczytany z pliku jest zgodny z wyliczonym na podst punktów trójkąta
       atWektor n1=f.at(f.size()-1)->normalny(); //wektor normalny obliczony z punktów
       atWektor n2=getWektor(normalny);          //wektor normalny odczytany
       if(!n1.rowny(n2,normalny_chk) || !n1.zgodny(n2) ){
           smsg+="\n---------------------------------------\nElement: "+QString::number(i)+"/"+QString::number(Elements.size())+
                 "\n"+Elements.at(i)+
                 "\np1="+f.at(f.size()-1)->p1().toString()+
                 "\np2="+f.at(f.size()-1)->p2().toString()+
                 "\np3="+f.at(f.size()-1)->p3().toString()+
                 "\n n1(zpunktow)="+n1.toString()+
                 "\nn2(normalny odczytany)="+n2.toString()+
                 "\niloczyn skalarny n1*n2: "+QString::number(n1.iloczyn_skalarny(n2));
           serr=true;
       }
       //----------------------------------------------------------------------------------
     }
   }
}
//--------------------------------------------------------
// ta funkcja zamienia bryłę złożoną z wielu ścian na wiele czworościanów czyli objektów atBulk
//
// pętla wykonywana jest na tablicy v która w trakcie procesu jest modyfikowana.
// w momencie gdy tablica v ulega zmianie (Process1_1 zwraca true) resetujemy pętlę
//
// można też na to spojrzeć w ten sposób, że pętla służy nam do tego by znaleźć pierwszy nadający się do zamiany objekt
//
void atFileSTL::Process1(){
    int i=v.size()-1;
    while(i>=0){
        if( Process1_1(v.at(i)) ) { i=v.size()-1; }
        else if(serr){ return; }
        else { i--; } //ten przypadek chyba nie występuje, a jeśli wystąpi to powinien byc chyba potraktowany jako: serr=true ..a nie i--
    }
}
//--------------------------------------------------------
// sprawdzenie Vertexa czyli wierzchołka, podwójna pętla po ścianach należących do verteksa
// pętla wybierająca dwie ściany i uruchamia dla nich następną procedurę sprawdzenia
bool atFileSTL::Process1_1(atVertex *v0){
       serr=false;
       if( v0->facets()<3 ){  smsg="błąd, Process1, "+v0->toString( ) +"\nfacets: "+QString::number( v0->facets()  );   serr=true;  return false;  }
       for(int n=0; n<v0->facets();n++){
            atFacet *f0= v0->Facet(n);
            for(int j=n+1; j<v0->facets();j++){
                  atFacet *f1= v0->Facet(j);
                  bool ret=Process1_2(v0,f0,f1);
                  //QMessageBox::warning(NULL,"Uwaga",smsg,QMessageBox::Ok, QMessageBox::Ok);
                  if( ret )  return true;
                  else if(serr) return false;
            }
       }
       return false;
}
//--------------------------------------------------------
// Badanie dwóch ścian należących do verteksa..
// funkcja zwraca true jeśli ściany zostały pomyślnie zredukowane czyli gdy wycięto wyznaczoną przez nie przestrzeń i utworzono Bulk
// w pozostałych przypadkach zwraca false
// na zmienną smsg zapisywany jest raport z działania funkcji.. ważne na etapie debuga,potem można to wywalić
bool atFileSTL::Process1_2(atVertex *v0, atFacet *f0, atFacet *f1 ){

    smsg="";
    //to odremować do debugowania
    //smsg+="start:     "+v0->facetsToString()+"\nf0:  "+f0->toString(v0)+"\nf1:  "+f1->toString(v0);

    if(  !f0->orientacja( f1 ) ){ smsg+="\n---orientacja BE"; return false; }

    atVertex *v1=f1->outerVertex(f0);                                   //vertexy skrajne
    atVertex *v2=f0->outerVertex(f1);                                   //..
    atVertex *v3=f0->Vertex( f0->commonIndexVertex(f1,v0) );            //wspólny Vertex (drugi oprócz wierzchołka v0)

    //to odremować do debugowania
    //smsg+="\nv1 "+v1->toString()+" facets("+QString::number(v1->facets())+")";
    //smsg+="\nv2 "+v2->toString()+" facets("+QString::number(v2->facets())+")";
    //smsg+="\nv3 "+v3->toString()+" facets("+QString::number(v3->facets())+")";

    atFacet *ft=NULL; v0->findFacet(v1,v2,&ft);                         //trzecia ściana ..szukamy ściany należącej do v0 zawierającej v1 i v2
    atFacet *fp=NULL; v3->findFacet(v1,v2,&fp);                         //podstawa  ..szukamy ściany należącej do v3 zawierającej v1 i v2

    if( ft==NULL && fp==NULL && v1->commonIndexFacet(v2)>=0 ){  smsg+="\n---jest sciana pomiedzy v1 z v2";  return false;    }
    atBulk *b0=NULL; v0->findBulk(v1,v2,v3,&b0);
    if(b0!=NULL){  smsg+="\n---jest już Bulk:\n"+b0->toString(); return false;  }

    if( ft==NULL ){                                                     //jeśli nie znaleziono ściany to ją dodajemy
        ft=new atFacet(v1,v2,v0);  addFacet(ft);                        //dorabiamy trzecią ścianę
        atFacet *ftd=new atFacet(v1,v2,v0); addFacet(ftd);              //dorabiamy lustro
        if( ! ft->orientacja(f0) ) ft->odwroc();  else ftd->odwroc();   //odwracamy odpowiednio
    }
    else if( ! ft->orientacja(f0) ){  smsg="błąd STL, orientacja ft"+ft->toString(); serr=true; return false; }

    if( fp==NULL ){                                                     //jeśli nie znaleziono ściany
        fp=new atFacet(v1,v2,v3); addFacet(fp);                         //dorabiamy podstawę
        atFacet *fpd=new atFacet(v1,v2,v3);addFacet(fpd);               //dorabiamy lustro
        if( ! fp->orientacja(f0) ) fp->odwroc(); else fpd->odwroc();    //odwracamy odpowiednio
    }
    else if( ! fp->orientacja(f0) ){ smsg="błąd STL, orientacja fp"+fp->toString(); serr=true; return false;  }

    //to odremować do debugowania
    //smsg+="\nft:"+ft->toString(v0)+"\nfp:"+fp->toString(v0);

    moveFacet(fp);   moveFacet(ft);   moveFacet(f0);   moveFacet(f1);
    addBulk(new atBulk(fp,v0));

    //to odremować do debugowania
    //smsg+="\n\nKoniec: "+v0->facetsToString();
    //smsg+="\nv1 "+v1->toString()+" facets("+QString::number(v1->facets())+")";
    //smsg+="\nv2 "+v2->toString()+" facets("+QString::number(v2->facets())+")";
    //smsg+="\nv3 "+v3->toString()+" facets("+QString::number(v3->facets())+")";
    //smsg+="\nBulk:\n "+(b.at(b.size()-1))->toString();
    //smsg+="\nBulk vol:\n "+QString::number((b.at(b.size()-1))->przestrzen());
    //smsg+="\n\nBVertex tab size: "+QString::number(vb.size());
    //smsg+="\nBulk tab size: "+QString::number(b.size());
    //smsg+="\nVertex tab: "+ vToString();

    return true;
}
//--------------------------------------------------------
void atFileSTL::moveFacet( atFacet *ff ){
    ff->Vertex1()->moveFacet(ff);
    ff->Vertex2()->moveFacet(ff);
    ff->Vertex3()->moveFacet(ff);
    for(int i=0;i<f.size();i++){     if(f.at(i)==ff){  f.remove(i);  break;        }    }
}
//-----------------------------------------------------------
atVertex * atFileSTL::getVertex(QString s){
    QStringList sL = s.trimmed().split(" ",QString::SkipEmptyParts);
    double x=0;
    double y=0;
    double z=0;
    if(sL.size()>=1){ x= sL.at(0).toFloat();  }
    if(sL.size()>=2){ y= sL.at(1).toFloat();  }
    if(sL.size()>=3){ z= sL.at(2).toFloat();  }
    atPoint p=atPoint( x, y, z  );
    for(int i=0;i<v.size();i++){ if(v.at(i)->rowny(p)) return v.at(i);}
    addVertex( new atVertex(x,y,z) );
    return v.at(v.size()-1);
}
//-----------------------------------------------------------
atWektor atFileSTL::getWektor(QString s){
    QStringList sL = s.trimmed().split(" ",QString::SkipEmptyParts);
    double x=0;
    double y=0;
    double z=0;
    if(sL.size()>=1){ x= sL.at(0).toFloat();  }
    if(sL.size()>=2){ y= sL.at(1).toFloat();  }
    if(sL.size()>=3){ z= sL.at(2).toFloat();  }
    return atWektor( x, y, z  );
}
//--------------------------------------------------------
double atFileSTL::Volume(){
    double vol=0;
    for(int i=0;i<b.size();i++){   vol+=b.at(i)->przestrzen();    }
    return vol;
}
//--------------------------------------------------------
bool atFileSTL::Czy_zamkniete_vertexy(){
    smsg="";
    serr=false;
    for(int i=0;i<v.size();i++){   if(!v.at(i)->zamkniety()){ smsg+=v.at(i)->Message(); serr=true;}    }
    return !serr;
}
//--------------------------------------------------------
void atFileSTL::addVertex(atVertex *v0){ insertVertexByFacets(v0); }

//--------------------------------------------------------
//void atFileSTL::moveVertex( int i ){ vb.push_back(v.at(i)); v.remove(i); }
//--------------------------------------------------------
void atFileSTL::insertVertexByBulks( atVertex *v0 ){  vb.push_back(v0);   }
//--------------------------------------------------------
// ta funkcja dodaje wprawdzie faceta do tablicy f
// ale musimy z tego powodu uporządkować tablicę v
void atFileSTL::addFacet(atFacet *f0){
    f.push_back(f0);
    removeVertex(f0->Vertex1());
    removeVertex(f0->Vertex2());
    removeVertex(f0->Vertex3());
    insertVertexByFacets(f0->Vertex1());
    insertVertexByFacets(f0->Vertex2());
    insertVertexByFacets(f0->Vertex3());
}
//--------------------------------------------------------
void atFileSTL::addBulk(atBulk *b0){
    b.push_back(b0);
    removeVertex(b0->Vertex1());
    removeVertex(b0->Vertex2());
    removeVertex(b0->Vertex3());
    removeVertex(b0->Vertex4());
    if(b0->Vertex1()->facets()==0) insertVertexByBulks(b0->Vertex1()); else insertVertexByFacets(b0->Vertex1());
    if(b0->Vertex2()->facets()==0) insertVertexByBulks(b0->Vertex2()); else insertVertexByFacets(b0->Vertex2());
    if(b0->Vertex3()->facets()==0) insertVertexByBulks(b0->Vertex3()); else insertVertexByFacets(b0->Vertex3());
    if(b0->Vertex4()->facets()==0) insertVertexByBulks(b0->Vertex4()); else insertVertexByFacets(b0->Vertex4());
//    if(b0->Vertex1()->facets()==0) vb.push_back(b0->Vertex1()); else insertVertexByFacets(b0->Vertex1());
//    if(b0->Vertex2()->facets()==0) vb.push_back(b0->Vertex2()); else insertVertexByFacets(b0->Vertex2());
//    if(b0->Vertex3()->facets()==0) vb.push_back(b0->Vertex3()); else insertVertexByFacets(b0->Vertex3());
//    if(b0->Vertex4()->facets()==0) vb.push_back(b0->Vertex4()); else insertVertexByFacets(b0->Vertex4());
}
//--------------------------------------------------------
void atFileSTL::removeVertex(atVertex *v0){
    for(int i=v.size()-1;i>=0;i--){ if(v.at(i)==v0 ) { v.remove(i);break;}  }
}
//--------------------------------------------------------
void atFileSTL::insertVertexByFacets(atVertex *v0){
    for(int i=v.size()-1;i>=0;i--){
        if( v0->facets()<=v.at(i)->facets() ){
            v.insert( i+1,v0 );
            return;
        }
    }
    v.insert( 0,v0 );
}
//--------------------------------------------------------
QString atFileSTL::vToString(){
    QString smsg="";
    for(int i=0; i<v.size(); i++ ){
        smsg+="\n"+v.at(i)->toString()+" "+QString::number(v.at(i)->facets());
    }
    return smsg;
}
//--------------------------------------------------------
void atFileSTL::Scale(double factor){
    for(int i=v.size()-1;i>=0;i--){
         v.at(i)->pomnoz(factor);
         int a=i;
    }
}
//--------------------------------------------------------
void atFileSTL::Save(QString savefile){
    QString s="solid atSTL_Convert\n";
    for(int i=0;i<f.size();i++){
        atWektor n1=f.at(i)->normalny();
        s+="  facet normal "+n1.toString(""," ","\n");
        s+="    outer loop\n";
        atPoint* p=f.at(i)->Vertex1();
        s+="      vertex " + f.at(i)->Vertex1()->toString(""," ","\n");
        s+="      vertex " + f.at(i)->Vertex2()->toString(""," ","\n");
        s+="      vertex " + f.at(i)->Vertex3()->toString(""," ","\n");
        s+="    endloop\n";
        s+="  endfacet\n";
    }
    s+="endsolid atSTL_Convert\n";
    atFile* SaveFile=new atFile("",savefile,".stl");
    SaveFile->Append(s);
    SaveFile->ZapiszPlik();
}


