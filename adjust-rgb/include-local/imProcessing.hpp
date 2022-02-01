// Put together getting images from the host and applying effects
// Returning images to host

#ifndef IM_PROC
#define IM_PROC

#include "ofxImageEffect.h"
#include "imHost.hpp"
#include "imHandler.hpp"

////////////////////////////////////////////////////////////////////////////////
// iterate over our pixels and process them
template <class T, int MAX>
void PixelProcessing(double params[3],
                     OfxImageEffectHandle instance,
                     Image &src,
                     Image &output,
                     OfxRectI renderWindow)
{
  // see how many channels
  int nComps = output.nComponents();
  std::cout<<"Render Window Cols: "<<renderWindow.y2-renderWindow.y1<<"\n";
  std::cout<<"Render Window Rows: "<<renderWindow.x2-renderWindow.x1<<"\n";
  std::cout<<"Params: "<<params[0]<<" "<<params[1]<<" "<<params[2]<<"\n\n";

  // get image copier initalised
  int rows = renderWindow.y2 - renderWindow.y1;
  int cols = renderWindow.x2 - renderWindow.x1;
  imageCopy im(rows, cols, nComps);


  // copy source image
  for(int row = renderWindow.y1; row < renderWindow.y2; row++) {
    for(int col = renderWindow.x1; col < renderWindow.x2; col++) {
      T *srcPix = src.pixelAddress<T>(col, row);
      im.initCopy(srcPix[0]);
      im.initCopy(srcPix[1]);
      im.initCopy(srcPix[2]);
      if(nComps == 4){
        im.initCopy(srcPix[3]);
      }
    }
  }

  // and do some processing
  for(int row = 0; row < rows; row++) {
    for(int col = 0; col < cols; col++) {
      im.setPix(row, col, 0, im.getPix(row, col, 0)+params[0]);
      im.setPix(row, col, 1, im.getPix(row, col, 1)+params[1]);
      im.setPix(row, col, 2, im.getPix(row, col, 2)+params[2]);
    }
  }

  // copy back to output image
  int rowCounter;
  int colCounter;
  rowCounter = 0;
  for(int row = renderWindow.y1; row < renderWindow.y2; row++) {
    // get start of output image memory row
    T *dstPix = output.pixelAddress<T>(renderWindow.x1, row);
    colCounter = 0;
    for(int col = renderWindow.x1; col < renderWindow.x2; col++) {
      // apply to output
      dstPix[0] = im.getPix(rowCounter, colCounter, 0);
      dstPix[1] = im.getPix(rowCounter, colCounter, 1);
      dstPix[2] = im.getPix(rowCounter, colCounter, 2);
      if(nComps == 4) { // if we have an alpha
        dstPix[3] = im.getPix(rowCounter, colCounter, 3);
      }
      dstPix += nComps;
      colCounter++;
    }
    rowCounter++;
  }
}

#endif
