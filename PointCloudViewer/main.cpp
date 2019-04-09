#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>

using namespace cv;
using namespace std;

static void help()
{
	cout
		<< "--------------------------------------------------------------------------" << endl
		<< "This program shows how to use makeTransformToGlobal() to compute required pose,"
		<< "how to use makeCameraPose and Viz3d::setViewerPose. You can observe the scene "
		<< "from camera point of view (C) or global point of view (G)" << endl
		<< "Usage:" << endl
		<< "./main [ G | C ]" << endl
		<< endl;
}


static Mat cvcloud_load()
{
	Mat cloud(1, 1889, CV_32FC3);

	Point3f* data = cloud.ptr<cv::Point3f>();
	
	for (size_t i = 0; i < 1889; ++i)
	{
		data[i].x = 0;
		data[i].y = i * 0.01;
		data[i].z = 0;
	}
	
	return cloud;
}


int main()
{
	help();
	
	bool camera_pov = false;
	viz::Viz3d myWindow("Coordinate Frame");
	myWindow.showWidget("Coordinate Widget", viz::WCoordinateSystem());
	Vec3f cam_pos(3.0f, 3.0f, 3.0f), cam_focal_point(3.0f, 3.0f, 2.0f), cam_y_dir(-1.0f, 0.0f, 0.0f);
	Affine3f cam_pose = viz::makeCameraPose(cam_pos, cam_focal_point, cam_y_dir);
	Affine3f transform = viz::makeTransformToGlobal(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, -1.0f), cam_pos);//原点を基準にしたカメラのAffine変換
	Mat bunny_cloud = cvcloud_load();
	viz::WCloud cloud_widget(bunny_cloud, viz::Color::green());
	Affine3f cloud_pose = Affine3f().translate(Vec3f(0.0f, 0.0f, 3.0f));
	Affine3f cloud_pose_global = transform * cloud_pose;
	if (!camera_pov)
	{
		viz::WCameraPosition cpw(0.5); // Coordinate axes
		viz::WCameraPosition cpw_frustum(Vec2f(0.889484, 0.523599)); // Camera frustum
		myWindow.showWidget("CPW", cpw, cam_pose);
		myWindow.showWidget("CPW_FRUSTUM", cpw_frustum, cam_pose);
	}
	myWindow.showWidget("bunny", cloud_widget, cloud_pose_global);
	if (camera_pov)
		myWindow.setViewerPose(cam_pose);
	myWindow.spin();

	return 0;
}