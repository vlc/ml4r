require 'mkmf'

$libs += " -lstdc++ --std=c++0x"
# $CPPFLAGS += " --std=c++0x"

dir_config('boost')
create_makefile('ml4r/ml4r')