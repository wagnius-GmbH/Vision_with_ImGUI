#include "header.h"

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
	constexpr size_t size() const { return 2; }
	int& operator[] (size_t idx) { assert(idx == 0 || idx == 1); return reinterpret_cast<int*>(this)[idx]; }
	int  operator[] (size_t idx) const { assert(idx == 0 || idx == 1); return reinterpret_cast<const int*>(this)[idx]; }
};


struct CamResult3D{
	float x, y, z;
	constexpr CamResult3D() : x(0.0f), y(0.0f),  z(0.0f) { }
	constexpr CamResult3D(float _x, float _y,float _z) : x(_x), y(_y), z(_z)  { }
	constexpr size_t size() const { return 3; }
	int& operator[] (size_t idx) { assert(idx == 0 || idx == 1); return reinterpret_cast<int*>(this)[idx]; }
	int  operator[] (size_t idx) const { assert(idx == 0 || idx == 1); return reinterpret_cast<const int*>(this)[idx]; }
};

/// <summary>
/// utility structure for realtime plot
/// </summary>
struct ScrollingBuffer {
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	ScrollingBuffer(int max_size = 20) {
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
		}
	}
};


/// <summary>
/// utility structure for realtime plot
/// </summary>
struct RollingBuffer {
	float Span;
	ImVector<ImVec2> Data;
	RollingBuffer() {
		Span = 10.0f;
		Data.reserve(2000);
	}
	void AddPoint(float x, float y) {
		float xmod = fmodf(x, Span);
		if (!Data.empty() && xmod < Data.back().x)
			Data.shrink(0);
		Data.push_back(ImVec2(xmod, y));
	}
};



inline const char* convertStringToConstChar(const std::string& str) {
	return str.c_str();
}

inline std::string convertIntToString(int number) {
	return std::to_string(number);
}

inline float vectorMagnitude(CamResult3D& found_faces, CamResult3D& found_faces_last)
{
	float temp = (float)sqrt(
		pow((found_faces.x) - (found_faces_last.x), 2) + 
		pow((found_faces.y) - (found_faces_last.y), 2) + 
		pow((found_faces.z) - (found_faces_last.z), 2)
	);
	return temp;
}

inline void printCamResultVector(vector<CamResult3D>& found_faces)
{
	for (int ii = 0; ii < found_faces.size(); ii++)
	{
		cout << "face " << ii << " x: " << found_faces[ii].x << "  y: " << found_faces[ii].y << "  z: " << found_faces[ii].z << endl;
	}
	
}