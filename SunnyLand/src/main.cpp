#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <exception>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

int main()
{
    nlohmann::json j;
    try {
        std::ifstream input_file("assets/config.json");
        j = nlohmann::json::parse(input_file);
        input_file.close();
        spdlog::info("{}", j.dump(4));
    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
        return EXIT_FAILURE;
    }

    nlohmann::json j2 = j.at("input_mappings");
    nlohmann::json j3 = j2.at("pause");
    for(const auto&p:j3){
        std::cout << p << std::endl;
    }

    return 0;
}