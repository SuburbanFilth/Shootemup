#define GLEW_STATIC

#include <memory>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Game.hpp"
#include "Loader.hpp"
#include "Entity.hpp"
#include "Systems/Physics/AABB.hpp"
#include "Components/PhysicsComponent.hpp"
#include "Components/RenderingComponent.hpp"
#include "Components/TransformComponent.hpp"


void APIENTRY openglCallbackFunction(GLenum source,
                                           GLenum type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar* message,
                                           const void* userParam)
{
    using namespace std;
    cout << "---------------------opengl-callback-start------------" << endl;
    cout << "message: "<< message << endl;
    cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        cout << "OTHER";
        break;
    }
    cout << endl;
 
    cout << "id: " << id << endl;
    cout << "severity: ";
    switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
        cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        cout << "HIGH";
        break;
    }
    cout << endl;
    cout << "---------------------opengl-callback-end--------------" << endl;
}

Game::Game(int width, int height) :
    width(width), height(height) , 
    physicsSystem(70.f, 5.f)
{
    this->Init();
}

void Game::Init()
{
    this->InitConfiguration();
    std::string filename = "/home/martin/Documents/Projects/Adventures-of-Lowpo/Resources/terrain_patch.dae";
    this->InitScene(filename, this->entities);
}

void Game::InitConfiguration()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Create window
    this->window = glfwCreateWindow(this->width, this->height, "Adventures of Lowpo", nullptr, nullptr);
    glfwMakeContextCurrent(this->window);
    // Disable mouse within window
    

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
    glewInit();
    glGetError();


    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
}

void Game::InitScene(std::string filename, std::vector<std::shared_ptr<Entity>>& entities)
{
    tinyxml2::XMLDocument document;
    // TODO : check for the extension and report error if different from .dae
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    if (error != 0)
        return;
    // the document represents the "whole" file so we need to qu
    // is always Collada
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    
    // parse visual scenes for the world transforms.
    std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>> instanceGeometries;
    tinyxml2::XMLElement* libraryVisualScenes = collada->FirstChildElement("library_visual_scenes");
    instanceGeometries = Loader::ParseVisualScenesStatic(libraryVisualScenes);

    // parse geometries
    // one object - > many colliders
    std::unordered_map<std::string, std::vector<std::shared_ptr<Collider>> > objectToColliders;
    std::unordered_map<std::string, std::shared_ptr<Geometry>> geometry;
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    geometry = Loader::ParseGeometry(libraryGeometries);
    // first iteration to get hitboxes
    for (std::unordered_map<std::string, std::shared_ptr<Geometry>>::iterator it = geometry.begin(); it != geometry.end(); it++)
    {
        // Three scenarios
        // is hitbox
        // is not hitbox
        // is terrain
        bool isHitbox = it->first.find("_hitbox") != it->first.npos;
        if (isHitbox)
        {
            int hitboxIndex = it->first.find("_hitbox");
            std::string objectName = it->first.substr(0, hitboxIndex);
            // take thhe vertices and make a collider.
            glm::vec3 center = glm::vec3(0.f,0.f,0.f);
            for (int i = 0; i < it->second->vertices.size(); i += 3)
            {
                float x = it->second->vertices[i];
                float y = it->second->vertices[i + 1];
                float z = it->second->vertices[i + 2];
                glm::vec3 point = glm::vec3(x,y,z);
                center += point;
            }
            int numOfPoints = it->second->vertices.size() / 3;
            center /= numOfPoints;
            float x = fabs(center.x - it->second->vertices[0]);
            float y = fabs(center.y - it->second->vertices[1]);
            float z = fabs(center.z - it->second->vertices[2]);
            glm::vec3 axisRadii = glm::vec3( x, y, z);
            center = instanceGeometries[it->first]->matrix * glm::vec4(center, 1.0f);
            std::shared_ptr<AABB> collider = std::make_shared<AABB>(AABB(center, it->first, axisRadii, ColliderType::BOX, nullptr));
            objectToColliders[objectName].push_back(collider);
        }

        std::vector<float> bufferData;
        glm::mat4 worldTransform;
        // second iteration to create game entities
        for (std::unordered_map<std::string, std::shared_ptr<Geometry>>::iterator it = geometry.begin(); it != geometry.end(); it++)
        {
            bool isHitbox = it->first.find("_hitbox") != it->first.npos;
            if (isHitbox)
                continue;

            std::shared_ptr<Geometry> current = it->second;
            bufferData = Loader::BuildBufferData(current);
            worldTransform = instanceGeometries[it->first]->matrix;
            std::pair<unsigned int, unsigned int> buffers = this->SetupBuffers(bufferData.data(), bufferData.size() * sizeof(float), false);
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(worldTransform, scale, rotation, translation, skew, perspective);
            rotation = glm::conjugate(rotation);
            // RenderingComponent
            // we need to have
            // VAO, VBO and Texture loaded.
            std::shared_ptr<RenderingComponent> renderingComponent = std::make_shared<RenderingComponent>(RenderingComponent())
            // ===================
            // TODO
            // we need to figure out the mass and the type of physics component
            // ===================
            
            // PhysicsComponent
            std::shared_ptr<PhysicsComponent> physicsComponent = std::make_shared<PhysicsComponent>(PhysicsComponent(1.f, translation, rotation, glm::mat3(1.f), DynamicType::Static));
            std::shared_ptr<TransformComponent> transformComponent= std::make_shared<TransformComponent>(TransformComponent(translation, rotation));
            // Entity

            // push_back
        }
        // Push back an entity
    }
}

std::pair<unsigned int,unsigned int> Game::SetupBuffers(float* data, int size, bool animated)
{
    int floatCount = 8;
    if (animated)
    {
        floatCount = 16;
    }
    unsigned int VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatCount * sizeof(float),(void*)(sizeof(float)*3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*6));

    if (animated)
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*8));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*12));
    }
    return std::make_pair(VAO,VBO);
}

void Game::Update(float deltaTime)
{
    // FIXME : this should not be here
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
    
    // System Update
    this->physicsSystem.Update(deltaTime, this->entities);
    this->inputSystem.Update(this->window, this->entities);
    this->animationSystem.Update(deltaTime, this->entities);
    this->renderingSystem.Update(this->entities);
}

void Game::Run()
{
    double previous = glfwGetTime();
    double lag = 0.f;
    float deltaTime = 1.f/60.f;
    while (!glfwWindowShouldClose(window))
    {   
        glfwPollEvents();
        this->Update(deltaTime);
        glfwSwapBuffers(window);
        unsigned int error = glGetError();
        if (error != 0)
            std::cout << error << std::endl;
    }

    glfwTerminate();
}