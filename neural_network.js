/*
  Xarxa neuronal:

  Traducció a Javascript de l'article
  https://towardsdatascience.com/how-to-build-your-own-neural-network-from-scratch-in-python-68998a08e4f6
  sense dependències
*/

/*Activation function and its derivative*/

//sigmoid function
//  y     = 1/( 1+e^(-x) )
//  dy/dx = x*(1-x)
function sigmoid(matrix){
  return matrix.map(row=>{
    return row.map(x=>{
      return 1.0/(1.0+Math.exp(-x));
    });
  });
}
function sigmoid_derivative(matrix){
  return matrix.map(row=>{
    return row.map(x=>{
      return x*(1.0-x);
    });
  });
}

//mini biblioteca per treballar amb matrius
let np={
  dot_rows(arr1, arr2){
    if(arr1.length!=arr2.length){
      throw "arrays have different length";
    }
    return arr1.map((x,i)=>{
      return arr1[i]*arr2[i];
    }).reduce((p,c)=>(p+c));;
  },

  dot(matrix1, matrix2){ //matrix multiplication (in numpy: np.dot())
    let result=[];
    matrix1.forEach((r,i)=>{
      let row=[];
      matrix2[0].forEach((c,j)=>{
        row.push(
          this.dot_rows( matrix1[i], matrix2.map(row=>row[j]))
        );
      });
      result.push(row);
    });
    return result;
    //tested & working
  },

  tra(matrix){ //transpose a matrix (in numpy: matrix.T)
    let result=[];
    matrix[0].forEach((col,i)=>{
      let column = matrix.map(row=>row[i]);
      result.push(column);
    });
    return result;
    //tested & working
  },

  sum(matrix1, matrix2){ //matrix sum (in numpy: A+B)
    return matrix1.map((row,i)=>{
      return row.map((el,j)=>{
        return el+matrix2[i][j];
      });
    });
    //tested: works
  },

  sub(matrix1, matrix2){ //matrix subtraction (in numpy: A-B)
    return matrix1.map((row,i)=>{
      return row.map((el,j)=>{
        return el-matrix2[i][j];
      });
    });
    //tested: works
  },

  mul(scalar, matrix){ //matrix scalar multiplication (in numpy: 2*A)
    return matrix.map(row=>{
      return row.map(x=>{
        return x*scalar;
      });
    });
  },

  bwise_mul(matrix1, matrix2){ //matrix blockwise multiplication (in numpy: A*B)
    return matrix1.map((row,i)=>{
      return row.map((el,j)=>{
        return el*matrix2[i][j];
      });
    });
    //tested: works
  },
};

//helper function
function generate_matrix(rows, cols, number_generator){
  //rows: integer
  //cols: integer
  //number_generator: function

  let matrix=[]; //return value

  for(let i=0;i<rows;i++){
    let row=[];
    for(let j=0;j<cols;j++){
      row.push(number_generator());
    }
    matrix.push(row);
  }

  return matrix;
}

class NeuralNetwork{
  constructor(x,y){
    /*
      Example:
        - 4 input variables
        - 3 output variables:
        - 5 observations

       x1  x2  x3  x4   y1  y2  y3
       [0 , 0 , 2 , 1]  [0 , 1 , 0]
       [0 , 1 , 2 , 0]  [1 , 1 , 0]
       [1 , 0 , 1 , 3]  [1 , 0 , 0]
       [1 , 1 , 1 , 4]  [0 , 0 , 0]
       [0 , 1 , 1 , 4]  [1 , 0 , 0]
    */

    this.input = x; //input matrix
    this.y     = y; //result matrix (desired output)

    this.times_trained = 0;

    //init weights with random numbers
    this.weights1 = generate_matrix(
      this.input[0].length, //input matrix variables (columns)
      this.input.length,    //input matrix observations (rows)
      function(){return Math.random()}, //random number [0,1]
    );

    //init weights with random numbers
    this.weights2 = generate_matrix(
      this.y.length,    //output observations (rows)
      this.y[0].length, //output variables (columns)
      function(){return Math.random()}, //random number [0,1]
    );

    //init result predicted by the neural network
    //with a matrix of all zeros
    this.output = generate_matrix(
      this.y.length,    //rows
      this.y[0].length, //cols
      function(){return 0},
    );
  }

  //feedforward: update layers and output
  feedforward(){
    //python: self.layer1 = sigmoid(np.dot(self.input, self.weights1))
    this.layer1 = sigmoid(np.dot(this.input, this.weights1)); //matrix

    //python: self.output = sigmoid(np.dot(self.layer1, self.weights2))
    this.output = sigmoid(np.dot(this.layer1, this.weights2)); //matrix
  }

  //backprop: update weights
  backprop(){
    //application of the chain rule to find derivative of the loss function with respect to weights2 and weights1
    //in python: d_weights2 = np.dot(self.layer1.T, (2*(self.y - self.output) * sigmoid_derivative(self.output)))
    let d_weights2 = np.dot(
      np.tra(this.layer1),
      np.bwise_mul(
        np.mul(2,np.sub(this.y,this.output)),
        sigmoid_derivative(this.output)
      )
    );

    //in python: d_weights1 = np.dot(self.input.T,  (np.dot(2*(self.y - self.output) * sigmoid_derivative(self.output), self.weights2.T) * sigmoid_derivative(self.layer1)))
    let d_weights1 = np.dot(
      np.tra(this.input),
      np.bwise_mul(
        np.dot(
          np.bwise_mul(
            np.mul(2, np.sub(this.y,this.output)),
            sigmoid_derivative(this.output)
          ),
          np.tra(this.weights2)
        ),
        sigmoid_derivative(this.layer1)
      )
    );

    //update the weights with the derivative (slope) of the loss function
    this.weights1 = np.sum(this.weights1, d_weights1);
    this.weights2 = np.sum(this.weights2, d_weights2);
  }

  //calculate sum of squares of difference of y and output
  //lower is better
  loss(){
    let loss = 0;
    let sub = np.sub(this.y, this.output);
    sub.forEach(row=>{
      row.forEach(n=>{
        loss += Math.pow(n,2);
      });
    });
    return loss;
  }

  status(){
    let times_trained = this.times_trained; //number of times NN has been trained
    let loss          = this.loss(); //current accuracy
    console.log(`[status] Times trained: ${times_trained}. Current loss: ${loss}`);
  }

  train(){
    let n_iterations = 5e4;     //amount of training

    console.log("Start training");
    for(let i=0; i<n_iterations; i++){
      this.feedforward();
      this.backprop();

      if(i%10000==0){
        this.status();
      }

      this.times_trained++;
    }
    this.status()
    console.log(`End training\n`);

    console.log('Desired output | Predicted output');
    this.output.forEach((row,i)=>{
      let rowf = row.map(n=>n.toFixed(4));
      console.log(`${this.y[i]}          | ${rowf}`);
    });
    console.log()
  }

  //use the trained NN to predict
  predict(matrix){
    let layer1 = sigmoid(np.dot(matrix, this.weights1)); //matrix
    let output = sigmoid(np.dot(layer1, this.weights2)); //matrix
    console.log("New prediction:");
    console.log(output.map(row=>row.map(n=>n.toFixed(2))));
    return output;
  }
};

//numeric case
(function main(){
  //input
  let X = [
    [0,0,1,1],
    [0,1,1,0],
    [1,0,1,1],
    [1,1,1,3],
    [1,1,1,5],
  ];

  //desired output
  let y = [
    [0,1,0],
    [1,1,0],
    [1,0,0],
    [0,0,0],
    [1,0,0],
  ];

  //create new NN and train it
  let nn = new NeuralNetwork(X,y);
  nn.train();

  //model is trained: use the NN to predict new case
  nn.predict([
    [1,1,1,3],
    [0,0,0,0],
    [0,0,0,0],
    [0,0,0,0],
    [0,0,1,1],
  ]);
})();

