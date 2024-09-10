
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>
#include <math.h>

// pattern recognition parameter definitions
#define WL  160         //windlow length
#define WINC    20      //window increment
#define TD_FEATURES 4   //number of features
#define CHANNEL 6       //number of channels
#define CLASS 7      //number of classes
#define FEATURE_DIM (TD_FEATURES*CHANNEL) //feature vector dimension
#define TRAINDATA_ROW	Feature_DIM     //number of rows in the training feature matrix (i.e. the feature vector dimension)
#define	TRAINDATA_COLUMN	WINDOW_NUM  //number of columns in the training feature matrix (i.e. the total number of windows in the training data)
#define WL_DIV_WINC (WL/WINC-1)
#define MV_NUM 5    //majority vote windows

#define TRIAL_PER_CLASS 3   //number of data trials collected for each class
#define DATA_PER_TRIAL  2000 //number of data samples per trial per channel
#define WIN_PER_TRIAL   (DATA_PER_TRIAL/WINC-WL_DIV_WINC)

// feature extraction parameter definitions
#define DEADZONE_ZC	0.025       //estimated baseline noise level for calculating zero crossings
#define DEADZONE_TURN	0.015   //estimated noise level for calculating slope sign changes
#define SCALE_ZC	15          //parameter for zero crossing
#define SCALE_MAV	2           //parameter for mean absolute value

#define FEATURE_MAV_IDX     0      //feature index MAV: mean absolute value
#define FEATURE_LEN_IDX     1      //feature index LEN: waveform length
#define FEATURE_ZERO_IDX    2      //feature index ZERO: number of zero crossings
#define FEATURE_TURN_IDX    3      //feature index TURN: number of slope sign changes

// EMG_PR.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "EMG_PR.h"

/*  Matrix Multiply
    C=A*B
    A:(am x an)
    B:(an x bn)
    C:(am x bn)
*/
void mulAB(float *A,float *B,float *C,int am,int an,int bn)
{
    int i,j,l,u;

    for (i=0; i<am; i++)
        for (j=0; j<bn; j++)
        {
            u=i*bn+j; C[u]=0.0;
            for (l=0; l<an; l++)
                C[u]=C[u]+A[i*an+l]*B[l*bn+j];
        }
    return;
}

/*  Matrix Addition
    C=A+B
    A,B,C:(m x n)
*/
void addition(float *A,float *B,float *C, int m,int n)
{
    int i,j;

    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
          {
              C[i*m+j]=A[i*m+j]+B[i*m+j];
          }
    return;
}
/*  Matrix Subtraction
    C=A-B
    A,B,C:(m x n)
*/
void subtract(float *A,float *B,float *C, int m,int n)
{
    int i,j;

    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
          {
              C[i*m+j]=A[i*m+j]-B[i*m+j];
          }
    return;
}

/*  Matrix Transpose
    Input: A (m x n)
    Output: B (n x m)
*/
void Transpose(float *A,float *B,int m,int n)
{
    int i,j;

    for (j=0; j<n; j++)
        for (i=0; i<m; i++)
          {
              B[j*m+i]=A[i*n+j];
          }
    return;
}

/*  Covariance Matrix
    Input: A (row x col)
    Output: B (col x col)
*/
void cov(float *A,float *B,int row,int col)
{
    int i,j;
    float *tmp;
    float *tmp1;
    float *t_tmp1;
    float sum=0;

    printf("row:%d, col:%d\n",row,col);
    t_tmp1=(float*)malloc(col*row*sizeof(float));
    tmp1=(float*)malloc(row*col*sizeof(float));
    tmp=(float*)malloc(col*sizeof(float));

    memset(tmp,0,col*sizeof(float));
    memset(tmp1,0,row*col*sizeof(float));
    memset(t_tmp1,0,row*col*sizeof(float));
    //printf("tmp:%d, tmp1:%d, t_tmp1:%d\n",&tmp[0],&tmp1[0],&t_tmp1[0]);
    //printf("cov1\n");
    //calculate mean of each column
    for (i=0; i<col; i++)
    {
        sum = 0;
        for (j=0; j<row; j++)
        {
            sum+=A[j*col+i];
        }
        tmp[i]=sum/row;
		//printf("%f\n",tmp[i]);
    }

    //substract matrix A with mean
    for (i=0; i<col; i++)
    {
        for (j=0; j<row; j++)
        {
            tmp1[j*col+i]=A[j*col+i]-tmp[i];
            //float tmptmp = tmp1[j*n+i];
        }
    }
    //printf("cov2\n");
    //multiply tmp1 with transpose of tmp1
    Transpose(tmp1,t_tmp1,row,col);
    //printf("cov3\n");
    mulAB(t_tmp1,tmp1,B,col,row,col);
    //printf("cov4\n");
    //calculate mean of each element of B
    //printf("tmp:%d, tmp1:%d, t_tmp1:%d\n",&tmp[0],&tmp1[0],&t_tmp1[0]);
   // printf("tmp:%d, tmp1:%d, t_tmp1:%d\n",&tmp[col-1],&tmp1[row*col-1],&t_tmp1[row*col-1]);
    for (i=0; i<col; i++)
    {
        for (j=0; j<col; j++)
        {
            B[j*col+i]=B[j*col+i]/(row-1);
        }
    }

    free(t_tmp1);
    free(tmp1);
    //printf("cov5\n");
    free(tmp);

    return;
}

void tdfeats(float *DataSet_in,int win_length,int channel, int Nframe, float *Features)
{
    float ruler=0;
    float rulersq;
    float lscale;
    float tscale;

    int Nsig;

    int i,j;
    float sum=0;
    float *mean;
	float *DataSet;

    float zero_count;
    float len;
    float mav;
    float turns;
    int index;
    int range;
    float sum_sig;
    int flag1;
    int flag2;
    int idx;
    float fst;
    float mid;
    float lst;
    int SigNum;

    ruler=1/(float)win_length;
    rulersq=ruler*ruler;
    lscale=(float)win_length/40;
    tscale=((float)win_length/40)*10;


    Nsig=channel;


    mean=(float*)malloc(1*channel*sizeof(float));
	DataSet=(float*)malloc(1*win_length*channel*sizeof(float));

    for(i=0;i<channel;i++)
    {
        sum=0;
        for(j=0;j<win_length;j++)
        {
            sum=sum+DataSet_in[j*channel+i];
        }
        mean[i]=sum/win_length;//mean of a sigal (column)
    }

    for(i=0;i<win_length;i++)
    {
        for(j=0;j<channel;j++)
        {
            DataSet[i*channel+j]=DataSet_in[i*channel+j]-mean[j];
        }
    }

    free(mean);
    /*for(i=0;i<DataSet_Row;i++)
    {
        for(j=0;j<channel;j++)
        {
            printf("%.4f ",DataSet[i*channel+j]);
        }
        printf("\n");
    }
    printf("***************************************\n");*/

    for(SigNum=0;SigNum<Nsig;SigNum++)
    {
        zero_count=0;
        len=0;
        mav=0;
        turns=0;

        index=0;
        sum_sig=0;
        for(range=index;range<index+win_length;range++)
        {
            sum_sig=sum_sig+(float)fabs(DataSet[range*channel+SigNum]);//sum of a signal (a column)
        }
        mav=sum_sig/win_length;//mean of a signal

        flag1=1;
        flag2=1;
        for(i=1;i<win_length-1;i++)//win_length=DATASET_ROW
        {
            idx=i;    //idx=2:DATASET_ROW-1
            fst=(float)fabs(DataSet[(idx-1)*channel+SigNum]);//DataSet[0,0]:DataSet[DATASET_ROW-3,0]
            mid=(float)fabs(DataSet[(idx)*channel+SigNum]);  //DataSet[1,0]:DataSet[DATASET_ROW-2,0]
            lst=(float)fabs(DataSet[(idx+1)*channel+SigNum]);//DataSet[2,0]:DataSet[DATASET_ROW-1,0]

            //% Compute Zero Crossings
            if(((DataSet[(idx)*channel+SigNum]>=0) &&(DataSet[(idx-1)*channel+SigNum]>=0))
                ||((DataSet[(idx)*channel+SigNum]<=0) &&(DataSet[(idx-1)*channel+SigNum]<=0)))
            {
                flag1=flag2;//if (DataSet[1,0]>=0&&DataSet[0,0]>=0)||(DataSet[1,0]<=0&&DataSet[0,0]<=0)
            }else
            {
                if((mid<DEADZONE_ZC)&&(fst<DEADZONE_ZC))//approximately zero
                {
                    flag1=flag2;
                }
                else
                {
                    flag1=(-1)*flag2;
                }
            }
            if(flag1!=flag2)
            {
                zero_count=zero_count+1;
            }
            //% Compute Turns (Slope Changes
            if(((mid>fst)&&(mid>lst))||((mid<fst)&&(mid<lst)))
            {
                //% turns threshold of 15mV (i.e. 3uV noise)
                if((fabs(mid)-fabs(fst))>DEADZONE_TURN||(fabs(mid)-fabs(lst))>DEADZONE_TURN)
                {
                    turns=turns+1;
                }
            }
            //% Compute Waveform Length
            //len=len+(float)sqrt(((fst-mid)/20.0)*((fst-mid)/20.0)+rulersq);
            len=len+(float)sqrt(((fst-mid)/20.0)*((fst-mid)/20.0)+rulersq);//rulersq=(1/DATASET_ROW)^2
        }

        //% Scale the features to normalize for the neural network

        zero_count=(zero_count/SCALE_ZC)*40/win_length;

        //% scaling based on 40 ms

        mav=mav/SCALE_MAV;
        len=(len-1)/lscale;
        turns=turns/tscale;
        Features[4*channel*Nframe+SigNum*TD_FEATURES] = mav;
        Features[4*channel*Nframe+SigNum*TD_FEATURES+1] = len;
        Features[4*channel*Nframe+SigNum*TD_FEATURES+2] = zero_count;
        Features[4*channel*Nframe+SigNum*TD_FEATURES+3] = turns;

    }
	free(DataSet);
}

void feature_normalization(float *features, float * xmean, float * xstd,int num)
{
	int j,m;
	//float *tmp;
	//float *tmp1;
	float *xstd_mean;
	//int test_pro;
	float sum=0.0;
	float xstd_sum=0.0;


	xstd_mean=(float*)malloc(1*FEATURE_DIM*sizeof(float));

	for(m=0;m<FEATURE_DIM;m++)
    {
		sum=0;
		xstd_sum=0;
        for(j=0;j<num;j++)
        {
            sum+=features[m+FEATURE_DIM*j];
        }
        xmean[m]=sum/num;
	printf("sum[%d]:%f\n",m,sum);

		for(j=0;j<num;j++)
        {
            features[m+FEATURE_DIM*j]=features[m+FEATURE_DIM*j]-xmean[m];
			xstd_sum+=features[m+FEATURE_DIM*j]*features[m+FEATURE_DIM*j];
        }
		xstd_mean[m]=xstd_sum/(num-1);
		xstd[m]=(float)sqrt(xstd_mean[m]);

		for(j=0;j<num;j++)
        {
            features[m+FEATURE_DIM*j]=features[m+FEATURE_DIM*j]/xstd[m];
        }
    }

	free(xstd_mean);

	return;
}

void feature_normalization_apply(float *features, float * xmean, float * xstd)
{
	int m;

	for(m=0;m<FEATURE_DIM;m++)
    {
		features[m]=features[m]-xmean[m];
        features[m]=features[m]/xstd[m];
    }

	return;
}

int LDA_test(float *TestData, float *Wg, float *Cg, float *xmean, float *xstd)
{
	int j;
	float *Feature_test;
	float *tmp;
	float *tmp1;
	float maxdata=-9999.0;
	int test_decision;


	maxdata=-9999.0;
	Feature_test = (float*)malloc(FEATURE_DIM*sizeof(float));
	tmp=(float*)malloc(1*CLASS*sizeof(float));
	tmp1=(float*)malloc(1*CLASS*sizeof(float));

	tdfeats(TestData,WL,CHANNEL, 0, Feature_test);
	feature_normalization_apply(Feature_test,xmean,xstd);

	mulAB(Feature_test,Wg,tmp,1,FEATURE_DIM,CLASS);
	addition(Cg,tmp,tmp1,1,CLASS);

	for(j=0;j<CLASS;j++){
		if(tmp1[j]>maxdata)
		{
			maxdata=tmp1[j];
			test_decision=j+1;
		}
	}

	free(Feature_test);
	free(tmp);
	free(tmp1);
	return test_decision;
}   

int Lower_Triangular_Solve(float *L, float B[], float x[], int n)
{
   int i, k;

//         Solve the linear equation Lx = B for x, where L is a lower
//         triangular matrix.

   for (k = 0; k < n; L += n, k++) {
      if (*(L + k) == 0.0) return -1;           // The matrix L is singular
      x[k] = B[k];
      for (i = 0; i < k; i++) x[k] -= x[i] * *(L + i);
      x[k] /= *(L + k);
   }

   return 0;
}                                      
int Upper_Triangular_Solve(float *U, float B[], float x[], int n)
{
   int i, k;

//         Solve the linear equation Ux = B for x, where U is an upper
//         triangular matrix.

   for (k = n-1, U += n * (n - 1); k >= 0; U -= n, k--) {
      if (*(U + k) == 0.0) return -1;           // The matrix U is singular
      x[k] = B[k];
      for (i = k + 1; i < n; i++) x[k] -= x[i] * *(U + i);
      x[k] /= *(U + k);
   }

   return 0;
}
                                                                         
int Choleski_LU_Decomposition(float *A, int n)
{
   int i, k, p;
   float *p_Lk0;                   // pointer to L[k][0]
   float *p_Lkp;                   // pointer to L[k][p]
   float *p_Lkk;                   // pointer to diagonal element on row k.
   float *p_Li0;                   // pointer to L[i][0]
   float reciprocal;

   for (k = 0, p_Lk0 = A; k < n; p_Lk0 += n, k++) {

//            Update pointer to row k diagonal element.

      p_Lkk = p_Lk0 + k;

//            Calculate the difference of the diagonal element in row k
//            from the sum of squares of elements row k from column 0 to
//            column k-1.

      for (p = 0, p_Lkp = p_Lk0; p < k; p_Lkp += 1,  p++)
         *p_Lkk -= *p_Lkp * *p_Lkp;

//            If diagonal element is not positive, return the error code,
//            the matrix is not positive definite symmetric.

      if ( *p_Lkk <= 0.0 ) return -1;

//            Otherwise take the square root of the diagonal element.

      *p_Lkk = sqrt( *p_Lkk );
      reciprocal = 1.0 / *p_Lkk;

//            For rows i = k+1 to n-1, column k, calculate the difference
//            between the i,k th element and the inner product of the first
//            k-1 columns of row i and row k, then divide the difference by
//            the diagonal element in row k.
//            Store the transposed element in the upper triangular matrix.

      p_Li0 = p_Lk0 + n;
      for (i = k + 1; i < n; p_Li0 += n, i++) {
         for (p = 0; p < k; p++)
            *(p_Li0 + k) -= *(p_Li0 + p) * *(p_Lk0 + p);
         *(p_Li0 + k) *= reciprocal;
         *(p_Lk0 + i) = *(p_Li0 + k);
      }
   }
   return 0;
}
                                                                        
int Choleski_LU_Solve(float *LU, float B[], float x[], int n)
{

//         Solve the linear equation Ly = B for y, where L is a lower
//         triangular matrix.

   if ( Lower_Triangular_Solve(LU, B, x, n) < 0 ) return -1;

//         Solve the linear equation Ux = y, where y is the solution
//         obtained above of Ly = B and U is an upper triangular matrix.

   return Upper_Triangular_Solve(LU, x, x, n);
}



int majority_vote(int * decision_buffer, int size)
{
	int i,j;
	int maxdata=0;
	int modes[CLASS+1];
	int test_decision=0;

	for(i=0;i<CLASS+1;i++)
	{
		modes[i]=0;
	}
	for(i=0;i<size;i++)
	{
		modes[decision_buffer[i]]=modes[decision_buffer[i]]+1;
	}
	for(j=0;j<CLASS+1;j++){
		if(modes[j]>maxdata)
		{
			maxdata=modes[j];
			test_decision=j;
		}
	}
	return test_decision;
}


/*
 Class ID number: 
 NoMovement: 1
 HandClose: 2
 HandOpen: 3
 Pronation: 4
 Supination: 5
 WristFlexion: 6
 WristExtension: 7
*/ 

float Wg[FEATURE_DIM * CLASS] = {
    -27.187414, 58.471958, -2.244706, -18.857361, -8.853570, 20.747629, -22.077387, 
    17.364420, -27.425165, -4.231650, 12.105726, 5.696319, -12.862558, 9.353340, 
    6.332460, 10.168828, -3.031059, -3.024451, -5.529425, 18.972343, -23.889305, 
    -20.177244, 7.115144, 7.916430, -6.751305, -1.634405, -0.009761, 13.541560, 
    -4.615982, -37.094845, -30.185205, 7.586791, -18.750572, 84.754707, -1.694381, 
    7.124239, 12.659680, 14.966351, -5.520625, 8.800712, -30.491936, -7.538836, 
    14.590693, -9.385255, -16.402544, 6.798468, -4.065749, 23.870092, -15.405696, 
    -14.369049, 2.168241, 1.453007, -2.583697, -3.754423, 3.960223, 13.125747, 
    -3.415135, 33.523289, -15.762914, 5.622562, 0.226427, -21.607548, 1.413317, 
    9.984710, 20.796961, -4.407976, -0.853953, 0.523243, -2.447813, -23.595251, 
    -2.663765, 3.695914, -3.944426, -3.460593, 2.980941, 6.030308, -2.638658, 
    -4.278034, -1.187661, 0.424796, 2.025982, 1.214335, 2.492591, -0.691830, 
    -60.823502, 40.251431, 29.272532, -30.007944, -18.773481, -40.043724, 80.124847, 
    40.102829, -30.883316, -29.269260, 15.973817, 9.864521, -5.360301, -0.428302, 
    -11.226702, 5.783944, 3.562052, -5.689164, 2.876976, -4.508313, 9.201044, 
    -31.805008, 10.121118, 3.573610, -0.558782, 9.389293, -0.654629, 9.934812, 
    -66.227310, -45.147514, 65.717964, -14.853122, 15.160642, -22.264484, 67.614975, 
    50.388767, -6.020882, -25.804470, 9.171989, -12.293757, -5.712678, -9.729795, 
    -29.982845, -3.855878, 16.715845, -10.212946, 2.796122, -5.109118, 29.649778, 
    -26.047701, -0.376809, 11.751267, 1.191841, 7.534623, -2.160675, 8.107151, 
    -28.562449, 4.796143, 28.038752, -28.235750, -4.676010, 9.761995, 18.877909, 
    26.397627, -22.929756, -15.596963, 22.329462, 2.520857, 0.975060, -13.696510, 
    -13.301920, 8.543428, 5.524446, -15.367639, -9.274214, 19.056181, 4.819311, 
    -15.936728, 4.754780, 9.375063, -15.444491, -0.673914, 8.687138, 9.238109
};

float Cg[CLASS] = {
    -188.615799, -118.453552, -52.959263, -40.837193, -19.987963, -149.928650, -181.273376
};


// Initialize xmean and xstd arrays directly from the PSFile data
float xmean[FEATURE_DIM] = {
    0.026724, 0.018970, 0.345887, 0.703379, 0.021710, 0.015446, 0.269031, 0.641666,
    0.033966, 0.026681, 0.547065, 0.985381, 0.067762, 0.095997, 0.493865, 0.884536,
    0.098602, 0.165948, 0.503071, 0.898323, 0.029763, 0.027265, 0.419723, 0.795763
};

float xstd[FEATURE_DIM] = {
    0.024344, 0.031480, 0.252970, 0.341241, 0.024234, 0.035754, 0.230112, 0.320246,
    0.028380, 0.033061, 0.227045, 0.199830, 0.076735, 0.167935, 0.245113, 0.304911,
    0.100134, 0.255021, 0.247107, 0.297019, 0.024145, 0.041606, 0.303517, 0.337042
};


int main( int argc, char **argv )
{
	float *TestData;
	FILE *TestdataFile=NULL;
	float test_accuracy;
	int num_correct;
	int total_win_num;
	int test_decision;
	int class_idx=0;
	int i,j,m;

	TestData = (float*)malloc(CHANNEL*DATA_PER_TRIAL*sizeof(float));  //raw EMG data of the test dataset

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////   testing phase///////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class_idx=0;
	total_win_num=0;
	num_correct=0;

    //class 1, trial 1
	TestdataFile=fopen("..\\data\\test\\NoMovement2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	printf("Reading testdata file for class1 trial 1 ... \n");
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==1)
            num_correct++;
		m++;
		total_win_num++;
	}
	
	test_accuracy = (float)num_correct / total_win_num * 100;
	printf("Testing accuracy: %.2f%%\n", test_accuracy);

	// Free memory
	free(TestData);

 	return 0;

}
