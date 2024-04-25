#!/usr/bin/ruby
# -----------------------------------------------------------------------------
#
# Test regenerator
#
# Re-populates folder if changes are so huge that all tests need to be regen
# Use wisely
#
# run: genansws.rb -f ./build/apps/allperms -t ./tests/allperms/
#
# -----------------------------------------------------------------------------

require 'fileutils'
require 'open3'
require 'optparse'

ARGV << '-h' if ARGV.empty?

def parse_options
  options = {}
  options[:addargs] = ""

  OptionParser.new do |opts|
    opts.banner = "Usage: genansws.rb [options]"

    opts.on("-a", "--addargs ARGS", "Additional arguments to pass to testing program.") do |v|
      options[:addargs] << " #{v}"
    end

    opts.on("-f", "--filename ARGS", "Name of program to run.") do |v|
      options[:filename] = v
    end

    opts.on("-t", "--testdir ARGS", "Name of folder to regenerate.") do |v|
      options[:topdir] = v
    end

    opts.on("-v", "--[no-]verbose", "Run verbosely. No default value.") do |v|
      options[:verbose] = v
    end

    opts.on("-h", "--help", "Prints this help.") do
      puts opts
      exit
    end
  end.parse!

  if options[:filename] == nil
    puts "You need to specify --filename to run test"
    exit 1
  end

  if options[:topdir] == nil
    puts "You need to specify --testdir to run test"
    exit 1
  end

  p options if options[:verbose]
  options
end


def main
  options = parse_options

  dat = Dir["#{options[:topdir]}/*.dat"]

  raise "You need some dat files in #{options[:topdir]} folder" if dat.nil?

  dat.each do |s| 
    outf = "#{File.dirname(s)}/#{File.basename(s, ".dat")}.ans"
    puts "generating #{outf} from #{s}"
    system(%("#{options[:filename]}" #{options[:addargs]} < #{s} > #{outf}))
  end
end

main