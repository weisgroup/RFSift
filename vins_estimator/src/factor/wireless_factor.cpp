/*******************************************************
 * wireless measurement costfuction
 * Author: Sheyang Tang
 *******************************************************/

#include "wireless_factor.h"

Eigen::Matrix3d WirelessFactor::wireless_covariance;

bool WirelessFactor::Evaluate(double const *const *parameters, double *residuals, double **jacobians) const
{
    Eigen::Vector3d Pi(parameters[0][0], parameters[0][1], parameters[0][2]);
    Eigen::Quaterniond Qi(parameters[0][6], parameters[0][3], parameters[0][4], parameters[0][5]);
    double range_no_bias = uwb_range;

    double cos_aoa = cos(uwb_aoa);
    double sin_aoa = sin(uwb_aoa);
    Eigen::Vector3d wireless_direction = Eigen::Vector3d(cos_aoa, sin_aoa, 0);

    Eigen::Map<Eigen::Vector3d> residual(residuals);
    residual = uwb_node_orientation.toRotationMatrix() * range_no_bias * wireless_direction - Pi + uwb_node_position;
    wireless_covariance = UWB_DIS_COV * Eigen::Matrix3d::Identity();
    Eigen::Matrix<double, 3, 3> sqrt_info = Eigen::LLT<Eigen::Matrix<double, 3, 3>>(wireless_covariance.inverse()).matrixL().transpose();
    residual = sqrt_info * residual;

    if (jacobians)
    {
        if (jacobians[0])
        {
            Eigen::Map<Eigen::Matrix<double, 3, 7, Eigen::RowMajor>> jacobian_pose(jacobians[0]);
            jacobian_pose.setZero();

            jacobian_pose.block<2, 2>(0, 0) = -Eigen::Matrix2d::Identity();
            jacobian_pose = sqrt_info * jacobian_pose;
        }
    }
    return true;
}

