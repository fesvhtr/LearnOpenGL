
//
// Created by fesvh on 2022/1/18.
//

#ifndef CGCHUYAN_TEXTURE_H
#define CGCHUYAN_TEXTURE_H

class TEXTURE{
private:
public:
    unsigned int ID;
    const char* file_path;
    int width, height, nrChannels;
    TEXTURE(const char* f1):file_path(f1){
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(f1, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    void textureBind(){
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};
#endif //CGCHUYAN_TEXTURE_H
