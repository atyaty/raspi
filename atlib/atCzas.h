/*
byAT
*/

#ifndef StoperH
#define StoperH


#include <QDateTime>


//---------------------------------------------------------------------------
// stoper.
//
class atCzas{
private:
	QDateTime tStoperStart;
	QDateTime tStoperPause;              //czas ostatniego zatrzymania
	qint64 tPause;                       //zsumowany czas przew
	bool going;
	bool pause;
public:
	atCzas(){ Reset(0);}
	void Reset(qint64 t);
	void Reset()        { Reset(0);}
	void Start()        { Reset(0);   going=true;}
	void Start(qint64 t){ Reset(t);   going=true;}
	void Stop()         { going=false;}
	void Pause()        { if(going && !pause){ pause=true;  tStoperPause=QDateTime::currentDateTime(); }}
	void Continue()     { if(going && pause) { pause=false; tPause+=tStoperPause.msecsTo(QDateTime::currentDateTime());  } }
	bool Going()        { return going; }
	bool Pausing()      { return pause; }
	quint64 PauseTime() { if(!going) return 0; return tPause+(pause?(tStoperPause.msecsTo(QDateTime::currentDateTime())):0);}
	quint64 Time()      { return going?tStoperStart.msecsTo(QDateTime::currentDateTime())-PauseTime():0;}
	quint64 TotalTime() { return going?tStoperStart.msecsTo(QDateTime::currentDateTime()):0;}
	double Seconds()    { return ((double)Time())/1000.;}
};
//---------------------------------------------------------------------------

#endif
