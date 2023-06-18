#include "header.h"
#include "vision.hpp"


struct PictDim {
	int  x, y;
	constexpr PictDim() : x(0), y(0) { }
	constexpr PictDim(int _x, int _y) : x(_x), y(_y) { }
	int& operator[] (size_t idx) { IM_ASSERT(idx == 0 || idx == 1); return ((int*)(void*)(char*)this)[idx]; } // We very rarely use this [] operator, so the assert overhead is fine.
	int  operator[] (size_t idx) const { IM_ASSERT(idx == 0 || idx == 1); return ((const int*)(const void*)(const char*)this)[idx]; }
#ifdef IM_VEC2_CLASS_EXTRA
	IM_VEC2_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imconfig.h to convert back and forth between your math types and ImVec2.
#endif
};

struct Point2D {
	float  x, y;
	constexpr Point2D() : x(0), y(0) { }
	constexpr Point2D(float _x, float _y) : x(_x), y(_y) { }
	int& operator[] (size_t idx) { IM_ASSERT(idx == 0 || idx == 1); return ((int*)(void*)(char*)this)[idx]; } // We very rarely use this [] operator, so the assert overhead is fine.
	int  operator[] (size_t idx) const { IM_ASSERT(idx == 0 || idx == 1); return ((const int*)(const void*)(const char*)this)[idx]; }
};

/// <summary>
/// ImGui Window creation
/// </summary>
class UseImGui {
private:

	VideoForImGui textureCam0;
	VideoForImGui textureCam1;

	static const char* imageFilePath; // Declaration of static member variable
	
	VideoForImGui image;
	PictDim last_frame_dimensions;
	PictDim last_image_dimensions;

	facedetection facedetectionCam0;
	Point2D lastPos;

public:

	CameraClass cam_access0;
	CameraClass cam_access1;

	ImVector<ImPlotPoint> facePos;

	UseImGui() {
		// webcam
		cam_access0.init(cam0);
		textureCam0.initVideo(cam_access0.frame);
		// Picture
		image.loadImage(imageFilePath);
		//Motion track
		lastPos = Point2D(0, 0);

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
		// Vision
		facedetectionCam0.detectAndDraw(cam_access0.frame);

		// get face positions from detection
		for (int ii = 0; ii < facedetectionCam0.found_faces.size(); ii++)
		{
			facePos.push_back(ImPlotPoint(float(facedetectionCam0.found_faces[ii].x), -float(facedetectionCam0.found_faces[ii].y)));
		}
		// delete if the buffer full
		if (facePos.size() > 20) {
			facePos.erase(facePos.begin());
		}

		// Show actually detected facedetection in Plot
		ImGui::Begin("Facedetection");
		static float size = 0.67f;
		ImGui::SliderFloat("Size", &size, 0, 1);
		// trace points to plot
		static float x[n_points];
		static float y[n_points];
		
		if (ImPlot::BeginPlot("Detection Results")) {
		ImPlot::SetupAxesLimits(0, double(frameWidth),0, -double(frameHeight));
		for (int ii = 0; ii < facePos.size(); ii++)
			{
				x[ii] = facePos[ii].x;
				y[ii] = facePos[ii].y;
			}
			ImPlot::PlotScatter("Face", &x[n_points - 1], &y[n_points - 1], 1);
			ImPlot::EndPlot();
			ImGui::End();
		}

		if (ImPlot::BeginPlot("Show motion trace")) {
			ImPlot::SetupAxesLimits(0, double(frameWidth), 0, -double(frameHeight));
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
			// Show face Motion trace in Plot
			static float xs[n_points], ys[n_points];
			// Euklidian Distance
			std::vector<float>  euklidianDistance;
			if (facePos.size() > 1) {
				for (int ii = 0; ii < (facePos.size() - 1); ii++)
				{
					euklidianDistance.push_back(sqrt(pow(float(facePos[ii].x) - float(facePos[ii + 1].x), 2) + pow(float(facePos[ii].y) - float(facePos[ii + 1].y), 2)));

					if (euklidianDistance[ii] < 50) {
						xs[ii] = float(facePos[ii].x);
						ys[ii] = float(facePos[ii].y);
						lastPos.x = xs[ii];
						lastPos.y = ys[ii];
					}
					else
					{
						xs[ii] = lastPos.x;
						ys[ii] = lastPos.y;
					}
				}
			}
			xs[19] = lastPos.x;
			ys[19] = lastPos.y;
			ImPlot::PlotScatter("Face 1", xs, ys, n_points, ImPlotLineFlags_Segments);
			ImPlot::EndPlot();
		}

		// Show video cam0
		ImGui::Begin("cam0");
		ImGui::Checkbox("Horizontal flip", &cam_access0.horizontalflip);
		ImGui::SameLine();
		ImGui::Text("    FPS: %.2f    ", ImGui::GetIO().Framerate); // Framerate
		ImGui::SameLine();
		ImGui::Text("pointer = %p", textureCam0.imageTexture);
		ImGui::SameLine();
		ImGui::Text("size = %d x %d", frameWidth, frameHeight);
		ImGui::Image((void*)(intptr_t)textureCam0.imageTexture, ImVec2(frameWidth, frameHeight));
		ImGui::End();

		// Show picture ()
		ImGui::Begin("Picture");
		textureCam0.BindCVMat2GLTexture(cam_access0.frame);
		ImVec2 window_Size_picture = ImGui::GetWindowSize() - ImGui::GetWindowContentRegionMin();
		if (window_Size_picture.x != last_image_dimensions.x || window_Size_picture.y != last_image_dimensions.y) {
			image.image_width  = int(window_Size_picture.x);
			image.image_height = int(window_Size_picture.y);
			image.loadImage(imageFilePath);
			last_image_dimensions.x = int(window_Size_picture.x);
			last_image_dimensions.y = int(window_Size_picture.y);
		}

		ImGui::Text("pointer = %p", image.imageTexture);
		ImGui::Text("size = %d x %d", window_Size_picture.x, window_Size_picture.y);
		ImGui::Image((void*)(intptr_t)image.imageTexture, ImVec2(window_Size_picture.x, window_Size_picture.y));
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

const char* UseImGui::imageFilePath = "G:/Meine Ablage/001_wagnius GmbH/100_Marketing/home page data/Wagnius EN.jpg";