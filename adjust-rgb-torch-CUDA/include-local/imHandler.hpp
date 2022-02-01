// class to store and manipulate image objects in an easy to use form
//

#ifndef IM_HANDLER
#define IM_HANDLER

#include <iostream>
#include <vector>
#include <torch/torch.h>

class imageCopy
{
  private:
    int n_rows;
    int n_cols;
    int n_chans;
    int vec_ind;
    // data stored in RGB chunks going along rows within cols
    int getInd(int row, int col, int chan){
      return (row*n_cols*n_chans + col*n_chans + chan);}
  public:
    imageCopy(int rows, int cols, int chans);
    std::vector<float> imageVec;
    torch::Tensor imTensor;

    void initCopy(float value);
    void setPix(int row, int col, int chan, float value);
    void toTensor();
    void toVec();

    float getPix(int row, int col, int chan);
    float getTensorPix(int row, int col, int chan);
    void printIm();

    torch::Tensor getTensor(){return imTensor;}
    int getNumRows(){return n_rows;}
    int getNumCols(){return n_cols;}
    int getNumChans(){return n_chans;}
};

#endif
