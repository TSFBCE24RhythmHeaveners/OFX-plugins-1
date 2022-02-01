#ifndef FETCH_DATA
#define FETCH_DATA

#include "ofxImageEffect.h"

////////////////////////////////////////////////////////////////////////////////
// our instance data, where we are caching away clip and param handles
struct MyInstanceData {
  // are we in the general context
  bool isGeneralContext;

  // handles to the clips we deal with
  OfxImageClipHandle sourceClip;
  OfxImageClipHandle maskClip;
  OfxImageClipHandle outputClip;

  // handles to a our parameters
  OfxParamHandle saturationParamR;
  OfxParamHandle saturationParamG;
  OfxParamHandle saturationParamB;

  MyInstanceData()
    : isGeneralContext(false)
    , sourceClip(NULL)
    , maskClip(NULL)
    , outputClip(NULL)
    , saturationParamR(NULL)
    , saturationParamG(NULL)
    , saturationParamB(NULL)
  {}
};

MyInstanceData *FetchInstanceData(OfxPropertySetHandle    effectProps,
                                  OfxImageEffectSuiteV1 *_gImageEffectSuite,
                                  OfxPropertySuiteV1    *_gPropertySuite);

MyInstanceData *FetchInstanceData(OfxImageEffectHandle    effect,
                                  OfxImageEffectSuiteV1 *_gImageEffectSuite,
                                  OfxPropertySuiteV1    *_gPropertySuite);

////////////////////////////////////////////////////////////////////////////////
// get the named suite and put it in the given pointer, with error checking
template <class SUITE>
void FetchSuite(SUITE *& suite,
                const char *suiteName,
                int suiteVersion,
                OfxHost *_gHost)
{
  suite = (SUITE *) _gHost->fetchSuite(_gHost->host, suiteName, suiteVersion);
  if(!suite) {
    ERROR_ABORT_IF(suite == NULL,
                   "Failed to fetch %s verison %d from the host.",
                   suiteName,
                   suiteVersion);
  }
}

#endif
