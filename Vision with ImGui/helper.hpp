inline const char* convertStringToConstChar(const std::string& str) {
    return str.c_str();
}

inline std::string convertIntToString(int number) {
    return std::to_string(number);
}