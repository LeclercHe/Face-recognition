#include <iostream>
#include <opencv2/opencv.hpp>
#include "face.h"

using namespace cv;
using namespace std;
using namespace aip;

int main(int,char**)
{
	VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
		cout << "camera open err!"<<endl; 
		return -1;
	}
	cout << "camera open success!"<<endl; 

	Mat ColorImage;
	Mat GrayImage;
	CascadeClassifier Classifier("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
	vector<Rect> AllFace;
	Mat MatFace;
	vector<uchar> JpgFace;
	Json::Value result;
	string Base64Face;
	time_t sec;
	
	aip::Face client("31275602", "Kle5QFu1dGbKzuj24NRcnOY4", "3Y5fbuchaYhsZLXcSGiSC4WUAathLC5T"); // connect baiduyun
		
	for(;;)
	{
    cap >> ColorImage;     // get a new frame from camera
	cvtColor(ColorImage, GrayImage, CV_BGR2GRAY); // Converts an image from one color space to Gray
	equalizeHist(GrayImage, GrayImage);    //image equalize
	Classifier.detectMultiScale(GrayImage, AllFace); //detect face
	if(AllFace.size())
	{
		rectangle(GrayImage, AllFace[0], Scalar(255,255,255)); // draw rectangle to image
		MatFace = GrayImage(AllFace[0]);
		imencode(".jpg", MatFace, JpgFace); // transfrom image Mat to Jpg 
		Base64Face = base64_encode((char *)JpgFace.data(), JpgFace.size()); // transfrom image Jpg to BASE64 
		result = client.search(Base64Face, "BASE64", "Student" ,aip::null); // search image in Student  list of baiduyun 

		if(!result["result"].isNull())
		{
			if(result["result"]["user_list"][0]["score"].asInt() > 80)
			{
				cout << result["result"]["user_list"][0]["user_id"] <<endl;
				sec = time(NULL);
				cout << ctime(&sec) <<endl;
				putText(GrayImage, result["result"]["user_list"][0]["user_id"].asString(), Point(0,20), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
				putText(GrayImage, ctime(&sec), Point(0,40), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255));
			}
		}
 
	}
	
	imshow("video", GrayImage); //show Image by window
	waitKey(40); // wait ms 
	
		
	}

	return 0;
}

