%module(threads="1") libzerogomoku


%{
#define SWIG_FILE_WITH_INIT
#include "gomoku.h"
#include "neural_network.h"
#include "mcts.h"
%}

%include "numpy.i"
%apply (int DIM1, int DIM2, int* INPLACE_ARRAY2) {(int dim1, int dim2, int *data)};

%include "std_vector.i"
namespace std {
  %template(IntVector) vector<int>;
  %template(IntVectorVector) vector<vector<int>>;
  %template(DoubleVector) vector<double>;
  %template(DoubleVectorVector) vector<vector<double>>;
  %template(CharVector) vector<char>;
}

%include "std_string.i"

%include "gomoku.h"
%include "mcts.h"

class NeuralNetwork {
 public:
  NeuralNetwork(std::string model_path, bool use_gpu, unsigned int batch_size);
  ~NeuralNetwork();
  void set_batch_size(unsigned int batch_size);
};

%{
  static struct InitNumpy {
      InitNumpy() { _import_array(); }
  } _init_numpy;
%}