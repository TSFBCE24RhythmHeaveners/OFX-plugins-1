// Class to manage ofx images from host's memory

#ifndef IM_HOST
#define IM_HOST

#include "ofxImageEffect.h"

class Image {
public    :
  OfxImageEffectSuiteV1 *_gImageEffectSuite;
  OfxPropertySuiteV1    *_gPropertySuite;
  // OfxPropertySuiteV1    *_gPropertySuite;
  // construct from a property set that represents the image
  Image(OfxPropertySetHandle propSet);

  // construct from a clip by fetching an image at the given frame
  Image(OfxImageClipHandle clip,
        double frame,
        OfxImageEffectSuiteV1 *gImageEffectSuite_arg,
        OfxPropertySuiteV1    *gPropertySuite_arg);

  // destructor
  ~Image();

  // get a pixel address, cast to the right type
  template <class T>
  T *pixelAddress(int x, int y)
  {
    return reinterpret_cast<T *>(rawAddress(x, y));
  }

  // Is this image empty?
  operator bool();

  // bytes per component, 1, 2 or 4 for byte, short and float images
  int bytesPerComponent() const { return bytesPerComponent_; }

  // number of components
  int nComponents() const { return nComponents_; }

protected :
  void construct();

  // Look up a pixel address in the image. returns null if the pixel was not
  // in the bounds of the image
  void *rawAddress(int x, int y);

  OfxPropertySetHandle propSet_;
  int rowBytes_;
  OfxRectI bounds_;
  char *dataPtr_;
  int nComponents_;
  int bytesPerComponent_;
  int bytesPerPixel_;
};


#endif
