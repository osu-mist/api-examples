from oauthlib.oauth2 import BackendApplicationClient
from requests_oauthlib import OAuth2Session

import json

CONSUMER_KEY     = "consumer_key"
CONSUMER_SECRET  = "consumer_secret"
ACCESS_TOKEN_URL = "https://api.oregonstate.edu/<API-ENDPOINT>/token"

client = BackendApplicationClient(client_id=CONSUMER_KEY)
oauth  = OAuth2Session(client=client)
token  = oauth.fetch_token(token_url=ACCESS_TOKEN_URL, client_id=CONSUMER_KEY, client_secret=CONSUMER_SECRET)

print json.dumps(token, sort_keys=True, indent=4, separators=(',', ': '))
