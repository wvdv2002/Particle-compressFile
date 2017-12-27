#include "compressFile.h"

CompressFile::CompressFile(void){state=COMPRESS_ERROR;error=COMPRESS_STATEERROR;};

void CompressFile::task(void){
  switch(state){
    case COMPRESS_IDLE:
    break;
    case COMPRESS_RUNNING:
      compressLoop();
      break;
    case COMPRESS_FINALIZING:
      compressFinalizing();
      break;
    case COMPRESS_ERROR:
      break;
    case COMPRESS_DONE:
      break;
    default:
      state=COMPRESS_ERROR;
      error=COMPRESS_STATEERROR;
    break;
  }
}

bool CompressFile::startCompress(String anInPath,String anOutPath){

  state=COMPRESS_RUNNING;
  error=COMPRESS_NOERROR;
  heatshrink_encoder_reset(&hse);

  if(inFile.isOpen()){inFile.close();}
  if(!inFile.open(anInPath,O_READ)){
    state=COMPRESS_ERROR;error=COMPRESS_FILENOTFOUND;
  }

  if(outFile.isOpen()){outFile.close();}
  if (!outFile.open(anOutPath, O_CREAT | O_TRUNC | O_WRITE)) {
    state=COMPRESS_ERROR;error=COMPRESS_CANNOTCREATEFILE;
  }
  if(state==COMPRESS_RUNNING && error==COMPRESS_NOERROR){
    return 1;
  }
  return 0;
}

compressFileError CompressFile::getError(void){
  return error;
}

compressFileState CompressFile::getState(void){
  return state;
}

bool CompressFile::isDone(void){
  if (state==COMPRESS_ERROR || state==COMPRESS_DONE){
    return 1;
  }
  return 0;
}

void CompressFile::compressFinalizing(void){
    HSE_finish_res fres;
    HSE_poll_res pres;
    size_t poll_sz = 0;
    fres = heatshrink_encoder_finish(&hse);
    if(fres<0){state=COMPRESS_ERROR;error=COMPRESS_CANNOTFINALIZE;}
    if(fres==HSER_FINISH_MORE){
//          sd.errorPrint("More data available");
    pres = heatshrink_encoder_poll(&hse,outBuf,sizeof(outBuf),&poll_sz);
    if(pres<0) {state=COMPRESS_ERROR;error=COMPRESS_CANNOTCOMPRESS;}
    if(outFile.write(outBuf,poll_sz)!=poll_sz){error=COMPRESS_CANNOTWRITECOMPRESSFILE;state=COMPRESS_ERROR;}
  }
      fres = heatshrink_encoder_finish(&hse);
  //        sd.errorPrint("not finished");
      if(fres==HSER_FINISH_DONE){state=COMPRESS_DONE;}
      else{state=COMPRESS_ERROR;error=COMPRESS_CANNOTFINALIZE;}
      inFile.close();
      outFile.close();
}

void CompressFile::compressLoop(void){
  /*      if (nr == 0) {
      sd.errorPrint("read end");
      file.close();
    }
  */
    HSE_sink_res sres;
    HSE_poll_res pres;
    size_t sink_sz = 0;
    size_t poll_sz = 0;
    size_t sunk = 0;
    size_t nr = inFile.read(inBuf, sizeof(inBuf));


    do{
      if(nr>0){
          sres = heatshrink_encoder_sink(&hse,&inBuf[sunk],nr-sunk,&sink_sz);
          if(sres<0){state=COMPRESS_ERROR;error=COMPRESS_CANNOTSINK;}
          sunk += sink_sz;
      }

      do {
        pres = heatshrink_encoder_poll(&hse,outBuf,sizeof(outBuf),&poll_sz);
        if(pres<0) {state=COMPRESS_ERROR;error=COMPRESS_CANNOTCOMPRESS;}
        if(outFile.write(outBuf,poll_sz)!=poll_sz){state=COMPRESS_ERROR;error=COMPRESS_CANNOTWRITECOMPRESSFILE;}
      }while(pres == HSER_POLL_MORE);

      if(poll_sz==0 && nr == 0){
        state=COMPRESS_FINALIZING;
        return;
      }
    }while(sunk<nr);
    if (nr==0){state=COMPRESS_FINALIZING;}
}

String CompressFile::toString(void){
  String info;
  info = String(state)+ "," + String(error);
  return info;
}
