require 'test/unit'
p $:.inspect
#require 'ml4r'

#class TestCompilation < Test::Unit::TestCase
#  def test_hello_world
#    assert_equal("hello world", TestCppExtension.bonjour())
#  end
#end

require 'ml4r/example'

class TestCompilation < Test::Unit::TestCase
  def test_hello_world
    p Module.constants
    assert_equal("hello world", ::Example.fact(5))
  end
end
