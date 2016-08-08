from oauthlib.oauth2 import BackendApplicationClient
from requests_oauthlib import OAuth2Session

import json
import re
import requests
import urllib


def getAccessToken(client_id, client_secret, token_url):
	client    = BackendApplicationClient(client_id=client_id)
	oauth     = OAuth2Session(client=client)
	token_res = oauth.fetch_token(token_url=token_url, client_id=client_id, client_secret=client_secret)

	print json.dumps(token_res, sort_keys=True, indent=4, separators=(',', ': '))
	return token_res


def getResponse(token, params, api_url):
	query_params = urllib.urlencode(params)
	api_call_url = api_url + "?" + query_params

	return requests.get(api_call_url, headers={'Authorization': token})


config_file = open("configuration_example.json")
config_data = json.load(config_file)

base_url  = config_data["hostname"] + config_data["version"] + config_data["api"]
token_url = base_url + config_data["token_endpoint"]
token_res = getAccessToken(config_data["client_id"], config_data["client_secret"], token_url)
token     = re.sub("Token", "", token_res["token_type"]) + " " + token_res["access_token"]  # remove trailing wrong token_type: "Token"

api_url  = base_url
params   = {"q": "library"}
response = getResponse(token, params, api_url)
print json.dumps(response.json(), indent=4, separators=(',', ': '))
