require 'test/unit'
require 'ml4r'

class TestCompilation < Test::Unit::TestCase
  def test_hello_world
    assert_equal("hello world", TestCppExtension.bonjour())
  end
end