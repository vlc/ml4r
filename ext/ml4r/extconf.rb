require 'mkmf'

$libs += " -lstdc++ " # --std=c++0x"
# $CPPFLAGS += " --std=c++0x"

dir_config('boost')

$srcs = Dir.glob("**/*.c*")
$objs = $srcs.map { |e| e.gsub(/cpp$/, "o") }

create_makefile('ml4r/ml4r')

dat = IO.readlines("Makefile")
File.open("Makefile", 'w') { |f|
  dat.each { |line| f.puts(line.gsub(/\$\</, "$< -o $@")) }
}