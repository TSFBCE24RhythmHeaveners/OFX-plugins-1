#include "fetchData.hpp"

////////////////////////////////////////////////////////////////////////////////
// get my instance data from a property set handle
MyInstanceData *FetchInstanceData(OfxPropertySetHandle    effectProps,
                                  OfxImageEffectSuiteV1 *_gImageEffectSuite,
                                  OfxPropertySuiteV1    *_gPropertySuite)
{
  MyInstanceData *myData = 0;
  _gPropertySuite->propGetPointer(effectProps,
                                  kOfxPropInstanceData,
                                  0,
                                  (void **) &myData);
  return myData;
}

////////////////////////////////////////////////////////////////////////////////
// get my instance data
MyInstanceData *FetchInstanceData(OfxImageEffectHandle    effect,
                                  OfxImageEffectSuiteV1 *_gImageEffectSuite,
                                  OfxPropertySuiteV1    *_gPropertySuite)
{
  // get the property handle for the plugin
  OfxPropertySetHandle effectProps;
  _gImageEffectSuite->getPropertySet(effect, &effectProps);

  // and get the instance data out of that
  return FetchInstanceData(effectProps, _gImageEffectSuite, _gPropertySuite);
}
