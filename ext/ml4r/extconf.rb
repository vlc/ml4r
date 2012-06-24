require 'mkmf'

$libs += " -lstdc++ "
dir_config('boost')
create_makefile('ml4r/ml4r')