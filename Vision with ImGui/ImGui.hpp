
#include "vision.hpp"

/// <summary>
/// ImGui Window creation
/// </summary>
class UseImGui {
private:
	VideoForImGui textureCam0;
	VideoForImGui textureCam1;

	static const char* imageFilePath; // Declaration of static member variable (logo)

	VideoForImGui image;
	PictDim last_frame_dimensions;
	PictDim last_image_dimensions;

	facedetection facedetectionCam0;
	Point2D lastPos1, lastPos2;

public:

	CameraClass cam_access0;
	CameraClass cam_access1;

	/// <summary>
	/// Constructor
	/// </summary>
	UseImGui() {
		// webcam
		cam_access0.init(cam0);
		textureCam0.initVideo(cam_access0.frame);
		// Picture
		image.loadImage(imageFilePath);
		//Motion track
		lastPos1 = Point2D(0, 0);
		lastPos2 = Point2D(0, 0);
	}

	/// <summary>
	/// Init GLF Window
	/// </summary>
	/// <param name="window"></param>
	/// <param name="glsl_version"></param>
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

	/// <summary>
	/// ImGUI New Frame
	/// </summary>
	void NewFrame() {
		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	};

	/// <summary>
	/// ImGui Update
	/// </summary>
	void Update() {
		// Webcam frames
		cam_access0.readFrame();

		ShowFaceDetection();
		ShowVideo();
		ShowPicture();
	}

	/// <summary>
	/// ImGui Render
	/// </summary>
	void Render() {
		// Render dear imgui into screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	/// <summary>
	/// ImGui shutdown
	/// </summary>
	void Shutdown() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	/// <summary>
	/// Update Show video
	/// </summary>
	void ShowVideo() {
		// Show video cam0
		ImGui::Begin("cam0");
		ImGui::Checkbox("Horizontal flip", &cam_access0.horizontalflip);
		ImGui::SameLine();
		ImGui::Text("    FPS: %.2f    ", ImGui::GetIO().Framerate); // Framerate
		ImGui::SameLine();
		ImGui::Text("pointer = %p", &textureCam0.imageTexture);
		ImGui::SameLine();
		ImGui::Text("size = %d x %d", frameWidth, frameHeight);
		ImGui::Image((void*)(intptr_t)textureCam0.imageTexture, ImVec2(frameWidth, frameHeight));
		ImGui::End();
	};

	/// <summary>
	/// Update Show picture
	/// </summary>
	void ShowPicture() {
		// Show picture ()
		ImGui::Begin("Picture");
		textureCam0.BindCVMat2GLTexture(cam_access0.frame);
		ImVec2 window_Size_picture = ImGui::GetWindowSize() - ImGui::GetWindowContentRegionMin();
		if (window_Size_picture.x != last_image_dimensions.x || window_Size_picture.y != last_image_dimensions.y) {
			image.image_width = int(window_Size_picture.x);
			image.image_height = int(window_Size_picture.y);
			image.loadImage(imageFilePath);
			last_image_dimensions.x = int(window_Size_picture.x);
			last_image_dimensions.y = int(window_Size_picture.y);
		}

		ImGui::Text("pointer = %p", &image.imageTexture);
		ImGui::Text("size = %d x %d",(int)window_Size_picture.x,(int)window_Size_picture.y);
		ImGui::Image((void*)(intptr_t)image.imageTexture, ImVec2(window_Size_picture.x, window_Size_picture.y));
		ImGui::End();
	}

	/// <summary>
	/// Update Show face detection
	/// </summary>
	void ShowFaceDetection() {
		// Vision
		facedetectionCam0.detectAndDraw(cam_access0.frame);
		// Actual vision results
		static float x[n_faces];
		static float y[n_faces];
		static ScrollingBuffer faceTrace1(n_points);
		static ScrollingBuffer faceTrace2(n_points);
		// get actuall face positions from detection
		for (int ii = 0; ii < facedetectionCam0.found_faces.size(); ii++)
		{
			x[ii] = (float)facedetectionCam0.found_faces[ii].x;
			y[ii] = -(float)facedetectionCam0.found_faces[ii].y;
		}
		// store actual face positions to buffer
		if (facedetectionCam0.n_ever_found_faces > 0) 
			faceTrace1.AddPoint((float)facedetectionCam0.found_faces[0].x, -(float)facedetectionCam0.found_faces[0].y);
		if (facedetectionCam0.n_ever_found_faces > 1) 
		{
			faceTrace1.AddPoint((float)facedetectionCam0.found_faces[0].x, -(float)facedetectionCam0.found_faces[0].y);
			faceTrace2.AddPoint((float)facedetectionCam0.found_faces[1].x, -(float)facedetectionCam0.found_faces[1].y);
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Show actually detected facedetection in Plot
		ImGui::Begin("Facedetection");
		if (ImGui::Button("Remove a Face")) {
			if (facedetectionCam0.n_ever_found_faces > 0) {
				facedetectionCam0.n_ever_found_faces--;
			}
		}
		if (ImPlot::BeginPlot("Actuall face position")) {
			ImPlot::SetupAxesLimits(0, double(frameWidth), 0, -double(frameHeight));
			if (facedetectionCam0.n_ever_found_faces > 0) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 20);
				ImPlot::PlotScatter("Face 1", &x[0], &y[0], 1, ImPlotLineFlags_Segments);
			}
			if (facedetectionCam0.n_ever_found_faces > 1) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 20);
				ImPlot::PlotScatter("Face 2", &x[1], &y[1], 1, ImPlotLineFlags_Segments);
			}
			ImPlot::EndPlot();
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Show face Motion trace in Plot
		if (ImPlot::BeginPlot("Face tracking")) {
			ImPlot::SetupAxesLimits(0, double(frameWidth), 0, -double(frameHeight));
			if (facedetectionCam0.n_ever_found_faces > 0) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 6);
				ImPlot::PlotScatter("Facetrace 1", &faceTrace1.Data[0].x, &faceTrace1.Data[0].y, n_points, 0, faceTrace1.Offset, 2 * sizeof(float));
			}
			if (facedetectionCam0.n_ever_found_faces > 1) {
				ImPlot::SetNextMarkerStyle(ImPlotMarker_Cross, 6);
				ImPlot::PlotScatter("Facetrace 2", &faceTrace2.Data[0].x, &faceTrace2.Data[0].y, n_points, 0, faceTrace2.Offset, 2 * sizeof(float));
			}
			ImPlot::EndPlot();
		}
		ImGui::End();
	};
};

const char* UseImGui::imageFilePath = "Picts/wagnius.png";

