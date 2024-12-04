#include "helpers.h"

std::string projectPath = std::filesystem::path(__FILE__).parent_path();

std::ifstream get_input(const std::string& filename) {
    std::ifstream infile(projectPath + "/" + filename);

    return infile;
}
