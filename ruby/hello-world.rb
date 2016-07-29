require "net/http"
require 'uri'
require 'json'
require 'net/https'

def get_access_token(url, client_id, client_secret)
    post_data = "client_id=#{client_id}&client_secret=#{client_secret}&grant_type=client_credentials"
    puts post_data
    url = URI.parse(url)
    res = Net::HTTP::Post.new(url)
    res.body = post_data
    begin
        response = Net::HTTP.start(url.hostname, url.port) do |https|
            https.request(res)
        puts "--2--"
        end
    rescue Exception
        puts "--2--"
        puts "error:#{$!} at:#{$@}!"
        puts "post failed!"
        return 1
    end
    puts response.code
    response = response.body
    puts response
    return response.to_json
end

def get_locations(url, access_token, params)
    query_params = URI.encode_www_form(params)
    api_call_url = URI.parse(url)
    request = Net::HTTP::Get.new(api_call_url)
    request.body = query_params
    request.add_field('Authorization', "Bearer #{access_token}")
    begin
        response = Net::HTTP.new(api_call_url.hostname, api_call_url.port).start do |http|
            http.request(request)
        end
    rescue Exception
        puts "error:#{$!} at:#{$@}!"
        puts "get failed!"
        return 1
    end
    response = response.body
    return response.to_json
end

config_data_file = File.read("configuration.json")
config_data = JSON.parse(config_data_file)

base_url = config_data["hostname"] + config_data["version"] + config_data["api"]
access_token_url = "#{base_url}#{config_data["token_endpoint"]}"

# access_token_response = get_access_token(access_token_url, config_data["client_id"], config_data["client_secret"])

# access_token  = access_token_response["access_token"]
access_token = "gaHbo6eJQ6Mh0G2UEU6rNjvzMWTZ"
locations_url = "#{base_url}#{config_data["locations_endpoint"]}"
puts locations_url
params = { :q => "library", :campus => "corvallis" }
response = get_locations(locations_url, access_token, params)
puts "response: #{response}"
