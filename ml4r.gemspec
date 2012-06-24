Gem::Specification.new do |s|
  s.name        = 'ml4r'
  s.version     = '0.1.0'
  s.date        = '2012-06-24'
  s.summary     = "A ruby based library of Maching Learning (ML) algorithms"
  s.description = "A ruby based library of Maching Learning (ML) algorithms"
  s.authors     = ["Jamie Cook", "Tim Veitch"]
  s.email       = ['jamie@ieee.org', "tim.veitch@veitchlister.com.au"]
  s.files       = Dir.glob('lib/**/*.rb') +
            	    Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions = ['ext/ml4r/extconf.rb']                
  s.homepage    = 'https://github.com/vlc/ml4r'
end
