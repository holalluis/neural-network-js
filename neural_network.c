/*
  Xarxa neuronal senzilla

  Traducció a C de l'article
  https://towardsdatascience.com/how-to-build-your-own-neural-network-from-scratch-in-python-68998a08e4f6
  sense dependències
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*biblioteca interna per treballar amb matrius*/
typedef struct {
  int nrows;
  int ncols;
  double** data; //"array of pointers"
} Matrix;

Matrix* Matrix_create(int nrows, int ncols){
  Matrix* M = malloc(sizeof(Matrix));
  M->nrows = nrows;
  M->ncols = ncols;
  M->data = malloc(nrows*sizeof(double*));
  for(int i=0; i<nrows; i++){
    M->data[i] = malloc(ncols*sizeof(double));
  }
  return M;
}

void Matrix_print(Matrix* M){
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      if(!j){ printf("  "); }
      double x = M->data[i][j];
      printf("%.2lf ",x);
    }
    printf("\n");
  }
}

void Matrix_destroy(Matrix* M){
  if(M==NULL) return;
  for(int i=0; i<M->nrows; i++){
    free(M->data[i]);
  }
  free(M->data);
  free(M);
}

//matrix sum
Matrix* Matrix_sum(Matrix* A, Matrix* B){
  if(A->nrows != B->nrows){ return NULL; }
  if(A->ncols != B->ncols){ return NULL; }

  //init result matrix
  Matrix* S = Matrix_create(A->nrows, A->ncols);

  //compute A+B
  for(int i=0; i<A->nrows; i++){
    for(int j=0; j<B->ncols; j++){
      S->data[i][j] = A->data[i][j] + B->data[i][j];
    }
  }
  return S;
}

//matrix subtraction
Matrix* Matrix_sub(Matrix* A, Matrix* B){
  if(A->nrows != B->nrows){ return NULL; }
  if(A->ncols != B->ncols){ return NULL; }

  //init result matrix
  Matrix* S = Matrix_create(A->nrows, A->ncols);

  //compute A+B
  for(int i=0; i<A->nrows; i++){
    for(int j=0; j<B->ncols; j++){
      S->data[i][j] = A->data[i][j] - B->data[i][j];
    }
  }
  return S;
}

//matrix scalar multiplication
Matrix* Matrix_scale(Matrix* M, double scalar){
  //init result matrix
  Matrix* S = Matrix_create(M->nrows, M->ncols);

  //compute scalar·M
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      S->data[i][j] = scalar*M->data[i][j];
    }
  }
  return S;
}

//matrix multiplication
Matrix* Matrix_multiply(Matrix* A, Matrix* B){
  if(A->ncols != B->nrows){ return NULL; }

  //create result matrix
  Matrix* M = Matrix_create(A->nrows, B->ncols);

  //compute AxB
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      double mij=0; //init element M[i][j]
      for(int k=0; k<A->ncols; k++) {
        mij += A->data[i][k] * B->data[k][j];
      }
      M->data[i][j] = mij;
    }
  }
  return M;
}

//matrix transposition
Matrix* Matrix_transpose(Matrix* M){
  //init result matrix
  Matrix* T = Matrix_create(M->ncols, M->nrows);

  //transpose
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      T->data[j][i] = M->data[i][j];
    }
  }
  return T;
}

//matrix blockwise multiplication
Matrix* Matrix_bwise_mul(Matrix* A, Matrix* B){
  if(A->nrows != B->nrows){ return NULL; }
  if(A->ncols != B->ncols){ return NULL; }

  //init result matrix
  Matrix* M = Matrix_create(A->nrows, A->ncols);

  //compute A*B
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      M->data[i][j] = A->data[i][j] * B->data[i][j];
    }
  }
  return M;
}

/*Activation function and its derivative*/
//sigmoid function
//  y     = 1/( 1+e^(-x) )
//  dy/dx = x*(1-x)
Matrix* sigmoid(Matrix* M){
  Matrix* S = Matrix_create(M->nrows, M->ncols);
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      double x = M->data[i][j];
      S->data[i][j] = 1.0/(1.0+exp(-x));
    }
  }
  return S;
}
Matrix* sigmoid_derivative(Matrix* M){
  Matrix* S = Matrix_create(M->nrows, M->ncols);
  for(int i=0; i<M->nrows; i++){
    for(int j=0; j<M->ncols; j++){
      double x = M->data[i][j];
      S->data[i][j] = x*(1.0-x);
    }
  }
  return S;
}

/*Neural network class*/
typedef struct {
  Matrix* x; //input matrix
  Matrix* y; //desired output matrix
  int times_trained;
  Matrix* weights1;
  Matrix* weights2;
  Matrix* layer1;
  Matrix* output;
} Neural_Network;

Neural_Network* Neural_Network_create(Matrix* x, Matrix* y){
  Neural_Network* nn = malloc(sizeof(Neural_Network));
  nn->x = x;
  nn->y = y;
  nn->times_trained = 0;

  //init weights with random numbers between 0 and 1
  nn->weights1 = Matrix_create(x->ncols, x->nrows);
  for(int i=0; i<nn->weights1->nrows; i++){
    for(int j=0; j<nn->weights1->ncols; j++){
      nn->weights1->data[i][j] = ((double)rand())/RAND_MAX;
    }
  }
  puts("init weights1 ok");

  nn->weights2 = Matrix_create(y->nrows, y->ncols);
  for(int i=0; i<nn->weights2->nrows; i++){
    for(int j=0; j<nn->weights2->ncols; j++){
      nn->weights2->data[i][j] = ((double)rand())/RAND_MAX;
    }
  }
  puts("init weights2 ok");

  nn->layer1 = Matrix_create(nn->x->nrows, nn->weights1->ncols);
  puts("init layer1 ok");

  nn->output = Matrix_create(y->nrows, y->ncols);
  for(int i=0; i<nn->output->nrows; i++){
    for(int j=0; j<nn->output->ncols; j++){
      nn->output->data[i][j] = 0;
    }
  }
  puts("init output ok");

  printf("Neural network created at %p (%ld bytes)\n",nn,sizeof(Neural_Network));

  return nn;
}

void Neural_Network_print(Neural_Network* nn){
  puts("nn->x:");
  Matrix_print(nn->x);
  puts("nn->y:");
  Matrix_print(nn->y);
  puts("nn->weights1:");
  Matrix_print(nn->weights1);
  puts("nn->weights2:");
  Matrix_print(nn->weights2);
}

void Neural_Network_feedforward(Neural_Network* nn){
  Matrix_destroy(nn->layer1);
  nn->layer1 = sigmoid(Matrix_multiply(nn->x,      nn->weights1)); //matrix

  Matrix_destroy(nn->output);
  nn->output = sigmoid(Matrix_multiply(nn->layer1, nn->weights2)); //matrix
}

//backprop: update weights
void Neural_Network_backprop(Neural_Network* nn){
  //application of the chain rule to find derivative of the loss function with respect to weights2 and weights1
  //in python: d_weights2 = np.dot(self.layer1.T, (2*(self.y - self.output) * sigmoid_derivative(self.output)))

  //Matrix* d_weights2 = Matrix_multiply(
  //  Matrix_transpose(nn->layer1),
  //  Matrix_bwise_mul(
  //    Matrix_scale(Matrix_sub(nn->y, nn->output),2),
  //    sigmoid_derivative(nn->output)
  //  )
  //);

  Matrix* sub        = Matrix_sub(nn->y, nn->output);
  Matrix* sca        = Matrix_scale(sub,2);
  Matrix* sde        = sigmoid_derivative(nn->output);
  Matrix* bwm        = Matrix_bwise_mul(sca,sde);
  Matrix* tra        = Matrix_transpose(nn->layer1);
  Matrix* d_weights2 = Matrix_multiply(tra,bwm);

  Matrix_destroy(sub);
  Matrix_destroy(sca);
  Matrix_destroy(sde);
  //Matrix_destroy(bwm);
  Matrix_destroy(tra);

  //in python: d_weights1 = np.dot(self.input.T,  (np.dot(2*(self.y - self.output) * sigmoid_derivative(self.output), self.weights2.T) * sigmoid_derivative(self.layer1)))
  //Matrix* d_weights1 = Matrix_multiply(
  //  Matrix_transpose(nn->x),
  //  Matrix_bwise_mul(
  //    Matrix_multiply(
  //      Matrix_bwise_mul(
  //        Matrix_scale(Matrix_sub(nn->y, nn->output),2),
  //        sigmoid_derivative(nn->output)
  //      ),
  //      Matrix_transpose(nn->weights2)
  //    ),
  //    sigmoid_derivative(nn->layer1)
  //  )
  //);

  Matrix* tra2 = Matrix_transpose(nn->weights2);
  Matrix* mul = Matrix_multiply(bwm, tra2);
  Matrix* sde2 = sigmoid_derivative(nn->layer1);

  Matrix* bwm2 = Matrix_bwise_mul(mul, sde2);
  Matrix* trax = Matrix_transpose(nn->x);
  Matrix* d_weights1 = Matrix_multiply(trax, bwm2);

  //update the weights with the derivative (slope) of the loss function
  nn->weights2 = Matrix_sum(nn->weights2, d_weights2);
  nn->weights1 = Matrix_sum(nn->weights1, d_weights1);

  Matrix_destroy(d_weights2);
  Matrix_destroy(d_weights1);
  Matrix_destroy(trax);
  Matrix_destroy(bwm2);
  Matrix_destroy(bwm);
  Matrix_destroy(sde2);
  Matrix_destroy(mul);
  Matrix_destroy(tra2);
}

double Neural_Network_loss(Neural_Network* nn){
  double loss=0;
  Matrix* sub = Matrix_sub(nn->y, nn->output);
  for(int i=0; i<sub->nrows; i++){
    for(int j=0; j<sub->ncols; j++){
      double n = sub->data[i][j];
      loss += pow(n,2);
    }
  }
  Matrix_destroy(sub);
  return loss;
}

void Neural_Network_status(Neural_Network* nn){
  int times_trained = nn->times_trained; //number of times NN has been trained
  double loss       = Neural_Network_loss(nn); //current accuracy
  printf("[status] Times trained: %d. Current loss: %.10lf\n",times_trained,loss);
}

void Neural_Network_train(Neural_Network* nn){
  int n_iterations = 5e5; //number of training iterations

  puts("Training start");
  Neural_Network_status(nn);
  for(int i=0; i<n_iterations; i++){
    Neural_Network_feedforward(nn);
    Neural_Network_backprop(nn);
    nn->times_trained++;
    if(nn->times_trained%10000==0){
      Neural_Network_status(nn);
    }
  }
  puts("Training ended");

  //log x, y and trained output
  puts("Input:");          Matrix_print(nn->x);
  puts("Desired output:"); Matrix_print(nn->y);
  puts("Trained output:"); Matrix_print(nn->output);
}

int main(){
  Matrix* x = Matrix_create(5,4);
  double xdata[5][4]={
    {0,0,1,1},
    {0,1,1,0},
    {1,0,1,1},
    {1,1,1,3},
    {1,1,1,5},
  };
  for(int i=0;i<5;i++){
    for(int j=0;j<4;j++){
      x->data[i][j] = xdata[i][j];
    }
  }

  Matrix* y = Matrix_create(5,3);
  double ydata[5][3]={
    {0,1,0},
    {1,1,0},
    {1,0,0},
    {0,0,0},
    {1,0,0},
  };
  for(int i=0;i<5;i++){
    for(int j=0;j<3;j++){
      y->data[i][j] = ydata[i][j];
    }
  }

  Neural_Network* nn = Neural_Network_create(x,y);
  Neural_Network_train(nn);

  return 0;
}
