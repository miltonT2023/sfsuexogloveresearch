%%-------calculate the LDA training accuracy---------%%
% Inputs: 
%     TrainFeature: the feature matrix of the training data (feature vector dimension x # of windows)
%     TrainClass: the class label for the feature matrix (feature vector dimension x # of windows)
%     wins: total number of windows in the training dataset (number of rows in TrainFeature)
%     classes: number of classes (feature vector dimension = classes * 4)
%     Wg: parameter of the LDA classifier (feature vector dimension x # of classes)
%     Cg: parameter of the LDA classifier (1 x # of classes)
% Output:
%     train_accuracy: LDA training accuracy
%%% By Xiaorong Zhang, 7/28/2014  %%%%%%%%

function train_accuracy = LDA_train_accuracy(TrainFeature,TrainClass,wins,classes, Wg, Cg)

decision=zeros(1,wins);

Atr = TrainFeature'*Wg + ones(wins,1)*Cg;
num_correct=0;
for i=1:wins
    decision(i) = find(Atr(i,:)==max(Atr(i,:)));
    if decision(i)==TrainClass(i)
        num_correct=num_correct+1;
    end
end
train_accuracy=num_correct/wins;