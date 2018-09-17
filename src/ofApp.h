#pragma once

#include "ofMain.h"

const static int cKinectNum = 2;
const static int cKinectFrameNum = 240;
const static float cFrameTime = 1 / 24.0f;
class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void drawNode();

	void keyPressed(int key);

private:
	float _timer;

private:
	void loadKinect(int kinectIdx, int start, int end, bool includeColor = false);
	void ctrl(int val);

private:
	int _ctrlIdx;
	int _frameIdx;
	ofEasyCam _cam;
	array<array<ofVboMesh, cKinectNum>, cKinectFrameNum> _kinectList;
	array<ofVec3f, cKinectNum> _kPos, _kRot;
	float _frameTime;
	enum eCtrl : int
	{
		ePosX = 0,
		ePosY,
		ePosZ,
		eRotateX,
		eRotateY,
		eRotateZ
	}_eCtrlMode;

};
