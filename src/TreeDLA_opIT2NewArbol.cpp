#include "TreeDLA.h"

#define Nw 320      
#define Nh 240
#define MAXTREELEVEL 2
//#define Xoffset 0 testin as global variable
ofFbo FboBackground;

int NoutX;
int NoutY;


unsigned char Imin =0;
unsigned char Imax = 255;
ofDirectory dir;
int FondSize;
int FondSelect = 1;
const int Nob=40;
const int Ntg=40;

 int TargInd[Nob];
ofVec2f TheObjects[Nob];
//vector<ofVec2f>TheObjects;
ofVec2f TheVelocities[Nob];
int ImW,ImH;

ofVbo Tree1Vbo;
ofVbo Tree2Vbo;
ofVbo Tree3Vbo;

ofVbo TrunksVbo;

ofShader shaderStem;
int NumLineas[3] = {0,0,0};

float XYOffset[2];//={325.0,6.0};
float XYScale[2];//={2.25,1.8};

// trunk texture

ofImage TextuTrunk;
ofShader TrunkShader;

// Shadow

ofShader ShadowShader;
ofVec2f MassXcenters[3];
int LeavesperTree[3];

ofShader BackShader;

// leaves

ofVbo Leaves1Vbo;
ofVbo Leaves2Vbo;
ofVbo Leaves3Vbo;

ofTexture LeaveTexture;

ofShader LeavesShader;
bool EquaHist = false;

const int HashRow = 12;
const int HashColumn = 16;


// target hash
vector <ofVec2f> TargetHash[HashRow][HashColumn];
// Object hash
list <ofVec2f> ObjectHash[HashRow][HashColumn];
// Index hash
list <int> ObjectIndHash[HashRow][HashColumn];

const float hashdec = 20.0;

// array with tree vertex data
vector <ofVec3f> treeVec1;
vector <ofVec3f> treeVec2;
vector <ofVec3f> treeVec3;


//--------------------------------------------------------------
void TreeDLA::setup() {
 //   NoutX = 1280;//960;  // some fixed resolution tests
 //   NoutY = 720;//480;
    NoutX = ofGetScreenWidth();
    NoutY = ofGetScreenHeight();
    cout<< NoutX<<","<<NoutY<<endl;
    
    FboBackground.allocate(NoutX, NoutY);
    
    
    
    px    = new unsigned char [Nw*Nh];
    fixedThresholdPx  = new unsigned char [Nw*Nh];
    ofBackground(255, 255, 255);
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(Nw,Nh);
    
	colorImg.allocate(Nw, Nh);
	grayImage.allocate(Nw, Nh);

    
	ofSetFrameRate(60);

    
    G_InputImage =false;
  
    

    
    
    
    ThDis =13.4;
    Ktar = 19.25;
    Kdam = 2.05;
   
    TreeThres1 = Nw/3.0;
    TreeThres2 = 2*Nw/3.0;
    G_FrameCounter =1;
    

    
    
    // texture
    
    FondSize = dir.listDir("fondos");
    Fondo.loadImage(dir.getPath(FondSelect));

    for(int i =0;i< Nob;i++){
        
        ofVec2f TheValue(ofRandom(Nw),
                         ofRandom(Nh));
        TheObjects[i] =TheValue;//.set(0.0,0.0);//.push_back(ofVec2f(0.0,0.0));
 //       TheObjects.push_back(TheValue);
        TargInd[i] =-1;
        TheVelocities[i].set(0.0,0.0);
        
    }
    

    
    // control globals
	G_ThTarget =  25;
    Kn=2.9;
    Kc=5.0;
    joinTh=1;
    G_alpha = 0.215;
//    G_alpha = 0.44;
 //   G_beta =.63;
    G_ItFact=2/25.0;
    G_DithThreshold = 130.0;
 
    // the image
    DepthMap = cvCreateImage( cvSize(Nw,Nh),IPL_DEPTH_8U,1);
    ofHideCursor();
    

   
    Colorsave.allocate(Nw, Nh, OF_IMAGE_COLOR);
    G_capture  =   false;
    G_SaveCounter = 1;
    G_NewModo =0;
    
        
    G_TheSource = 0;
    G_Quality=0.001;
    G_minDistance=8;
    Xoffset =0;
    
    G_ThresWhiter = 0.5;
    
  
    
    // aparience definer variables.
    
    
 //   AlphaSombra = 24;
    AlphaSombra = 8;
    ColorSombra = 50;
 //   ScaleHojas = 0.56; //lamarca
    // 4*0.56  para la primera
    ScaleHojas = 0.86;
//    SlopeSombra = -1.37; //lamarca
    SlopeSombra = 17.0; //lamarca
//    ScaleXSombra = 1.0;  //lamarca
//    ScaleYSombra = 0.224; //lamarca
    ScaleXSombra = 5;  //lamarca
    ScaleYSombra = 0.084; //lamarca    
    BrincoR = 0.24;
    BrincoG = 0.35; 

    
    
    shaderStem.load("shaders/StemShad150.vert", "shaders/StemShad150.frag");
    
    TextuTrunk.loadImage("images/trunk1.jpg");
    TextuTrunk.update();
    
    TrunkShader.load("shaders/TrunkShad150.vert", "shaders/TrunkShad150.frag");
    
    ShadowShader.load("shaders/ShadowShad150.vert", "shaders/ShadowShad150.frag");
    
    
    BackShader.load("shaders/BackShad150.vert", "shaders/BackShad150.frag");
    
    ofDisableArbTex();
    ofLoadImage(LeaveTexture, "images/LeavesNew2.png");
    
    LeavesShader.load("shaders/LeavesPSshad150.vert", "shaders/LeavesPSshad150.frag");
  
  //test for the tree builder
  
//  TheNode NodeTest(0, 100);
//  for (int k =0; k<20; k++) {
//     NodeTest.addLeave(ofVec2f(ofRandom(200),1.0));
//  }
//  
//  LeavesCentroid(&NodeTest);
 
  
    
}

//--------------------------------------------------------------
void TreeDLA::update() {
   
     ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

	ofBackground(100, 100, 100);
    bool bNewFrame = false;
    
    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();
    if (bNewFrame){
		// load grayscale depth image from the kinect source
		ofPixels ThePixels;
		ThePixels = vidGrabber.getPixels();
        colorImg.setFromPixels(ThePixels);
        
        grayImage = colorImg;

     
        TheTargets.clear();
        TargetHits.clear();
        cv::Mat tempCv = grayImage.getCvImage();
      // histogram equalization
        if (EquaHist == true){
            cv::equalizeHist(tempCv,tempCv);
        }
		/////////////////////////////////////////
		// note: hacer esto en un shader
        for (int i = 0; i < Nw*Nh; i++) {
            px[i] = (uchar)
            (Imin +255*((Imax -Imin)/255.0*(pow((float)(tempCv.data[i]/255.0),(float)G_alpha))));

        }
        
//        for (int i = 0; i < Nw*Nh; i++) {
//            px[i] = (uchar)( Imin +255*((Imax -Imin)/255.0*(pow((float)(grayImage.getPixels()[i]/255.0),(float)G_alpha))));
//        }
        
//        for (int i = 0; i < Nw*Nh; i++) {
//            px[i] = (uchar)(255*(G_beta + (1-G_beta )*pow((float)(grayImage.getPixels()[i]/255.0),(float)G_alpha)));
//        }

        while (!PointQueue.empty()){
            PointQueue.pop();
        }
        
        int cuantosBlack;  
        
        BWTest (px, fixedThresholdPx, Nw, Nh);
        DepthMap.data = (unsigned char *)fixedThresholdPx;
        cv::flip(DepthMap,DepthMap,1);
		

		MatchObjetTargetsPart1();

		MatchObjetTargetsPart2();


			// now filling the  unassigned objects





		//	// reorganizing  targets in the hash table
		//for (int kt = 0; kt < TheTargets.size(); kt++) {
		//	int indrow = floor(TheTargets[kt].y / hashdec);
		//	int indcol = floor(TheTargets[kt].x / hashdec);
		//	TargetHash[indrow][indcol].push_back(TheTargets[kt]);
		//}


        // filling the targets first
        
        // reseting


    
            // now brute force comparicion for the not matched ones

            //for (int kt=0; kt<TheTargets.size(); kt++) {
            //    float MinDis =10000*10000;
            //    int MinIndi =0;
            //    for (int ko=0; ko<Nob; ko++) {
            //        
            //        ofVec2f ErrorVec;
            //        ErrorVec = TheTargets[kt]-
            //        TheObjects[ko];
            //        float dis = ErrorVec.lengthSquared();
            //        if ((dis < MinDis)&&(TargInd[ko]==-1)){
            //            MinIndi = ko;
            //            MinDis = dis;
            //        }
            //    }
            //    TargInd[MinIndi] = kt;
            //    TargetHits[kt]++;
            //}
            

        

        
    // filling the tree structure
      // changed to only one dimensional lists
       vector<ofVec2f> PosList1;
        vector<ofVec2f> PosList2;
        vector<ofVec2f> PosList3;
        ofVec2f CurrentPoint(320,10);
        float TheError = 10;
        int itThres = 0;
        float cordX1=0;
        float cordX2=0;
        float cordX3=0;
        while ((TheError>2)&&(itThres<5)) {
            PosList1.clear();
            PosList2.clear();
            PosList3.clear();
            // coping the objects to the list
            // finding te center of mass
            // only using X coordinate
            
            cordX1=0;
            cordX2=0;
            cordX3=0;
            
            itThres++;
            int sideCounter1=0;
            int sideCounter2=0;
            int sideCounter3=0;
            
            for (int k=0; k < Nob;k++){
                if (TheObjects[k].x<TreeThres1) {
                    cordX1+=TheObjects[k].x;
                    PosList1.push_back(ofVec2f(TheObjects[k].x,TheObjects[k].y));
                    sideCounter1++;
                }
                else if((TheObjects[k].x>=TreeThres1)&&(TheObjects[k].x<TreeThres2))
                {
                    cordX2+=TheObjects[k].x;
                    PosList2.push_back(ofVec2f(TheObjects[k].x,TheObjects[k].y));
                    sideCounter2++;
                }
                else{
                    cordX3+=TheObjects[k].x;
                    PosList3.push_back(ofVec2f(TheObjects[k].x,TheObjects[k].y));
                    sideCounter3++;
                }
            } // end for 0..Nob
            if (sideCounter1!=0){
                cordX1/=(float)sideCounter1;
            }
            else{
                cordX1 =  Nw/6.0;
            }
            if (sideCounter2!=0){
                cordX2/=(float)sideCounter2;
            }
            else{
                cordX2 =  Nw/2.0;
            }
            if (sideCounter3!=0){
                cordX3/=(float)sideCounter3;
            }
            else{
                cordX3 =  5*Nw/6.0;
            }       
            TheError = abs(TreeThres1-(cordX2+cordX1)/2.0)
            + abs(TreeThres2-(cordX3+cordX2)/2.0);
            
            TreeThres1 = cordX1  + (cordX2-cordX1)/2.0;
            TreeThres2 = cordX2  + (cordX3-cordX2)/2.0;
        
            } // end while of the k-means
        // calculating the vec of vecs structures
        LeavesperTree[0] = PosList1.size();
        LeavesperTree[1] = PosList2.size();
        LeavesperTree[2] = PosList3.size();
        

      // recursive functions to find horizontal means
      TheNode RootTree1(0,ofVec2f(cordX1, Nh*1.45));
      TheNode RootTree2(0,ofVec2f(cordX2, Nh*1.58));
      TheNode RootTree3(0,ofVec2f(cordX3, Nh*1.54));
      
      
      
      for (int l1=0; l1<PosList1.size(); l1++) {
           RootTree1.addLeave(PosList1[l1]);
      }
      
      for (int l2=0; l2<PosList2.size(); l2++) {
        RootTree2.addLeave(PosList2[l2]);
      }
      for (int l3=0; l3<PosList3.size(); l3++) {
        RootTree3.addLeave(PosList3[l3]);
      }
      
      
      LeavesCentroid(&RootTree1);
      LeavesCentroid(&RootTree2);
      LeavesCentroid(&RootTree3);
      
//      cout<<"rot->left->left: "<<RootTree1.Left->Left->getListSize()<<endl;
      treeVec1.clear();
      treeVec2.clear();
      treeVec3.clear();
      
      generatVertexArray1(&RootTree1);
      generatVertexArray2(&RootTree2);
      generatVertexArray3(&RootTree3);
      NumLineas[0] = treeVec1.size();
       NumLineas[1] = treeVec2.size();
       NumLineas[2] = treeVec3.size();
      
//      cout<<NumLineas[0]<<endl;
//      cout<<NumLineas[1]<<endl;
//      cout<<NumLineas[2]<<endl;
      
      // coping to the vbo
      Tree1Vbo.setVertexData(&treeVec1[0], treeVec1.size(), GL_DYNAMIC_DRAW);
      Tree2Vbo.setVertexData(&treeVec2[0], treeVec2.size(), GL_DYNAMIC_DRAW);
      Tree3Vbo.setVertexData(&treeVec3[0], treeVec3.size(), GL_DYNAMIC_DRAW);
      
  //    treeVec1.clear();
  //    treeVec2.clear();
  //    treeVec3.clear();

      
 // TODO  hacer funcion que recorra el arbol y gener un VBO para pintarlo y calcular angulo de las hojas
      
      
//
//		vector<ofVec2f> nodes1;
//		nodes1.push_back(ofVec2f(cordX1, Nh*1.45));
//		NumLineas[0] = CalculateTreeStructure(PosList1, nodes1, Tree1Vbo, Leaves1Vbo);
//	
//		vector<ofVec2f> nodes2;
//		nodes2.push_back(ofVec2f(cordX2, Nh*1.58));
//
//		NumLineas[1] = CalculateTreeStructure(PosList2, nodes2, Tree2Vbo, Leaves2Vbo);
//
//		vector<ofVec2f> nodes3;
//		nodes3.push_back(ofVec2f(cordX3, Nh*1.54));
//
//		NumLineas[2] = CalculateTreeStructure(PosList3, nodes3, Tree3Vbo, Leaves3Vbo);
    //CalculateTreeStructure(PosList1,ofVec2f(cordX1,Nh*1.45), 0.85);
    //CalculateTreeStructure(PosList2,ofVec2f(cordX2,Nh*1.58), 0.9);
    //CalculateTreeStructure(PosList3,ofVec2f(cordX3,Nh*1.54), 0.85);
        MassXcenters[0] = ofVec2f(cordX1,Nh*1.45);
        MassXcenters[1] = ofVec2f(cordX2,Nh*1.58);
        MassXcenters[2] = ofVec2f(cordX3,Nh*1.54);
        //NumLineas[0] =  CopyToVbo(PosList1,Tree1Vbo, Leaves1Vbo, ofVec2f(cordX1, Nh*1.45));
        //NumLineas[1] =  CopyToVbo(PosList2,Tree2Vbo, Leaves2Vbo, ofVec2f(cordX2, Nh*1.58));
        //NumLineas[2] =  CopyToVbo(PosList3,Tree3Vbo, Leaves3Vbo, ofVec2f(cordX3, Nh*1.54));
        
    // Vertex for the trunks
        float deltaTronco1 = 2;
        float deltaTronco2 = 3.5;
        float deltaTronco3 = 3;
        
        ofVec3f TrunkVert[9] =
                {ofVec3f(cordX1,Nh,0.0),
                 ofVec3f(cordX1 -deltaTronco1,Nh*1.45,0.0),
                 ofVec3f(cordX1 +deltaTronco1,Nh*1.45,0.0),
                 ofVec3f(cordX2,Nh*.8,0.0),
                 ofVec3f(cordX2 -deltaTronco2,Nh*1.58,0.0),
                 ofVec3f(cordX2 +deltaTronco2,Nh*1.58,0.0),
                 ofVec3f(cordX3,Nh,0.0),
                 ofVec3f(cordX3 -deltaTronco3,Nh*1.54,0.0),
                 ofVec3f(cordX3 +deltaTronco3,Nh*1.54,0.0)
                };
        
        //Texture coordinates
        
        ofVec2f TrunkTexCoords[9]={
            ofVec2f(TextuTrunk.getWidth()/16.0,TextuTrunk.getHeight()/2.0),
            ofVec2f(0.0,TextuTrunk.getHeight()),
            ofVec2f(TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight()),
            ofVec2f(6*TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight()/2.0),
            ofVec2f(5*TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight()),
            ofVec2f(7*TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight()),
            ofVec2f(4*TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight()/2.0),
            ofVec2f(3*TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight()),
            ofVec2f(5*TextuTrunk.getWidth()/8.0,TextuTrunk.getHeight())
        };
        
        TrunksVbo.setVertexData( &TrunkVert[0], 9, GL_DYNAMIC_DRAW );
        TrunksVbo.setTexCoordData(&TrunkTexCoords[0],9,GL_STATIC_DRAW);

        
	}

}


// funcion recursiv



//--------------------------------------------------------------
void TreeDLA::draw() {
    
      
    ofxCvGrayscaleImage ToShow;
    
    FboBackground.begin();
        BackShader.begin();
            // XYOffset and XYScale relative to background image
            // dimentions   (1280 x 720)
            XYOffset[0] = 325.0;
            XYOffset[1] = 6.0;
            XYScale[0] = 2.25;
            XYScale[1] = 1.8;
            BackShader.setUniform2fv("XYoffset", XYOffset);
            BackShader.setUniform2fv("XYScale", XYScale);
            float Cmass[2]={MassXcenters[0].x,MassXcenters[0].y};
            BackShader.setUniform2fv("Cmass1", Cmass);
            Cmass[0] = MassXcenters[1].x;
            Cmass[1] = MassXcenters[1].y;
            BackShader.setUniform2fv("Cmass2", Cmass);
            Cmass[0] = MassXcenters[2].x;
            Cmass[1] = MassXcenters[2].y;
            BackShader.setUniform2fv("Cmass3", Cmass);
            ofSetColor(255, 255, 255,255);
			//Fondo.draw(-1, -1, 2, 2);
            Fondo.draw(0, 0, NoutX,NoutY);
        BackShader.end();
    FboBackground.end();
    // shadow
    
    // Other scale factor relative to screen dimensions
    XYOffset[0] = 325.0*NoutX/1280.0;
    XYOffset[1] = 6.0*NoutY/720.0;
    XYScale[0] = 2.25*NoutX/1280.0;
    XYScale[1] = 1.8*NoutY/720.0;
    
    
    
    ofSetColor(255, 255, 255,255);
    FboBackground.draw(0, 0, NoutX,NoutY);
  // 8/3/2018 commented shadow
//    ShadowShader.begin();
//        ShadowShader.setUniform2fv("XYoffset", XYOffset);
//        ShadowShader.setUniform2fv("XYScale", XYScale);
//        Cmass[0] = MassXcenters[0].x;
//        Cmass[1] = MassXcenters[0].y;
////        Cmass[2]={MassXcenters[0].x,MassXcenters[0].y};
//        ShadowShader.setUniform2fv("Cmass", Cmass);
//        Tree1Vbo.drawElements( GL_LINES,NumLineas[0]);
//        TrunksVbo.draw(GL_TRIANGLES, 0, 3);
//        Cmass[0] = MassXcenters[1].x;
//        Cmass[1] = MassXcenters[1].y;
//        ShadowShader.setUniform2fv("Cmass", Cmass);
//        Tree2Vbo.drawElements( GL_LINES,NumLineas[1]);
//        TrunksVbo.draw(GL_TRIANGLES, 3, 3);
//        Cmass[0] = MassXcenters[2].x;
//        Cmass[1] = MassXcenters[2].y;
//        ShadowShader.setUniform2fv("Cmass", Cmass);
//        Tree3Vbo.drawElements( GL_LINES,NumLineas[2]);
//        TrunksVbo.draw(GL_TRIANGLES, 6, 3);
//    ShadowShader.end();

    shaderStem.begin();

        shaderStem.setUniform2fv("XYoffset", XYOffset);
        shaderStem.setUniform2fv("XYScale", XYScale);
  
        Tree1Vbo.draw(GL_LINES,0,NumLineas[0]);
        Tree2Vbo.draw(GL_LINES,0,NumLineas[1]);
        Tree3Vbo.draw(GL_LINES,0,NumLineas[2]);
  
//        Tree1Vbo.drawElements( GL_LINES,NumLineas[0]);
//        Tree2Vbo.drawElements( GL_LINES,NumLineas[1]);
//        Tree3Vbo.drawElements( GL_LINES,NumLineas[2]);
  
    
    shaderStem.end();

    
    ofTexture tex2 = TextuTrunk.getTextureReference();
    
    TrunkShader.begin();
        TrunkShader.setUniform2fv("XYoffset", XYOffset);
        TrunkShader.setUniform2fv("XYScale", XYScale);
        TrunkShader.setUniformTexture("TrunkTextu",  tex2, 1 );
        TrunksVbo.draw(GL_TRIANGLES, 0, 9);
    TrunkShader.end();
   
    
    ofEnablePointSprites();
    //ofEnableAlphaBlending();
    ofSetColor(255,255,255,255);
    
 //   glDepthMask(GL_FALSE);
  
  
  // commented leaves shader
//    LeavesShader.begin();
//    LeavesShader.setUniform2fv("XYoffset", XYOffset);
//    LeavesShader.setUniform2fv("XYScale", XYScale);
//    float TheDims[2];
//    TheDims[0] = NoutX;
//    TheDims[1] = NoutY;
//    LeavesShader.setUniform2fv("XYDim", TheDims);
//    
//	LeaveTexture.bind();
//    Leaves1Vbo.draw(GL_POINTS, 0, LeavesperTree[0]);
//	Leaves2Vbo.draw(GL_POINTS, 0, LeavesperTree[1]);
//    Leaves3Vbo.draw(GL_POINTS, 0, LeavesperTree[2]);
//	LeaveTexture.unbind();
//
//	LeavesShader.end();
  
  
  
    ofDisablePointSprites();
 //   ofDisableAlphaBlending();
 //   glDepthMask(GL_TRUE);
    
    if(G_InputImage){
        ofSetColor(255, 255, 255,255);
        ToShow.allocate(Nw, Nh);
        ToShow.setFromPixels(DepthMap.data,Nw,Nh);
        ToShow.draw(0, 0);
        
        ofSetColor(0, 0, 0);
        for (int sisi =0; sisi<Nob; sisi++) {
            ofCircle(Nw + TheObjects[sisi].x, TheObjects[sisi].y, 4);
        }
        
    }  
}
////////////////////////////////////////////////////////////////////
// TODO cambiarlo para que pinte aunque no se haya corrido el 
// calculate tree

int TreeDLA:: CopyToVbo (const vector< vector<ofVec2f> >& PosList, ofVbo& TheVbo, ofVbo& TheLeavesVbo,
	ofVec2f TargetPoint){

    // coping the structure to the ofVbo
    
    
	vector <ofVec3f> treeVec;
  
    int elecont =0;
    
	vector <unsigned int> treeInd;


    int indicount=0;
	vector <ofVec3f> leavesVec;

    
	// first point of the array is the target
	//treeVec.push_back(ofVec3f(TargetPoint.x
	//	, TargetPoint.y, 0.0));
	
	
    for (int sv =0; sv < PosList.size(); sv++) {
        for (int iv = 0; iv < PosList[sv].size(); iv++) {

			float tang = tan((PosList[sv][iv].y- TargetPoint.y) /(float)Nh*PI / 3.0);
            treeVec.push_back(ofVec3f(PosList[sv][iv].x
                                        , PosList[sv][iv].y,0.0));
			treeInd.push_back(elecont);
			elecont++;

			float yval = MIN(abs(PosList[sv][iv].x - TargetPoint.x) / tang, abs(PosList[sv][iv].y - TargetPoint.y));
                float ForangleX;
                float ForangleY;
       
                 ForangleX= PosList[sv][iv].x -
					 TargetPoint.x;
                 ForangleY= yval;
      
                leavesVec.push_back(ofVec3f(PosList[sv][0].x
                                        , PosList[sv][0].y,atan2(-ForangleX,ForangleY)));
        
				treeVec.push_back(ofVec3f(TargetPoint.x
					, PosList[sv][iv].y+yval, 0.0));
				treeInd.push_back(elecont);
        
            elecont++;
        }
    }
    
    
    
    TheVbo.setVertexData( &treeVec[0], treeVec.size(), GL_DYNAMIC_DRAW );
    
    TheVbo.setIndexData( &treeInd[0], treeInd.size(), GL_DYNAMIC_DRAW );
    
    TheLeavesVbo.setVertexData( &leavesVec[0], leavesVec.size(), GL_DYNAMIC_DRAW );
    


    return treeInd.size();

}




 int TreeDLA::CalculateTreeStructure(vector< vector<ofVec2f> >& PosList,vector<ofVec2f> TargetPoints, ofVbo& TheVbo, ofVbo& TheLeavesVbo){
    
	 // vectors for the VBos
	 vector <ofVec3f> treeVec;
	 int elecont = 0;
	 vector <unsigned int> treeInd;
	 int indicount = 0;
	 vector <ofVec3f> leavesVec;

	 // adding all elements + nodes

	 for (int te = 0; te < PosList.size();te++) {
		 treeVec.push_back(ofVec3f(PosList[te][0].x, PosList[te][0].y, 0));
	 }
	 for (int tn = 0; tn < TargetPoints.size(); tn++) {
		 treeVec.push_back(ofVec3f(TargetPoints[tn].x, TargetPoints[tn].y, 0));
	 }



	 // finding closest node
	 
	 for (int c = 0; c < PosList.size(); c++) {
	 
		 float minDis = 10000000;
		 int minNode = 0;
		 for (int n = 0; n < TargetPoints.size(); n++) {
			 float dist = PosList[c][0].distanceSquared(TargetPoints[n]);
			 if (dist < minDis) {
				 minDis = dist;
				 minNode = n;
			 }
			 treeInd.push_back(c);
			 treeInd.push_back(PosList.size() + minNode -1);
			 float ForangleX;
			 float ForangleY;
			 ForangleX = PosList[c][0].x -
				 TargetPoints[minNode].x;
			 ForangleY = PosList[c][0].y -
				 TargetPoints[minNode].y;

			 leavesVec.push_back(ofVec3f(PosList[c][0].x
				 , PosList[c][0].y, atan2(-ForangleX, ForangleY)));

		 }
	 
	 }


	 TheVbo.setVertexData(&treeVec[0], treeVec.size(), GL_DYNAMIC_DRAW);

	 TheVbo.setIndexData(&treeInd[0], treeInd.size(), GL_DYNAMIC_DRAW);

	 TheLeavesVbo.setVertexData(&leavesVec[0], leavesVec.size(), GL_DYNAMIC_DRAW);
	 return treeInd.size();

  //  Xoffset =0;//NoutX*(Nw/width)*(1.0-width/Nw)/2.0;
  //  int it  = 0;
  //  int fullsize = PosList.size();
  //  int EndCount = fullsize;
  //  vector<int> DeadStems(fullsize,0);
  //  while ((EndCount>0)&&(it<60)) {//it<100
  //      it++;
  //      float dt = .2 + it/10000.0;
		//
  //      
  //      // cout<<PosList.size()<<"\n";
  //      for (int c = 0; c<PosList.size(); c++) {
  //          float minDis =10000000;
  //          int minIndi = -1;
  //          if ((PosList[c].size()>it)||
  //              (DeadStems[c]==1)){
  //              continue;
  //          }
  //          
  //          for (int cn =c+1; cn<PosList.size();cn++){
  //              // looking  for the closer point
  //              if ((PosList[cn].size()==it)&&
  //                  (DeadStems[cn]==0)){
  //                  float dist =PosList[c][it-1].distanceSquared(PosList[cn][it-1]);
  //                  if (dist<minDis) {
  //                      minIndi = cn;
  //                      minDis=dist;
  //                  }
  //              }
  //          }
  //          if (minIndi!=-1) {
  //              ofVec2f TheF1;
  //              ofVec2f TheF2;
  //              ofVec2f LocalTarget;
  //              LocalTarget.x = TargetPoint.x;
  //              LocalTarget.y = TargetPoint.y - CenFac*(TargetPoint.y -MAX(PosList[c][it-1].y, PosList[minIndi][it-1].y));
  //              ofVec2f VecN = PosList[minIndi][it-1]-PosList[c][it-1];
  //              ofVec2f VecC1,VecC2;
  //              ofVec2f GradValueC;
  //              ofVec2f GradValueMinIndi;
  //              
  //              
  //              VecC1 = LocalTarget - PosList[c][it-1];
  //              //      VecC1.normalize();
  //              VecC2 = LocalTarget - PosList[minIndi][it-1];
  //              //    VecC2.normalize();
  //              
  //              
  //              VecN.normalize();
  //              VecC1.normalize();
  //              VecC2.normalize();
  //              float lKc;
  //              lKc = ((float)it*(G_ItFact))*Kc;
  //              
  //              TheF1 =VecC1*lKc + VecN*Kn;
  //              
  //              
  //              TheF2 = VecC2*lKc + VecN*(-Kn);
  //              
  //              
  //              // update position
  //              
  //              ofVec2f newPos1 = PosList[c][it-1]+ TheF1*dt;
  //              if(((newPos1.x - LocalTarget.x)*
  //                  (PosList[c][it-1].x - LocalTarget.x))<0){
  //                  newPos1.x = LocalTarget.x;
  //              }
  //              if(newPos1.y>LocalTarget.y){
  //                  newPos1.y=LocalTarget.y;
  //              }
  //              
  //              ofVec2f newPos2 = PosList[minIndi][it-1]+ TheF2*dt;
  //              if(((newPos2.x - LocalTarget.x)*
  //                  (PosList[minIndi][it-1].x - LocalTarget.x))<0){
  //                  newPos2.x = LocalTarget.x;
  //              }
  //              if(newPos2.y>LocalTarget.y){
  //                  newPos2.y=LocalTarget.y;
  //              }
  //              
  //              
  //              // creating the new  list
  //              
  //              PosList[c].push_back(newPos1);
  //              if (newPos1.distanceSquared(newPos2)>joinTh){
  //                  PosList[minIndi].push_back(newPos2);
  //              }
  //              else{
  //                  PosList[minIndi].push_back(newPos1);
  //                  DeadStems[minIndi]=1;
  //                  EndCount--;
  //             if(newPos1.distanceSquared(LocalTarget)<G_ThTarget)
  //                  {
  //                      PosList[c].push_back(LocalTarget);
  //                      PosList[c].push_back(TargetPoint);
  //                      DeadStems[c]=1;
  //                      EndCount--;
  //                  }
  //              }
  //              
  //              
  //          }//end if pair found
  //          else{ //if not pair paint single (NumN closest were occupy)
  //              
  //              ofVec2f TheF1;
  //              ofVec2f VecC;
  //              ofVec2f LocalTarget;
  //              LocalTarget.x = TargetPoint.x;
  //              LocalTarget.y = TargetPoint.y - CenFac*(TargetPoint.y -PosList[c][it-1].y);
  //              float lKc;
  //              lKc = ((float)it*(G_ItFact))*Kc;
  //              VecC = LocalTarget - PosList[c][it-1];
  //              VecC.normalize();
  //              TheF1 = VecC*lKc;
  //              ofVec2f newPos1 = PosList[c][it-1]+ TheF1*(dt);
  //              if(((newPos1.x - LocalTarget.x)*
  //                  (PosList[c][it-1].x - LocalTarget.x))<0){
  //                  newPos1.x = LocalTarget.x;
  //              }
  //              if(newPos1.y>LocalTarget.y){
  //                  newPos1.y=LocalTarget.y;
  //              }
  //              
  //               PosList[c].push_back(newPos1);
  //              if(newPos1.distanceSquared(LocalTarget)<G_ThTarget)
  //              {
  //                  PosList[c].push_back(LocalTarget);
  //                  PosList[c].push_back(TargetPoint);
  //                  DeadStems[c]=1;
  //                  EndCount--;
  //              }
  //          }// end no pair found
  //      }// end loop all the elements

  //  } // end while iterations
  //  
  ////  return PosList;

}




//--------------------------------------------------------------
void TreeDLA::exit() {
    ////	kinect.setCameraTiltAngle(0); // zero the tilt on exit
    ////	kinect.close();
    //	kinectPlayer.close();
    //	kinectRecorder.close();
}



void TreeDLA::LeavesCentroid(TheNode* Node){

  if ((Node->level < MAXTREELEVEL)&&
      (Node->getListSize()>0)){
     // create left
    auto LeftNode = make_shared<TheNode>(Node->level+1);
    auto RightNode = make_shared<TheNode>(Node->level+1);
 //    TheNode LeftNode(Node->level+1);
    // create right
 //   TheNode RightNode(Node->level+1);

    
    
    // going through the list
    while(!Node->TheList.empty()){
      ofVec2f aLeave = Node->TheList.back();
      Node->TheList.pop_back();
      
      // move to the left
      if (aLeave.x < Node->TheMean.x){
        LeftNode->addLeave(aLeave);
      }
      else{
        RightNode->addLeave(aLeave);
      }
   }
    cout<<"node size: "<<Node->TheList.size()<<endl;
    // calculate means
    LeftNode->calculateMean();
    RightNode->calculateMean();
    

    // Call itself on left and right
//    LeftNode.printLeaves("Left");
//    RightNode.printLeaves("Right");
    LeavesCentroid(LeftNode.get());
    LeavesCentroid(RightNode.get());
    Node->assignLeft(LeftNode.get());
    Node->assignRight(RightNode.get());
  }
  else{
    return;
  }
  
}

void TreeDLA::generatVertexArray1(TheNode* Node)
{
  cout<<"inside genVArray : "<<Node->getListSize()<<endl;
  // check if the list is empty
  if (Node->getListSize()>0){
  // add lines from node mean to every list elemment
    
    for(int k=0; k < Node->getListSize(); k++){
      treeVec1.push_back(ofVec3f(Node->TheMean.x,
                                Node->TheMean.y, 0.0));
      treeVec1.push_back(ofVec3f((Node->getElementAt(k)).x,
                                (Node->getElementAt(k)).y,0.0));
    }

    return;
  }
  // TODO check if it is better to use nullpointers
  else{ // if list empty check nodes
    
    if (Node->Left!=nullptr){
      treeVec1.push_back(ofVec3f(Node->TheMean.x,
                                Node->TheMean.y, 0.0));
      treeVec1.push_back(ofVec3f(Node->Left->TheMean.x,
                                Node->Left->TheMean.y, 0.0));
      // call itself
      generatVertexArray1(Node->Left);
    }
    if (Node->Right!=nullptr){
      treeVec1.push_back(ofVec3f(Node->TheMean.x,
                                Node->TheMean.y, 0.0));
      treeVec1.push_back(ofVec3f(Node->Right->TheMean.x,
                                Node->Right->TheMean.y, 0.0));
      // call itself
      generatVertexArray1(Node->Right);
    }
  
    return;
  }


}




void TreeDLA::generatVertexArray2(TheNode* Node)
{
  // check if the list is empty
  if (Node->getListSize()>0){
    // add lines from node mean to every list elemment
    
    for(int k=0; k < Node->getListSize(); k++){
      treeVec2.push_back(ofVec3f(Node->TheMean.x,
                                Node->TheMean.y, 0.0));
      treeVec2.push_back(ofVec3f((Node->getElementAt(k)).x,
                                (Node->getElementAt(k)).y,0.0));
    }
    
    return;
  }
  // TODO check if it is better to use nullpointers
  else{ // if list empty check nodes
    
    if (Node->Left!=nullptr){
      treeVec2.push_back(ofVec3f(Node->TheMean.x,
                                Node->TheMean.y, 0.0));
      treeVec2.push_back(ofVec3f(Node->Left->TheMean.x,
                                Node->Left->TheMean.y, 0.0));
      // call itself
      generatVertexArray2(Node->Left);
    }
    if (Node->Right!=nullptr){
      treeVec2.push_back(ofVec3f(Node->TheMean.x,
                                Node->TheMean.y, 0.0));
      treeVec2.push_back(ofVec3f(Node->Right->TheMean.x,
                                Node->Right->TheMean.y, 0.0));
      // call itself
      generatVertexArray2(Node->Right);
    }
    
    return;
  }
  
  
}


void TreeDLA::generatVertexArray3(TheNode* Node)
{
  // check if the list is empty
   if (Node->getListSize()>0){
    // add lines from node mean to every list elemment
    
    for(int k=0; k <Node->getListSize(); k++){
      treeVec3.push_back(ofVec3f(Node->TheMean.x,
                                 Node->TheMean.y, 0.0));
      treeVec3.push_back(ofVec3f((Node->getElementAt(k)).x,
                                 (Node->getElementAt(k)).y,0.0));
    }
    
    return;
  }
  // TODO check if it is better to use nullpointers
  else{ // if list empty check nodes
    
    if (Node->Left!=nullptr){
      treeVec3.push_back(ofVec3f(Node->TheMean.x,
                                 Node->TheMean.y, 0.0));
      treeVec3.push_back(ofVec3f(Node->Left->TheMean.x,
                                 Node->Left->TheMean.y, 0.0));
      // call itself
      generatVertexArray3(Node->Left);
    }
    if (Node->Right!=nullptr){
      treeVec3.push_back(ofVec3f(Node->TheMean.x,
                                 Node->TheMean.y, 0.0));
      treeVec3.push_back(ofVec3f(Node->Right->TheMean.x,
                                 Node->Right->TheMean.y, 0.0));
      // call itself
      generatVertexArray3(Node->Right);
    }
    
    return;
  }
  
  
}

//--------------------------------------------------------------
void TreeDLA::keyPressed (int key) {
	switch (key) {

        case 'a':
            G_alpha-= .005;
            if (G_alpha<0.0){G_alpha=0.0;}
            cout<<"alpha:"<<G_alpha<<"\n";
            break;        
        
        case 'z':
            G_alpha+= .005;
           // if (G_alpha>1.0){G_alpha=1.0;}
            cout<<"alpha:"<<G_alpha<<"\n";           
            break;
               
        case 's':
            Imin++;
            if (Imin>=Imax){Imin =Imax-1;}
            cout<<"Imin:"<<Imin<<"\n";
            
            break;
        case 'x':
            Imin--;
            if (Imin<=0){Imin=0;}
            cout<<"Imin:"<<Imin<<"\n";
            break;    
        case OF_KEY_UP:
            XYOffset[1]++;
            
            cout<<"Yoffset:"<<XYOffset[1]<<"\n";
            
            break;
        case OF_KEY_DOWN:
            XYOffset[1]--;
            cout<<"Yoffset:"<<XYOffset[1]<<"\n";
            break;
            
        case OF_KEY_RIGHT:
            XYOffset[0]++;
            
            cout<<"Xoffset:"<<XYOffset[0]<<"\n";
            
            break;
        case OF_KEY_LEFT:
            XYOffset[0]--;
            cout<<"Xoffset:"<<XYOffset[0]<<"\n";
            break;    
        case 'v':
            G_DithThreshold++;
            cout <<"G_DithThreshold:"<<G_DithThreshold<<"\n";
            if (G_DithThreshold>255){G_DithThreshold=255;}
            break;
        case 'f':
            G_DithThreshold--;
            cout <<"G_DithThreshold:"<<G_DithThreshold<<"\n";
            if (G_DithThreshold<0){G_DithThreshold=0;}
            break;
        
        case '1':
            G_NewModo++;
            if (G_NewModo>3){G_NewModo=0;}
            break;  
        case '2':
            G_NewModo--;
            if (G_NewModo<0){G_NewModo=3;}
            break;   
        case '6':
            G_InputImage=!G_InputImage;
            
			break;
        case '5':
           
            break;
        case ' ':
            G_capture=true;
            break;
            
        case 'o':
            XYScale[0]-= 0.05;
            cout<<"Xscale:"<<XYScale[0]<<"\n";
            
            break;
        case 'p':
            XYScale[0]+= 0.05;
            cout<<"Xscale:"<<XYScale[0]<<"\n";
            break;
        case 'k':
            XYScale[1]-= 0.05;
            cout<<"Yscale:"<<XYScale[1]<<"\n";
            break;
        case 'l':
            XYScale[1]+= 0.05;
            cout<<"Yscale:"<<XYScale[1]<<"\n";
            break;
            
            
        case 'c':
            Imax--;
            if (Imax<=Imin){Imax =Imin+1;}
            cout<<"Imax:"<<Imax<<"\n";
            break; 
            
        case 'd':
            Imax++;
            if (Imax>=255){Imax =255;}
            cout<<"Imax:"<<Imax<<"\n";
            break; 
        case 'n':
            FondSelect--;
            if (FondSelect <0){
                FondSelect = FondSize-1;
            }
            Fondo.loadImage(dir.getPath(FondSelect));
            cout<<dir.getPath(FondSelect)<<endl;
            break;  
        case 'm':
            FondSelect++;
            if (FondSelect >FondSize-1){
                FondSelect = 0;
            }
            Fondo.loadImage(dir.getPath(FondSelect)); 
            cout<<dir.getPath(FondSelect)<<endl;
            break;

        case 'g':
            EquaHist = !EquaHist;
          
            break;  
        case 'y':

            break;  
            
  

	}
}

//--------------------------------------------------------------
void TreeDLA::mouseMoved(int x, int y) {
    //	pointCloudRotationY = x;
}

//--------------------------------------------------------------
void TreeDLA::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void TreeDLA::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void TreeDLA::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void TreeDLA::windowResized(int w, int h)
{}





void TreeDLA::BWTest(const unsigned char * src, unsigned char * dst, int w, int h) {
    
    int pxPos;
    int oldPx = 0;
    int newPx = 0;
    int qError = 0;
    
    int threshold = G_DithThreshold;
    memcpy(dst, src, w*h);
    int r1, r2,  bl1, bc1, br1, bc2;
    int new1,new2;
    for (int j = 0; j < h-2; j++) {
        for (int i = 1; i < w-2; i++) {
            pxPos = j*w+i;
            oldPx = dst[j*w+i];
            newPx = oldPx < threshold ? 0 : 255;
            dst[j*w+i] = newPx;
            
            qError = oldPx - newPx;
            
            //if (qError <0)
            {qError*=2*G_ThresWhiter;}
            
            // super duper ugly 2 part value clipping (to avoid values beyond 0-255)
            r1  = MIN(dst[j*w+(i+1)]     + (int)(.125f * qError), 255);
            r2  = MIN(dst[j*w+(i+2)]     + (int)(.125f * qError), 255);
            bl1 = MIN(dst[(j+1)*w+(i-1)] + (int)(.125f * qError), 255);
            bc1 = MIN(dst[(j+1)*w+(i)]   + (int)(.125f * qError), 255);
            br1 = MIN(dst[(j+1)*w+(i+1)] + (int)(.125f * qError), 255);
            bc2 = MIN(dst[(j+2)*w+(i)]   + (int)(.125f * qError), 255);
//            new1 = MIN(dst[(j+1)*w+(i+2)] + (int)(.125f * qError), 255);
//            new2 = MIN(dst[(j+2)*w+(i+1)]   + (int)(.125f * qError), 255);
            
            
            dst[j*w+(i+1)]     = MAX(r1 , 0);
            dst[(j+1)*w+(i-1)] = MAX(bl1, 0);
            dst[(j+1)*w+(i)]   = MAX(bc1, 0);
            dst[(j+1)*w+(i+1)] = MAX(br1, 0);
            dst[(j+2)*w+(i)]   = MAX(bc2, 0);
//            dst[(j+1)*w+(i+2)] = MAX(new1, 0);
//            dst[(j+2)*w+(i+1)]   = MAX(new2, 0);            
            if (qError>0){
                PointQueue.push(ThePoints(Nw-i,j,qError));
            }
            
        }
    }
}



void TreeDLA::MatchObjetTargetsPart1() {

	ThePoints tempoPoint;
	int TheLimit = MIN(Ntg, PointQueue.size());

	for (int k = 0; k < TheLimit; k++) {
		tempoPoint = PointQueue.top();
		TheTargets.push_back(ofVec2f(tempoPoint.Pos.x, tempoPoint.Pos.y));
		PointQueue.pop();
		//     TargetHits.push_back(tempoPoint.Error);
		TargetHits.push_back(0);
	}

	for (int i = 0; i< Nob; i++) {
		TargInd[i] = -1;
	}

	if (!TheTargets.empty()) {
		// empty the hash tables

		for (int roH = 0; roH < HashRow; roH++) {
			for (int coH = 0; coH < HashColumn; coH++) {

				ObjectHash[roH][coH].clear();

				ObjectIndHash[roH][coH].clear();
			
			}
		}






		// move objects to the hash cells
		for (int ko = 0; ko < Nob; ko++) {
			int indrow = floor(TheObjects[ko].y / hashdec);
			indrow = MAX(0, indrow);
			indrow = MIN(HashRow - 1, indrow);
			int indcol = floor(TheObjects[ko].x / hashdec);
			indcol = MAX(0, indcol);
			indcol = MIN(HashColumn - 1, indcol);

			ObjectHash[indrow][indcol].push_back(TheObjects[ko]);
			ObjectIndHash[indrow][indcol].push_back(ko);
		}
		// running through the targets and finding a close free object

		for (int kt = 0; kt < TheTargets.size(); kt++) {
			int indrow = floor(TheTargets[kt].y / hashdec);
			int indcol = floor(TheTargets[kt].x / hashdec);
			indrow = MAX(0, indrow);
			indrow = MIN(HashRow - 1, indrow);
			indcol = MAX(0, indcol);
			indcol = MIN(HashColumn - 1, indcol);
			int ww = 0; // window width
			int wh = 0; // window height
			bool MatchFound = false;
			int winsize = 0;
			while (!MatchFound) {
				ww = winsize;
				wh = winsize;
				float MinDis = 10000 * 10000;
				int MinIndi = 0;
				for (int n = -ww; n <= ww; n++) {
					for (int m = -wh; m <= wh; m++) {
						// if the cell is not empty
						if ((indcol + n >= 0) &&
							(indrow + m >= 0) &&
							(indcol + n < 16) &&
							(indrow + m < 12) &&
							(!ObjectHash[indrow + m][indcol + n].empty())
							) {
							std::list<ofVec2f>::iterator
								it1 = ObjectHash[indrow + m][indcol + n].begin();
							std::list<int>::iterator
								it2 = ObjectIndHash[indrow + m][indcol + n].begin();
							int elemcount = 0;
							int Theelement = 0;
							while (it1 != ObjectHash[indrow + m][indcol + n].end())
							{
								ofVec2f theobj = (*it1);
								float currDist = TheTargets[kt].squareDistance(theobj);
								if (currDist < MinDis) {
									MinDis = currDist;
									MinIndi = (*it2);
									Theelement = elemcount;
								}
								it1++;
								it2++;
								elemcount++;
							}
							TargInd[MinIndi] = kt;
							MatchFound = true;
							// delete element for the list
							it1 = ObjectHash[indrow + m][indcol + n].begin();
							it2 = ObjectIndHash[indrow + m][indcol + n].begin();
							for (int ec = 0; ec < Theelement; ec++) {
								it1++;
								it2++;
							}
							ObjectHash[indrow + m][indcol + n].erase(it1);
							ObjectIndHash[indrow + m][indcol + n].erase(it2);
							n = ww + 1;
							m = wh + 1;
							break;
						}
						else { // increment area counter
							   // if last window cell
							if ((n == ww) && (m == wh)) {
								winsize++;
							}
						}
					}// for m
				}// for n
			} // while
		} // loop targets
	}  // end if targets non empty
} // End of function




void TreeDLA::MatchObjetTargetsPart2() {
	float dt = 0.25;
	for (int ko = 0; ko<Nob; ko++) {

		float MinHits = 10000;
		ofVec2f UpdateVec;
		float MinDis = 10000 * 10000;
		int MinIndi = 0;

		if (TargInd[ko] == -1) {
			MinDis = 10000 * 10000;
			for (int kt = 0; kt<TheTargets.size(); kt++) {
				ofVec2f ErrorVec;
				ErrorVec = TheTargets[kt] - TheObjects[ko];
				float dis = TargetHits[kt] * ErrorVec.lengthSquared();
				if (dis < MinDis) {
					MinDis = dis;
					MinIndi = kt;
				}

			}

			TargetHits[MinIndi]++;
			TargInd[ko] = MinIndi;
		}

		UpdateVec = TheTargets[TargInd[ko]] - TheObjects[ko];
		float newDis = UpdateVec.length();
		UpdateVec.normalize();


		ofVec2f acc;

		if (newDis < ThDis) {
			acc = (newDis / 10.0)*(Ktar*UpdateVec) - Kdam*TheVelocities[ko];
		}
		else {
			acc = (Ktar*UpdateVec) - Kdam*TheVelocities[ko];
		}
		TheVelocities[ko] = TheVelocities[ko] - (-dt)*acc;
		TheObjects[ko] = TheObjects[ko] - (-dt)*TheVelocities[ko];
	}

}// closing function