require 'optparse'
require 'libmediainfo'

module Libmediainfo
  class CLI
    module ClassMethods
      def puts(*args)
        @iostream.puts(*args)
      end

      def config
        @settings ||= Libmediainfo.config
      end

      def options
        @options ||= {
          :action => nil,
          :path   => nil
        }
      end

      def mandatory_options
        @mandatory_options ||= %w(  )
      end

      def missing_mandatory_options?
        mandatory_options.find do |option|
          options[option.to_sym].nil?
        end
      end

      def set_action(val)
        if options[:action]
          warn "Ignoring "
        else
          options[:action] = val
        end
      end

      def option_parser
        @option_parser ||= OptionParser.new do |opts|
          opts.banner = "usage: #{File.basename($0)} <action> [options]"
          opts.version = VERSION

          opts.separator ""
          opts.separator "  ACTIONS:"
          opts.on("-p", "--path PATH", String,
            "Scan a file and add it to the database."
            ) { |arg|
            options[:path] = arg
            set_action :scan
          }

          opts.on("--list x,y,z", Array,
            "Example 'list' of arguments") do |list|
            warn "type = #{list}"
          end
          opts.on("--type [TYPE]", [:text, :binary, :auto],
              "FIXME Select some type)") do |t|
            warn "type = #{t}"
          end
          opts.separator ""
          opts.separator "  SETTINGS:"

          opts.on("-f", "--foo PATH", String,
            "FIXME Some foo setting",
            "Default: #{config[:database]}"
            ) { |arg| config[:foo] = arg }
        end
      end

      def parse_arguments!
        option_parser.parse!(@arguments)

        if missing_mandatory_options?
          puts "ERROR: Missing required option!"
          puts opts
          exit(-1)
        end
      end

      def run_cli!
        options[:action] ||= :help
        exit(0)
      end

      def run_scan

      end

      def execute(cli_iostream, cli_arguments = [])
        @iostream  = cli_iostream
        @arguments = cli_arguments

        parse_arguments!
        run_cli!
      end
    end
    extend ClassMethods
  end
end
