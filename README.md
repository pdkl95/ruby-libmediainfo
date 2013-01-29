# libmediainfo

Ruby bindings for [mediainfo][] that direct use `libmediainfo.so`
instead of parsing the command-line utility.

[mediainfo]: http://mediainfo.sourceforge.net

* [Homepage](https://github.com/pdkl95/ruby-libmediainfo#readme)
* [Issues](https://github.com/pdkl95/ruby-libmediainfo/issues)
* [Documentation](http://rubydoc.info/gems/libmediainfo/frames)
* [Email](mailto:pdkl95@thoughtnoise.net)


# Requirements

* [libmediainfo.so][] itself and any of its requirements (`libzen`)
* `MediaInfoDLL.h` which should come with the library, but may
require installing an additional "`-dev`" package if you use the
pre-compiled package available for some distros.

[libmediainfo]: http://mediainfo.sourceforge.net/en/Download

# Installation

```bash
gem install libmediainfo
```

# Usage

See the [examples][] directory. I'll write some proper documentation
some day.

# Copyright

Copyright 2013 Brent Sanders

See LICENSE.txt for details.
