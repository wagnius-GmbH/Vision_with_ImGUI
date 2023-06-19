
#include "vision.hpp"

/// <summary>
/// struct to save int dimensions (pixels)
/// </summary>
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

/// <summary>
/// structur to save float points
/// </summary>
struct Point2D {
	float x, y;

	constexpr Point2D() : x(0), y(0) { }
	constexpr Point2D(float _x, float _y) : x(_x), y(_y) { }

	constexpr size_t size() const {
		return 2;
	}

	int& operator[] (size_t idx) {
		assert(idx == 0 || idx == 1);
		return reinterpret_cast<int*>(this)[idx];
	}

	int operator[] (size_t idx) const {
		assert(idx == 0 || idx == 1);
		return reinterpret_cast<const int*>(this)[idx];
	}
};


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

	int cnt1 = 0;
	int cnt2 = 0;

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
		
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Actual vision results
		static float x[n_faces];
		static float y[n_faces];

		// get actuall face positions from detection and store in buffer vector
		for (int ii = 0; ii < facedetectionCam0.found_faces.size(); ii++)
		{
			x[ii] =  (float)facedetectionCam0.found_faces[ii].x;
			y[ii] = -(float)facedetectionCam0.found_faces[ii].y;
		}
		
		// Show actually detected facedetection in Plot
		ImGui::Begin("Facedetection");
		static int size = n_points/2;
		ImGui::SliderInt("Size", &size,0,n_points);

		if (ImPlot::BeginPlot("Actuall face Position")) {
			ImPlot::SetupAxesLimits(0, double(frameWidth), 0, -double(frameHeight));
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
			ImPlot::PlotScatter("Face 1", &x[0], &y[0], n_points, ImPlotLineFlags_Segments);
			ImPlot::PlotScatter("Face 2", &x[1], &y[1], n_points, ImPlotLineFlags_Segments);
			ImPlot::EndPlot();
		}


		///////////////////////////////////////////////////////////////////////////////////////////////
		// Show face Motion trace in Plot
		static float xs1[n_points], ys1[n_points];
		static float xs2[n_points], ys2[n_points];

		// push to diagram 
		if (facedetectionCam0.found_faces.size() == 1) 
		{
			xs1[cnt1] = (float)facedetectionCam0.found_faces[0].x;
			ys1[cnt1] = -(float)facedetectionCam0.found_faces[0].y;
			lastPos1.x = xs1[cnt1];
			lastPos1.y = ys1[cnt1];
			cnt1++;
		}
		else if (facedetectionCam0.found_faces.size() == 2)
		{
			xs1[cnt1] = (float)facedetectionCam0.found_faces[0].x;
			ys1[cnt1] = -(float)facedetectionCam0.found_faces[0].y;
			xs2[cnt2] = (float)facedetectionCam0.found_faces[1].x;
			ys2[cnt2] = -(float)facedetectionCam0.found_faces[1].y;
			lastPos1.x = xs1[cnt1];
			lastPos1.y = ys1[cnt1];
			lastPos2.x = xs2[cnt2];
			lastPos2.y = ys2[cnt2];
			cnt1++;
			cnt2++;
		}	
		
		if (cnt1 > 20) 
			cnt1 = 0;
		
		if (cnt2 > 20)
			cnt2 = 0;

		for (int ii = 0; ii < 20; ii++)
		{
			cout << "xs1: " << xs1[ii] << " ys1: " << ys1[ii] << "xs2: " << xs2[ii] << " ys2: " << ys2[ii] << endl;
		}

		if (ImPlot::BeginPlot("Show motion trace")) {
			ImPlot::SetupAxesLimits(0, double(frameWidth), 0, -double(frameHeight));
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
			ImPlot::PlotScatter("Face 1", xs1, ys1, n_points, ImPlotLineFlags_Segments);
			ImPlot::PlotScatter("Face 2", xs2, ys2, n_points, ImPlotLineFlags_Segments);
			ImPlot::EndPlot();
		}
		ImGui::End();
	};
};

const char* UseImGui::imageFilePath = "Picts/wagnius.png";

