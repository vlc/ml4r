require 'test/unit'
require 'ml4r/ml4r'

class TestLinearRegression < Test::Unit::TestCase
  def test_getters_and_setters
  	lr = Ml4r::LinearRegression.new
  	assert_equal(42, lr.foo)
  end
end
