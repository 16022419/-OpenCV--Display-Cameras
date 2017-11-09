#include<opencv2\highgui\highgui.hpp>
#include<opencv2\video\video.hpp>
#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>

using namespace std;
using namespace cv;

const CvScalar WHITE = cvScalar(255, 255, 255);
const CvPoint POINT = cvPoint(10, 40);
const double thickNess = 1.7;
const int lineType = 2;
const char DATA_FILE[] = "Cameras_Data_Base.txt";

class DISPLAY_SCREEN {
private:
	int DISPLAY_SCREEN_HEIGHT;
	int DISPLAY_SCREEN_WIDTH;
	VideoCapture* capture ;
	vector<string> camList;

public:
	DISPLAY_SCREEN(int DISPLAY_SCREEN_HEIGHT, int DISPLAY_SCREEN_WIDTH) {
		setScreen(DISPLAY_SCREEN_HEIGHT, DISPLAY_SCREEN_WIDTH);
	}
	void setScreen(int DISPLAY_SCREEN_HEIGHT, int DISPLAY_SCREEN_WIDTH) {
		this->DISPLAY_SCREEN_HEIGHT = DISPLAY_SCREEN_HEIGHT;
		this->DISPLAY_SCREEN_WIDTH = DISPLAY_SCREEN_WIDTH;
		
	}
	int getWidth() { return DISPLAY_SCREEN_WIDTH; }
	int getHeight() { return DISPLAY_SCREEN_HEIGHT; }
	
public:
	/// SET SIZE OF DISPLAY SCREEN
	DISPLAY_SCREEN() {
		DISPLAY_SCREEN_HEIGHT = 1284;
		DISPLAY_SCREEN_WIDTH = 960;
	}	 

	/// READ CAMERAS FROM TEXT FILE
	void readSource() {
		ifstream file(DATA_FILE);
		if (!file.is_open())
			cout << "Cannot open source file" << endl;
		else if (file.is_open()) {
			string cam;
			while (file >> cam)
				camList.push_back(cam);
			file.close();
		}
		capture = new VideoCapture[camList.size()];
		if (camList.size() > 0) {
			for (int i = 0; i < camList.size(); i++) {
				capture[i].open(camList[i]);
			}
		}
	}

	/// GET LOCAL TIME 
	void printTime(Mat frame[]) {
		time_t current = time(0);
		struct tm* now = localtime(&current);
		stringstream str;
		str << now->tm_hour;
		str << "h:";
		str << now->tm_min;
		str << "m:";
		str << now->tm_sec;
		str << "s";
		CvPoint textPoint = POINT;
		CvScalar color = WHITE;
		for (int i = 0; i < sizeof(frame); i++)
			putText(frame[i], str.str(), textPoint, FONT_HERSHEY_COMPLEX_SMALL, thickNess, color, lineType, CV_AA);
	}

public: 
	/// DISPLAY ALL CAMERAS
	void GENERATE() {
		readSource();
		int height = getHeight();
		int width = getWidth();
		Mat display = Mat(Size(height, width), CV_8UC3);
		Mat defaultFrame = Mat(Size(height, width), CV_8UC3);
		CvPoint textPoint = POINT;
		CvScalar color = WHITE;
		putText(defaultFrame, "DISCONECT", textPoint, FONT_HERSHEY_COMPLEX_SMALL, thickNess, color, lineType, CV_AA);
		int frameSize = camList.size();	
		Mat* frame = new Mat[sizeof(camList)];
		int framePerRow;
		(frameSize < 2) ? (framePerRow = 1) : (frameSize < 5) ? (framePerRow = 2) : (frameSize < 10) ?
			(framePerRow = 3) : (frameSize < 17) ? (framePerRow = 4) : (framePerRow = 0);
		if (framePerRow == 0) {
			cout << "ERROR: DATA INPUT INVALID !!!" << endl;
			system("pause");
		}
		int frameHeight = floor(DISPLAY_SCREEN_HEIGHT / framePerRow);
		int frameWidth = floor(DISPLAY_SCREEN_WIDTH / framePerRow);
		Mat temp;
		vector<Mat> tmpArray;
		while (true) {
			for (int i = 0; i < frameSize; i++) {
				capture[i] >> frame[i];
				resize(frame[i], frame[i], Size(frameHeight, frameWidth), 0, 0, INTER_CUBIC);
				printTime(frame);
			}			
			for (int i = 0; i < DISPLAY_SCREEN_HEIGHT; i += frameHeight) {
				for (int j = 0; j < DISPLAY_SCREEN_WIDTH; j += frameWidth) {
					temp = Mat(display, Rect(i, j, frameHeight, frameWidth));
					tmpArray.push_back(temp);
				}
			}
			for (int i = 0; i < frameSize; i++) {
				frame[i].copyTo(tmpArray[i]);
			}
			imshow("DISPLAY_SCREEN", display);
			if (waitKey(30) == 27) {
				cout << "Esc key is pressed by user" << endl;
				break;
			}
		}
	}
};

int main(int args, char* argv[]) {
	DISPLAY_SCREEN *DISPLAY;
	DISPLAY = new DISPLAY_SCREEN();
	DISPLAY->GENERATE();
}