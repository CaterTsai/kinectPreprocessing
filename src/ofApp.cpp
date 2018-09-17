#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	auto camPos = _cam.getPosition();
	
	for (int i = 0; i < cKinectFrameNum; i++)
	{
		for (int j = 0; j < cKinectNum; j++)
		{
			_kinectList[i][j].clear();
			_kinectList[i][j].setMode(ofPrimitiveMode::OF_PRIMITIVE_POINTS);
		}
	}

	for (int i = 0; i < cKinectNum; i++)
	{
		_kPos[i].set(0);
		_kRot[i].set(0);
	}

	loadKinect(0, 232, cKinectFrameNum, false);
	loadKinect(1, 232, cKinectFrameNum, false);
	_kPos[1].set(-136, 0, -424);
	_kRot[1].set(0, 223, 0);

	_ctrlIdx = 0;
	_frameIdx = 0;
	_eCtrlMode = ePosX;
	_frameTime = cFrameTime;
	ofBackground(0);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){

	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	_frameTime -= delta;
	if (_frameTime <= 0.0f)
	{
		_frameIdx = (_frameIdx + 1) % cKinectFrameNum;
		_frameTime = cFrameTime;
	}


	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	_cam.begin();
	ofSetColor(255);
	for (int i = 0; i < cKinectNum; i++)
	{
		ofPushMatrix();
		ofTranslate(_kPos[i]);
		ofRotateX(_kRot[i].x);
		ofRotateY(_kRot[i].y);
		ofRotateZ(_kRot[i].z);
		_kinectList[_frameIdx][i].drawVertices();
		ofPopMatrix();
	}

	drawNode();
	_cam.end();
	ofDrawBitmapStringHighlight("Ctrl Index:" + ofToString(_ctrlIdx), 0, 10);
	ofDrawBitmapStringHighlight("Ctrl Mode:" + ofToString(_eCtrlMode), 0, 25);
	ofDrawBitmapStringHighlight(
		"Pos:(" + 
		ofToString(_kPos[_ctrlIdx].x) + "," + ofToString(_kPos[_ctrlIdx].y) + "," + ofToString(_kPos[_ctrlIdx].z) + ")",
		0, 40);
	ofDrawBitmapStringHighlight("Rotate:" +	ofToString(_kRot[_ctrlIdx]),0, 55);
	ofDrawBitmapStringHighlight("Frame:" + ofToString(_frameIdx), 0, 70);
}

//--------------------------------------------------------------
void ofApp::drawNode()
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(_kPos[_ctrlIdx]);
	ofRotateX(_kRot[_ctrlIdx].x);
	ofRotateY(_kRot[_ctrlIdx].y);
	ofRotateZ(_kRot[_ctrlIdx].z);

	ofSetColor(255);
	ofDrawSphere(5);

	ofSetLineWidth(3);
	ofSetColor(255, 0, 0);
	ofLine(0, 0, 0, 10, 0, 0);

	ofSetColor(0, 255, 0);
	ofLine(0, 0, 0, 0, 10, 0);

	ofSetColor(0, 0, 255);
	ofLine(0, 0, 0, 0, 0, 10);

	ofPopMatrix();
	ofPopStyle();

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case '1':
	{
		_ctrlIdx = 0;
		break;
	}
	case '2':
	{
		_ctrlIdx = 1;
		break;
	}
	case 'q':
	{
		_eCtrlMode = ePosX;
		break;
	}
	case 'w':
	{
		_eCtrlMode = ePosY;
		break;
	}
	case 'e':
	{
		_eCtrlMode = ePosZ;
		break;
	}
	case 'r':
	{
		_eCtrlMode = eRotateX;
		break;
	}
	case 't':
	{
		_eCtrlMode = eRotateY;
		break;
	}
	case 'y':
	{
		_eCtrlMode = eRotateZ;
		break;
	}

	case 'd':
	{
		
		break;
	}
	case OF_KEY_UP:
	{
		ctrl(1);
		break;
	}
	case OF_KEY_DOWN:
	{
		ctrl(-1);
		break;
	}
	}
}

//--------------------------------------------------------------
void ofApp::loadKinect(int kinectIdx, int start, int size, bool includeColor)
{
	int frameIdx = 0;
	for (int photoIdx = start; photoIdx < (start + size); photoIdx++, frameIdx++)
	{
		cout << frameIdx << endl;
		ofShortImage depth;
		ofImage color;
		if (kinectIdx == 0)
		{
			depth.load("kinect0/depth/depth_" + ofToString(photoIdx) + ".png");
			if (includeColor)
			{
				color.load("kinect0/rgb/rgb_" + ofToString(photoIdx) + ".jpg");
			}
		}
		else
		{
			depth.load("kinect1/depth/depth_" + ofToString(photoIdx) + ".png");
			if (includeColor)
			{
				color.load("kinect1/rgb/rgb_" + ofToString(photoIdx) + ".jpg");
			}
		}
		
		auto dPixel = depth.getPixels();
		
		for (int i = 0; i < (512*424); i+=2)
		{
			ofVec3f pos;
			pos.x = ofMap(dPixel[i * 3], 100, 65535, -400, 400);
			pos.y = ofMap(dPixel[i * 3 + 1], 100, 65535, -400, 400);
			pos.z = ofMap(dPixel[i * 3 + 2], 100, 65535, -50, -400);
			_kinectList[frameIdx][kinectIdx].addVertex(pos);
		}

		if (includeColor)
		{
			auto cPixel = color.getPixels();
			for (int i = 0; i < (512 * 424); i+=2)
			{
				ofColor color;
				color.r = cPixel[i * 3];
				color.g = cPixel[i * 3 + 1];
				color.b = cPixel[i * 3 + 2];
				_kinectList[frameIdx][kinectIdx].addColor(color);
			}
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::ctrl(int val)
{
	switch (_eCtrlMode)
	{
	case ePosX:
	{
		_kPos[_ctrlIdx].x += val;
		break;
	}
	case ePosY:
	{
		_kPos[_ctrlIdx].y += val;
		break;
	}
	case ePosZ:
	{
		_kPos[_ctrlIdx].z += val;
		break;
	}
	case eRotateX:
	{
		_kRot[_ctrlIdx].x += val;
		break;
	}
	case eRotateY:
	{
		_kRot[_ctrlIdx].y += val;
		break;
	}
	case eRotateZ:
	{
		_kRot[_ctrlIdx].z += val;
		break;
	}
	}
}
