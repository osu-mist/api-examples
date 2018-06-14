# Motorcycle Parking
The purpose of this script is to illustrate basic interaction with OSU's OAuth2 and Locations APIs using C++. The script prints the names of parking zones that have a minimum number of motorcycle parking spaces, designated by a command line argument.

## Libraries
This script is dependent on two libraries:
* [libcurl](https://curl.haxx.se/libcurl/) - Used for making HTTP requests and receiving responses
* [RapidJSON](http://rapidjson.org/index.html) - Used to map data from API responses to C++ objects

## Installing
Libraries should be installed in your compiler's system include path. If a compiler other than g++ is used, change the CC variable in the Makefile.

The "Make" command in the application directory will build the executable "a.out".

### Configuration
A configuration file containing an Oauth2 client id and secret must be included before runtime. A client id and secret can be obtained by registering an application on the [OSU Developer Portal](https://developer.oregonstate.edu/)

The format of the configuration file is as follows:
* A line containing "client_id", at least one space character " ", and the client id.
* A line containing "client_secret", at least one space character " ", and the client secret.

Example:

client_id 1234fake_client_id1234 <br />
client_secret 1234_fake_client_secret1234

The configuration file should either be placed in the application directory with the name ".config", or should be passed by command line argument at runtime (see runtime).

## Runtime
Two command line arguments can be taken:
* Path of configuration file, flagged by "-c". Example: "-c ./.config"
* Minimum number of motorcycle parking spaces, flagged by "-n". Example: "-n 7"

If no command line arguments are passed, script defaults to ".config" in the application directory for the configuration file path, and 0 for parking spaces.
