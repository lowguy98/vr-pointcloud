#ifndef VR_PTXREADER_H
#define VR_PTXREADER_H
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <pointcloud.h>
#include <memory>

namespace MasterProject {
    class PTXReader {
    public:
        PTXReader();
        std::shared_ptr<Pointcloud> read_and_transform_pointcloud(const std::string& file, int position_scanner, float rotation_scanner, float height_user, float height_room);
        std::shared_ptr<Pointcloud> _pointcloud;

        std::shared_ptr<Pointcloud> read_data(const std::string& file, int position_scanner, float rotation_scanner);


        float angle_x = glm::radians(0.0);
        float angle_y = glm::radians(0.0);
        float angle_z = glm::radians(0.0);

        glm::mat3 rotmat_y = glm::mat3(1);
        glm::mat3 rotmat_x = glm::mat3(1); 
        glm::mat3 rotmat_z = glm::mat3(1);

        int number_of_points = 0; 
        void TranslateToHeightUser(std::shared_ptr<Pointcloud> _pointcloud, float height_user, float height_room);
        float change_level; 
        float translation_level = 0; 

        void checkExtremeValues(glm::vec3 positions);

        float max_x = 0; 
        float min_x = 0;

        float max_y = 0;
        float min_y = 0;

        float max_z = 0;
        float min_z = 0;

        float max_value_x_max = 0;
        float max_value_y_max = 0;
        float max_value_z_max = 0;

        float max_value_x_min = 0;
        float max_value_y_min = 0;
        float max_value_z_min = 0;

        float calculateMax(float min_x, float  max_x, float  min_y, float max_y, float min_z, float max_z);


        glm::mat3 rotationmatrix = glm::mat3(1);
        void calculateBoundaries(std::shared_ptr<Pointcloud> _pointcloud);

        glm::vec3 translation = glm::vec3(0); 

        float calculateFloorlevelTranslation();

        glm::mat3 calculate_rotation_matrix(float angle_x, float angle_y, float angle_z, int position_scanner, float rotation_scanner); 

    };

};
#endif // VR_PTXREADER_H