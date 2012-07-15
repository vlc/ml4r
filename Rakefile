require 'rake/testtask'
require 'rake/clean'
$: << 'tasks'
require 'gem_tasks'

NAME = 'ml4r'
EXT  = `uname` =~ /Linux/ ? "so" : "bundle"

def hack_wrapper_to_include_boost_earlier
  dat = IO.readlines("ml4r_wrap.cxx")
  File.open("ml4r_wrap.cxx", 'w') { |f| 
    f.puts "// Haack to make swig/rice/boost play nice together"
    f.puts "#include <boost/numeric/ublas/matrix.hpp>"
    dat.each { |line| f.puts line }
  }
end

# rule to build the extension: this says
# that the extension should be rebuilt
# after any change to the files in ext
file "lib/#{NAME}/#{NAME}.#{EXT}" => Dir.glob("ext/#{NAME}/*{.rb,.c,.cpp,.cxx,*.i,*.h}") do
  Dir.chdir("ext/#{NAME}") do
  	# Regenerate the c++ wrappers if the swig interface files have changed
    p `swig -ruby -c++ ml4r.i` 
  	hack_wrapper_to_include_boost_earlier()

    # this does essentially the same thing as what RubyGems does
    ruby "extconf.rb --with-boost-dir=/opt/local"
    sh "make"
  end
  cp "ext/#{NAME}/#{NAME}.#{EXT}", "lib/#{NAME}"
end

# make the :test task depend on the shared
# object, so it will be built automatically
# before running the tests
task :test => "lib/#{NAME}/#{NAME}.#{EXT}"

# use 'rake clean' and 'rake clobber' to
# easily delete generated files
CLEAN.include('ext/**/*{.o,.log,.so,.bundle}')
CLEAN.include('ext/**/Makefile')
CLOBBER.include("lib/**/*.#{EXT}")

# the same as before
Rake::TestTask.new do |t|
  t.libs << 'test'
end

desc "Run tests"
task :default => :test

desc "Build the gem"
task :build_gem do
  version = increment_gemspec_version
  `gem build ml4r.gemspec`
  `gem push ml4r-#{version}.gem`
end
