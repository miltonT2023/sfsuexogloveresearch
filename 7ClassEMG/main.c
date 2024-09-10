

/****************************************************************************
*
*   Copyright (c) 2012 Xiaorong Zhang
*
*
****************************************************************************/



/* ---- Include Files ---------------------------------------------------- */
//#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
//#include <pthread.h>
//#include <getopt.h>
//#include <termios.h>
#include <time.h>
//#include <unistd.h>
#include <assert.h>
#include <math.h>

//#include <sys/ioctl.h>
//#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/unistd.h>
#include "EMG_PR.h"


int main( int argc, char **argv )
{
	float *TrainData;
	float *TestData;
	int *TrainClass;
	float *Feature_matrix;
	FILE *TraindataFile=NULL;
	FILE *TestdataFile=NULL;
	FILE *PSFile=NULL;
	FILE *WgCgFile=NULL;
	FILE *FeatureMatrixFile=NULL;
	int i,j,m;
	float *Wg;
    float *Cg;
	float accuracy;
	float test_accuracy;
	int num_correct;
	int total_win_num;
	int test_decision;
	float *xmean;
	float *xstd;
	int trial_idx=0;
	int class_idx=0;
	int feature_matrix_idx=0;


	TrainData = (float*)malloc(CHANNEL*DATA_PER_TRIAL*sizeof(float));  //raw EMG data of the training dataset
    TestData = (float*)malloc(CHANNEL*DATA_PER_TRIAL*sizeof(float));  //raw EMG data of the training dataset
	TrainClass = (int*)malloc(CLASS*WIN_PER_TRIAL*TRIAL_PER_CLASS*sizeof(int));     //class label for the training dataset
	Feature_matrix = (float*)malloc(CLASS*WIN_PER_TRIAL*TRIAL_PER_CLASS*FEATURE_DIM*sizeof(float));
	Wg=(float*)malloc(FEATURE_DIM*CLASS*sizeof(float));
    Cg=(float*)malloc(1*CLASS*sizeof(float));
	xmean=(float*)malloc(1*FEATURE_DIM*sizeof(float));
	xstd=(float*)malloc(1*FEATURE_DIM*sizeof(float));



	printf("********************************************************\n");
	printf("*******      Training Session Starts      **************\n");
	printf("********************************************************\n\n");

	printf("********************************************************\n");
	printf("*******      Class 1, Trial 1  NoMovement1.txt  **************\n");
	printf("********************************************************\n\n");

	//class 1, trial 1
	TraindataFile=fopen("..\\data\\train\\NoMovement1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;
	feature_matrix_idx=0;
	m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}


	printf("********************************************************\n");
	printf("*******      Class 1, Trial 2 NoMovement3.txt   **************\n");
	printf("********************************************************\n\n");

    //class 1, trial 2
	TraindataFile=fopen("..\\data\\train\\NoMovement3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class1 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 1, Trial 3 NoMovement5.txt   **************\n");
	printf("********************************************************\n\n");


    //class 1, trial 3
	TraindataFile=fopen("..\\data\\train\\NoMovement5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class1 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 2, Trial 1 HandClose1.txt   **************\n");
	printf("********************************************************\n\n");


    class_idx++; //class 2
	//class 2, trial 1
	TraindataFile=fopen("..\\data\\train\\HandClose1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class2 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 2, Trial 2 HandClose3.txt   **************\n");
	printf("********************************************************\n\n");


    //class 2, trial 2
	TraindataFile=fopen("..\\data\\train\\HandClose3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class2 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 2, Trial 3 HandClose5.txt   **************\n");
	printf("********************************************************\n\n");


    //class 2, trial 3
	TraindataFile=fopen("..\\data\\train\\HandClose5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class1 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}


	printf("********************************************************\n");
	printf("*******      Class 3, Trial 1 HandOpen1.txt   **************\n");
	printf("********************************************************\n\n");

	class_idx++; //class 3
	//class 3, trial 1
	TraindataFile=fopen("..\\data\\train\\HandOpen1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class3 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 3, Trial 2 HandOpen3.txt   **************\n");
	printf("********************************************************\n\n");

    //class 3, trial 2
	TraindataFile=fopen("..\\data\\train\\HandOpen3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class1 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 3, Trial 3 HandOpen5.txt   **************\n");
	printf("********************************************************\n\n");

    //class 3, trial 3
	TraindataFile=fopen("..\\data\\train\\HandOpen5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class3 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 4, Trial 1 Pronation1.txt   **************\n");
	printf("********************************************************\n\n");
	class_idx++; //class 4

	//class 4, trial 1
	TraindataFile=fopen("..\\data\\train\\Pronation1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class4 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 4, Trial 2 Pronation3.txt   **************\n");
	printf("********************************************************\n\n");

	
	//class 4, trial 2
	TraindataFile=fopen("..\\data\\train\\Pronation3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class4 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}
	printf("********************************************************\n");
	printf("*******      Class 4, Trial 3 Pronation5.txt   **************\n");
	printf("********************************************************\n\n");

	
	//class 4, trial 3
	TraindataFile=fopen("..\\data\\train\\Pronation5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class4 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}
	printf("********************************************************\n");
	printf("*******      Class 5, Trial 1 Supination1.txt  **************\n");
	printf("********************************************************\n\n");
	class_idx++; //class 5

		
	//class 5, trial 1
	TraindataFile=fopen("..\\data\\train\\Supination1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class5 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 5, Trial 2 Supination3.txt  **************\n");
	printf("********************************************************\n\n");

		//class 5, trial 2
	TraindataFile=fopen("..\\data\\train\\Supination3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class5 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 5, Trial 3 Supination5.txt   **************\n");
	printf("********************************************************\n\n");

		//class 5, trial 3
	TraindataFile=fopen("..\\data\\train\\Supination5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class5 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 6, Trial 1 WristFlexion1.txt  **************\n");
	printf("********************************************************\n\n");
	class_idx++; //class 6
	//class 6, trial 1
	TraindataFile=fopen("..\\data\\train\\WristFlexion1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class 6 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 6, Trial 2 WristFlexion3.txt  **************\n");
	printf("********************************************************\n\n");

	//class 6, trial 2
	TraindataFile=fopen("..\\data\\train\\WristFlexion3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class 6 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 6, Trial 3 WristFlexion5.txt  **************\n");
	printf("********************************************************\n\n");

	//class 6, trial 3
	TraindataFile=fopen("..\\data\\train\\WristFlexion5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class 6 trial 3... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	printf("********************************************************\n");
	printf("*******      Class 7, Trial 1 WristExtension1.txt   **************\n");
	printf("********************************************************\n\n");
	class_idx++; //class 7
	
	//class 7, trial 1
	TraindataFile=fopen("..\\data\\train\\WristExtension1.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class7 trial 1... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	
	printf("********************************************************\n");
	printf("*******      Class 7, Trial 2 WristExtension3.txt  **************\n");
	printf("********************************************************\n\n");

	//class 7, trial 2
	TraindataFile=fopen("..\\data\\train\\WristExtension3.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class7 trial 2... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}

	
	printf("********************************************************\n");
	printf("*******      Class 7, Trial 3 WristExtension5.txt  **************\n");
	printf("********************************************************\n\n");

	//class 7, trial 3
	TraindataFile=fopen("..\\data\\train\\WristExtension5.txt","r");
	if (TraindataFile == NULL) {
		printf("Unable to open traindata file for class7 trial 3... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TraindataFile, "%f", &TrainData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TraindataFile);

	trial_idx++;

    m=0;
	while(m<WIN_PER_TRIAL)
	{
		tdfeats(TrainData+WINC*CHANNEL*m,WL,CHANNEL, feature_matrix_idx, Feature_matrix);
		TrainClass[feature_matrix_idx] = class_idx+1;
		feature_matrix_idx++;
		m++;
	}


    printf("feature_matrix_idx=%d,m=%d\n", feature_matrix_idx, m);

	j=0;
	/**/

    FeatureMatrixFile=fopen("FeatureMatrix_before_norm.txt","w");
    for(i=0;i<feature_matrix_idx;i++){
        for(j=0;j<FEATURE_DIM;j++){
            fprintf(FeatureMatrixFile, "%f ", Feature_matrix[j+i*FEATURE_DIM]);
        }
        fprintf(FeatureMatrixFile, "\n ");
    }
	fclose(FeatureMatrixFile);

	feature_normalization(Feature_matrix, xmean, xstd,feature_matrix_idx);
	printf("Feature normalization complete!!!\n\n");
	LDA_train(Feature_matrix,TrainClass, Wg, Cg);

	FILE *file = fopen("TrainClass.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write TrainClass values to the file with line numbers
    for (int i = 0; i < 1953; i++) {
        fprintf(file, "Line %d: Train Class: %d\n", i + 1, TrainClass[i]);
    }

    // Close the file
    fclose(file);

    printf("Data successfully written to TrainClass.txt");

	printf("training complete!!!\n\n");


	for(j=0;j<FEATURE_DIM;j++)
    {
        printf("\nxmean[%d]:%.4g\t,xstd:%.4g",j,xmean[j],xstd[j]);
    }
	for(i=0;i<CLASS;i++){
		printf("\nWg[%d]=\n",i);
		for(j=0;j<FEATURE_DIM;j++)
        {
            printf("%.4g\t",Wg[i+j*CLASS]);
        }
        printf("\nCg[%d]=%.4g\t",i,Cg[i]);
		printf("\n");
	}


	accuracy=LDA_train_accuracy(Feature_matrix,TrainClass, Wg, Cg);
	

	PSFile=fopen("PS.txt","w");
	for(j=0;j<FEATURE_DIM;j++){
		fprintf(PSFile, "%f ", xmean[j]);
		fprintf(PSFile, "%f\n", xstd[j]);
	}
	fclose(PSFile);

	WgCgFile=fopen("WgCg.txt","w");
	for(j=0;j<FEATURE_DIM;j++){
		for(i=0;i<CLASS;i++){
			fprintf(WgCgFile, "%f ", Wg[i+j*CLASS]);
		}
		fprintf(WgCgFile, "\n ");
	}
	for(i=0;i<CLASS;i++){
		fprintf(WgCgFile, "%f ", Cg[i]);
	}

	fclose(WgCgFile);

	

    FeatureMatrixFile=fopen("FeatureMatrix.txt","w");
    for(i=0;i<feature_matrix_idx;i++){
        for(j=0;j<FEATURE_DIM;j++){
            fprintf(FeatureMatrixFile, "%f ", Feature_matrix[j+i*FEATURE_DIM]);
        }
        fprintf(FeatureMatrixFile, "\n ");
    }
	fclose(FeatureMatrixFile);

	

	

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////   testing phase///////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////



    //class 1, trial 1
	TestdataFile=fopen("..\\data\\test\\NoMovement2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);


	class_idx=0;
	total_win_num=0;
	num_correct=0;
    m=0;


	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}
	

	    //class 1, trial 2
	TestdataFile=fopen("..\\data\\test\\NoMovement4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 1, trial 3
	TestdataFile=fopen("..\\data\\test\\NoMovement6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}


    //class 2, trial 1
	TestdataFile=fopen("..\\data\\test\\HandClose2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class2 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);


	class_idx++;
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 2, trial 2
	TestdataFile=fopen("..\\data\\test\\HandClose4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class2 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 2, trial 3
	TestdataFile=fopen("..\\data\\test\\HandClose6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class2 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}


    //class 3, trial 1
	TestdataFile=fopen("..\\data\\test\\HandOpen2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);


	class_idx++;
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 3, trial 2
	TestdataFile=fopen("..\\data\\test\\HandOpen4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

    //class 3, trial 3
	TestdataFile=fopen("..\\data\\test\\HandOpen6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}
	
    //class 4, trial 1
	TestdataFile=fopen("..\\data\\test\\Pronation2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);

	class_idx++;
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}
	

	    //class 4, trial 2
	TestdataFile=fopen("..\\data\\test\\Pronation4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 4, trial 3
	TestdataFile=fopen("..\\data\\test\\Pronation6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class1 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}


    //class 5, trial 1
	TestdataFile=fopen("..\\data\\test\\Supination2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class2 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);


	class_idx++;
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 5, trial 2
	TestdataFile=fopen("..\\data\\test\\Supination4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class2 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 5, trial 3
	TestdataFile=fopen("..\\data\\test\\Supination6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class2 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}


    //class 6, trial 1
	TestdataFile=fopen("..\\data\\test\\WristFlexion2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 1 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);


	class_idx++;
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	    //class 6, trial 2
	TestdataFile=fopen("..\\data\\test\\WristFlexion4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 2 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

    //class 6, trial 3
	TestdataFile=fopen("..\\data\\test\\WristFlexion6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	 //class 7, trial 1
	TestdataFile=fopen("..\\data\\test\\WristExtension2.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
	//printf("address0:%d, data:%f,address1:%d, data:%f\n", &TrainData[0], TrainData[0],&TrainData[1],TrainData[1]);
	while(fscanf(TestdataFile, "%f", &TestData[i])!=EOF){
		//printf("%d: %f\n",i,TrainData[i]);
		i++;
	}
	printf("fscanf complete!\n");
	fclose(TestdataFile);
	class_idx++;
    m=0;
	while(m<WIN_PER_TRIAL)
	{

        test_decision = LDA_test(TestData+WINC*CHANNEL*m, Wg, Cg, xmean, xstd);
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	 //class 7, trial 2
	TestdataFile=fopen("..\\data\\test\\WristExtension4.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	//class 7, trial 3
	TestdataFile=fopen("..\\data\\test\\WristExtension6.txt","r");
	if (TestdataFile == NULL) {
		printf("Unable to open testdata file for class3 trial 3 ... \n");
		perror("fopen");
		exit(0);
	}
	i=0;
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
        if(test_decision==class_idx+1)
            num_correct++;
		m++;
		total_win_num++;
	}

	test_accuracy=((float)num_correct)/total_win_num;
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////   testing phase end    ///////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("\ntraining accuracy=%.4f\t\n",accuracy);
    printf("\ntesting accuracy=%.4f\t\n",test_accuracy);

	free(TrainData);
	free(TestData);
    free(TrainClass);
	free(Feature_matrix);
	free(Wg);
    free(Cg);
	free(xmean);
	free(xstd);

	time_t current_time;
    time(&current_time);

    // Convert to local time format and print it
    printf("Current time: %s", ctime(&current_time));

 	return 0;

}
