#include "atSocketServer.h"

//---------------------------------------------
atSocketServer::atSocketServer(atDataBuffer *dr,atDataBuffer *dw, goData gd, uint16_t p ) {

    dbr=dr;
    dbw=dw;
    goDataFun=gd;

    portno=p;


    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if( listenfd<0 ) sError = "Error - socked open";

    int on = 1;
    int rc = setsockopt(listenfd, SOL_SOCKET,  SO_REUSEADDR,  (char *)&on, sizeof(on));    /* Allow socket descriptor to be reuseable                   */
    if(rc < 0){ sError="Error - socketopt"; close(listenfd); return;   }

    rc = ioctl(listenfd, FIONBIO, (char *)&on);                                        /* Set socket to be nonblocking. All of the sockets for the incoming connections will also be nonblocking since they will inherit that state from the listening socket.   */
    if (rc < 0){  sError="Error - ioctl"; close(listenfd); return;    }

    int n = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(n < 0)  sError = "Error - bind";

    listen(listenfd, 10);
    max_sd = listenfd;

    FD_ZERO(&master_set);
    FD_SET(listenfd, &master_set);        /* Initialize the master fd_set                              */

    timeout.tv_sec  = 0;
    timeout.tv_usec = 1;
}

//---------------------------------------------
atSocketServer::~atSocketServer() {    close( listenfd ); }


//-------------------------------------------
bool atSocketServer::go(){

    memcpy(&working_set, &master_set, sizeof(master_set));
    int desc_ready = select(max_sd + 1, &working_set, NULL, NULL, &timeout);         /* timeout=0   */
    if (desc_ready < 0) { sError="Error - select failed";  return false; }
    if (desc_ready == 0){ sError="Error - select time expired";  return false; }

    end_server=false;
    for (int i=0; i <= max_sd  &&  desc_ready > 0; ++i) {    /* One or more descriptors are readable.  Need to determine which ones they are.                         */
       if (FD_ISSET(i, &working_set)) {                      /* Check to see if this descriptor is ready            */
          desc_ready -= 1;                                   /* A descriptor was found that was readable - one less has to be looked for.  This is being done so that we can stop looking at the working set once we have found all of the descriptors that were ready.                                      */
          if(i == listenfd){       goAaccept();          }
          else{                    goReadWrite(i);           }
       }
    }
    return end_server;
}
//-------------------------------------------------------------------------------
void atSocketServer::closeConnection(int i){
   close(i);
   FD_CLR(i, &master_set);
   if (i == max_sd)  {
          while (FD_ISSET(max_sd, &master_set) == 0)  max_sd -= 1;
   }
}
//---------------------------------------------------------------------------------
void atSocketServer::closeAll(){
   for (int i=0; i <= max_sd; ++i){ if(FD_ISSET(i, &master_set))close(i); } /* Clean up all of the sockets that are open */
}
//---------------------------------------------------------------------------------
void atSocketServer::goAaccept(){
    do{                                             /* Accept all incoming connections that are queued up on the listening socket before we loop back and call select again.              */
       int sd = accept(listenfd, NULL, NULL);       /* Accept each incoming connection.  If accept fails with EWOULDBLOCK, then we have accepted all of them.  Any other failure on accept will cause us to end the server.                                    */
       if (sd < 0) { if(errno != EWOULDBLOCK){ sError="Error - accept"; end_server=true; }   break;     }
       FD_SET(sd, &master_set);                     /* Add the new incoming connection to the master read set                            */
       if (sd > max_sd) max_sd = sd;
    } while (true);                                 /* Loop back up and accept another incoming connection                                 */
}
/*
//-------------------------------------------------------------------------------
void atSocketServer::goReadWrite(int i){
    dbr->Clear();
    do{
        //
      int rc = recv(i, dbr->getFrame(), dbr->getFrameLen(), 0);
      if (rc < 0) { if (errno != EWOULDBLOCK) {sError="Error - recv"; closeConnection(i);}  break;  }
      if (rc == 0){ closeConnection(i); break;  }    // closed by the client
      dbr->setDataLen(rc);

      //
      dbw->Clear();
      int n =goDataFun( dbr, dbw );
      if (n < 0)  {    sError="Error - goDataFun";   closeConnection(i);   break;     }
      else{  cout << dbr->charToHex( dbr->getFrame(), dbr->getDataLen() );   }

      //
      int se = send(i, dbw->getFrame(), dbw->getDataLen(), 0);
      if (se < 0)  {    sError="Error - send";   closeConnection(i);   break;     }

   } while (true);
}
      */
//-------------------------------------------------------------------------------
void atSocketServer::goReadWrite(int i){
    dbr->Clear();
    int rc = recv(i, dbr->getFrame(), dbr->getFrameLen(), 0);
    if (rc < 0) { if (errno != EWOULDBLOCK) {sError="Error - recv"; closeConnection(i);}  return;  }
    if (rc == 0){ closeConnection(i); return;  }    /* closed by the client */
    dbr->setDataLen(rc);

    string s=dbr->charToHex();
    dbw->Clear();
    int n =goDataFun( dbr, dbw );
    if (n < 0)  {    sError="Error - goDataFun";   closeConnection(i);   return;     }

    int se = send(i, dbw->getFrame(), dbw->getDataLen(), 0);
    if (se < 0)  {    sError="Error - send";   closeConnection(i);   return;     }
}
