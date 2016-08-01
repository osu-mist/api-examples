import json
import pycurl
import urllib
import StringIO  

def getAccessToken(url, client_id, client_secret):
	post_data = "client_id=" + client_id + "&client_secret=" + client_secret + "&grant_type=client_credentials"
	
	storage = StringIO.StringIO()
	curl    = pycurl.Curl()

	# Set options 
	curl.setopt(pycurl.URL, url)                     # CURLOPT_URL in PHP
	curl.setopt(pycurl.POST, 1)                      # CURLOPT_POST in PHP
	curl.setopt(pycurl.POSTFIELDS, post_data)        # CURLOPT_POSTFIELDS in PHP
	curl.setopt(pycurl.WRITEFUNCTION, storage.write) # CURLOPT_RETURNTRANSFER in PHP

	# Send the request and save response
	curl.perform()
	
	# Close request to clear up some resources
	curl.close()

	response = storage.getvalue()
	return json.loads(response)


def getLocations(url, access_token, params):
	query_params = urllib.urlencode(params)
	api_call_url = url + "?" + query_params;
	headers      = ['Authorization: Bearer '+ access_token]

	storage = StringIO.StringIO()
	curl    = pycurl.Curl()

	# Set options 
	curl.setopt(pycurl.URL, api_call_url)
	curl.setopt(pycurl.HTTPHEADER, headers) # CURLOPT_HTTPHEADER in PHP
	curl.setopt(pycurl.WRITEFUNCTION, storage.write)

	curl.perform()
	curl.close()

	response = storage.getvalue()
	return json.loads(response)


# Read configuration file in JSON format
config_data_file = open('configuration.json')
config_data      = json.load(config_data_file)

base_url         = config_data["hostname"] + config_data["version"] + config_data["api"]
access_token_url = base_url + config_data["token_endpoint"]

access_token_response = getAccessToken(access_token_url, config_data["client_id"], config_data["client_secret"]);

access_token  = access_token_response["access_token"]
locations_url = base_url + config_data["locations_endpoint"]
params        = {"q": "library"}

response = getLocations(locations_url, access_token, params)
print "response: ", response


