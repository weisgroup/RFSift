# RFSift
This is the project page for the paper **"Conquering Textureless with RF-referenced Monocular Vision for MAV State Estimation"**.

RFSift is a new state estimator that conquers the textureless challenge with RF-referenced monocular vision. It achieves centimeter-level accuracy in textureless scenes, e.g., dark corridor, white wall, and solid color floor.

This project page contains:
- Overview of our proposed method.
- System implementation and real-world tests.

## The proposed method
Our method consists of two components:
- **RF-sifting** algorithm that sifts best visual features by leveraging 3D UWB measurements.
- **RF-visual-inertial sensor fusion** that achieves robust state estimation by fusing measurements from multiple sensors with complementary advantages.

The system overview is illustrated in the following figure.
<p align="center">
  <img src="figs/overview.png">
</p>

## System implementation and real-world tests

### Implementation

#### Prerequisites
- **Ubuntu** and **ROS**
<br/>Ubuntu 64-bit 16.04 or 18.04.
ROS Kinetic or Melodic. [ROS Installation](http://wiki.ros.org/ROS/Installation)
- **Ceres Solver**
<br/>Follow [Ceres Installation](http://ceres-solver.org/installation.html).
```shell
    cd ~
    git clone https://ceres-solver.googlesource.com/ceres-solver
    sudo apt-get -y install cmake libgoogle-glog-dev libatlas-base-dev libeigen3-dev libsuitesparse-dev
    sudo add-apt-repository ppa:bzindovic/suitesparse-bugfix-1319687
    sudo apt-get update && sudo apt-get install libsuitesparse-dev
    mkdir ceres-bin
    cd ceres-bin
    cmake ../ceres-solver
    make -j3
    sudo make install
```
- **Install MYNTEYE SDK**
<br/>We use the left camera of MYNT EYE S1040-120/Mono for monocular visual sensing and follow the following installation process: [MYNT-EYE-S-SDK](https://github.com/slightech/MYNT-EYE-S-SDK)

- **UWB Node Configuration**
<br/>We use [Nooploop LinkTrack AoA UWB nodes](https://www.nooploop.com/en/linktrack-aoa/). The UWB AoAs need to be calibrated once. Please find the software and user manual in [Nooploop](https://www.nooploop.com/en/download/).

#### Compile the code
Clone the repository and catkin_make:
```shell
    cd ~/catkin_ws/src
    git clone https://github.com/weisgroup/RFSift.git
    cd ../
    catkin_make
    source ~/catkin_ws/devel/setup.bash
```
**Note:** If you fail in this step, try to find another computer with clean system or reinstall Ubuntu and ROS)

#### Configure and run RFSift
**Hardware setup**
<br/>Connect the MYNT EYE camera and UWB tag to your MAV's onboard computer. Connect the UWB anchor to a charger.
<p align="center">
  <img src="https://github.com/weisgroup/RFSift/blob/master/RFSIFT_hardware.png" width = "687" height = "452">
</p>

**Run MYNT EYE SDK**
```shell
    cd path/to/MYNT-EYE-S-SDK
    source wrappers/ros/devel/setup.bash
    roslaunch mynt_eye_ros_wrapper vins_fusion.launch
```
**Run UWB node**
```shell
    cd ~/catkin_ws
    source devel/setup.bash
    roslaunch nlink_parser linktrack_aoa.launch
```
You can view the topic `/nlink_linktrack_aoa_nodeframe0` in by `rostopic echo /nlink_linktrack_aoa_nodeframe0` in another terminal. If everything goes well, UWB measurements will be printed on the screen.

**Run RFSift**
```shell
    cd ~/catkin_ws
    source devel/setup.bash
    roslaunch vins mynteye-s-mono-imu.launch
```
Initialization is needed. Move the MAV around at the same height as the UWB anchor. This may take up to a minute. Once it is finished, you can view the MAV and UWB nodes in Rviz.
<br/>**Note:** the MAV should move infront of the UWB anchor, otherwise the AoAs and ranges cannot be measured correctly.

### Customization
You can customize your own experiment by editing the config file `/config/mynteye-s/mynt_mono_config.yaml`. For example, you can set `uwb_optimize: 0` to ignore uwb measurements in optimization. For a more detailed description, please refer to the config file and read the comments.

### Run on DJI M100
We use [djiros and n3ctrl](https://github.com/INFINITSY/DJIROS-N3CTRL) to test RFSift on DJI M100. Some of the codes are modified to work with the N1 flight controller on M100. Please refer to them if you are interested.

## Demo video

<a href="https://youtu.be/S5VIiH1Nw6U" target="_blank"><img src="figs/demo.jpeg" alt="Sift" width="320" height="240" border="10" /></a>

## Acknowledgement
This code borrows heavily from [VINS-Fusion](https://github.com/HKUST-Aerial-Robotics/VINS-Fusion.git).

## License
The source code is released under [GPLv3](https://www.gnu.org/licenses/) license.

For any technical issues, please contact Sheyang Tang <sheyangtangATgmail.com> and Shengkai Zhang <shengkai.zhangATgmail.com>
