# A simple hash to hold any configurable options, that is
# exposed in several ways for ease of use.
module Libmediainfo
  class Config
    # the name of the app, that should match
    # how it appears when run through the command line
    # @return [String] base name of the gem or app
    def app_name
      'libmediainfo'
    end

    # the top level directory that programs should write
    # user-level configuration information to. This conforms
    # to the XDG specification, and tries the directory named
    # in the environment variable XDG_CONFIG_DIR first, and
    # returning to ~/.config if otherwise.
    # @return [String] path to the config directory
    def xdg_config_dir
      ENV['XDG_CONFIG_DIR'] || "~/.config"
    end

    # our configuration directory, which is under xdg_config_dir
    # @return [String] path to this gem's config directory
    def config_dir
      @config_dir ||= File.expand_path("#{xdg_config_dir}/#{app_name}")
    end

    # Search for an exectuable, using the PATH environment variable
    # @param prog_name [String] 
    # @return [String] full, absolute path to the executable
    def find_in_path(prog_name)
      (ENV['PATH'] || '').split(':').each do |dir|
        path = "#{dir}/#{prog_name}"
        return path if File.executable? path
      end
      nil
    end

    # FIXME: an example referencing an external executable
    # @return [String] full-path to the executable
    def default_foo
      @default_foo ||= find_in_path 'foo'
    end

    # initial configuration values
    # @return [Hash] config initial values
    def default_attributes
      @default_attributes ||= { app_name: app_name, foo: default_foo }
    end

    # the hash that stores the configuraton options
    # @return [Hash] config storage
    def attributes
      @attributes ||= default_attributes
    end

    # enable: Libmediainfo.config[:option]
    # @param name [Symbol] name of a config option
    def [](name)
      attributes[name]
    end

    # enable: Libmediainfo.config[:option] = value
    # @param name [Symbol] name of a config option
    # @param value [Object] new value to set for the named config option
    def []=(name, value)
      attributes[name] = value
    end

    # enable: Libmediainfo::Config[:option]
    # @param name [Symbol] name of a config option
    def self.[](name)
      ::Libmediainfo.config[name]
    end

    # enable: Libmediainfo::Config[:option] = value
    # @param name [Symbol] name of a config option
    # @param value [Object] new value to set for the named config option
    def self.[]=(name, value)
      ::Libmediainfo.config[name] = value
    end
  end

  # provide easy-access to an instance of Config in the main gem scope
  # @return [Libmediainfo::Config] gem configuration instance
  def config
    @config ||= Config.new
  end

  module_function :config
end
