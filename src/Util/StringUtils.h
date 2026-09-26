#pragma once
#define NOMINMAX
#include <string>
#include <unordered_map>
#include <imgui/imgui.h>

#undef min
#undef max



inline bool TryCastToInt(std::string s, int& result) {
	if (s[s.size() - 1] == 'x' && s[s.size() - 2] == 'p') s.resize(s.size() - 2); // Remove 'px' suffix if present  

	char* endptr;  
	long val = std::strtol(s.c_str(), &endptr, 10); // Base 10  

	// Check if the entire string was consumed (no invalid characters)  
	// and if the string was not empty.  
	if (endptr == s.c_str() || *endptr != '\0') {  
		return false; // Not a valid integer or contains non-numeric characters  
	}  

	// Check for overflow/underflow relative to 'int'  
	if (val > (long)std::numeric_limits<int>::max() || val < (long)std::numeric_limits<int>::min()) {  
		return false; // Value out of 'int' range  
	}  

	result = static_cast<int>(val);  
	return true;  
}
inline bool TryCastToFloat(std::string& s, float& result) {  
	bool isPercentage = s[s.size() - 1] == '%';
	if (isPercentage)  
	{  
		s.resize(s.size() - 1);  
	}  

	char* endptr;  
	float val = std::strtof(s.c_str(), &endptr); // Base 10  

	// Check if the entire string was consumed (no invalid characters)  
	// and if the string was not empty.  
	if (endptr == s.c_str() || *endptr != '\0') {  
		return false; // Not a valid integer or contains non-numeric characters  
	}  

	if (isPercentage) val /= 100.0f;  

	result = static_cast<float>(val);  
	return true;  
}
inline bool TryCastPercentage(std::string& s, Percentage& result)
{
	bool isPercentage = s[s.size() - 1] == '%';
	if (isPercentage)
	{
		float floatRatio;
		TryCastToFloat(s, floatRatio);
		result = Percentage(floatRatio);
		return true;
	}
	return false;
}

