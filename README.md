# libmediainfo
==============

TODO: Description

* [Homepage](https://github.com/pdkl95/libmediainfo#readme)
* [Issues](https://github.com/pdkl95/libmediainfo/issues)
* [Documentation](http://rubydoc.info/gems/libmediainfo/frames)
* [Email](mailto:pdkl95@thoughtnoise.net)

Installation
------------

Add this to your application's Gemfile:

```ruby
gem 'mediainfo'
```

and then run:

```bash
bundle
```

Or install it manually with:

```bash
gem install libmediainfo
```

Usage
-----

```ruby
require 'mediainfo'

# load the media file for inspection
mi = MediaInfo.new("/some/file/to/examine.mkv")

# returns the brief text report, similar to what
# you get from `/usr/bin/mediainfo` by default
puts mi.simple_report



```

FIXME

Configuration
-------------

none, for the moment...

Requirements
------------

As this gem is a wrapper around it, the [libmediainfo][] library must
be installed. There are several versions listed on the website,
including various user-interfaces that are not necessary. What this
gem requires is the "SO"/"DLL" package, which provides the programming
interface. At the time of this writing, the needed file
was [MediaInfo_DLL_0.7.60_GNU_FromSource.tar.bz2][].

[libmediainfo][]: http://mediainfo.sourceforge.net/
[MediaInfo_DLL_0.7.60_GNU_FromSource.tar.bz2][]: http://mediaarea.net/download/binary/libmediainfo0/0.7.60/MediaInfo_DLL_0.7.60_GNU_FromSource.tar.bz2

Copyright
---------

Copyright (c) 2012 Brent Sanders

See LICENSE.txt for details.
