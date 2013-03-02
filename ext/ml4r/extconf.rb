require 'mkmf'

$libs += " -lstdc++ " # --std=c++0x"
# $CPPFLAGS += " --std=c++0x"
CONFIG['warnflags'].gsub!('-Wdeclaration-after-statement',   '')
CONFIG['warnflags'].gsub!('-Wimplicit-function-declaration', '')

dir_config('boost')

$srcs = Dir.glob("**/*.c*")
$objs = $srcs.map { |e| e.gsub(/cpp$/, "o") }

create_makefile('ml4r/ml4r')