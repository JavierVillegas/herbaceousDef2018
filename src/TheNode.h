//
//  TheNode.h
//  TreeMoxi
//
//  Created by Javier Villegas on 7/30/2018.
//

#ifndef TreeMoxi_TheNode_h
#define TreeMoxi_TheNode_h
#include "ofMain.h"

class TheNode {

public:
  int level;
	TheNode* Left;
	TheNode* Right;
	ofVec2f TheMean;
  ofVec2f TheJoin;
  bool emptyNode;
  float NodeId;
	//deque<ofVec2f> TheList;
  TheNode();
  TheNode(int lev);
  TheNode(int lev,float meanX);
  TheNode(int lev,ofVec2f join);
  void addLeave(ofVec2f leave);
  void calculateMean();
  void assignLeft(TheNode* LeftNode);
  void assignRight(TheNode* RightNode);
  void setMean(ofVec2f mean);
  void setJoin(ofVec2f join);
  void setMeanY(float);
  //int getListSize();
  ofVec2f getElementAt(int k);
  
 
  
  void printLeaves(string dir);
};






#endif
