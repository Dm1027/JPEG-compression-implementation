#include<iostream>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<fstream>
using namespace std;
 
#define NUM 8
#define PI 3.1415926

const int blockSize = 8;
const int m = 8, n = 8;


int q10[8][8] = {{80,60,50,80,120,200,255,255},
                  {50,60,70,95,130,255,255,255},
                  {70,65,80,120,200,255,255,255},
                  {70,85,110,145,255,255,255,255},
                  {90,110,185,255,255,255,255,255},
                  {120,175,255,255,255,255,255,255},
                  {245,255,255,255,255,255,255,255},
                  {255,255,255,255,255,255,255,255}};

int q90[8][8] = {{3,2,2,3,5,8,10,12},
                {2,2,3,4,5,12,12,11},
                {3,3,3,5,8,11,14,11},
                {3,3,4,6,10,17,16,12},
                {4,4,7,11,14,22,21,15},
                {5,7,11,13,16,12,23,18},
                {10,13,16,17,21,24,24,21},
                {14,18,19,20,22,20,20,20}};


void quantization(int inputArray[][blockSize],int quantizationArray[][blockSize])
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      inputArray[i][j] = inputArray[i][j]/quantizationArray[i][j];
    }
  }
  // return inputArray[blockSize][blockSize];
}


void iQuantization(int inputArray[][blockSize], int quantizationArray[][blockSize])
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      inputArray[i][j] = inputArray[i][j]*quantizationArray[i][j];
    }
  }
  //return inputArray[blockSize][blockSize];
}





 
int around(double a)
{
    if(a >= 0)
    {
        return int(a+0.5);
    }
    else
    {
        return int(a-0.5);
    }
 
}
// DCT - Discrete Cosine Transform
void DCT(int data[NUM][NUM])
{
    int output[NUM][NUM];
    double alpha,beta;//C(k)  C(l)
    int m=0,n=0,k=0,l=0;
    for(k = 0;k < NUM;k++)
    {
        for(l = 0;l < NUM;l++)
        {
            if(k == 0)
            {
                alpha = sqrt(1.0/NUM);
            }
            else
            {
                alpha = sqrt(2.0/NUM);
            }
            if(l == 0)
            {
                beta = sqrt(1.0/NUM);
            }
            else
            {
                beta = sqrt(2.0/NUM);
            }
            double temp = 0.0;
            for(m = 0;m < NUM;m++)
            {
                for(n = 0;n < NUM;n++)
                {
                    temp += data[m][n] * cos((2*m+1)*k*PI/(2.0*NUM)) * cos((2*n+1)*l*PI/(2.0*NUM));
                }
            }
            output[k][l] = around(alpha * beta *temp);
        }
    }
    memset(data,0,sizeof(int)*NUM*NUM);
    memcpy(data,output,sizeof(int)*NUM*NUM);
 
}
//Inverse DCT
void IDCT(int data[NUM][NUM])
{
    int output[NUM][NUM];
    double alpha,beta;
    int m=0,n=0,k=0,l=0;
    for(m = 0;m < NUM;m++)
    {
        for(n = 0;n < NUM;n++)
        {
            double temp = 0.0;
            for(k = 0;k < NUM;k++)
            {
                for(l = 0;l < NUM;l++)
                {
                    if(k == 0)
                    {
                        alpha = sqrt(1.0/NUM);
                    }
                    else
                    {
                        alpha = sqrt(2.0/NUM);
                    }
                    if(l == 0)
                    {
                        beta = sqrt(1.0/NUM);
                    }
                    else
                    {
                        beta = sqrt(2.0/NUM);
                    }
 
                    temp += alpha * beta * data[k][l] * cos((2*m+1)*k*PI/(2*NUM)) * cos((2*n+1)*l*PI/(2*NUM));
 
                }
            }
            output[m][n] = around(temp);
        }
    }
    memset(data,0,sizeof(int)*NUM*NUM);
    memcpy(data,output,sizeof(int)*NUM*NUM);
 
}
 
int main(int argc, char *argv[])
{
    int input[NUM][NUM];
    ifstream input_1;
    ofstream output_1, output_2;

    string header;
    int matrix[m][n];
    int output[m][n];
    string s_w, s_h;

    if (argc > 1) {
    // file name
    input_1.open(argv[1], ios::in | ios::binary);
    output_1.open("dct.ppm", ios::out | ios::binary);



    }
    else {
    fprintf(stderr, "argv[1] for reading photo to apply DCT");
    exit(0);
    }

    getline(input_1, header);
    s_w = header.substr(3, 3);
    s_h = header.substr(7, 3);
    int w = stoi(s_w), h = stoi(s_h);

    //matrix for first channel
    int** in_matrix_ch1 = new int*[h];
    for (int i = 0; i < h; ++i)
        in_matrix_ch1[i] = new int[w];

    //matrix for second channel
    int** in_matrix_ch2 = new int*[h];
    for (int i = 0; i < h; ++i)
        in_matrix_ch2[i] = new int[w];

    //matrix for thrid channel
    int** in_matrix_ch3 = new int*[h];
    for (int i = 0; i < h; ++i)
        in_matrix_ch3[i] = new int[w];



    for (int i_h = 0; i_h<h; i_h++) {
        for (int i_w = 0; i_w<w; i_w++) {
            char r, g, b;
            input_1.get(r);
            input_1.get(g);
            input_1.get(b);
            in_matrix_ch1[i_h][i_w] = (int)r ;
            in_matrix_ch2[i_h][i_w] = (int)g ;
            in_matrix_ch3[i_h][i_w] = (int)b ;
        }
    }

    int outputMatrix[w][h];
    int dctMatrix[w][h];
    //initialize matrices 
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            outputMatrix[i][j] = 0;
            dctMatrix[i][j] = 0;
        }
    }

 

    for (int i = 0; i < w / 8; i++)
    {
    for (int j = 0; j < h  / 8; j++)
    {
      //find the gray value of 8x8 square and store it in 8x8 matrix 
        for (int n = i * 8; n < (i + 1 ) * 8; n++)
        {
            for (int m = j * 8; m < (j + 1 ) * 8; m++)
            {
          matrix[n%8][m%8] = ((in_matrix_ch1[n][m] + in_matrix_ch2[n][m] + in_matrix_ch3[n][m]) / 3);
        
            }

        }
      

      DCT(matrix);

      for (int n = i * 8; n < (i + 1 ) * 8; n++)
        {
            for (int m = j * 8; m < (j + 1 ) * 8; m++)
            {
          
          dctMatrix[n][m] = matrix[n%8][m%8];
          //cout << dctMatrix[n][m] << endl;
          

            }

        }

    }

  }

for (int i = 0; i < w / 8; i++)
  {
    for (int j = 0; j < h  / 8; j++)
    {
      for (int n = i * 8; n < (i + 1 ) * 8; n++)
      {
        for (int m = j * 8; m < (j + 1 ) * 8; m++)
        {
          matrix[n%8][m%8] = dctMatrix[n][m];
          
        }

      }

      quantization(matrix,q10);
      iQuantization(matrix, q10);
      IDCT(matrix);

      for (int n = i * 8; n < (i + 1 ) * 8; n++)
      {
        for (int m = j * 8; m < (j + 1 ) * 8; m++)
        {
          
          outputMatrix[n][m] = matrix[n%8][m%8];
          

        }

      }

    }

  }



  output_1 << header << endl;
    for (int i_h = 0; i_h<h; i_h++) {
        for (int i_w = 0; i_w<w; i_w++) {
        output_1 << (char)outputMatrix[i_h][i_w];
        output_1 << (char)outputMatrix[i_h][i_w];
        output_1 << (char)outputMatrix[i_h][i_w];

        }

    }





    

   
    return 0;
}

