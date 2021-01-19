#ifndef JSON_HELPER_H_
#define JSON_HELPER_H_

#include <iostream>
#include <string>
#include <picojson.h>
#include <vector>

using std::vector;

namespace csci3081 {

/// @brief A class used to interact with JSON objects.
/**
 * This class allows for easy accessing of JSON object data fields.
 */
class JsonHelper {
 public:
  /**
   * @brief Encode an array as a picojson value.
   */
  static picojson::value EncodeArray(const vector<vector<float>> arr) {
    vector<picojson::value> result;
    for (vector<float> subarr : arr) {
    vector<picojson::value> subarr_encode;
    for (float val : subarr) {
      subarr_encode.push_back(picojson::value(val));
    }
      result.push_back(picojson::value(subarr_encode));
    }
    return picojson::value(result);
  }

  /**
   * @brief Returns the picojson value attached to key in passed picojson object.
   */
  static const picojson::value& GetValue(const picojson::object& obj, std::string key) {
      return obj.find(key)->second;
  }

  /**
   * @brief Returns the picojson object attached to key in passed picojson object.
   */
  static const picojson::object& GetObject(const picojson::object& obj, std::string key) {
      return GetValue(obj, key).get<picojson::object>();
  }

  /**
   * @brief Returns the picojson array attached to key in passed picojson object.
   */
  static const picojson::array& GetArray(const picojson::object& obj, std::string key) {
      return GetValue(obj, key).get<picojson::array>();
  }

  /**
   * @brief Returns the string attached to key in passed picojson object.
   */
  static std::string GetString(const picojson::object& obj, std::string key) {
      return GetValue(obj, key).get<std::string>();
  }

  /**
   * @brief Returns the double attached to key in passed picojson object.
   */
  static double GetDouble(const picojson::object& obj, std::string key) {
      return GetValue(obj, key).get<double>();
  }

  /**
   * @brief Returns whether or not passed picojson object contains key.
   */
  static bool ContainsKey(const picojson::object& obj, std::string key) {
      return obj.find(key) != obj.end();
  }

  /**
   * @brief Prints all key values in the picojson object.
   */
  static void PrintKeyValues(const picojson::object& obj, std::string prefix = "  ") {
      std::cout << "PrintKeyValues ---------" << std::endl;
      for (picojson::object::const_iterator it = obj.begin(); it != obj.end(); it++) {
          std::cout << prefix << it->first << ": " << it->second << std::endl;
      }
      std::cout << "End PrintKeyValues ---------" << std::endl;
  }

  /**
   * @brief Prints a serialized version of the picojson object.
   */
  static void Print(const picojson::object& obj, std::string prefix = "  ") {
      picojson::value val(obj);
      std::cout << prefix << val.serialize() << std::endl;
  }

  /**
   * @brief Prints a detailed view of all data contained within picojson object.
   */
  static void PrintEntityDetails(const picojson::object& val) {
    std::cout << "\n------JSON:------" << std::endl;
    JsonHelper::Print(val);

    std::cout << "------Key Values:------" << std::endl;
    JsonHelper::PrintKeyValues(val);

    std::cout << "------Entity Type:------" << std::endl;
    std::string type = JsonHelper::GetString(val, "type");
    std::cout << "  " << type << std::endl;

    std::cout << "------Contains Key:------" << std::endl;
    std::cout << "  Contains type: " << JsonHelper::ContainsKey(val, "type") << std::endl;
    std::cout << "  Contains otherKey: " << JsonHelper::ContainsKey(val, "otherKey") << std::endl;
    std::cout << "  Contains position: " << JsonHelper::ContainsKey(val, "position") << std::endl;

    std::cout << "------Position array:------" << std::endl;
    if (JsonHelper::ContainsKey(val, "position")) {
        const picojson::array& position = JsonHelper::GetArray(val, "position");
        for (int f = 0; f < position.size(); f++) {
          std::cout << "  position[" << f << "]: " << position[f] << std::endl;
        }
    }
    std::cout << std::endl;
  }
};

}  // namespace csci3081
#endif
