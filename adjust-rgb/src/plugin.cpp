#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <iostream>
// OpenFX header containing all other includes
#include "ofxImageEffect.h"
// local functions
#include "defineMacros.h"
#include "imHost.hpp"
#include "imProcessing.hpp"
#include "fetchData.hpp"

// macros to define plugin descriptors etc
#define PLUGIN_NAME "adjust rgb Torch CPU"
#define PLUGIN_GROUPING "NuPlugins"
#define PLUGIN_ID "plugin.id.nulight.adjust.rgb.torch.cpu"
// name of our three params
#define SATURATION_PARAM_NAME_R "R Channel"
#define SATURATION_PARAM_NAME_G "G Channel"
#define SATURATION_PARAM_NAME_B "B Channel"
// global vars for quick access to change version release numbers
int ofxAPIVersion = 1;
int versionMajor = 1;
int versionMinor = 0;

////////////////////////////////////////////////////////////////////////////////
// set of suite pointers provided by the host
OfxHost               *gHost;
OfxPropertySuiteV1    *gPropertySuite    = 0;
OfxImageEffectSuiteV1 *gImageEffectSuite = 0;
OfxParameterSuiteV1   *gParameterSuite   = 0;

// anonymous namespace to hide our symbols in
// namespace {


  ////////////////////////////////////////////////////////////////////////////////
  // The first _action_ called after the binary is loaded (three boot strapper functions will be howeever)
  OfxStatus LoadAction(void)
  {
    // fetch our three suites
    FetchSuite(gPropertySuite,    kOfxPropertySuite,    1, gHost);
    FetchSuite(gImageEffectSuite, kOfxImageEffectSuite, 1, gHost);
    FetchSuite(gParameterSuite,   kOfxParameterSuite,   1, gHost);

    return kOfxStatOK;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // the plugin's basic description routine
  OfxStatus DescribeAction(OfxImageEffectHandle descriptor)
  {
    // get the property set handle for the plugin
    OfxPropertySetHandle effectProps;
    gImageEffectSuite->getPropertySet(descriptor, &effectProps);

    // set some labels and the group it belongs to
    gPropertySuite->propSetString(effectProps,
                                  kOfxPropLabel,
                                  0,
                                  PLUGIN_NAME);
    gPropertySuite->propSetString(effectProps,
                                  kOfxImageEffectPluginPropGrouping,
                                  0,
                                  PLUGIN_GROUPING);

    // define the image effects contexts we can be used in, in this case a filter
    // and a general effect
    gPropertySuite->propSetString(effectProps,
                                  kOfxImageEffectPropSupportedContexts,
                                  0,
                                  kOfxImageEffectContextFilter);

    gPropertySuite->propSetString(effectProps,
                                  kOfxImageEffectPropSupportedContexts,
                                  1,
                                  kOfxImageEffectContextGeneral);

    // set the bit depths the plugin can handle
    gPropertySuite->propSetString(effectProps,
                                  kOfxImageEffectPropSupportedPixelDepths,
                                  0,
                                  kOfxBitDepthFloat);
    // gPropertySuite->propSetString(effectProps,
    //                               kOfxImageEffectPropSupportedPixelDepths,
    //                               1,
    //                               kOfxBitDepthShort);
    // gPropertySuite->propSetString(effectProps,
    //                               kOfxImageEffectPropSupportedPixelDepths,
    //                               2,
    //                               kOfxBitDepthByte);

    // say that a single instance of this plugin canNOT be rendered in multiple threads
    gPropertySuite->propSetString(effectProps,
                                  kOfxImageEffectPluginRenderThreadSafety,
                                  0,
                                  kOfxImageEffectRenderUnsafe);

    // say that the host should NOT manage SMP threading over a single frame
    gPropertySuite->propSetInt(effectProps,
                               kOfxImageEffectPluginPropHostFrameThreading,
                               0,
                               0);
    return kOfxStatOK;
  }

  ////////////////////////////////////////////////////////////////////////////////
  //  describe the plugin in context
  OfxStatus
  DescribeInContextAction(OfxImageEffectHandle descriptor,
                          OfxPropertySetHandle inArgs)
  {
    // get the context we are being described for
    char *context;
    gPropertySuite->propGetString(inArgs, kOfxImageEffectPropContext, 0, &context);

    OfxPropertySetHandle props;
    // define the mandated single output clip
    gImageEffectSuite->clipDefine(descriptor, "Output", &props);

    // set the component types we can handle on out output
    gPropertySuite->propSetString(props,
                                  kOfxImageEffectPropSupportedComponents,
                                  0,
                                  kOfxImageComponentRGBA);
    gPropertySuite->propSetString(props,
                                  kOfxImageEffectPropSupportedComponents,
                                  1,
                                  kOfxImageComponentRGB);

    // define the mandated single source clip
    gImageEffectSuite->clipDefine(descriptor, "Source", &props);

    // set the component types we can handle on our main input
    gPropertySuite->propSetString(props,
                                  kOfxImageEffectPropSupportedComponents,
                                  0,
                                  kOfxImageComponentRGBA);
    gPropertySuite->propSetString(props,
                                  kOfxImageEffectPropSupportedComponents,
                                  1,
                                  kOfxImageComponentRGB);


    // first get the handle to the parameter set
    OfxParamSetHandle paramSet;
    gImageEffectSuite->getParamSet(descriptor, &paramSet);

    // properties on our parameter RED
    OfxPropertySetHandle paramPropsR;

    // now define a 'saturation' parameter and set its properties
    gParameterSuite->paramDefine(paramSet,
                                 kOfxParamTypeDouble,
                                 SATURATION_PARAM_NAME_R,
                                 &paramPropsR);
    gPropertySuite->propSetString(paramPropsR,
                                  kOfxParamPropDoubleType,
                                  0,
                                  kOfxParamDoubleTypeScale);
    gPropertySuite->propSetDouble(paramPropsR,
                                  kOfxParamPropDefault,
                                  0,
                                  0.0);
    gPropertySuite->propSetDouble(paramPropsR,
                                  kOfxParamPropDisplayMin,
                                  0,
                                  -1.0);
    gPropertySuite->propSetDouble(paramPropsR,
                                  kOfxParamPropDisplayMax,
                                  0,
                                  1.0);
    gPropertySuite->propSetString(paramPropsR,
                                  kOfxPropLabel,
                                  0,
                                  "RED");
    gPropertySuite->propSetString(paramPropsR,
                                  kOfxParamPropHint,
                                  0,
                                  "Adjust RED Channel");


    // properties on our parameter GREEN
    OfxPropertySetHandle paramPropsG;

    // now define a 'saturation' parameter and set its properties
    gParameterSuite->paramDefine(paramSet,
                                 kOfxParamTypeDouble,
                                 SATURATION_PARAM_NAME_G,
                                 &paramPropsG);
    gPropertySuite->propSetString(paramPropsG,
                                  kOfxParamPropDoubleType,
                                  0,
                                  kOfxParamDoubleTypeScale);
    gPropertySuite->propSetDouble(paramPropsG,
                                  kOfxParamPropDefault,
                                  0,
                                  0.0);
    gPropertySuite->propSetDouble(paramPropsG,
                                  kOfxParamPropDisplayMin,
                                  0,
                                  -1.0);
    gPropertySuite->propSetDouble(paramPropsG,
                                  kOfxParamPropDisplayMax,
                                  0,
                                  1.0);
    gPropertySuite->propSetString(paramPropsG,
                                  kOfxPropLabel,
                                  0,
                                  "GREEN");
    gPropertySuite->propSetString(paramPropsG,
                                  kOfxParamPropHint,
                                  0,
                                  "Adjust GREEN Channel");


    // properties on our parameter BLUE
    OfxPropertySetHandle paramPropsB;

    // now define a 'saturation' parameter and set its properties
    gParameterSuite->paramDefine(paramSet,
                                 kOfxParamTypeDouble,
                                 SATURATION_PARAM_NAME_B,
                                 &paramPropsB);
    gPropertySuite->propSetString(paramPropsB,
                                  kOfxParamPropDoubleType,
                                  0,
                                  kOfxParamDoubleTypeScale);
    gPropertySuite->propSetDouble(paramPropsB,
                                  kOfxParamPropDefault,
                                  0,
                                  0.0);
    gPropertySuite->propSetDouble(paramPropsB,
                                  kOfxParamPropDisplayMin,
                                  0,
                                  -1.0);
    gPropertySuite->propSetDouble(paramPropsB,
                                  kOfxParamPropDisplayMax,
                                  0,
                                  1.0);
    gPropertySuite->propSetString(paramPropsB,
                                  kOfxPropLabel,
                                  0,
                                  "BLUE");
    gPropertySuite->propSetString(paramPropsB,
                                  kOfxParamPropHint,
                                  0,
                                  "Adjust BLUE Channel");

    return kOfxStatOK;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// instance construction
  OfxStatus CreateInstanceAction( OfxImageEffectHandle instance)
  {
    OfxPropertySetHandle effectProps;
    gImageEffectSuite->getPropertySet(instance, &effectProps);

    // To avoid continual lookup, put our handles into our instance
    // data, those handles are guaranteed to be valid for the duration
    // of the instance.
    MyInstanceData *myData = new MyInstanceData;

    // Set my private instance data
    gPropertySuite->propSetPointer(effectProps, kOfxPropInstanceData, 0, (void *) myData);

    // is this instance made for the general context?
    char *context = 0;
    gPropertySuite->propGetString(effectProps, kOfxImageEffectPropContext, 0,  &context);
    myData->isGeneralContext = context && (strcmp(context, kOfxImageEffectContextGeneral) == 0);

    // Cache the source and output clip handles
    gImageEffectSuite->clipGetHandle(instance, "Source", &myData->sourceClip, 0);
    gImageEffectSuite->clipGetHandle(instance, "Output", &myData->outputClip, 0);

    // Cache away the param handles
    OfxParamSetHandle paramSet;
    gImageEffectSuite->getParamSet(instance, &paramSet);
    gParameterSuite->paramGetHandle(paramSet,
                                    SATURATION_PARAM_NAME_R,
                                    &myData->saturationParamR,
                                    0);
    gParameterSuite->paramGetHandle(paramSet,
                                    SATURATION_PARAM_NAME_G,
                                    &myData->saturationParamG,
                                    0);
    gParameterSuite->paramGetHandle(paramSet,
                                    SATURATION_PARAM_NAME_B,
                                    &myData->saturationParamB,
                                    0);

    return kOfxStatOK;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // instance destruction
  OfxStatus DestroyInstanceAction( OfxImageEffectHandle instance)
  {
    // get my instance data
    MyInstanceData *myData = FetchInstanceData(instance, gImageEffectSuite, gPropertySuite);
    delete myData;

    return kOfxStatOK;
  }


  ////////////////////////////////////////////////////////////////////////////////
  // Render an output image
  OfxStatus RenderAction( OfxImageEffectHandle instance,
                          OfxPropertySetHandle inArgs,
                          OfxPropertySetHandle outArgs)
  {
    // get the render window and the time from the inArgs
    OfxTime time;
    OfxRectI renderWindow;
    OfxStatus status = kOfxStatOK;

    gPropertySuite->propGetDouble(inArgs,
                                  kOfxPropTime,
                                  0,
                                  &time);
    gPropertySuite->propGetIntN(inArgs,
                                kOfxImageEffectPropRenderWindow,
                                4,
                                &renderWindow.x1);

    // get our instance data which has out clip and param handles
    MyInstanceData *myData = FetchInstanceData(instance, gImageEffectSuite, gPropertySuite);

    // get our param values
    double params [3] = {0.0, 0.0, 0.0};
    gParameterSuite->paramGetValueAtTime(myData->saturationParamR, time, &params[0]);
    gParameterSuite->paramGetValueAtTime(myData->saturationParamG, time, &params[1]);
    gParameterSuite->paramGetValueAtTime(myData->saturationParamB, time, &params[2]);

    std::cout <<"Time: " << time << "\n";
    // the property sets holding our images
    OfxPropertySetHandle outputImg = NULL, sourceImg = NULL;
    try {
      // fetch image to render into from that clip
      Image outputImg(myData->outputClip, time, gImageEffectSuite, gPropertySuite);
      if(!outputImg) {
        throw " no output image!";
      }

      // fetch image to render into from that clip
      Image sourceImg(myData->sourceClip, time, gImageEffectSuite, gPropertySuite);
      if(!sourceImg) {
        throw " no source image!";
      }

      // now do our render depending on the data type
      if(outputImg.bytesPerComponent() == 4) {
        PixelProcessing<float, 1>(params,
          instance,
          sourceImg,
          outputImg,
          renderWindow);
        }
      else {
        throw " bad data type!";
        throw 1;
      }

    }
    catch(const char *errStr ) {
      bool isAborting = gImageEffectSuite->abort(instance);

      // if we were interrupted, the failed fetch is fine, just return kOfxStatOK
      // otherwise, something wierd happened
      if(!isAborting) {
        status = kOfxStatFailed;
      }
      ERROR_IF(!isAborting, " Rendering failed because %s", errStr);
    }

    // all was well
    return status;
  }

  // are the settings of the effect making it redundant and so not do anything to the image data
  OfxStatus IsIdentityAction( OfxImageEffectHandle instance,
                              OfxPropertySetHandle inArgs,
                              OfxPropertySetHandle outArgs)
  {
    MyInstanceData *myData = FetchInstanceData(instance, gImageEffectSuite, gPropertySuite);

    double time;
    gPropertySuite->propGetDouble(inArgs, kOfxPropTime, 0, &time);

    // get our param values
    double params [3] = {0.0, 0.0, 0.0};
    gParameterSuite->paramGetValueAtTime(myData->saturationParamR, time, &params[0]);
    gParameterSuite->paramGetValueAtTime(myData->saturationParamG, time, &params[1]);
    gParameterSuite->paramGetValueAtTime(myData->saturationParamB, time, &params[2]);

    // if all the params values are 0 (or nearly so) say we aren't doing anything
    if(fabs(params[0]) < 0.000000001 && fabs(params[1]) < 0.000000001 && fabs(params[2]) < 0.000000001) {
      // we set the name of the input clip to pull default images from
      gPropertySuite->propSetString(outArgs, kOfxPropName, 0, "Source");
      // and say we trapped the action and we are at the identity
      return kOfxStatOK;
    }

    // say we aren't at the identity
    return kOfxStatReplyDefault;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // The main entry point function, the host calls this to get the plugin to do things.
  OfxStatus MainEntryPoint(const char *action, const void *handle, OfxPropertySetHandle inArgs,  OfxPropertySetHandle outArgs)
  {
    MESSAGE(": START action is : %s \n", action );
    // cast to appropriate type
    OfxImageEffectHandle effect = (OfxImageEffectHandle) handle;

    OfxStatus returnStatus = kOfxStatReplyDefault;

    if(strcmp(action, kOfxActionLoad) == 0) {
      // The very first action called on a plugin.
      returnStatus = LoadAction();
    }
    else if(strcmp(action, kOfxActionDescribe) == 0) {
      // the first action called to describe what the plugin does
      returnStatus = DescribeAction(effect);
    }
    else if(strcmp(action, kOfxImageEffectActionDescribeInContext) == 0) {
      // the second action called to describe what the plugin does in a specific context
      returnStatus = DescribeInContextAction(effect, inArgs);
    }
    else if(strcmp(action, kOfxActionCreateInstance) == 0) {
      // the action called when an instance of a plugin is created
      returnStatus = CreateInstanceAction(effect);
    }
    else if(strcmp(action, kOfxActionDestroyInstance) == 0) {
      // the action called when an instance of a plugin is destroyed
      returnStatus = DestroyInstanceAction(effect);
    }
    else if(strcmp(action, kOfxImageEffectActionIsIdentity) == 0) {
      // Check to see if our param settings cause nothing to happen
      returnStatus = IsIdentityAction(effect, inArgs, outArgs);
    }
    else if(strcmp(action, kOfxImageEffectActionRender) == 0) {
      // action called to render a frame
      returnStatus = RenderAction(effect, inArgs, outArgs);
    }

    MESSAGE(": END action is : %s \n", action );
    /// other actions to take the default value
    return returnStatus;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Call back passed to the host in the OfxPlugin struct to set our host pointer
  //
  // This must be called AFTER both OfxGetNumberOfPlugins and OfxGetPlugin, but
  // before the pluginMain entry point is ever touched.
  void SetHostFunc(OfxHost *hostStruct)
  {
    gHost = hostStruct;
  }

// } // end of anonymous namespace


////////////////////////////////////////////////////////////////////////////////
// The plugin struct passed back to the host application to initiate bootstrapping\
// of plugin communications
static OfxPlugin effectPluginStruct =
{
  kOfxImageEffectPluginApi,                  // The API this plugin satisfies.
  ofxAPIVersion,                             // The version of the API it satisifes.
  PLUGIN_ID,                                 // The unique ID of this plugin.
  versionMajor,                              // The major version number of this plugin.
  versionMinor,                              // The minor version number of this plugin.
  SetHostFunc,                               // Function used to pass back to the plugin the OFXHost struct.
  MainEntryPoint                             // The main entry point to the plugin where all actions are passed to.
};

////////////////////////////////////////////////////////////////////////////////
// The first of the two functions that a host application will look for
// after loading the binary, this function returns the number of plugins within
// this binary.
//
// This will be the first function called by the host.
EXPORT int OfxGetNumberOfPlugins(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
// The second of the two functions that a host application will look for
// after loading the binary, this function returns the 'nth' plugin declared in
// this binary.
//
// This will be called multiple times by the host, once for each plugin present.
EXPORT OfxPlugin * OfxGetPlugin(int nth)
{
  if(nth == 0)
    return &effectPluginStruct;
  return 0;
}
