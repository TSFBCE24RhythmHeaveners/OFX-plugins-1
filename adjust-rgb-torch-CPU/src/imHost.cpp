#include <string>
#include "imHost.hpp"

////////////////////////////////////////////////////////////////////////////////
// class to manage OFX images

// construct from a property set - need to update to prop/effects suites?
Image::Image(OfxPropertySetHandle propSet)
  : propSet_(propSet)
{
  construct();
}

// construct by fetching from a clip
Image::Image(OfxImageClipHandle clip,
             double time,
             OfxImageEffectSuiteV1 *gImageEffectSuite_arg,
             OfxPropertySuiteV1    *gPropertySuite_arg)
  : propSet_(NULL)
{
  _gImageEffectSuite = gImageEffectSuite_arg;
  _gPropertySuite = gPropertySuite_arg;
  if (clip && (_gImageEffectSuite->clipGetImage(clip, time, NULL, &propSet_) == kOfxStatOK)) {
    construct();
  }
  else {
    propSet_ = NULL;
  }
}

// assemble it all togther
void Image::construct()
{
  if(propSet_) {
    _gPropertySuite->propGetInt(propSet_, kOfxImagePropRowBytes, 0, &rowBytes_);
    _gPropertySuite->propGetIntN(propSet_, kOfxImagePropBounds, 4, &bounds_.x1);
    _gPropertySuite->propGetPointer(propSet_, kOfxImagePropData, 0, (void **) &dataPtr_);

    // how many components per pixel?
    char *cstr;
    _gPropertySuite->propGetString(propSet_, kOfxImageEffectPropComponents, 0, &cstr);

    if(strcmp(cstr, kOfxImageComponentRGBA) == 0) {
      nComponents_ = 4;
    }
    else if(strcmp(cstr, kOfxImageComponentRGB) == 0) {
      nComponents_ = 3;
    }
    else if(strcmp(cstr, kOfxImageComponentAlpha) == 0) {
      nComponents_ = 1;
    }
    else {
      throw " bad pixel type!";
    }

    // what is the data type
    _gPropertySuite->propGetString(propSet_, kOfxImageEffectPropPixelDepth, 0, &cstr);
    if(strcmp(cstr, kOfxBitDepthByte) == 0) {
      bytesPerComponent_ = 1;
    }
    else if(strcmp(cstr, kOfxBitDepthShort) == 0) {
      bytesPerComponent_ = 2;
    }
    else if(strcmp(cstr, kOfxBitDepthFloat) == 0) {
      bytesPerComponent_ = 4;
    }
    else {
      throw " bad pixel type!";
    }

    bytesPerPixel_ = bytesPerComponent_ * nComponents_;
  }
  else {
    rowBytes_ = 0;
    bounds_.x1 = bounds_.x2 = bounds_.y1 = bounds_.y2 = 0;
    dataPtr_ = NULL;
    nComponents_ = 0;
    bytesPerComponent_ = 0;
  }
}

// destructor
Image::~Image()
{
  if(propSet_)
    _gImageEffectSuite->clipReleaseImage(propSet_);
}

// get the address of a location in the image as a void *
void *Image::rawAddress(int x, int y)
{
  // Inside the bounds of this image?
  if(x < bounds_.x1 || x >= bounds_.x2 || y < bounds_.y1 || y >= bounds_.y2)
    return NULL;

  // turn image plane coordinates into offsets from the bottom left
  int yOffset = y - bounds_.y1;
  int xOffset = x - bounds_.x1;

  // Find the start of our row, using byte arithmetic
  char *rowStart = (dataPtr_) + yOffset * rowBytes_;

  // finally find the position of the first component of column
  return rowStart + (xOffset * bytesPerPixel_);
}

// are we empty?
Image:: operator bool()
{
  return propSet_ != NULL && dataPtr_ != NULL;
}
