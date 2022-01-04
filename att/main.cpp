#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <math.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

void ClearOpenGLErrors() {
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::cout << gluErrorString(error) << std::endl;
	}
	
}
std::vector<glm::vec3> vert;
std::vector<unsigned int> indices;
unsigned int EBO;
SDL_Window* mainwindow;
SDL_GLContext maincontext;
float deltatime = 0.0f;
float lastframe = 0.0f;
bool running = true;
glm::vec3 lightPos(-10.0f, 15.0f, 15.0f);
Camera camera(glm::vec3(0.0, 5.0f, 6.0f));

bool Init();

void InputProcess(SDL_Event event);

unsigned int createTerrain();

unsigned int createAxes();

unsigned int createLamp();

int main(int argc, char* args[]){
	std::cout<<"start"<<std::endl;
	Init();
	
	Shader ourShader("shader.vs", "shader.fs");
	Shader our2Shader("shader2.vs", "shader2.fs");

	unsigned int VAO = createTerrain();
	
	unsigned int VAO2 = createAxes();

	unsigned int lampVAO = createLamp();

	GLuint heightsTexture;
	glGenTextures(1, &heightsTexture);
	glBindTexture(GL_TEXTURE_2D, heightsTexture);
	
	// set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
	
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("cores.jpg", &width, &height, &nrChannels, 0);
	
    if (data != NULL){
    	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		GLenum error = glGetError();
		if(error != GL_NO_ERROR) std::cout << gluErrorString(error)<<std::endl;
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
   
	ourShader.use();
	//ourShader.setInt("texture2", 1);
	ourShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	glEnable(GL_DEPTH_TEST);
	

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
	unsigned int projectLoc = glGetUniformLocation(ourShader.ID, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

	
	while(running){
		ClearOpenGLErrors();
		SDL_Event event;
		float currentframe = (float)SDL_GetTicks()/100;
		deltatime = currentframe - lastframe;
		lastframe = currentframe;

		glClearColor(0.2, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		ourShader.use();
		glBindVertexArray(VAO);
		glm::mat4 view;
	
		view = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
	
		glm::mat4 model = glm::mat4(1.0);
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ClearOpenGLErrors();

		ourShader.setVec3("lightPos", lightPos); 

		ourShader.setVec3("viewPos", camera.Position); 

		//glDrawArrays(GL_POINTS, 0, vert.size());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		ClearOpenGLErrors();
		our2Shader.use();

		glBindVertexArray(VAO2);

		projectLoc = glGetUniformLocation(our2Shader.ID, "projection");
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

		viewLoc = glGetUniformLocation(our2Shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model2 = glm::mat4(1.0);
		/*model2 = glm::translate(model2, glm::vec3(0.0f, 3.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(1.0f));*/
		modelLoc = glGetUniformLocation(our2Shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));


		glDrawArrays(GL_LINES, 0, 12);

		our2Shader.use();

		glBindVertexArray(lampVAO);

		projectLoc = glGetUniformLocation(our2Shader.ID, "projection");
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

		viewLoc = glGetUniformLocation(our2Shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		model2 = glm::mat4(1.0);
		model2 = glm::translate(model2, lightPos);
		model2 = glm::scale(model2, glm::vec3(1.0f));
		modelLoc = glGetUniformLocation(our2Shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
	

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		SDL_GL_SwapWindow(mainwindow);

		InputProcess(event);	
	}

	glDeleteVertexArrays(1, &VAO);
	

	SDL_Quit();

	return 0;

}

bool Init(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
	    std::cout << "Failed to init SDL\n";
	    return false;
	}

 
	// Create our window centered at 512x512 resolution
	mainwindow = SDL_CreateWindow(
	    "Incrivel janela", 
	    SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED,
	    800,
	    600,
	    SDL_WINDOW_OPENGL
	);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	maincontext = SDL_GL_CreateContext(mainwindow);
	if(maincontext == NULL){
		std::cout<<"failed to create context"<<std::endl;
		return false;
	}

	glewExperimental = GL_TRUE;

	GLenum status = glewInit();

	if(status != GLEW_OK){
		std::cout<<"failed to init glew"<<std::endl;
		return false;
	}

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	return true;
}

void InputProcess(SDL_Event event){

	const Uint8* keyboardSnapshot = SDL_GetKeyboardState(NULL);

	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT)
			running = false;

		if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;

	}

	if(keyboardSnapshot[SDL_SCANCODE_UP]) camera.ProcessKeyboard(FORWARD, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_DOWN]) camera.ProcessKeyboard(BACKWARD, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_RIGHT]) camera.ProcessKeyboard(RIGHT, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_LEFT]) camera.ProcessKeyboard(LEFT, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_W]) camera.ProcessMouseMovement(0, 10.0f, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_S]) camera.ProcessMouseMovement(0, -10.0f, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_D]) camera.ProcessMouseMovement(10.0f, 0, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_A]) camera.ProcessMouseMovement(-10.0f, 0, deltatime);
	if(keyboardSnapshot[SDL_SCANCODE_L]) lightPos.x += 0.2;
	if(keyboardSnapshot[SDL_SCANCODE_J]) lightPos.x	-= 0.2;
	if(keyboardSnapshot[SDL_SCANCODE_I]) lightPos.y	+= 0.2;
	if(keyboardSnapshot[SDL_SCANCODE_K]) lightPos.y	-= 0.2;

}

unsigned int createTerrain(){
	
	
	float curZ = +15.0;
	for(int i=0; i<150; i++){
		curZ-=0.2;
		float curX = -15.0;
		
		for(int j=0; j<150; j++){
			curX+=0.2f;

			vert.push_back(glm::vec3(-0.2f + curX, 0.5f, -0.2f+curZ));
			vert.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		
	
		}

	}
	
	for(int i=0; i<150*150*2; i+=2){
		if(vert[i].z != vert[i+2].z) continue;
		if((i+2)%150==0) continue;
		indices.push_back(i);
		indices.push_back(i+2);
		indices.push_back(i+(150*2));
		indices.push_back(i+2);
		indices.push_back((i+2)+(150*2));
		indices.push_back(i+(150*2));
		std::cout<<i<<" "<<i+2<<" "<<i+(150*2)<<" "<<i+2<<" "<<(i+2)+(150*2)<<" "<<i+(150*2)<<std::endl;
		//std::cout<<vert[i].x<<" "<<vert[i].z<<"   -----  "<<vert[i+(150*2)].x<<" "<<vert[i+(150*2)].z<<"   -----  "<<vert[i+2].x<<" "<<vert[i+2].z<<std::endl;
		
	}

	for(int i=0; i<indices.size(); i+=3){
		glm::vec3 v1 = vert[indices[i+1]] - vert[indices[i]];
		glm::vec3 v2 = vert[indices[i+2]] - vert[indices[i]];

		glm::vec3 normal = glm::cross(v1, v2);
		vert[indices[i]+1] += normal;
		vert[indices[i+1]+1] += normal;
		vert[indices[i+2]+1] += normal;  
	}




	//------------------------------BUFFERS----------------------------------------//

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); 
	//Position attribute
	

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1); 
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

unsigned int createAxes(){
	float lines[] = {
	 	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 	100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	 	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 	0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	 	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 1.0f 
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;

}

unsigned int createLamp(){
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 

		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;

}