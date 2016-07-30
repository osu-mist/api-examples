require 'json'
require 'curb'

def get_access_token(url, client_id, client_secret)
    post_data = "client_id=#{client_id}&client_secret=#{client_secret}&grant_type=client_credentials"
    response = Curl.post(url,post_data)
    return JSON.parse(response.body)
end

def get_locations(url, access_token, params)
    api_call_url = Curl::urlalize(url, params)
    response = Curl::Easy.new(api_call_url) do |curl|
        curl.headers['Authorization'] = "Bearer #{access_token}"
        curl.perform
    end
    return JSON.parse(response.body)
end

config_data_file = File.read("configuration.json")
config_data = JSON.parse(config_data_file)

base_url = config_data["hostname"] + config_data["version"] + config_data["api"]

access_token_url = "#{base_url}#{config_data["token_endpoint"]}"
access_token_response = get_access_token(access_token_url, config_data["client_id"], config_data["client_secret"])
access_token  = access_token_response["access_token"]

locations_url = "#{base_url}#{config_data["locations_endpoint"]}"
params = { :q => "library", :campus => "corvallis", :type => "dining" }
response = get_locations(locations_url, access_token, params)

puts "response: #{response}"
