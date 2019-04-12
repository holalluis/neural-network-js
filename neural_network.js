/*
  Xarxa neuronal:
    Traducció a Javascript de l'article
    https://towardsdatascience.com/how-to-build-your-own-neural-network-from-scratch-in-python-68998a08e4f6
    sense fer servir cap biblioteca
*/

function sigmoid(matrix){
  return matrix.map(row=>{
    return row.map(x=>{
      return 1.0/(1+Math.exp(-x));
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

//implementa les funcions de numpy en una mini biblioteca 'np'
let np={
  dot:function(matrix1, matrix2){ //matrix multiplication (in numpy: np.dot())
    function dot_rows(arr1, arr2){
      if(arr1.length!=arr2.length) throw "arrays have different length";
      return arr1.map((x,i)=>{
        return arr1[i]*arr2[i];
      }).reduce((p,c)=>(p+c));;
    };
    let result =[];
    matrix1.forEach((r,i)=>{
      let row = [];
      matrix2[0].forEach((c,j)=>{
        row.push(dot_rows(matrix1[i], matrix2.map(row=>row[j])));
      });
      result.push(row);
    });
    return result;
    //tested & working
  },

  tra:function(matrix){ //transpose a matrix (in numpy: matrix.T)
    let result = [];
    matrix[0].forEach((col,i)=>{
      let column = matrix.map(row=>row[i]);
      result.push(column);
    });
    return result;
    //tested & working
  },

  sum:function(matrix1, matrix2){ //matrix sum (in numpy: A+B)
    return matrix1.map((row,i)=>{
      return row.map((el,j)=>{
        return el+matrix2[i][j];
      });
    });
    //tested: works
  },

  sub:function(matrix1, matrix2){ //matrix subtraction (in numpy: A-B)
    return matrix1.map((row,i)=>{
      return row.map((el,j)=>{
        return el-matrix2[i][j];
      });
    });
    //tested: works
  },

  mul:function(scalar, matrix){ //matrix scalar multiplication (in numpy: 2*A)
    return matrix.map((row,i)=>{
      return row.map((el,j)=>{
        return el*scalar;
      });
    });
  },

  bwise_mul:function(matrix1,matrix2){ //matrix blockwise multiplication (in numpy: A*B)
    return matrix1.map((row,i)=>{
      return row.map((el,j)=>{
        return el*matrix2[i][j];
      });
    });
    //tested: works
  },
};

class NeuralNetwork{
  constructor(x,y){
    this.input = x;
    this.weights1 = (function(rows,cols){
      let matrix = [];
      for(let i=0;i<rows;i++){
        let row = [];
        for(let j=0;j<cols;j++) row.push(Math.random());
        matrix.push(row);
      }
      return matrix;
    })(this.input[0].length, 4);
    this.weights2 = (function(rows,cols){
      let matrix = [];
      for(let i=0;i<rows;i++){
        let row = [];
        for(let j=0;j<cols;j++) row.push(Math.random());
        matrix.push(row);
      }
      return matrix;
    })(4,1);
    this.y = y;
    this.output = (function(rows,cols){
      let matrix = [];
      for(let i=0;i<rows;i++){
        let row = [];
        for(let j=0;j<cols;j++) row.push(0);
        matrix.push(row);
      }
      return matrix;
    })(this.y.length, this.y[0].length);
  }

  feedforward(){
    //python: self.layer1 = sigmoid(np.dot(self.input, self.weights1))
    this.layer1 = sigmoid(np.dot(this.input, this.weights1));
    //python: self.output = sigmoid(np.dot(self.layer1, self.weights2))
    this.output = sigmoid(np.dot(this.layer1, this.weights2));
  }
  backprop(){
    //application of the chain rule to find derivative of the loss function with respect to weights2 and weights1
    //python: d_weights2 = np.dot(self.layer1.T, (2*(self.y - self.output) * sigmoid_derivative(self.output)))
    let d_weights2 = np.dot( np.tra(this.layer1), np.bwise_mul( np.mul(2,np.sub(this.y,this.output)), sigmoid_derivative(this.output)));
    //python: d_weights1 = np.dot(self.input.T,  (np.dot(2*(self.y - self.output) * sigmoid_derivative(self.output), self.weights2.T) * sigmoid_derivative(self.layer1)))
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
};

(function main(){
  let X = [
    [0,0,1],
    [0,1,1],
    [1,0,1],
    [1,1,1]
  ];
  let y = [ [0],[1],[1],[0] ];
  let nn = new NeuralNetwork(X,y);

  for(let i=0;i<1500;i++){
    console.log(`Entrenament num ${i}:`);
    console.log(nn.output);
    nn.feedforward()
    nn.backprop()
  }

  console.log('Resultat final:');
  console.log(nn.output);
  //fi
})();
