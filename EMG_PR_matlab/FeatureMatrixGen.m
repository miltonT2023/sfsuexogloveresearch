%%-------Real-time pattern recognition training procedure---------%%
%%%%%%%%%%%%%%%%%%%%% By Xiaorong Zhang, 05/03/2013  %%%%%%%%%%%%%%%%%%%%

clear all;

WL=160;         %window length
WInc=20;        %window increment
TD_Features=4;  %number of features
CHANNELS=6;     %number of channels
CLASS=7;        %number of classes

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%% Training Phase %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
file(1).name= dir('..\data\train\NoMovement*.txt');
file(2).name= dir('..\data\train\HandClose*.txt');
file(3).name = dir('..\data\train\HandOpen*.txt');
file(4).name= dir('..\data\train\Pronation*.txt');
file(5).name= dir('..\data\train\Supination*.txt');
file(6).name = dir('..\data\train\WristFlexion*.txt');
file(7).name= dir('..\data\train\WristExtension*.txt');


for i=1:length(file)
    Feature_matrix(i).feat=[];
end
        
for j=1:length(file)
    for i=1:length(file(j).name)
        RawEMG = load (strcat('..\data\train\',file(j).name(i).name));
        Index=1:WL;
        while Index(end)<=size(RawEMG,1)
            Feature = tdfeats(RawEMG(Index,:),1,WL,WInc); % feature vector for one window
            Feature_matrix(j).feat=[Feature_matrix(j).feat,Feature];    % feature matrix 
            Index=Index+WInc;
        end  
    end
end
    
  
TrainFeature=[];
TrainClass=[];

for j=1:length(file)        
    TrainFeature=[TrainFeature,Feature_matrix(j).feat];
    TrainClass=[TrainClass,j*ones(1,size(Feature_matrix(j).feat,2))];
end
[TrainFeature,PS.set]=mapstd(TrainFeature); % normalize the feature vector
total_win_num = size(TrainFeature,2);   %total number of windows in the training dataset

TrainTable = [TrainFeature; TrainClass];
% [Wg, Cg]=LDA_train(TrainFeature,TrainClass,total_win_num,CLASS); % LDA training
% 
% train_accuracy = LDA_train_accuracy(TrainFeature,TrainClass,total_win_num,CLASS, Wg, Cg);

%%%%%%%%% End of Training Phase %%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%% Testing Phase %%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
file(1).name= dir('..\data\test\NoMovement*.txt');
file(2).name= dir('..\data\test\HandClose*.txt');
file(3).name = dir('..\data\test\HandOpen*.txt');
file(4).name= dir('..\data\test\Pronation*.txt');
file(5).name= dir('..\data\test\Supination*.txt');
file(6).name = dir('..\data\test\WristFlexion*.txt');
file(7).name= dir('..\data\test\WristExtension*.txt');


for i=1:length(file)
    Feature_matrix(i).feat=[];
end
        
for j=1:length(file)
    for i=1:length(file(j).name)
        RawEMG = load (strcat('..\data\test\',file(j).name(i).name));
        Index=1:WL;
        while Index(end)<=size(RawEMG,1)
            Feature = tdfeats(RawEMG(Index,:),1,WL,WInc); % feature vector for one window
            Feature_matrix(j).feat=[Feature_matrix(j).feat,Feature];    % feature matrix 
            Index=Index+WInc;
        end  
    end
end
    
  
TestFeature=[];
TestClass=[];

for j=1:length(file)        
    TestFeature=[TestFeature,Feature_matrix(j).feat];
    TestClass=[TestClass,j*ones(1,size(Feature_matrix(j).feat,2))];
end
[TestFeature,PS.set]=mapstd(TestFeature); % normalize the feature vector
total_win_num = size(TestFeature,2);   %total number of windows in the training dataset

TestTable = [TestFeature; TestClass];
% test_num_correct=0;   % number of correct test decisions
% test_num=0; % total number of testing windows
% for j=1:length(file)
%     for i=1:length(file(j).name)
%         RawEMG = load (strcat('..\data\test\',file(j).name(i).name));
%         Index=1:WL;
%         while Index(end)<=size(RawEMG,1)
%             Feature = tdfeats(RawEMG(Index,:),1,WL,WInc); % feature vector for one window
%             Feature=mapstd('apply',Feature,PS.set); % feature normalization
%             test_decision=LDA_test(Feature, Wg, Cg);
%             if test_decision==j
%                 test_num_correct=test_num_correct+1;
%             end
%             Index=Index+WInc;
%             test_num=test_num+1;
%         end  
%     end
% end
% test_accuracy=test_num_correct/test_num;

%%%%%%%%% End of Testing Phase %%%%%%%%%%%%

save FeatureMatrixGen.mat TrainTable TestClass TestFeature TrainClass TrainFeature;

 