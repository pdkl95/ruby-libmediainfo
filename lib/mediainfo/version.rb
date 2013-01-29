class MediaInfo
  # Probably overkill, but why. This is all a thin wrapper around
  # the string in VERSION::STRING
  module VERSION
    extend Comparable

    # The major version number of the gem. Anything that depends on
    # on this gem is likely to break when this changes.
    MAJOR     = 1

    # The minor version number. Changes here could break external
    # dependencies on rare occasions, but effort will be taken to
    # avoid that. New features usually increment this value.
    MINOR     = 0

    # The patch-level version number. Changes here should never
    # break anything, and usually represent bugfixes.
    PATCH     = 1

    # The whole version number, in array form.
    SIGNATURE = [MAJOR, MINOR, PATCH]

    # The whole version number, in gemspec-compatible string form.
    # When in doubt, you probably want this version.
    STRING    = SIGNATURE.join '.'

    # the VERSION module itself
    # @return [String] the version number
    def inspect
      STRING.inspect
    end
    alias_method :to_s, :inspect

    # @param other [Types] description
    # @return [-1] If 'self' sorts before 'other'
    # @return  [0] If 'self' sorts equal with 'other'
    # @return  [1] If 'self' sorts after 'other'
    def <=>(other)
      other = other.split('.').map { |i| i.to_i } if other.respond_to? :split
      SIGNATURE <=> Array(other)
    end

    # @!group Delegating Into STRING

    # @return [Int] is a thin wrapper around String
    def hash
      STRING.hash
    end

    # @param meth [Symbol] a method name
    # @return [Boolean]
    def respond_to?(meth, *)
      meth.to_s !~ /^__|^to_str$/ and STRING.respond_to? meth unless super
    end

    def method_missing(meth, *args, &block)
      return super unless STRING.respond_to?(meth)
      STRING.send(meth, *args, &block)
    end

    # @!endgroup

    module_function :hash, :<=>, :inspect, :to_s, :respond_to?, :method_missing
  end

  # provides easy-access to VERSION in the main gem scope
  # @return [Libmediainfo::VERSION] the main VERSION
  def version
    VERSION
  end
end
