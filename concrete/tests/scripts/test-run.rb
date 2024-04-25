#!/usr/bin/ruby
# -----------------------------------------------------------------------------
#
# Test runner
#
# Runs test with specified input and compares output with expected output
#
# run: test-run.rb --testfile=test.x --infile=001.dat --etafile=001.ans
#
# -----------------------------------------------------------------------------

require 'open3'
require 'optparse'
require 'tempfile'

ARGV << '-h' if ARGV.empty?

class TestRunner 
  def initialize(testf, inf, etaf, addargs)
    @testfile = testf
    @infile = inf
    @etafile = etaf
    @addargs = addargs

    if not File.file?(testf)
      puts "Cannot find program under test: #{testf}"
      exit 1
    end

    if not File.file?(inf)
      puts "Cannot find input file: #{inf}"
      exit 1
    end
    if not File.file?(etaf)
      puts "Cannot find etalon file: #{etaf}"
      exit 1
    end
  end

  def run
    tfile = Tempfile.new
    tfile.close
    result = system(%("#{@testfile}" #{@addargs} < "#{@infile}" > "#{tfile.path}"))
    if result != true
      p "command <#{@testfile} < #{@infile} > #{tfile.path}> failed"
      exit 1
    end
    result = system(%("diff" -s -w "#{@etafile}" "#{tfile.path}" > /dev/null))
    if result.nil?
      p "command <diff -s -w #{@etafile} #{tfile.path} > /dev/null> failed"
      exit 1
    end
    if result != true
      # need to materialize temp file here
      permpath = "#{File.basename(@etafile,'.*')}.out"
      FileUtils.cp(tfile.path, permpath)
      p "files #{@etafile} and #{permpath} differ"
      exit 1
    end
    tfile.unlink
  end
end

def parse_options
  options = {}
  options[:addargs] = ""

  OptionParser.new do |opts|
    opts.banner = "Usage: test-run.rb [options]"

    opts.on("-a", "--addargs ARGS", "Additional arguments to pass to testing program.") do |v|
      options[:addargs] << " #{v}"
    end

    opts.on("-e", "--etafile PATH", "Full path to etalon result.") do |v|
      options[:etafile] = v
    end

    opts.on("-f", "--folder PATH", "Full path to folder with tests.") do |v|
      options[:folder] = v
    end

    opts.on("-i", "--infile PATH", "Full path to single test input file.") do |v|
      options[:infile] = v
    end

    opts.on("-t", "--testfile PATH", "Full path to program under test.") do |v|
      options[:testfile] = v
    end

    opts.on("-v", "--[no-]verbose", "Run verbosely. No default value.") do |v|
      options[:verbose] = v
    end

    opts.on("-h", "--help", "Prints this help.") do
      puts opts
      exit
    end
  end.parse!

  bothfiles = !(options[:infile].nil? or options[:etafile].nil?)
  folder = !options[:folder].nil?

  if options[:testfile] == nil or !(bothfiles or folder)
    puts "You need to specify --testfile to run test"
    puts "You also need either --folder or both --infile and --testfile"
    exit 1
  end

  p options if options[:verbose]
  options
end

# --------------------------------- Main program -------------------------------

def main
  options = parse_options
  if (options[:folder].nil?)
    testrun = TestRunner.new(options[:testfile], options[:infile], options[:etafile], options[:addargs])
    testrun.run
    exit 0
  end

  # folder mode
  if not File.directory?(options[:folder])
    puts "Cannot find test folder: #{inf}"
    exit 1
  end

  Dir.foreach(options[:folder]) do |filename|
    next if filename == '.' or filename == '..'
    re = '(\w+).dat'
    matches = filename.match(re).to_a
    next if matches.empty?
    basename = "#{options[:folder]}/#{matches[1]}"
    if not File.file?("#{basename}.ans")
      puts "Cannot find answer file for: #{basename}"
      exit 1
    end
    puts "Running for #{basename}"
    testrun = TestRunner.new(options[:testfile], "#{basename}.dat", "#{basename}.ans", options[:addargs])
    testrun.run
  end
end

main
