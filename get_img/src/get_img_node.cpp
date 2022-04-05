#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
Mat img_src;
class mark{
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    int x_px,y_px;
    string file_name;
    int img_num=0;
public:
        mark():it_(nh_){
        nh_.getParam("img_weight",x_px);cout<<"x_px: "<<x_px<<endl;
        nh_.getParam("img_height",y_px);cout<<"y_px: "<<y_px<<endl;
        nh_.getParam("img_path",file_name);cout<<"file_name: "<<file_name<<endl;
        image_sub_ = it_.subscribe("/cam_img", 1,  &mark::Image_cb, this);
    }
    void Image_cb(const sensor_msgs::ImageConstPtr& msg)
    {
        //convert ROS image msg to opencv Mat
        try
        {
            img_src = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::BGR8)->image.clone();
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        imwrite(file_name+to_string(img_num)+".png",img_src);
        img_num++;
        line(img_src, Point(0,y_px/3),Point(x_px,y_px/3), Scalar(0,255,255));
        line(img_src, Point(0,2*y_px/3),Point(x_px,2*y_px/3), Scalar(0,255,255));
        line(img_src, Point(x_px/3,0),Point(x_px/3,y_px), Scalar(255,0,255));
        line(img_src, Point(2*x_px/3,0),Point(2*x_px/3,y_px), Scalar(255,0,255));
        imshow("img",img_src);
        waitKey(0);
    }
};
int main(int argc, char** argv){
    ros::init(argc, argv, "get_img_node");
    mark start;
    ros::spin();
    return 0;
}