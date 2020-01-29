/// \file
/// \brief Definition of frame struct

#include <stereo_visual_slam_main/frame.hpp>

namespace vslam
{

// bug narrowed down: when a class needs to be initialized
// without parameters (Frame new_frame;), the custom default
// constructor does not work.
// practice: have a default constructor defined by Frame() = default;

Frame::Frame(int id, double timestamp, const Mat &left, const Mat &right)
{
    id_ = id;
    time_stamp_ = timestamp;
    left_img_ = left;
    right_img_ = right;
    disparity_ = cv::Mat();
}

} // namespace vslam