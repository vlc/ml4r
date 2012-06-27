require 'test/unit'
p $:.inspect
#require 'ml4r'

#class TestCompilation < Test::Unit::TestCase
#  def test_hello_world
#    assert_equal("hello world", TestCppExtension.bonjour())
#  end
#end

#require 'ml4r/example'
require 'ml4r/ml4r'

class TestCompilation < Test::Unit::TestCase
  def test_hello_world
  	p Ml4r.methods - Object.methods
  	p Module.constants.grep(/Lin/)
  	p Ml4r::Jamie.methods
    # assert_equal("hello world", ::Ml4r.fact(5))
    assert_equal(42, Ml4r::Jamie.new.foo)
  end
end
