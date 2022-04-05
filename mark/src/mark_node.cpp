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
    int x_px,y_px,max_fps;
    VideoWriter vw;
    string rcd_path_;
    string file_name;
    Mat cam_mat=(Mat_<double>(3,3) << 876.7098761620152, 0, 639.3930965306506,0, 876.2985055104125, 101.7140442167709,0, 0, 1);
    Mat dist=(Mat_<double>(3,3)<<-0.1289502040542916, 0.2532410519325077, 0.0006802266708216269, -0.002591760947858819, -0.2736025314101113);
public:
        mark():it_(nh_){
        nh_.getParam("img_weight",x_px);cout<<"x_px: "<<x_px<<endl;
        nh_.getParam("img_height",y_px);cout<<"y_px: "<<y_px<<endl;
        nh_.getParam("rcd_path", rcd_path_);cout<<"PATH: "<<rcd_path_<<endl;
        nh_.getParam("max_fps",max_fps);
        nh_.getParam("vedio_name",file_name);cout<<"vedio_name: "<<file_name<<endl;;
        vw.open(rcd_path_+file_name, VideoWriter::fourcc('D', 'I', 'V', 'X'), max_fps,Size(x_px,y_px)); 
        cout<<"ok";
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
        undistort(img_src,img_src,cam_mat,dist);
        vw.write(img_src);
        line(img_src, Point(0,y_px/3),Point(x_px,y_px/3), Scalar(0,255,255));
        line(img_src, Point(0,2*y_px/3),Point(x_px,2*y_px/3), Scalar(0,255,255));
        line(img_src, Point(x_px/3,0),Point(x_px/3,y_px), Scalar(255,0,255));
        line(img_src, Point(2*x_px/3,0),Point(2*x_px/3,y_px), Scalar(255,0,255));
        imshow("img",img_src);
        waitKey(1);
    }
};
int main(int argc, char** argv){
    ros::init(argc, argv, "mark_node");
    mark start;
    ros::spin();
    return 0;
}