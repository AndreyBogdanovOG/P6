#include "stdafx.h"
#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
//#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <boost/make_shared.hpp>

void showCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr showThis)
{
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Viewer"));
	//viewer.showCloud(showThis);
	viewer->setBackgroundColor(0,0,0);
	viewer->removeAllShapes();
	viewer->removeAllPointClouds();

	viewer->addPointCloud<pcl::PointXYZ>(showThis, "Uploaded1");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "Uploaded1");

	//viewer->spinOnce(100);
	while (!viewer->wasStopped())
	{
		viewer->spinOnce();
	}
}

void showNormals(pcl::PointCloud<pcl::PointXYZ>::Ptr showThis, pcl::PointCloud<pcl::Normal>::Ptr showThisNormal)
{
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("NormalViewer"));
	viewer->setBackgroundColor(0, 0, 0);
	viewer->removeAllShapes();
	viewer->removeAllPointClouds();

	viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(showThis, showThisNormal);
	//viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "Uploaded1");

	//viewer->spinOnce(100);
	while (!viewer->wasStopped())
	{
		viewer->spinOnce();
	}
}

//work under construction
void findNormals(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
	// Create the normal estimation class, and pass the input dataset to it
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);

	// Create an empty kdtree representation, and pass it to the normal estimation object.
	// Its content will be filled inside the object, based on the given input dataset (as no other search surface is given).
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
	ne.setSearchMethod(tree);

	// Output datasets
	pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);

	//ne.setKSearch(4);

	// Use all neighbors in a sphere of radius 3cm
	ne.setRadiusSearch(0.03);

	// Compute the features
	ne.compute(*cloud_normals);

	// cloud_normals->points.size () should have the same size as the input cloud->points.size ()*

	pcl::visualization::PCLVisualizer viewer("PCL Viewer");
	viewer.setBackgroundColor(0.0, 0.0, 0.5);
	viewer.addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud, cloud_normals);

	while (!viewer.wasStopped())
	{
		viewer.spinOnce();
	}

	//boost::shared_ptr<pcl::PointCloud<pcl::Normal>> cloud_normals_shared = boost::make_shared<pcl::PointCloud<pcl::Normal>>(*cloud_normals);

	//showCloud(cloud);
	//showNormals(cloud, cloud_normals_shared);
}

int main(int argc, char** argv)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("leg1.pcd", *cloud) == -1) //* load the file
	{
		PCL_ERROR("Couldn't read file \n");
		return (-1);
	}

	std::cout << "Loaded "
		
		<< cloud->width << " " << cloud->height
		/*
		<< " data points from cat.pcd with the following fields: "
		<< std::endl;
		for (size_t i = 0; i < cloud->points.size(); ++i)
		std::cout << "    " << cloud->points[i].x
		<< " " << cloud->points[i].y
		<< " " << cloud->points[i].z
		*/
		<< std::endl;

	findNormals(cloud);

	//show cloud
	//showCloud(cloud);

	return (0);
}