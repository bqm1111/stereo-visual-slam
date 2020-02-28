/// \file
/// \brief ROS Node to run the Vslam

#include <cmath>
#include <iostream>
#include "stereo_visual_slam_main/library_include.hpp"
#include "stereo_visual_slam_main/types_def.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include "stereo_visual_slam_main/visual_odometry.hpp"
#include <stereo_visual_slam_main/map.hpp>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "run_vslam");
    ros::NodeHandle nh;
    std::string dataset;
    nh.getParam("/dataset", dataset);

    vslam::Map my_map;
    vslam::VO my_VO(dataset, nh, my_map);
    my_VO.initialization();

    // debug printing
    std::cout << "Num of landmarks: " << my_map.landmarks_.size() << std::endl;
    std::cout << "Num of keyframes: " << my_map.keyframes_.size() << std::endl;
    for (int num_keyframe = 0; num_keyframe < my_map.keyframes_.size(); num_keyframe++)
    {
        std::cout << "Num of features in keyframe: " << num_keyframe << " - " << my_map.keyframes_[num_keyframe].features_.size() << std::endl;
    }
    
    ros::spin();

    return 0;
}