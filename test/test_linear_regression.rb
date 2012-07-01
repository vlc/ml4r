require 'test/unit'
require 'ml4r/ml4r'

class TestLinearRegression < Test::Unit::TestCase
  def test_getters_and_setters
  	lr = Ml4r::OLSLinearRegression.new
  	lr.setXs([[1,2][1,2]]);
  	# assert_equal(42, )
  end
end
