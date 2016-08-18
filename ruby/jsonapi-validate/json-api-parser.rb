#----------------------------------------------------------
# Intro: Parser for JSON API documents
# Install: gem install jsonapi_parser
# Fixes: no fixes needed
# Link: https://rubygems.org/gems/jsonapi_parser
#----------------------------------------------------------
require 'json/api'

EXAMPLE_LOCATION_JSON = '{"links":{"self":"https://api.oregonstate.edu/v1/locations?q=library&type=dining&campus=corvallis&page=1&page=10","first":"https://api.oregonstate.edu/v1/locations?q=library&type=dining&campus=corvallis&page[number]=1&page[size]=10","last":"https://api.oregonstate.edu/v1/locations?q=library&type=dining&campus=corvallis&page[number]=1&page[size]=10","prev":null,"next":null},"data":[{"id":"0da1b065ef522bc542fa2bc07efd2ad0","type":"locations","attributes":{"name":"Java II","abbreviation":null,"latitude":"44.565303","longitude":"-123.275673","summary":"Zone: Valley Library","description":null,"address":null,"city":null,"state":null,"zip":null,"county":null,"telephone":null,"fax":null,"thumbnails":null,"images":null,"departments":null,"website":null,"sqft":null,"calendar":null,"campus":"corvallis","type":"dining","openHours":{"1":[{"start":"2016-06-13T14:30:00Z","end":"2016-06-13T23:00:00Z"}],"2":[{"start":"2016-06-13T14:30:00Z","end":"2016-06-13T23:00:00Z"}],"3":[{"start":"2016-06-13T14:30:00Z","end":"2016-06-13T23:00:00Z"}],"4":[{"start":"2016-06-13T14:30:00Z","end":"2016-06-13T23:00:00Z"}],"5":[{"start":"2016-06-13T14:30:00Z","end":"2016-06-13T23:00:00Z"}],"6":[],"7":[]}},"links":{"self":"https://api.oregonstate.edu/v1/locations/0da1b065ef522bc542fa2bc07efd2ad0"}}]}'

document = JSON::Api.parse(EXAMPLE_LOCATION_JSON)
puts "name: #{ document["data"][0]["attributes"]["name"] }"
