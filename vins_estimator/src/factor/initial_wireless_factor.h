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
#include "../initial/initial_alignment.h"

class InitialWirelessFactor : public ceres::SizedCostFunction<3, 7>
{
  public:
    InitialWirelessFactor(const ImageFrame &_image_frame, 
                        const double &_s, const Matrix3d &_rot_diff, 
                        const Vector3d &_P0, const Matrix3d &_R0, const int &_i) :
                        image_frame(_image_frame),
                        s(_s), rot_diff(_rot_diff), P0(_P0), R0(_R0), i(_i)
    {
    }
    virtual bool Evaluate(double const *const *parameters, double *residuals, double **jacobians) const;
    //void check(double **parameters);

    ImageFrame image_frame;
    double s;
    int i;
    Matrix3d rot_diff, R0;
    Vector3d P0;

    static Matrix3d wireless_covariance;
};
