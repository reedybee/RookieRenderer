#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "gfx/shader.h"
#include "gfx/camera.h"
#include "gfx/mesh.h"
#include "gfx/texture.h"
#include "player/player.h"
#include "game/state.h"

#include "gfx/window.h"
 
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "util/util.h"

Player player;
Window window;
State state;

// main execution of the program. returns 0 on success, 1 on fail;
int main(int argc, char* argv[]) {
	create_window();

	InitImGui(window.handle);
	
	player = Player(glm::vec3(0.0f, 10.0f, 0.0f));
	player.camera->type = CAMERA_TYPE_FIRST_PERSON;

	player.camera->lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	Mesh devMesh = Mesh("res/objects/devscene");
	devMesh.shader = Shader("res/shaders/unlit/unlitvertex.glsl", "res/shaders/unlit/unlitfragment.glsl");

	//// main loop, anything that need to be updated while game running should be in here
	while (!glfwWindowShouldClose(window.handle)) {
		glClearColor(0.0f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draws objects to scene
		devMesh.Draw();
		
		player.Update();
		FixedUpdate([]{
				player.FixedUpdate(tickRate);
		});
		// debug window
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Debug Window");

			ImGui::Text("Player: ");
			ImGui::InputFloat3("Position", glm::value_ptr(player.position));
			ImGui::InputFloat3("Rotation", glm::value_ptr(player.camera->front));
			ImGui::InputFloat3("Velocity", glm::value_ptr(player.velocity));
			bool grounded = player.IsGrounded();
			ImGui::Checkbox("Grounded", &grounded);
			bool moving = player.IsMoving();
			ImGui::Checkbox("Moving", &moving);

			if (ImGui::Button("Close Game")) {
				glfwSetWindowShouldClose(window.handle, true);
			}

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		// renders contents and polls events
		glfwPollEvents();
		glfwSwapBuffers(window.handle);
	}
	destroy_window();
	return 0;
}