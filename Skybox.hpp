//
//  Skybox.hpp
//  waterfall
//
//  Created by simon on 2018/12/6.
//  Copyright © 2018 simon. All rights reserved.
//

#ifndef Skybox_h
#define Skybox_h

#include "tools/texture.hpp"

float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
    "material/right.jpg",
    "material/left.jpg",
    "material/top.jpg",
    "material/bottom.jpg",
    "material/front.jpg",
    "material/back.jpg"
};

class Skybox
{
private:
    GLuint skyboxVertexArray;
    GLuint skybox_vertex_buffer;
    GLuint cubemapTexture;
    Shader *shader;

public:
    Skybox(float sceneScale, glm::vec3 sceneCenter, Shader *skyboxShader)
    {
        shader = skyboxShader;
        // skybox VAO
        glGenVertexArrays(1, &skyboxVertexArray);
        glBindVertexArray(skyboxVertexArray);
        
        for(unsigned int i = 0;i < 36*3; i++)
        {
            skyboxVertices[i] *= sceneScale;
            skyboxVertices[i] += sceneCenter[i%3];
        }
        
        glGenBuffers(1, &skybox_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        
        
        cubemapTexture = loadCubemap(faces);
    }
    
    void draw(glm::mat4 ViewProjectionMatrix)
    {
        
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shader->use();
        shader->setMat4("VP", ViewProjectionMatrix);
        // skybox cube
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        shader->setInt("skybox", 0);
        
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDisableVertexAttribArray(0);
        
        glDepthFunc(GL_LESS); // set depth function back to default
    }
};
#endif /* Skybox_h */
