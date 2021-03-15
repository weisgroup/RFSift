/*******************************************************
 * wireless measurement costfuction
 * Author: Sheyang Tang
 *******************************************************/

#pragma once

#include <ros/assert.h>
#include <iostream>

#include <ceres/ceres.h>
#include <eigen3/Eigen/Dense>
#include "../utility/utility.h"
#include "../estimator/parameters.h"

class WirelessFactor : public ceres::SizedCostFunction<3, 7>
{
  public:
    WirelessFactor(const Eigen::Vector3d &_uwb_node_position, const Eigen::Quaterniond &_uwb_node_orientation, const double &_uwb_range, const double &_uwb_aoa) :
                   uwb_node_position(_uwb_node_position), uwb_node_orientation(_uwb_node_orientation), uwb_range(_uwb_range), uwb_aoa(_uwb_aoa)
    {
    }
    virtual bool Evaluate(double const *const *parameters, double *residuals, double **jacobians) const;

    Eigen::Vector3d uwb_node_position;
    Eigen::Quaterniond uwb_node_orientation;
    double uwb_range, uwb_aoa;
    static Eigen::Matrix3d wireless_covariance;
};
