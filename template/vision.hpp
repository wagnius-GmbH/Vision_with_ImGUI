#include "Header.h"


class CameraClass {
private:
	// Private member variables
	cv::VideoCapture vid_capture;

public:

	// Video frames
	cv::Mat frame;

	/// <summary>
	/// Constructor sets web cam settings
	/// </summary>
	void init (int camera = 0) {
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
		}
	}

	void readFrame() {
		frame.release();
		if (!vid_capture.isOpened())
		{
			std::cout << "Error opening camera" << std::endl;
		}
		vid_capture.read(frame);
	}

	cv::Mat getFrame() {
		readFrame();
		return frame;
	}

	void showImage() {
		cv::Mat frame;
		// Capture frames from cam0 and cam1
		vid_capture >> frame;

		// Display frames
		cv::imshow("Camera 0", frame);

		// Wait for a key press (optional)
		cv::waitKey(1);
	}
	~CameraClass() {
		vid_capture.release();
	}
};


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
	/// GPU memory Allocation for a picture
	/// </summary>
	/// <param name="image">image (single video frame)</param>
	/// <param name="imageTexture">handel for GPU Allocated memory</param>
	void initVideo (cv::Mat& image) {

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

};


