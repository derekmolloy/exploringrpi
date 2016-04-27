#include "json/json.h"
#include<iostream>
#include<fstream>
using namespace std;

int main(){
   Json::Value root;           // the parsed data is at this root
   Json::Reader reader;        // read from the data.json file
   std::ifstream data("data.json",  std::ifstream::binary);
   bool success = reader.parse(data, root, false);
   if(!success){               // has the parsing failed?
      cout << "Failed: " <<  reader.getFormattedErrorMessages() << endl;
   }
   // the deserialized data can be converted to a float and a bool
   float temperature = root.get("temperature", "UTF-8").asFloat();
   bool button = root.get("button", "UTF-8").asBool();
   cout << "The temperature is " << temperature << "°C" << endl;
   cout << "The button is " << ( button ? "pressed" : "no pressed") << endl;
   return 0;
}
