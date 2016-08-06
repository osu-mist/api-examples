Python - OAuth2 Library Testing Code
====================================

In order to make sure users can get valid APIs access token through OAuth2 with **client credentials** grant workflow, we tested several famous Python OAuth2 libraries to see the result.

1. [python-oauth2](https://github.com/joestump/python-oauth2), which may be the most popular OAuth2 library in Python community. To install python-oauth2, Python 2.6 or later version is necessary.

2. [OAuthLib](https://github.com/idan/oauthlib), which is also a very popular OAuth2 library.

## Results
------------

### [python-oauth2](https://github.com/joestump/python-oauth2)

```python
import oauth2 as oauth

CONSUMER_KEY     = "consumer_key"
CONSUMER_SECRET  = "consumer_secret"
ACCESS_TOKEN_URL = "https://api.oregonstate.edu/<API-ENDPOINT>/token"

consumer          = oauth.Consumer(key=CONSUMER_KEY, secret=CONSUMER_SECRET)
client            = oauth.Client(consumer)

print client.request(ACCESS_TOKEN_URL, method="POST", headers="Content-Type: application/x-www-form-urlencoded;charset=utf-8")

```

Since the `Client` class of python-oauth2 is implemented from `httplib2`, it caused the [TLS SNI issue](https://github.com/kennethreitz/requests/issues/749) (Server presented certificate that does not match host api.oregonstate.edu). Therefore, this library will not be recommended to communicate with our APIs.

### [OAuthLib](https://github.com/idan/oauthlib): [sample code link](oauthlib-token.py)

This library works correctly as expected. Since our APIs provide `getAccessToken` methods with **client_credential** grant type to allow users getting access tokens, we can use the [BackendApplicationClient](https://oauthlib.readthedocs.io/en/latest/oauth2/clients/backendapplicationclient.html) of OAuthLib to create a client object utilizing the client credentials grant workflow.

However, there is one minor issue we should take care of. The `token_type` we got from the response will be `BearerToken`, instead of the correct format: `Bearer`.
