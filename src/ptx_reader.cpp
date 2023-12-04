#include <ptx_reader.h>
#include <stdio.h>


namespace MasterProject {

	PTXReader::PTXReader() {}


	
	/// <summary>
	///Author: Sarah
	///combine reading functionality with boundary calculation and translation to height of user
	/// </summary>
	std::shared_ptr<Pointcloud> PTXReader::read_and_transform_pointcloud(const std::string& file, int position_scanner, float rotation_scanner, float height_user, float height_room)
	{

		_pointcloud = read_data(file, position_scanner, rotation_scanner); 

		calculateBoundaries(_pointcloud);
		TranslateToHeightUser(_pointcloud, height_user, height_room);

		printf("Boundaries have been calculated and translation of point cloud to align floor level with point of origin is completed \n");


		return _pointcloud;
	}

	/// <summary>
	///Author: Jie, Sarah (transformation, calculation of extreme values) 
	///reads from the PTX-File the positions and colors of the points and store it. 
	/// Transforms the data points to align the view of the VR-headset with the walls of the point cloud data.
	/// </summary>
	std::shared_ptr<Pointcloud> PTXReader::read_data(const std::string& file, int position_scanner, float rotation_scanner) {
		printf("Reading data...\n");

		//opens the file
		FILE* fp = fopen(file.c_str(), "r");

		//checks if file exists
		assert(fp);

		unsigned long n_rows, n_cols;
		glm::vec3 o_trans, trans; //translation vector of origin and vertices
		glm::mat3 o_rot, rot; //rotation matrices of origin and vertices

		//scans lines
		fscanf(fp, "%lu %lu", &n_rows, &n_cols);
		float tmp = 0.0;
		for (int i = 0; i < 3; ++i) {
			fscanf(fp, "%f", &tmp);
			o_trans[i] = tmp;
		}

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				fscanf(fp, "%f", &tmp);
				o_rot[i][j] = tmp;
			}
		}

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				fscanf(fp, "%f", &tmp);
				rot[i][j] = tmp;
			}
			fscanf(fp, "%f", &tmp);
		}

		for (int i = 0; i < 3; ++i) {
			fscanf(fp, "%f", &tmp);
			trans[i] = tmp;
		}
		fscanf(fp, "%f", &tmp);

		//creates a pointcloud
		std::shared_ptr<Pointcloud> cloud = std::make_shared<Pointcloud>(n_rows * n_cols);

		//gets vec where the positions and colors are stored
		std::vector<glm::vec3>& positions = cloud->getPositions();
		std::vector<glm::vec3>& colors = cloud->getColors();
		float x, y, z, intensity;
		unsigned int r, g, b;
		char line[128];

		//define the angles
		float angle_y = glm::radians((float)rotation_scanner);
		//float angle_y = glm::radians(-17.0);
		float angle_x = glm::radians(90.0);
		float angle_z = glm::radians(5.0);

		//create rotation matrix
		rotmat_y = glm::mat3(cos(angle_y), 0, sin(angle_y), 0, 1, 0, -sin(angle_y), 0, cos(angle_y));
		rotmat_x = glm::mat3(1, 0, 0, 0, cos(angle_x), -sin(angle_x), 0, sin(angle_x), cos(angle_x));
		rotmat_z = glm::mat3(cos(angle_z), -sin(angle_z), 0, sin(angle_z), cos(angle_z), 0, 0, 0, 1);

		//store x,y,z into vec3 & positions and rgb into colors.
		glm::vec3 p;

		//number of points in point cloud
		number_of_points = n_rows * n_cols; 

		printf("reading %d points\n\n", number_of_points); 

		//store point coordinate in positions & check extreme values
		for (int i = 0; i < n_rows * n_cols; ++i) {
			fscanf(fp, "%f %f %f %f %d %d %d", &x, &y, &z, &intensity, &r, &g, &b);
			
			p = glm::vec3(x, y, z) * rot + trans;
			//positions[i] = glm::vec3(p.x, p.z, p.y);
			
			positions[i] = ((rotmat_x) * p);
			positions[i] = glm::vec3(positions[i].x, positions[i].y, positions[i].z) * rotmat_y;
			positions[i] = glm::vec3(positions[i].x, positions[i].y, positions[i].z) * rotmat_z;
			
			if (i == (number_of_points) / 4)
			{
				printf("25 percent of data read and transformed\n");

			}
			if (i == (number_of_points) / 2)
			{
				printf("50 percent of data read and transformed\n");

			}
			if (i == (3*(number_of_points)) / 4)
			{
				printf("75 percent of data read and transformed\n");

			}
			checkExtremeValues(positions[i]);
			
			colors[i] = (glm::vec3(((float)r / 255), ((float)g / 255), ((float)b / 255)));
			//if (positions[i].z > 0)
				//colors[i] = glm::vec3(1.f, 0.f, 0.f);
			
		}
		printf("the extreme values for x_axis are: %f, %f\n", min_x, max_x);
		printf("the extreme values for y_axis are: %f, %f\n", min_y, max_y);
		printf("the extreme values for z_axis are: %f, %f\n", min_z, max_z);
		printf("\n"); 
		/*
		for (auto&& position : positions) {
			position.y -= min_y;
		}
		*/
		printf("Successfully read data, stored the extreme data points and rotated the data according to position of scanner\n");
		
		return cloud;
	};
	
	/// <summary>
	///Author: Sarah
	///check & update extreme values of each axis
	/// </summary>
	void PTXReader::checkExtremeValues(glm::vec3 positions)
	{
		if (positions.x > max_x)
		{
			max_x = positions.x;
		};
		if (positions.x < min_x)
		{
			min_x = positions.x;
		}

		if (positions.y > max_y)
		{
			max_y = positions.y;
		};
		if (positions.y < min_y)
		{
			min_y = positions.y;
		}

		if (positions.z > max_z)
		{
			max_z = positions.z;
		};
		if (positions.z < min_z)
		{
			min_z = positions.z;
		}

		return;
	}


	/// <summary>
	///Author: Sarah
	///translate to the user height with respect to the given height of the room
	/// </summary>
	void PTXReader::TranslateToHeightUser(std::shared_ptr<Pointcloud> _cloud, float height_room, float height_user)
	{

		for (int i = 0; i < number_of_points; ++i) {

			_cloud->positions[i].y = _cloud->positions[i].y - max_value_y_min; //- ((max_value_y_max - max_value_y_min)*(height_user/height_room));
		}
		printf("aligning floor level of point cloud with point of origin completed\n");
		return; 
	}

	
	/// <summary>
	///Author: Sarah
	///helper function calculate the max value
	/// </summary>
	float PTXReader::calculateMax(float min_x, float max_x, float min_y, float max_y, float  min_z, float  max_z)
	{
		float max_value = 0; 

		if (min_x >= max_x && min_x >= min_y && min_x >= max_y && min_x >= min_z && min_x >= max_z) return min_x;
		else if (max_x >= min_x && max_x >= min_y && max_x >= max_y && max_x >= min_z && max_x >= max_z) return max_x;

		else if (min_y >= min_x && min_y >= max_x && min_y >= max_y && min_y >= min_z && min_y >= max_z) return min_y;
		else if (max_y >= min_x && max_y >= max_x && max_y >= min_y && max_y >= min_z && max_y >= max_z) return max_y;

		else if (min_z >= min_x && min_z >= max_x && min_z >= min_y && min_z >= max_y && min_z >= max_z) return min_z;
		else return max_z;
	}

	/// <summary>
	///Author: Sarah
	///calculate the boundaries
	/// </summary>
	void PTXReader::calculateBoundaries(std::shared_ptr<Pointcloud> _cloud)
	{
		printf("Start calculating boundaries \n");

		float step_size = 0.2;

		float counter_x_max = 0;

		float counter_x_min = 0;

		float counter_x_max_before = -1;

		float counter_x_min_before = -1;

		float counter_y_max = 0;
		float counter_y_min = 0;
		float counter_y_max_before = 0;
		float counter_y_min_before = 0;

		float counter_z_max = 0;
		float counter_z_min = 0;
		float counter_z_max_before = -1;
		float counter_z_min_before = -1;

		float value = 0 + step_size;
		float value_before;

		float abs_max = calculateMax(abs(min_x), abs(max_x), abs(min_y), abs(max_y), abs(min_z), abs(max_z));
		float x, y, z;

		int break_x_max = 0;
		int break_y_max = 0;
		int break_z_max = 0;

		int break_x_min = 0;
		int break_y_min = 0;
		int break_z_min = 0;

		float max_counter_x_max = 0.0;
		float max_counter_x_min = 0.0;

		float max_counter_y_max = 0.0;
		float max_counter_y_min = 0.0;

		float max_counter_z_max = 0.0;
		float max_counter_z_min = 0.0;

		float update_x_min = 0;
		float update_x_max = 0;

		float update_y_min = 0;
		float update_y_max = 0;

		float update_z_min = 0;
		float update_z_max = 0;

		float update_value = 10;

		//printf("%f absolute max \n", abs_max);
		while (value <= abs_max)
		{
			//counts points within given range for all the axis
			value_before = value - step_size;
			for (int i = 0; i < number_of_points; ++i)
			{
				x = _cloud->positions[i].x;
				//printf("%f \n", x); 

				y = _cloud->positions[i].y;
				z = _cloud->positions[i].z;

				if (x > value_before && x <= value)
				{
					counter_x_max = counter_x_max + 1;
				}
				if (x >= -value && x <= -value_before)
				{
					counter_x_min = counter_x_min + 1;
				}

				if (y > value_before && y <= value)
				{
					counter_y_max = counter_y_max + 1;
				}
				if (y >= -value && y <= -value_before)
				{
					counter_y_min = counter_y_min + 1;
				}

				if (z > value_before && z <= value)
				{
					counter_z_max = counter_z_max + 1;
				}
				if (z >= -value && z <= -value_before)
				{
					counter_z_min = counter_z_min + 1;
				}

			}

			//looks if current the change of current point count to previous point count is max in both x directions
			if (value > step_size && break_x_max < 3 && update_x_max < update_value)
			{
				if (counter_x_max != 0.0 && counter_x_max_before != 0.0)
				{
					if ((counter_x_max / counter_x_max_before) > max_counter_x_max)
					{

						max_counter_x_max = (counter_x_max / counter_x_max_before);
						max_value_x_max = value;
						update_x_max = 0;

					}
					else
					{
						update_x_max = update_x_max + 1;
					}
				}

				if (counter_x_max == 0.0 && counter_x_max_before == 0.0)
				{
					break_x_max = break_x_max + 1;

				}

			}

			counter_x_max_before = counter_x_max;
			counter_x_max = 0.0;

			if (value > step_size && break_x_min < 3 && update_x_min < update_value)
			{


				if (counter_x_min != 0.0 && counter_x_min_before != 0.0)
				{
					if ((counter_x_min / counter_x_min_before) > max_counter_x_min)
					{
						max_counter_x_min = (counter_x_min / counter_x_min_before);
						max_value_x_min = -value;

						update_x_min = 0;
						//printf("updated %f \n \n", max_value_x_min);
					}
					else {
						update_x_min = update_x_min + 1;
					}
				}

				if (counter_x_min == 0.0 && counter_x_min_before == 0.0)
				{
					break_x_min = break_x_min + 1;

				}

			}

			counter_x_min_before = counter_x_min;
			counter_x_min = 0.0;

			//looks if current the change of current point count to previous point count is max in both y directions
			if (value > step_size && break_y_max < 3 && update_y_max < update_value)
			{
				if (counter_y_max != 0.0 && counter_y_max_before != 0.0)
				{
					if ((counter_y_max / counter_y_max_before) > max_counter_y_max)
					{
						max_counter_y_max = (counter_y_max / counter_y_max_before);
						max_value_y_max = value;
						update_y_max = 0;
						printf("updated y %f \n", max_value_y_max);
					}
					else
					{
						update_y_max = update_y_max + 1;
					}
				}

				if (counter_y_max == 0.0 && counter_y_max_before == 0.0)
				{
					break_y_max = break_y_max + 1;

				}

			}

			counter_y_max_before = counter_y_max;
			counter_y_max = 0.0;

			if (value > step_size && break_y_min < 3 && update_y_min < update_value)
			{

				if (counter_y_min != 0 && counter_y_min_before != 0)
				{
					if ((counter_y_min / counter_y_min_before) > max_counter_y_min)
					{
						max_counter_y_min = counter_y_min / counter_y_min_before;
						max_value_y_min = -value;
						update_y_min = 0;
					}
					else
					{
						update_y_min = update_y_min + 1;
					}
				}

				if (counter_y_min == 0.0 && counter_y_min_before == 0.0)
				{
					break_y_min = break_y_min + 1;

				}

			}

			counter_y_min_before = counter_y_min;
			counter_y_min = 0.0;

			//looks if current the change of current point count to previous point count is max in both z directions
			if (value > step_size && break_z_max < 3 && update_z_max < update_value)
			{
				if (counter_z_max != 0 && counter_z_max_before != 0)
				{
					if ((counter_z_max / counter_z_max_before) > max_counter_z_max)
					{
						max_counter_z_max = (counter_z_max / counter_z_max_before);
						max_value_z_max = value;
						update_z_max = 0;
					}
					else
					{
						update_z_max = update_z_max + 1;
					}
				}

				if (counter_z_max == 0.0 && counter_z_max_before == 0.0)
				{
					break_z_max = break_z_max + 1;

				}

			}

			counter_z_max_before = counter_z_max;
			counter_z_max = 0.0;


			if (value > step_size && break_z_min < 3 && update_z_min < update_value)
			{


				if (counter_z_min != 0.0 && counter_z_min_before != 0.0)
				{
					if ((counter_z_min / counter_z_min_before) > max_counter_z_min)
					{
						max_counter_z_min = (counter_z_min / counter_z_min_before);
						max_value_z_min = -value;
						update_z_min = 0;
					}
					else
					{
						update_z_min = update_z_min + 1;
					}
				}

				if (counter_z_min == 0.0 && counter_z_min_before == 0.0)
				{
					break_z_min = break_z_min + 1;

				}

			}
			counter_z_min_before = counter_z_min;
			counter_z_min = 0.0;

			//checks early return
			if (break_z_min >= 3 && /*break_z_max >= 3 &&*/ break_y_min >= 3 && break_y_max >= 3 /* && break_x_min >= 3 */ && break_x_max >= 3)
			{
				printf("Boundaries of the point cloud are calculated\n");
				printf("Boundaries of x_axis: %f, %f\n", max_value_x_min, max_value_x_max);
				printf("Boundaries of y_axis: %f, %f\n", max_value_y_min, max_value_y_max);
				printf("Boundaries of z_axis: %f, %f\n", max_value_z_min, max_value_z_max);

				return;
			}





			value = value + step_size;

		}



		printf("Boundaries of the point cloud are calculated\n");
		printf("Boundaries of x_axis: %f, %f\n", max_value_x_min, max_value_x_max);
		printf("Boundaries of y_axis: %f, %f\n", max_value_y_min, max_value_y_max);
		printf("Boundaries of z_axis: %f, %f\n", max_value_z_min, max_value_z_max);

		return;
	}




};
