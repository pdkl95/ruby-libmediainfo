module MediaInfo
end

[ 'version'
].each do |lib|
  require_relative "mediainfo/#{lib}"
end
