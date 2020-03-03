#include "atFileScad.h"
#include "math.h"



//====================================================================
atFileScad::atFileScad(){}
atFileScad::atFileScad(QString t,QString p, atConvert* c,bool i,QString f)  {
    Filetext=t; path=p; convert=c; convert->addFile(f); tryb_inc=i; file=f; module=nullptr; reset();
}
atFileScad::atFileScad(atScadModule* m,QString p, atConvert* c,bool i,QString f)  {
    Filetext=m->body; path=p; convert=c; convert->addFile(f); tryb_inc=i; file=f; module=m; reset();
}
atFileScad::atFileScad(atScadModule* m,QString t,QString p, atConvert* c,bool i,QString f)  {
    Filetext=t; path=p; convert=c; convert->addFile(f); tryb_inc=i; file=f; module=m; reset();
}

//---------------------------------------------------
//sprawdza czy to operator
bool atFileScad::isOperator() {
    QChar c = Filetext.at(position);
    if(c=='+' || c=='-' || c=='/'|| c=='*'|| c=='(') return true;
    return false;
}
//-----------------------------------------------------
bool atFileScad::isWhite() {
    QChar c = Filetext.at(position);
    if(c==' ' || c=='\n' || c=='\t') return true;
    if(isComment()) return true;
    return false;
}
//---------------------------------------------------
//sprawdza czy to komentarz
bool atFileScad::isComment() {
    QChar c = Filetext.at(position);
    if(c=='/' && (position+1)<Filetext.length()){
        QChar c1 = Filetext.at(position+1);
        if(c1=='/' || c1=='*') return true;
    }
    return false;
}
//---------------------------------------------------
//sprawdza czy to znak
bool atFileScad::isBlack() {
    QChar c = Filetext.at(position);
    if( (c>=48 && c<=57) || (c>=65&&c<=90) || (c>=97&&c<=122) || c==95 || c=='.') return true;
    return false;
}
//--------------------------------------------------------
//przesunięcie kurosra do następnego znaku
//metoda pomija komentarze i zlicza ilość linii
void atFileScad::nextPosition(){
    if( (position+1)<Filetext.length() && Filetext.at(position)=='/' &&  Filetext.at(position+1)=='/'){
        while( position<Filetext.length() && Filetext.at(position)!='\n') position++;
    }
    if( (position+3)<Filetext.length() && Filetext.at(position)=='/' &&  Filetext.at(position+1)=='*'){
        position+=3; linepos+=3;
        if(Filetext.at(position)=='\n'){line++;linepos=0;}
        if(Filetext.at(position-1)=='\n'){line++;linepos=0;}
        while( position<Filetext.length() && !(Filetext.at(position)=='/' && Filetext.at(position-1)=='*')  ){
            position++; linepos++;
            if(Filetext.at(position)=='\n'){line++;linepos=0;}
        }
    }
    if( position<Filetext.length() && Filetext.at(position)=='\n'){ line++; linepos=0; }
    if( position<Filetext.length() ) {  position++; linepos++; }
}
//--------------------------------------------------------
void atFileScad::goToNotWhite() {
    while(position<Filetext.length() && isWhite()){      nextPosition();    }
}

//--------------------------------------------------------
QString atFileScad::nawias(QChar c1,QChar c2) {
    QString s="";
    if( position>=Filetext.length() || Filetext.at(position)!=c1 ) { setError((QString)ERROR_NAWIAS_REQ+c1); return ""; }
    else{
        nextPosition();
        int rekurencja=0;
        while( position<Filetext.length() && (Filetext.at(position)!=c2 || rekurencja>0) ) {
            QChar c=Filetext.at(position);
            if( !isComment() ) s+=c;    //pomijamy komentarze, jeśli tego warunku nie ma, to żle działa pomijanie przez nexPosition.
            if(c==c1) rekurencja++;
            else if(c==c2) rekurencja--;
            nextPosition();
        }
        if(position>=Filetext.length()) {setError((QString)ERROR_NAWIAS_REQ_END+c2); return"";}
        nextPosition();
    }
    return s;
}
//--------------------------------------------------------
QString atFileScad::separator(QChar c2) {
    QString s="";
    if( position>=Filetext.length() ) { setError((QString)ERROR_SEPARATOR_REQ+c2); return ""; }
    else{

        int rekN=0,rekK=0;
        while( position<Filetext.length() && (Filetext.at(position)!=c2 || rekN>0 || rekK>0) ) {
            QChar c=Filetext.at(position);
            s+=c;
            if(c=='(') rekN++;
            else if(c==')') rekN--;
            else if(c=='[') rekK++;
            else if(c==']') rekK--;
            nextPosition();
        }
        if(position>=Filetext.length()) {setError((QString)ERROR_SEPARATOR_REQ_END+c2); return"";}
        nextPosition();
    }
    return s;
}


//--------------------------------------------------------
//wygląda na to że po każdym wołaniu tej funkcji jest goToNotWhite, rozważyć przeniesienie tego do wnętrza funkcji
// podobnie z innymi funkcjami np: nawias
QString atFileScad::slowoGet() {
    QString s="";
    if(position<Filetext.length() && isBlack() ) {
        s=goToNotBlack();
    }
    return s.trimmed();
}
//--------------------------------------------------------
QString atFileScad::goToNotBlack() {
    QString s="";
    while(position<Filetext.length() ){
        if(  isBlack() ) s+=Filetext.at(position);
        else{ break;}
        nextPosition();
    }
    return s;
}
//--------------------------------------------------------
void atFileScad::go() {
    while(position<Filetext.length()){
            goToNotWhite();
            if( position>=Filetext.length()  || !procMain() )
            break;
    }
}
//--------------------------------------------------------
bool atFileScad::procMain(){
   if(!isBlack()) {
       setError((QString)ERROR_PROC_NOTIS_BLACK);
       return false;
   }
   QString s=slowoGet();
   goToNotWhite();
   if(position>=Filetext.length() ){setError((QString)ERROR_PROC_UNEXPECTED_END); return false;}
   if(s=="use")            return procUse();
   else if(s=="include")   return procInclude();
   else if(s=="module")    return procModule();
   else if(s=="translate") return procGoTranslate(s);
   else if(s=="rotate")    return procGoRotate(s);
   else {
       QChar c=Filetext.at(position);
       if(c=='(')        return procGoModule(s);
       else if(c=='=')   return procGoZmienna(s);
       else {
           setError((QString)ERROR_PROC_OPERATOR_NEEDED) ; return false;
       }
   }
   setError((QString)ERROR_PROC_COMMAND);  return false;
}
//--------------------------------------------------------
bool atFileScad::procUse(){
    if(Filetext.at(position)!='<') {setError((QString)ERROR_PROCUSE_OPENBRACKET); return false;}
    else{
        QString s=nawias('<','>');
        if(!ok()) return false;
        atFile *file = new atFile(path,s,"");
        atFileScad *scad = new atFileScad(file->PobierzPlik(),path,convert,false,s );
        scad->go();
        if(!scad->ok()){ setError((QString)ERROR_INCLUDE+s+"\n"+scad->getError()); }
        delete scad;
        delete file;
    }
    return ok();
}
//--------------------------------------------------------
bool atFileScad::procInclude(){
    if(Filetext.at(position)!='<') {setError((QString)ERROR_PROCINCLUDE_OPENBRACKET); return false;}
    else{
        QString s=nawias('<','>');
        if(!ok()) return false;
        atFile *file = new atFile(path,s,"");
        atFileScad *scad = new atFileScad(file->PobierzPlik(),path,convert,true,s ); //uzupełnić inne.. o obsługę parametru include
        scad->go();
        if(!scad->ok()){ setError((QString)ERROR_INCLUDE+s+"\n"+scad->getError()); }
        delete scad;
        delete file;
    }
    return ok();
}
//--------------------------------------------------------
bool atFileScad::procModule(){
    QString sF=slowoGet();
    goToNotWhite();
    QString sP=nawias('(',')');
    goToNotWhite();
    QString sB=nawias('{','}');
    convert->addModule(sF,sP,sB,file);
    return ok();
}
//--------------------------------------------------------
bool atFileScad::procGoModule(QString s){
    QString sP=nawias('(',')');
    goToNotWhite();
    if(Filetext.at(position)!=';') {setError((QString)ERROR_PROCGOMODULE_SEMICOLON); return false;}
    else nextPosition();
    if(tryb_inc) convert->addGoModule(s,sP);
    return true;
}
//--------------------------------------------------------
bool atFileScad::procGoTranslate(QString s){
    QString sP=nawias('(',')');
    goToNotWhite();
    if(Filetext.at(position)!='{') { QString sB=nawias('{','}');}
    //else if... tu dokończyć
    else nextPosition();
    convert->addGoModule(s,sP);
    return true;
}
//--------------------------------------------------------
bool atFileScad::procGoRotate(QString s){
    QString sP=nawias('(',')');
    goToNotWhite();
    if(Filetext.at(position)!='{') { QString sB=nawias('{','}');}
    //else if... tu dokończyć
    else nextPosition();
    convert->addGoModule(s,sP);
    return true;
}

//--------------------------------------------------------
bool atFileScad::procGoZmienna(QString s){
    if(position>=Filetext.length() || Filetext.at(position)!='=' ) { setError((QString)ERROR_EQUSIGN_ONE); return ""; }
    else{
        nextPosition();
        if(tryb_inc){
            double val=procValue(';');
            if(ok() ) convert->addZmienna(s,"",val,file);
        }
        else{
            separator(';');
        }
    }
    return ok();
}

//--------------------------------------------------------
//
bool atFileScad::procBody( ){

    goToNotWhite();
    while( position<Filetext.length()  ){

        if(!isBlack()) {
            setError((QString)ERROR_PROCBODY_NOTIS_BLACK);   return false;       }
        QString s=slowoGet();
        goToNotWhite();
        if(position>=Filetext.length() ){setError((QString)ERROR_PROCBODY_UNEXPECTED_END); return false;}
        if(s=="translate"){
            setError((QString)ERROR_PROCBODY_DKONCZYCPROGRAM+" ..translate"); return false;}
        else if(s=="rotate"){
            setError((QString)ERROR_PROCBODY_DKONCZYCPROGRAM+" ..rotate"); return false;}
        else {
            QChar c=Filetext.at(position);
            if(c=='('){        if( !procBodyModule(s) ) return false; }
            else if(c=='=')  { if( !procBodyZmienna(s) ) return false; }
            else {  setError((QString)ERROR_PROCBODY_OPERATOR_NEEDED) ; return false; }
        }
        goToNotWhite();

    }
    return true;
}
//--------------------------------------------------------
bool atFileScad::procBodyModule(QString s){
    QString sP=nawias('(',')');
    goToNotWhite();
    if(Filetext.at(position)!=';') {setError((QString)ERROR_PROCBODYMODULE_SEMICOLON); return false;}
    else nextPosition();
    module->addGoModule(s,sP);
    return true;
}
//--------------------------------------------------------
bool atFileScad::procBodyZmienna(QString s){
    if(position>=Filetext.length() || Filetext.at(position)!='=' ) { setError((QString)ERROR_PROCBODYMODULE_EQUSIGN); return ""; }
    else{
        nextPosition();
        double val=procValue(';');
        if(ok() ) module->addZmienna(s,"",val,file);
    }
    return ok();
}


//--------------------------------------------------------
double atFileScad::procValue( QChar cEnd ){
    double val1=0;
    bool start=true; //przy pierwszym wywołaniu może nie byc żadnego operatora, dlatego rozróżniamy pierwsze wywołanie od innych
    while( position<Filetext.length()  ){
      goToNotWhite();
      QChar c=Filetext.at(position);
      if(c==cEnd)                         { nextPosition(); return val1; }
      else if((isBlack()||c=='(')&&start) {                 val1+=procValueMno(); }
      else if(c=='+')                     { nextPosition(); val1+=procValueMno(); }
      else if(c=='-')                     { nextPosition(); val1-=procValueMno(); }
      else { setError((QString)ERROR_PROCVALUE_OPERATOR); }
      if(!ok()) return 0;
      start=false;
    }
}
//--------------------------------------------------------
double atFileScad::procValueMno(){
    double val1=procValueOne();
    if(!ok()) {return 0;}
    goToNotWhite();
    QChar c=Filetext.at(position);
    if(c=='*'){
       nextPosition();
       return val1*procValueMno();
    }
    else if(c=='/') {
       nextPosition();
       double val2=procValueMno();
       if(val2==0){  setError((QString)ERROR_PROCVALUE_DEVBYZERO); return 0;}
       else return val1/val2;
    }
    return val1;
}
//--------------------------------------------------------
double atFileScad::procValueOne(){
    goToNotWhite();
    if(isBlack()) {
        QString s=slowoGet();
        goToNotWhite();
        if(Filetext.at(position)=='(')  {  nextPosition();   return procFunkcja(s,procValue(')'));     }
        else                            {                    return getValue(s);                       }
    }
    else if(Filetext.at(position)=='(') { nextPosition();    return procValue(')');    }
    setError((QString)ERROR_PROCVALUE_ONE);   return 0;
}
//--------------------------------------------------------
double atFileScad::getValue(QString s){
    bool okDouble=true;
    double v=s.toDouble(&okDouble);
    if(okDouble) return v;
    //---------
    if(module!=nullptr){
        okDouble=true;
        v=module->getZmienna1(s,&okDouble);
        if(okDouble) return v;
    }
    //--------
    okDouble=true;
    v=convert->getZmienna1(s,&okDouble);
    if(okDouble) return v;
    setError((QString)ERROR_PROCVALUE_GETVALUE+" ("+s+")");
    return 0;
}
//--------------------------------------------------------
double atFileScad::procFunkcja(QString f, double v){
    if(f=="sin"){ return sin(v); }
    else if(f=="cos"){ return cos(v); }
    else if(f=="tan"){ return tan(v); }
    else if(f=="sqrt"){ return sqrt(v); }
    else if(f=="atan"){ return atan(v)*180/M_PI; }
    else if(f=="asin"){ return asin(v)*180/M_PI; }
    else if(f=="acos"){ return acos(v)*180/M_PI; }
    else{ setError((QString)ERROR_PROCFUNKCJA_BRAK+f); return 0;}
}
