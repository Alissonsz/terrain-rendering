#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* tessControlPath, const GLchar* tessEvalPath, const GLchar* geometryPath, const GLchar* fragmentPath){

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string tessControlCode;
    std::string tessEvalCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream tessControlFile;
    std::ifstream tessEvalFile;
    std::ifstream fShaderFile;
    std::ifstream geometryFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    tessControlFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    tessEvalFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    geometryFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath);
        tessControlFile.open(tessControlPath);
        tessEvalFile.open(tessEvalPath);
        geometryFile.open(geometryPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, tcShaderStream, teShaderStream, geometryStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        tcShaderStream << tessControlFile.rdbuf();
        teShaderStream << tessEvalFile.rdbuf();
        geometryStream << geometryFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        tessControlFile.close();
        tessEvalFile.close();
        geometryFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        tessControlCode = tcShaderStream.str();
        tessEvalCode = teShaderStream.str();
        geometryCode = geometryStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* tessControlShaderCode = tessControlCode.c_str();
    const char* tessEvalShaderCode = tessEvalCode.c_str();
    const char* geometryShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, tessControl, tessEval, geometry, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // vertex Shader
    tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessControl, 1, &tessControlShaderCode, NULL);
    glCompileShader(tessControl);
    // print compile errors if any
    glGetShaderiv(tessControl, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(tessControl, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::TESSCONTROL::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // vertex Shader
    tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessEval, 1, &tessEvalShaderCode, NULL);
    glCompileShader(tessEval);
    // print compile errors if any
    glGetShaderiv(tessEval, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(tessEval, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::TESSEVAL::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &geometryShaderCode, NULL);
    glCompileShader(geometry);
    // print compile errors if any
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, tessControl);
    glAttachShader(ID, tessEval);
    glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(tessControl);
    glDeleteShader(tessEval);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
}



Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath){

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Shader::use() {
    glUseProgram(ID);
    char infoLog[512];
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    if (!success) {
        std::cout << infoLog << std::endl;
    }
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}


