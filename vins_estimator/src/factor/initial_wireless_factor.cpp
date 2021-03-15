/*******************************************************
 * wireless measurement costfuction
 * Author: Sheyang Tang
 *******************************************************/

#include "initial_wireless_factor.h"

Eigen::Matrix3d InitialWirelessFactor::wireless_covariance;

bool InitialWirelessFactor::Evaluate(double const *const *parameters, double *residuals, double **jacobians) const
{
    Eigen::Vector3d P_UWB(parameters[0][0], parameters[0][1], parameters[0][2]);
    Eigen::Quaterniond Q_world_uwb(parameters[0][6], parameters[0][3], parameters[0][4], parameters[0][5]);
    
    Eigen::Vector3d P_IMU_world = rot_diff * (s * image_frame.T - image_frame.R * TIC[0] - (s * P0 - R0 * TIC[0]));
    
    double cos_aoa = cos(image_frame.wireless_aoas[i]);
    double sin_aoa = sin(image_frame.wireless_aoas[i]);
    
    double range = image_frame.wireless_ranges[i];
    Eigen::Vector3d wireless_direction = Vector3d(cos_aoa, sin_aoa, 0);

    Eigen::Map<Eigen::Vector3d> residual(residuals);
    residual = Q_world_uwb.toRotationMatrix() * range * wireless_direction - P_IMU_world + P_UWB;

    if (jacobians)
    {
        if (jacobians[0])
        {
            Eigen::Map<Eigen::Matrix<double, 3, 7, Eigen::RowMajor>> jacobian_uwb_pos(jacobians[0]);
            jacobian_uwb_pos.setZero();

            jacobian_uwb_pos.block<2, 2>(0, 0) = Eigen::Matrix2d::Identity();
            jacobian_uwb_pos.block<3, 3>(0, 3) = -Utility::skewSymmetric(Q_world_uwb.toRotationMatrix() * range * wireless_direction);
        }
    }
    return true;
}

