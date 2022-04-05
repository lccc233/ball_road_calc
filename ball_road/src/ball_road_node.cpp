#include<opencv2/opencv.hpp>
#include<ros/ros.h>
#include<math.h>
#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
bool flag_click;
cv::Point start,over;
cv::Mat ball,big_img;
std::string rcd_path,result_path,now_work;
void MouseCallback(int event,int x,int y,int flag,void*);
int main(int argc, char** argv){
	ros::init(argc, argv, "ball_road_node");
	ros::NodeHandle n;
	n.getParam("/rcd_path",rcd_path);
	n.getParam("/result_path",result_path);
	n.getParam("/now_work",now_work);
	std::cout<<"now_work:"<<now_work<<std::endl;
	rcd_path+=now_work;
	now_work.pop_back();
	now_work.pop_back();
	now_work.pop_back();
	result_path+=(now_work+"txt");
	ros::Rate loop_rate(60);
	cv::namedWindow("ball",cv::WINDOW_AUTOSIZE);
	//cv::namedWindow("big",cv::WINDOW_AUTOSIZE);
	std::cout<<rcd_path<<std::endl;
	std::cout<<result_path<<std::endl;
	cv::VideoCapture vedio(rcd_path);
	freopen(result_path.c_str(),"w",stdout);
	while(ros::ok()){
		vedio>>ball;
		//ball=cv::imread("/home/lic/Desktop/ball/0.png");
		cv::imshow("ball",ball);
		cv::setMouseCallback("ball",MouseCallback,0);
		//cv::imshow("big",big_img);
		//cv::line(big_img,cv::Point(0,100),cv::Point(200,100),cv::Scalar(0,255,0));
		cv::waitKey(0);
		loop_rate.sleep();
	}
	return 0;
}
void MouseCallback(int event,int x,int y,int flag,void*){
	if(event==cv::EVENT_LBUTTONDOWN){
		//std::cout<<"Down: "<<x<<' '<<y<<std::endl;
		if(flag_click==0)flag_click=1,start=cv::Point(x,y);
		else if(flag_click==1){
			flag_click=0,over=cv::Point(x,y);
			cv::rectangle(ball,start,over,cv::Scalar(255,255,0));
			cv::imshow("ball",ball);
			//cv::imshow("big",big_img);
			//std::cout<<start<<" "<<over<<std::endl;
			std::cout<<2000*(start.x+over.x)/2/464-172<<' '<<915-2000*(start.y+over.y)/2/464<<std::endl;
		}
	}
	if(event==cv::EVENT_MOUSEMOVE){
		//std::cout<<x<<" "<<y<<std::endl;
		if(x>100&&y>100&&x<ball.cols-100&&y<ball.rows-100){
			ball(cv::Rect(x-100,y-100,200,200)).copyTo(big_img);
			cv::line(big_img,cv::Point(0,100),cv::Point(200,100),cv::Scalar(170,170,255));
			cv::line(big_img,cv::Point(100,0),cv::Point(100,200),cv::Scalar(170,170,255));
			cv::resize(big_img,big_img,cv::Size(200,200));
			big_img.copyTo(ball(cv::Rect(ball.cols-200,ball.rows-200,200,200)));
			cv::imshow("ball",ball);
		}
		//cv::imshow("ball",ball);
		//cv::imshow("big",big_img);
	}
}