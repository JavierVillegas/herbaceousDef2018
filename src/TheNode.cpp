//
//  TheNode.cpp
//  treeMoxi
//
//  Created by Javier Villegas on 7/31/18.
//
//

#include "TheNode.h"

TheNode::TheNode(){

}
TheNode::TheNode(int lev){
  level = lev;
  TheMean = ofVec2f(0.0,0.0);
  TheJoin = ofVec2f(0.0,0.0);
//  TheList.clear();
  //TheList = deque<ofVec2f>();
  Left = nullptr;
  Right = nullptr;
  emptyNode = true;
  NodeId = 0.0;
}
TheNode::TheNode(int lev, float meanX){
  level = lev;
  TheMean = ofVec2f(meanX,0.0);
  TheJoin = ofVec2f(0.0,0.0);
 // TheList.clear();
  //TheList = deque<ofVec2f>();
  Left = nullptr;
  Right = nullptr;
  emptyNode = true;
  NodeId = 0.0;
}

TheNode::TheNode(int lev, ofVec2f join){
  level = lev;
  TheMean = join;
  TheJoin = join;
 // TheList.clear();
  //TheList = deque<ofVec2f>();
  Left = nullptr;
  Right = nullptr;
  emptyNode = true;
  NodeId = 0.0;
}
//void TheNode::addLeave(ofVec2f leave){
//  TheList.push_back(leave);
//  
//}

//void TheNode::calculateMean(){
//  ofVec2f mean = ofVec2f(0.0,0.0);
//  for(int k = 0; k< TheList.size(); k++){
//    mean.x += TheList[k].x;
//    mean.y += TheList[k].y;
//  }
//  mean.x = (TheList.size()>0)?mean.x/(float)TheList.size():0.0;
//  mean.y = (TheList.size()>0)?mean.y/(float)TheList.size():0.0;
//  TheMean = mean;
//  
//}
void TheNode::assignLeft(TheNode* LeftNode){
  Left = LeftNode;
}
void TheNode::assignRight(TheNode* RightNode){
  Right = RightNode;
}

void TheNode::setMeanY(float meanY){
  TheMean.y = meanY;
}

void TheNode::setMean(ofVec2f mean){
  TheMean = mean;
}
void TheNode::setJoin(ofVec2f join){
  TheJoin = join;
}

//void TheNode::printLeaves(string dir){
//  
//  cout<<"Printing leave "<<dir<<" level: "<< level<<endl;
//  for(int k = 0; k< TheList.size(); k++){
//    cout<<TheList[k].x<<endl;
//  }
//}

//int TheNode::getListSize(){
//  return(TheList.size());
//}
//
//ofVec2f TheNode::getElementAt(int k){
//  
//  return(TheList[k]);
//}
