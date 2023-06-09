#include "header.h"
#include "vision.hpp"

/// <summary>
/// ImGui Window creation
/// </summary>
class UseImGui {
private:
	CameraClass cam_access0;
	CameraClass cam_access1;
	VideoForImGui textureCam0;
	VideoForImGui textureCam1;
	VideoForImGui image;



public:
	UseImGui() {
		cam_access0.init(cam0);
		textureCam0.initVideo(cam_access0.frame);
		
		// Picture
		image.loadImage("G:/Meine Ablage/001_wagnius GmbH/100_Marketing/home page data/Wagnius EN.jpg");
	}

	void Init(GLFWwindow* window, const char* glsl_version) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();

		// Include controls
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsDark();

	}

	void NewFrame() {
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	};
	void Update() {

		// Webcam frames
		cam_access0.readFrame();
		textureCam0.BindCVMat2GLTexture(cam_access0.frame);
		// Show video cam0
		ImGui::Begin("cam0");
		ImGui::Text("pointer = %p", textureCam0.imageTexture);
		ImGui::Text("size = %d x %d", frameWidth, frameHeight);
		ImGui::Image((void*)(intptr_t)textureCam0.imageTexture, ImVec2(frameWidth, frameHeight));
		ImGui::End();

		// Show video cam0
		ImGui::Begin("Picture");
		ImGui::Text("pointer = %p", image.imageTexture);
		ImGui::Text("size = %d x %d", frameWidth, frameHeight);
		ImGui::Image((void*)(intptr_t)image.imageTexture, ImVec2(frameWidth, frameHeight));
		ImGui::End();
	
	}
	void Render() {
		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Shutdown() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}
};
