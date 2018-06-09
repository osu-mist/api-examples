Python - OAuth2 Library Testing Code
====================================

In order to make sure users can get valid APIs access token through OAuth2 with **client credentials** grant workflow, we tested several famous Python OAuth2 libraries to see the result.

1. [python-oauth2](https://github.com/joestump/python-oauth2), which may be the most popular OAuth2 library in Python community. To install python-oauth2, Python 2.6 or later version is necessary.

2. [OAuthLib](https://github.com/idan/oauthlib), which is also a very popular OAuth2 library.

## Results

### [python-oauth2](https://github.com/joestump/python-oauth2)

```python
import oauth2 as oauth

CONSUMER_KEY     = "consumer_key"
CONSUMER_SECRET  = "consumer_secret"
ACCESS_TOKEN_URL = "https://api.oregonstate.edu/<API-ENDPOINT>/token"

consumer = oauth.Consumer(key=CONSUMER_KEY, secret=CONSUMER_SECRET)
client   = oauth.Client(consumer)

print client.request(ACCESS_TOKEN_URL, method="POST", headers="Content-Type: application/x-www-form-urlencoded;charset=utf-8")

```

Since the `Client` class of python-oauth2 is implemented from `httplib2`, it caused the [TLS SNI issue](https://github.com/kennethreitz/requests/issues/749) (Server presented certificate that does not match host api.oregonstate.edu). Therefore, this library will not be recommended to communicate with our APIs.

### [OAuthLib](https://github.com/idan/oauthlib): [sample code](oauthlib-token.py)

This library works correctly as expected. Since our APIs provide `getAccessToken` methods with **client_credential** grant type to allow users getting access tokens, we can use the [BackendApplicationClient](https://oauthlib.readthedocs.io/en/latest/oauth2/clients/backendapplicationclient.html) of OAuthLib to create a client object utilizing the client credentials grant workflow.

However, there is one minor issue we should take care of. The `token_type` we got from the response will be `BearerToken`, instead of the correct format: `Bearer`.

#### Usage

1. The dependencies can be installed by using pip:
`pip install requests requests_oauthlib oauthlib`
2. Modify [configuration_example.json](configuration_example.json) file as needed, and rename it as `configuration.json`
3. Execute `python oauthlib-token.py` to run the program.

## FAQ

###  How to solve the SNI missing issue?

If you get an error of `requests.exceptions.SSLError: hostname 'api.oregonstate.edu' doesn't match either of '*.apigee.net' and 'apigee.net'`, it's most likely because the version of Python, Java, Curl, etc you are using doesn't have the SNI support. With SNI, an extension of TLS/SSL, multiple HTTPS targets can be served off the same IP address and port without requiring all those targets to use the same certificate. However, not all clients support SNI, specifically for Python, it's supported in 2.x from 2.7.9rc1 and 3.x from 3.2alpha4 (in ssl, urllib[2] and httplib modules).

Please check [Apigee SNI support](http://docs.apigee.com/release-notes/content/150415-apigee-edge-cloud-release-notes#newfeaturesandenhancements-servernameindicationsnisupport) for more details. And for more information on SNI, see [SNI Wiki](https://www.wikiwand.com/en/Server_Name_Indication)
and [Server Name Indication](https://https.cio.gov/sni/)
