def increment_gemspec_version
  dat = IO.readlines("ml4r.gemspec")
  version = nil
  File.open("ml4r.gemspec", 'w') { |f| 
    dat.each { |line|
      if line =~ /s.version     = '(\d+).(\d+).(\d+)'/
        version = "#{$1}.#{$2}.#{$3.to_i+1}"
        f.puts(  "  s.version     = '#{version}'")
      else
        f.puts(line)
      end
    }
  }
  return version
end