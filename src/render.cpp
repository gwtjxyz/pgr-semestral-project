#include "render.h"

#include <string>

#include "utils.h"


glm::mat4 Render::projection() {
    return glm::perspective(glm::radians(Config::FOV),
                            (float) Config::WINDOW_WIDTH / (float) Config::WINDOW_HEIGHT,
                            Config::ZNEAR,
                            Config::ZFAR);
}

void renderDirectionalLight(const char * varName,
                            glm::vec3 direction,
                            glm::vec3 ambient,
                            glm::vec3 diffuse,
                            glm::vec3 specular) {
    std::string destination = varName;
    setUniform3f((destination + ".direction").c_str(), direction);
    setUniform3f((destination + ".ambient").c_str(), ambient);
    setUniform3f((destination + ".diffuse").c_str(), diffuse);
    setUniform3f((destination + ".specular").c_str(), specular);
}

void renderPointLight(const char * varName,
                      glm::vec3 position,
                      glm::vec3 ambient,
                      glm::vec3 diffuse,
                      glm::vec3 specular,
                      float constant,
                      float linear,
                      float quadratic) {
    std::string destination = varName;
    setUniform3f((destination + ".position").c_str(), position);
    setUniform3f((destination + ".ambient").c_str(), ambient);
    setUniform3f((destination + ".diffuse").c_str(), diffuse);
    setUniform3f((destination + ".specular").c_str(), specular);
    setUniform1f((destination + ".constant").c_str(), constant);
    setUniform1f((destination + ".linear").c_str(), linear);
    setUniform1f((destination + ".quadratic").c_str(), quadratic);
}

void renderSpotlight(const char * varName,
                     glm::vec3 position,
                     glm::vec3 direction,
                     glm::vec3 ambient,
                     glm::vec3 diffuse,
                     glm::vec3 specular,
                     float constant,
                     float linear,
                     float quadratic,
                     float cutoff,
                     float outerCutoff) {
    std::string destination = varName;
    setUniform3f((destination + ".position").c_str(), position);
    setUniform3f((destination + ".direction").c_str(), direction);
    setUniform3f((destination + ".ambient").c_str(), ambient);
    setUniform3f((destination + ".diffuse").c_str(), diffuse);
    setUniform3f((destination + ".specular").c_str(), specular);
    setUniform1f((destination + ".constant").c_str(), constant);
    setUniform1f((destination + ".linear").c_str(), linear);
    setUniform1f((destination + ".quadratic").c_str(), quadratic);
    setUniform1f((destination + ".cutoff").c_str(), cutoff);
    setUniform1f((destination + ".outerCutoff").c_str(), outerCutoff);

}