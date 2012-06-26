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
    assert_equal("hello world", ::Ml4r.fact(5))
  end
end
