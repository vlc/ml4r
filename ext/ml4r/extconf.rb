require 'mkmf'

$libs += " -lstdc++ "

# The following spam out the console with hard to read error messages, obscuring any real problems
CONFIG['warnflags'].gsub!('-Wdeclaration-after-statement',   '')
CONFIG['warnflags'].gsub!('-Wimplicit-function-declaration', '')
CONFIG['warnflags'].gsub!('-Wshorten-64-to-32', '')

dir_config('boost')

$srcs = Dir.glob("**/*.c*")
$objs = $srcs.map { |e| e.gsub(/cpp$/, "o") }

create_makefile('ml4r/ml4r')

dat = IO.readlines("Makefile")
File.open("Makefile", 'w') { |f|
  dat.each { |line| f.puts(line.gsub(/\$\</, "$< -o $@")) }
}
