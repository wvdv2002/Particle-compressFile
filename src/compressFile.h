#include "application.h"
#include "SdFat.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "heatshrink/heatshrink_decoder.h"
#include "heatshrink/heatshrink_encoder.h"
#ifdef __cplusplus
}
#endif


typedef enum compress_state{
  COMPRESS_IDLE,
  COMPRESS_STARTING,
  COMPRESS_RUNNING,
  COMPRESS_FINALIZING,
  COMPRESS_DONE,
  COMPRESS_ERROR,
} compressFileState;

typedef enum compressfileerror{
  COMPRESS_NOERROR,
  COMPRESS_FILENOTFOUND,
  COMPRESS_CANNOTCREATEFILE,
  COMPRESS_CANNOTSINK,
  COMPRESS_CANNOTCOMPRESS,
  COMPRESS_CANNOTWRITECOMPRESSFILE,
  COMPRESS_CANNOTFINALIZE,
  COMPRESS_STATEERROR,
  COMPRESS_BUSYERROR,
} compressFileError;

class CompressFile{
  public:
  CompressFile(void);
  bool startCompress(String anIn, String anOut);
  void task(void);
  bool isDone(void);
  compressFileError getError(void);
  compressFileState getState(void);
  String toString(void);
  protected:
  private:
    void compressLoop(void);
    void compressFinalizing(void);
    size_t data_sz;
    heatshrink_encoder hse;
    File inFile;
    File outFile;
    uint8_t inBuf[512];
    uint8_t outBuf[512];
    compressFileState state;
    compressFileError error;
};
