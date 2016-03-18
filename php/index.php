<?php

require_once("configuration.php");

$base_url = $hostname . $version . $api;
$access_token_url = $base_url . $token_endpoint;

$access_token_response = getAccessToken($access_token_url, $client_id, $client_secret);

// You will want to cache this access token (it's good for 24 hrs)
$access_token = $access_token_response->access_token;

$locations_url = $base_url . $locations_endpoint;
$params = array("q" => "library");

$resp = getLocations($locations_url, $access_token, $params);
echo $resp;


function getAccessToken($url, $client_id, $client_secret) {
    $postData = "client_id=$client_id&client_secret=$client_secret&grant_type=client_credentials";

    // Get cURL resource
    $curl = curl_init();
    
    // Set some options - we are passing in a useragent too here
    curl_setopt_array($curl, array(
        CURLOPT_RETURNTRANSFER  => 1,
        CURLOPT_URL             => $url,
        CURLOPT_POST            => 1,
        CURLOPT_POSTFIELDS      => $postData
    ));

    // Send the request & save response to $resp
    $resp = curl_exec($curl);

    // Close request to clear up some resources
    curl_close($curl);

    return json_decode($resp);
}

function getLocations($url, $access_token, $params) {
    $query_params = http_build_query($params);
    $headers = array(
        'Authorization: Bearer ' . $access_token
    );
    $api_call_url = $url . "?" . $query_params;

    // Get cURL resource
    $curl = curl_init();

    // Set some options - we are passing in a useragent too here
    curl_setopt_array($curl, array(
        CURLOPT_RETURNTRANSFER  => 1,
        CURLOPT_URL             => $api_call_url,
        CURLOPT_HTTPHEADER      => $headers
    ));

    // Send the request & save response to $resp
    $resp = curl_exec($curl);

    // Close request to clear up some resources
    curl_close($curl);

    return $resp;
}
