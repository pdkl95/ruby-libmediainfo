module Libmediainfo
end

[ 'config',
  'version'
].each do |lib|
  require_relative "libmediainfo/#{lib}"
end
