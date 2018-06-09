#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

CURL* initializedCurl();
std::string getConfigPath(int argc, char** argv);
std::string getSpaceCount(int argc, char** argv);
size_t writeFunction(char* buffer, size_t size, size_t nmemb, void *userp);
void getAuthCredentials(const std::string& configPath, std::string& authId, std::string& authSecret);
std::string getAccessToken(CURL* curl, const std::string& configPath);
void getParking(CURL* curl, const std::string& spaceCount, const std::string& token, rapidjson::Document& parkingJson);
void printParking(const rapidjson::Document& parkingJson);

std::string defaultConfigPath = "./.config";
std::string defaultSpaceCount = "0";

int main (int argc, char** argv) {
	std::string token = getAccessToken(initializedCurl(), getConfigPath(argc, argv));
	rapidjson::Document parkingJson;
	getParking(initializedCurl(), getSpaceCount(argc, argv), token, parkingJson);
	printParking(parkingJson);
	return 0;
}

/************************************************************************************************************************************************************
Function: CURL* initializedCurl()
Description: Returns an initialized curl handle
************************************************************************************************************************************************************/
CURL* initializedCurl() {
    CURL* curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    return curl;
}

/************************************************************************************************************************************************************
Function: std::string getConfigPath(int argc, char** argv)
Description: Returns the path of the configuration file. Returns command line argument for configuration file path if it exists, returns deafult path
	otherwise
Parameters: int argc, char** argv are command line arguments
************************************************************************************************************************************************************/
std::string getConfigPath(int argc, char** argv) {
	std::string configPath = defaultConfigPath;
	for (int i = 0; i < argc-1; i++) {
		if (argv[i][0] == '-' && argv[i][1] == 'c')
			configPath = argv[i+1];
	}
	return configPath;
}

/************************************************************************************************************************************************************
Function: std::string getSpaceCount(int argc, char** argv)
Description: Returns the number of motorcycle parking spaces to filter locations by. Returns command line argument if it is valid, returns default
	parking spaces otherwise
Parameters: int argc, char** argv are command line arguments
************************************************************************************************************************************************************/
std::string getSpaceCount(int argc, char** argv) {
	std::string spaceCount = defaultSpaceCount;
	for (int i = 0; i < argc-1; i++) {
		if (argv[i][0] == '-' && argv[i][1] == 'n') {
			spaceCount = argv[i+1];
		}
	}
	for (int i = 0; i < spaceCount.length(); i++) {
		if (spaceCount[i] < '0' || spaceCount[i] > '9') {
			    std::cout << "Invalid motorycle parking space number argument" << std::endl;
			    exit(1);
		    }
	}
	return spaceCount;
}

/************************************************************************************************************************************************************
Function: size_t writeTokenData
Description: Writes response from curl request to memory as it is recieved
Parameters: char* buffer is the portion of response received, size is the byte size of each element in buffer, nmemb is the number of elements in the buffer,
	and userp points to the string where the response should be stored
************************************************************************************************************************************************************/
size_t writeFunction(char* buffer, size_t size, size_t nmemb, void* userp) {
	std::string* writeData = static_cast<std::string*>(userp);
	writeData->append(buffer, size*nmemb);
	return size*nmemb;
}

/************************************************************************************************************************************************************
Function: void getAuthCredentials(const std::string& configPath, std::string& authId, std::string& authSecret)
Description: Populates authID and authSecret from values in configuration file
Parameters: const string& configPath is the path of the configuration file, string& authID stores a reference to field to be populated with the authorization
	ID, string* authSecret stores a reference to the field to be populated with the authorization secret
Pre-conditions: valid configuration file exists at configPath (see README for requirements)
Post-conditions: authID and authSecret are populated with values from configuration file
************************************************************************************************************************************************************/
void getAuthCredentials(const std::string& configPath, std::string& authId, std::string& authSecret) {
	std::ifstream config;
     config.open(configPath, std::ios::in);
     if (config.is_open()) {
     	while (!config.eof()) {
			std::string line;
			std::string field;
			getline(config, line);
			int j = 0;
			while (line[j] != ' ' && line.length() > 0) {
				field += line[j];
				j++;
			}
			while (line[j] == ' ') {
				j++;
			}
			if (field == "client_id") {
				authId = line.erase(0, j);
         		}
			if (field == "client_secret") {
           		authSecret = line.erase(0, j);
      		}
		}
		config.close();
	} else {
		std::cout << "No file at confiuration path" << std::endl;
		exit(1);
	}
}

/************************************************************************************************************************************************************
Function: void getAccessToken(CURL* curl, const std::string& configPath, std::string& token)
Description: Makes a request to OAuth2 API for an access token, then parses response for token and writes it to memory
Parameters: CURL* curl is an initialized curl handle, const string& configPath is the path to the
	configuraiton file
Pre-conditions: valid configuration file exists at configPath (see README for requirements)
************************************************************************************************************************************************************/
std::string getAccessToken(CURL* curl, const std::string& configPath) {
	std::string authId;
	std::string authSecret;
	getAuthCredentials(configPath, authId, authSecret);
	CURLcode response;
	std::string writeData;
	std::string token;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.oregonstate.edu/oauth2/token");
		std::string postFields = "client_id=" + authId + "&client_secret=" + authSecret + "&grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeData);
		response = curl_easy_perform(curl);
	}
     curl_easy_cleanup(curl);
	if (response == CURLE_OK) {
		long int httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		if (httpCode == 200) {
     		rapidjson::Document tokenJson;
     		tokenJson.Parse(writeData.c_str());
			if (tokenJson.HasParseError()) {
				std::cout << "Error parsing Authorization response: " << GetParseError_En(tokenJson.GetParseError()) << std::endl;
				exit(1);
			}
 			token = tokenJson["access_token"].GetString();
		} else {
			std::cout << "Authorization error, check id and secret in config file. Http code " << httpCode << std::endl;
			exit(1);
		}
	} else {
		std::cout << "Error connecting to authorzation endpoint: " << curl_easy_strerror(response) << std::endl;
		exit(1);
	}
	return token;
}

/************************************************************************************************************************************************************
Function: void getParking(CURL* curl, const std::string& spaceCount, const std::string& token, rapidjson::Document& parkingJson)
Description: Makes a request to Locations API for all parking locations with minumum motorcycle parking spaces designated in command line argument
Parameters: CURL* curl is an initialized curl handle, string spaceCount is the minimum number of parking space to filter for,
	string token is an authroization token, Document& parkingJson points to the document that will parse
	and store the response
Pre-conditions: token field has been populated with a valid access token
************************************************************************************************************************************************************/
void getParking(CURL* curl, const std::string& spaceCount, const std::string& token, rapidjson::Document& parkingJson) {
	CURLcode response;
	std::string writeData;
	if (curl) {
		std::string encodedSpaceCount = curl_easy_escape(curl, spaceCount.c_str(), spaceCount.length());
		std::string query = "https://api.oregonstate.edu/v1/locations/?campus=corvallis&page[size]=1000&motorcycleParkingSpaceCount=" + encodedSpaceCount;
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		struct curl_slist *slist = NULL;
		std::string headerFields = "Authorization: Bearer " + token;
		slist = curl_slist_append(slist, headerFields.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeData);
		response = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	if (response == CURLE_OK) {
		long int httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
		if (httpCode == 200) {
			parkingJson.Parse(writeData.c_str());
			if (parkingJson.HasParseError()) {
				std::cout << "Error parsing Locations response: " << GetParseError_En(parkingJson.GetParseError()) << std::endl;
				exit(1);
			}
		} else {
			std::cout << "Error with Locations response. Http code " << httpCode << std::endl;
			exit(1);
		}
	} else {
		std::cout << "Error connecting to Locations API: " << curl_easy_strerror(response) << std::endl;
		exit(1);
	}
}

/************************************************************************************************************************************************************
Function: void printParking
Description: Prints the name of each parking location
Pre-conditions: parkingJson.Parse() has completed succesfully
************************************************************************************************************************************************************/
void printParking(const rapidjson::Document& parkingJson) {
	for (int i = 0; i < parkingJson["data"].Size(); i++)
  		std::cout << parkingJson["data"][i]["attributes"]["name"].GetString() << std::endl;
}
