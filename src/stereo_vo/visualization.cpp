/// \file
/// \brief Visualization module

#include <cmath>
#include <iostream>
#include <stereo_visual_slam_main/library_include.hpp>
#include <stereo_visual_slam_main/frame.hpp>
#include <vector>
#include <string>
#include <unistd.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Image.h>
#include <stereo_visual_slam_main/visualization.hpp>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

namespace vslam
{

int VslamVisual::points_to_feature_map(const vector<cv::Point3f> &point_3d)
{
    if (point_3d.size() == 0)
    {
        std::cout << "Invalid input: empty 3D points" << std::endl;
    }

    const int num_channels = 3; // x y z

    feature_map_ = sensor_msgs::PointCloud2();
    feature_map_.header.stamp = ros::Time::now();

    // Modify this to current frame
    feature_map_.header.frame_id = "/camera";

    feature_map_.height = 1;
    feature_map_.width = point_3d.size();
    feature_map_.is_bigendian = false;
    feature_map_.is_dense = true;
    feature_map_.point_step = num_channels * sizeof(float);
    feature_map_.row_step = feature_map_.point_step * feature_map_.width;
    feature_map_.fields.resize(num_channels);

    std::string channel_id[] = {"x", "y", "z"};
    for (int i = 0; i < num_channels; i++)
    {
        feature_map_.fields[i].name = channel_id[i];
        feature_map_.fields[i].offset = i * sizeof(float);
        feature_map_.fields[i].count = 1;
        feature_map_.fields[i].datatype = sensor_msgs::PointField::FLOAT32;
    }

    feature_map_.data.resize(feature_map_.row_step * feature_map_.height);

    unsigned char *feature_map__data_ptr = &(feature_map_.data[0]);

    float data_array[num_channels];

    for (unsigned int i = 0; i < feature_map_.width; i++)
    {

        data_array[0] = point_3d.at(i).x;
        data_array[1] = point_3d.at(i).y;
        data_array[2] = point_3d.at(i).z;
        memcpy(feature_map__data_ptr + (i * feature_map_.point_step), data_array, num_channels * sizeof(float));
    }

    return 0;
}

int VslamVisual::publish_feature_map(const vector<cv::Point3f> &point_3d)
{
    points_to_feature_map(point_3d);
    feature_map_publisher_.publish(feature_map_);
    return 0;
}

int VslamVisual::publish_transform(const SE3 &T_c_w)
{
    SE3 T_w_c;
    T_w_c = T_c_w.inverse(); // T_world_current(camera)

    Eigen::Matrix3d rotation = T_w_c.rotationMatrix();
    Eigen::Vector3d translation = T_w_c.translation();

    // extract sohpus transformation to tf format
    tf::Matrix3x3 tf_rotation(rotation(0, 0), rotation(0, 1), rotation(0, 2),
                              rotation(1, 0), rotation(1, 1), rotation(1, 2),
                              rotation(2, 0), rotation(2, 1), rotation(2, 2));

    tf::Vector3 tf_translation(translation(0), translation(1), translation(2));

    tf::Transform tf_transformation(tf_rotation, tf_translation);


    // publish the tf
    static tf::TransformBroadcaster tf_broadcaster;
    tf_broadcaster.sendTransform(tf::StampedTransform(tf_transformation, ros::Time::now(), "/map", "/camera"));

}

} // namespace vslam
