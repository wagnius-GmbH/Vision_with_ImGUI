#include "Header.h"

/// <summary>
/// Webcam handling
/// </summary>
class CameraClass {
private:
	// Private member variables
	cv::VideoCapture vid_capture;

public:

	// Video frames
	cv::Mat frame;
	bool horizontalflip;

	/// <summary>
	/// Constructor sets web cam settings
	/// </summary>
	void init (int camera = 0, bool flip = true) {
		// Initialize cam0 and cam1 objects
		vid_capture = cv::VideoCapture(camera);  // Assuming cam0 is the first camera
		if (!vid_capture.isOpened())
		{
			std::cout << "Error opening camera" << std::endl;
		}
		else
		{	// settings
			vid_capture.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
			vid_capture.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
			vid_capture.read(frame);
			horizontalflip = flip;
			readFrame();
		}
	}

	void readFrame() {
		frame.release();
		if (!vid_capture.isOpened())
		{
			std::cout << "Error opening camera" << std::endl;
		}
		else if (!horizontalflip)
		{
			vid_capture.read(frame);
		}
		else {
			vid_capture.read(frame);
			cv::flip(frame, frame, flip_h); 
		}
	}

	cv::Mat getFrame() {
		readFrame();
		return frame;
	}

	void reseize(int x,int y) {
		cv::resize(frame, frame, cv::Size(x, y));
	}

	void showImage() {
		cv::Mat frame;
		// Capture frames from cam
		vid_capture >> frame;

		// Display frames
		cv::imshow("Camera", frame);

		// Wait for a key press (optional)
		cv::waitKey();
	}
	~CameraClass() {
		vid_capture.release();
	}
};

/// <summary>
/// OpenCV to glGenTextures handling
/// </summary>
class VideoForImGui {

public:
	// OpenGL Texture 
	GLuint imageTexture; // handle to texture`s (Texture ID)
	// Image Data 
	int image_width;
	int image_height;

	VideoForImGui() {
		// Image Data 
		image_width = 0;
		image_height = 0;
	}

	void loadImage(char const* fileNamePath) {
		unsigned char* image_data = stbi_load(fileNamePath, &image_width, &image_height, NULL, 4);
		if (image_data != NULL) {
			// Delete the previous texture if it exists
			if (glIsTexture(imageTexture))
				glDeleteTextures(1, &imageTexture);

			glGenTextures(1, &imageTexture);
			glBindTexture(GL_TEXTURE_2D, imageTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
			stbi_image_free(image_data);
		}
	}

	/// <summary>
	/// GPU memory Allocation for a picture or video frame
	/// </summary>
	/// <param name="image">image (single video frame)</param>
	void initVideo(cv::Mat& image) {

		if (image.empty()) {
			std::cout << "image empty" << std::endl;
		}
		else {
			//These settings stick with the texture that's bound. You only need to set them once.
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glGenTextures(1, &imageTexture); //Gen a new texture and store the handle
			glBindTexture(GL_TEXTURE_2D, imageTexture); // Allocate GPU memory for handle (Texture ID)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Set texture clamping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
	}

	/// <summary>
	/// Convert CV::Mat to texture, used in imGui
	/// </summary>
	/// <param name="image">image (single video frame)</param>
	/// <param name="imageTexture">handel for GPU Allocated memory</param>
	void BindCVMat2GLTexture(cv::Mat& image)
	{
		if (image.empty()) {
			std::cout << "image empty" << std::endl;
		}
		else {
			glBindTexture(GL_TEXTURE_2D, imageTexture); // Allocate GPU memory for handle (Texture ID)

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Set texture clamping method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

			glTexImage2D(GL_TEXTURE_2D,         // Type of texture
				0,                   // Pyramid level (for mip-mapping) - 0 is the top level
				GL_RGB,              // Internal colour format to convert to
				image.cols,          // Image width  i.e. 640 for Kinect in standard mode
				image.rows,          // Image height i.e. 480 for Kinect in standard mode
				0,                   // Border width in pixels (can either be 1 or 0)
				GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
				GL_UNSIGNED_BYTE,    // Image data type
				image.ptr());        // The actual image data itself
		}
	}

	void resizeImage(int new_width, int new_height) {
		if (new_width != image_width || new_height != image_height) {
			image_width = new_width;
			image_height = new_height;

			// Delete the previous texture if it exists
			if (glIsTexture(imageTexture)) {
				glDeleteTextures(1, &imageTexture);
				imageTexture = 0; // Reset the texture handle to 0
			}

			// Create a new texture with the updated size
			glGenTextures(1, &imageTexture);
			glBindTexture(GL_TEXTURE_2D, imageTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
	}
};


class facedetection {
private:
	cv::CascadeClassifier cascade, nestedCascade;
	double scale;

	string cascadeName;
	string nestedCascadeName;
	cv::Point center;

public:

	std::vector<ImVec2> found_faces;

	facedetection()
	{
		cascadeName = "Vision/haarcascade_frontalface_alt.xml";
		nestedCascadeName = "Vision/haarcascade_eye_tree_eyeglasses.xml";
		scale = 1;

		if (!nestedCascade.load(cv::samples::findFileOrKeep(nestedCascadeName)))
			cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
		if (!cascade.load(cv::samples::findFile(cascadeName)))
		{
			cerr << "ERROR: Could not load classifier cascade" << endl;
		}
	}

	void detectAndDraw(cv::Mat& img)
	{
		vector<cv::Rect> faces;
		for (int ii = 0; ii < found_faces.size(); ii++)
		{
			found_faces.erase(found_faces.begin());
		}

		const static cv::Scalar colors[] =
		{
			cv::Scalar(255,0,0),
			cv::Scalar(255,128,0),
			cv::Scalar(255,255,0),
			cv::Scalar(0,255,0),
			cv::Scalar(0,128,255),
			cv::Scalar(0,255,255),
			cv::Scalar(0,0,255),
			cv::Scalar(255,0,255)
		};
		cv::Mat gray, smallImg;
		cvtColor(img, gray, cv::COLOR_BGR2GRAY);
		double fx = 1 / scale;
		resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR_EXACT);
		equalizeHist(smallImg, smallImg);

		// find faces 
		cascade.detectMultiScale(smallImg, faces,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			| cv::CASCADE_SCALE_IMAGE,
			cv::Size(30, 30));

		// find eyes for the faces			

		int choose_face = 0;

		for (size_t i = 0; i < faces.size(); i++)
		{
			cv::Rect r = faces[i];
			cv::Mat smallImgROI;
			vector<cv::Rect> nestedObjects;
			
			cv::Scalar color = colors[choose_face % 8];
			int radius;
			double aspect_ratio = (double)r.width / r.height;
			if (0.75 < aspect_ratio && aspect_ratio < 1.3)
			{
				center.x = cvRound((r.x + r.width * 0.5) * scale);
				center.y = cvRound((r.y + r.height * 0.5) * scale);
				radius = cvRound((r.width + r.height) * 0.25 * scale);

			}
			else 
			{
				rectangle(img, cv::Point(cvRound(r.x * scale), cvRound(r.y * scale)),
					cv::Point(cvRound((r.x + r.width - 1) * scale), cvRound((r.y + r.height - 1) * scale)),
					color, 3, 8, 0);
			}
			if (nestedCascade.empty())
				continue;
			// ROI from found face
			smallImgROI = smallImg(r);

			// find eyes 
			nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
				1.1, 2, 0
				//| cv::CASCADE_FIND_BIGGEST_OBJECT
				//| cv::CASCADE_DO_ROUGH_SEARCH
				//| cv::CASCADE_DO_CANNY_PRUNING
				| cv::CASCADE_SCALE_IMAGE,
				cv::Size(30, 30));

			// consider face only if eyes are found
			if (nestedObjects.size() > 1) {

				// Draw Face
				circle(img, center, radius, color, 3, 8, 0);
				center.x = cvRound((r.x + r.width * 0.5) * scale);
				center.y = cvRound((r.y + r.height * 0.5) * scale);
				found_faces.push_back(ImVec2((float)center.x, (float)center.y));

				// Draw eyes  
				for (size_t j = 0; j < nestedObjects.size(); j++)
				{
					cv::Rect nr = nestedObjects[j];
					center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
					center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
					radius = cvRound((nr.width + nr.height) * 0.25 * scale);
					circle(img, center, radius, color, 3, 8, 0);

				}
				choose_face++;
			}
		}

	}
};

