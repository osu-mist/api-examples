require 'json'
require 'uri'
require "net/http"

def get_access_token(url, client_id, client_secret)
    post_data = "client_id=#{client_id}&client_secret=#{client_secret}&grant_type=client_credentials"
    url = URI.parse(url)
    request = Net::HTTP::POST.new(url.path)
    request.body = post_data
    begin
        response = Net::HTTP.new(url.path).start do |http|
            http.request(request)
        end
    rescue Exception
        puts "error:#{$!} at:#{$@}!"
        puts "post failed!"
        return 1
    end
    response = response.body
    puts "post result is #{response}"
    return response.to_json
end

def get_locations(url, access_token, params)
    query_params = URI::encode(params)
    api_call_url = URI.parse("#{url}?#{query_params}")
    request = Net::HTTP::GET.new(api_call_url.path)
    request.add_field('Authorization', "Bearer #{access_token}")
    begin
        response = Net::HTTP.new(url.path, port=nil).start do |http|
            http.request(request)
        end
    rescue Exception
        puts "error:#{$!} at:#{$@}!"
        puts "post failed!"
        return 1
    end
    response = response.body
    return response.to_json
end

