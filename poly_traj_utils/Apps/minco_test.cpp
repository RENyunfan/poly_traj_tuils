//
// Created by yunfan on 2021/3/19.
//

#include "poly_traj_utils/minco/minjerk/minco_jerk.hpp"
#include "poly_traj_utils/msg_utils.h"
#include "poly_traj_utils/minco/minsnap/minco_snap.hpp"
#include "poly_traj_utils/visualization_utils.hpp"
#include "fmt/color.h"


using namespace fmt;
/*
 * Test code:
 *      roslaunch simulator test_env.launch
 * */
#define BACKWARD_HAS_DW 1

#include "backward.hpp"

namespace backward {
    backward::SignalHandling sh;
}

SE3GCOPTER minco;
QNMOPT qnm;
int idx = 0;
SE3GCOPTER_HPP
ros::Publisher end_point_state_pub, start_point_state_pub, mkr_pub, mkr_pub2;
Vec3 points[2];
Vec3 vels[2];
Vec3 W(2, 0, 0);
geometry_msgs::Pose uavPoses[2];

void TwoPointCallback(StatePVA iState, StatePVA fState) {
    vector<Vec3> waypts;
    waypts.clear();
    double res = INFINITY;
    int itg = 8;
    double horizhalf = 0.25;
    double vertihalf = 0.15;
    double safe_mar = 0.25;
    double velmax = 5;
    double tacMax = 15;
    double tacMin = 3;
    double brmax = 4;
    double g = 9.81;
    double cost;
    ros::Time backend_start_time = ros::Time::now();
    Eigen::Vector4d ww(10000000.0, 10000.0, 10000.0, 10000.0);
    minco.setup(1000, 0.0, 1e-2, iState, fState, waypts, res, itg,
                velmax, tacMin, tacMax, brmax, g, ww,
                true);
    Trajectory traj;
    int retcode = minco.optimize(traj, cost);
    if (retcode < 0) {
        fmt::print(fg(fmt::color::red) | emphasis::bold, " -- [PLANNER] Backend failed, return!\n");
        cout << "iniState===============\n " << iState << endl;
        for (int i = 0; i < waypts.size(); i++) {
            cout << "Waypts " << i << " :" << waypts[i].transpose() << endl;
        }
        cout << "finState===============\n " << fState << endl;

    }
    fmt::print(fg(fmt::color::cadet_blue), " -- [PLANNER] MINCO optimize success!\n");
    double t = (ros::Time::now() - backend_start_time).toSec() * 1000;
    fmt::print(fg(fmt::color::light_green), "\t\tTotal backedn-time:\t{:0.3} ms.\n", t);
    fmt::print(fg(fmt::color::light_green), "\t\tBackend duration:\t{0:.3} s.\n",
               traj.getTotalDuration());
    fmt::print(fg(fmt::color::light_green), "\t\tBackend Max vel:\t{0:.3} m/s.\n",
               traj.getMaxVelRate());
    fmt::print(fg(fmt::color::light_green), "\t\tBackend Max acc:\t{0:.3} m/s.\n",
               traj.getMaxAccRate());

    VisualUtils::PublishTrajPosiToMarkerArray(mkr_pub, traj);
    VisualUtils::PublishTrajVelToMarkerArray(mkr_pub, traj);
    VisualUtils::PublishTrajAccToMarkerArray(mkr_pub, traj);

}

void MultiPointCallback(vector<Vec3> waypts) {
    int itg = 10;
    double velmax = 5;
    double accmax = 10;
    double tacMax = 15;
    double tacMin = 3;
    double brmax = 3;
    double g = 9.81;
    double smo = 1e-2;
    double cost;
    StatePVA iState, fState;
    iState << waypts[0], Vec3(0, 0, 0), Vec3(0, 0, 0);
    fState << waypts[waypts.size() - 1], Vec3(0, 0, 0), Vec3(0, 0, 0);

    vector<Vec3> waypts_no;
    for (int i = 1; i < waypts.size() - 1; i++) {
        waypts_no.push_back(waypts[i]);
    }
    Trajectory traj;

    Eigen::Vector4d ww(10000000.0, 10000.0, 10000.0, 10000.0);
    ros::Time backend_start_time = ros::Time::now();
    minco.setup(1000, 0.0, smo, iState, fState, waypts_no, itg,
                velmax, accmax, tacMin, tacMax, brmax, g, ww,
                true);
    cost = minco.optimize(traj, cost);
    double t = (ros::Time::now() - backend_start_time).toSec() * 1000;
    vector<Vec3> inp = minco.getInnerPts();
    vector<Vec3> iinp = minco.getInitInnerPts();
    fmt::print(fg(fmt::color::cadet_blue), " -- [PLANNER] MINCO optimize success!\n");
    fmt::print(fg(fmt::color::light_green), "\t\tTotal backedn-time:\t{:0.3} ms.\n", t);
    fmt::print(fg(fmt::color::light_green), "\t\tBackend duration:\t{0:.3} s.\n",
               traj.getTotalDuration());
    fmt::print(fg(fmt::color::light_green), "\t\tBackend Max vel:\t{0:.3} m/s.\n",
               traj.getMaxVelRate());
    fmt::print(fg(fmt::color::light_green), "\t\tBackend Max acc:\t{0:.3} m/s.\n",
               traj.getMaxAccRate());
    fmt::print(fg(fmt::color::light_green), "\t\tBackend Cost:\t{0:.3}\n",
               cost);

    VisualUtils::PublishTrajPosiToMarkerArray(mkr_pub, traj);
    VisualUtils::PublishTrajVelToMarkerArray(mkr_pub, traj);
    VisualUtils::PublishTrajAccToMarkerArray(mkr_pub, traj);
    VisualUtils::PublishTrajJerkToMarkerArray(mkr_pub, traj);
    VisualUtils::PublishVectorVec3ToMarkerArray(mkr_pub, inp);
    VisualUtils::PublishVectorVec3ToMarkerArray(mkr_pub, iinp, GREENYELLOW, "init_p");


    /*=============================================*/
    {
        int itg = 10;
        double velmax = 5;
        double accmax = 10;
        double tacMax = -1;
        double tacMin = -1;
        double brmax = -1;
        double g = 9.81;
        double smo = 1e-2;
        double cost;
        StatePVA iState, fState;
        iState << waypts[0], Vec3(0, 0, 0), Vec3(0, 0, 0);
        fState << waypts[waypts.size() - 1], Vec3(0, 0, 0), Vec3(0, 0, 0);

        vector<Vec3> waypts_no;
        for (int i = 1; i < waypts.size() - 1; i++) {
            waypts_no.push_back(waypts[i]);
        }
        Trajectory traj;

        Eigen::Vector4d ww(10000000.0, 10000.0, 10000.0, 10000.0);
        ros::Time backend_start_time = ros::Time::now();
        minco.setup(1000, 0.0, smo, iState, fState, waypts_no, itg,
                    velmax, accmax, tacMin, tacMax, brmax, g, ww,
                    true);
        cost = minco.optimize(traj, cost);
        double t = (ros::Time::now() - backend_start_time).toSec() * 1000;
        vector<Vec3> inp = minco.getInnerPts();
        vector<Vec3> iinp = minco.getInitInnerPts();
        fmt::print(fg(fmt::color::cadet_blue), " -- [PLANNER] MINCO optimize success!\n");
        fmt::print(fg(fmt::color::light_green), "\t\tTotal backedn-time:\t{:0.3} ms.\n", t);
        fmt::print(fg(fmt::color::light_green), "\t\tBackend duration:\t{0:.3} s.\n",
                   traj.getTotalDuration());
        fmt::print(fg(fmt::color::light_green), "\t\tBackend Max vel:\t{0:.3} m/s.\n",
                   traj.getMaxVelRate());
        fmt::print(fg(fmt::color::light_green), "\t\tBackend Max acc:\t{0:.3} m/s.\n",
                   traj.getMaxAccRate());
        fmt::print(fg(fmt::color::light_green), "\t\tBackend Cost:\t{0:.3}\n",
                   cost);

        VisualUtils::PublishTrajPosiToMarkerArray(mkr_pub2, traj,BLUE);
        VisualUtils::PublishTrajVelToMarkerArray(mkr_pub2, traj);
        VisualUtils::PublishTrajAccToMarkerArray(mkr_pub2, traj);
        VisualUtils::PublishTrajJerkToMarkerArray(mkr_pub2, traj);
        VisualUtils::PublishVectorVec3ToMarkerArray(mkr_pub2, inp);
        VisualUtils::PublishVectorVec3ToMarkerArray(mkr_pub2, iinp, GREENYELLOW, "init_p");

    }


}

nav_msgs::Odometry odom_;
vector<Vec3> waypts;

void wayPoint_callback(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    Vec3 cur_point;
    cur_point = Vec3(msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
    if (msg->pose.position.z > -0.1) {
        odom_.pose.pose = msg->pose;
        odom_.header.frame_id = "world";
        odom_.header.stamp = ros::Time::now();
        waypts.push_back(cur_point);
    } else {
        MultiPointCallback(waypts);
        waypts.clear();
    }

}

int main(int argc, char **argv) {

    ros::init(argc, argv, "kite");
    ros::NodeHandle nh("~");

    print(fg(color::light_sea_green), " -- [KRRT-Test] Begin.\n");

    /* Ros params*/
    double click_vel;
    nh.param("/click_vel", click_vel, 1.0);
    W.x() = click_vel;
    /* Publisher and subcriber */
    ros::Subscriber waypoints_sub = nh.subscribe("/goal", 1, wayPoint_callback);

    start_point_state_pub = nh.advertise<geometry_msgs::PoseStamped>("/start_point_vis", 10);
    end_point_state_pub = nh.advertise<geometry_msgs::PoseStamped>("/end_point_vis", 10);
    mkr_pub = nh.advertise<visualization_msgs::MarkerArray>("/goal_marker", 10);
    mkr_pub2 = nh.advertise<visualization_msgs::MarkerArray>("/goal_marker2", 10);
    ros::AsyncSpinner spinner(0);
    spinner.start();
    ros::Duration(1.0).sleep();


    ros::waitForShutdown();
    return 0;
}

