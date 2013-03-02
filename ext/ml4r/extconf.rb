require 'mkmf'

$libs += " -lstdc++ "
CONFIG['warnflags'].gsub!('-Wdeclaration-after-statement',   '')
CONFIG['warnflags'].gsub!('-Wimplicit-function-declaration', '')

dir_config('boost')

$srcs = Dir.glob("**/*.c*")
$objs = $srcs.map { |e| e.gsub(/cpp$/, "o") }

create_makefile('ml4r/ml4r')

dat = IO.readlines("Makefile")
File.open("Makefile", 'w') { |f|
  dat.each { |line| f.puts(line.gsub(/\$\</, "$< -o $@")) }
}
