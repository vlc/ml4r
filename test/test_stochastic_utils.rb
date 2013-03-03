require 'test/unit'
require 'ml4r/ml4r'

class TestStochasticUtils < Test::Unit::TestCase
  def test_utils
    lr = Ml4r::OLSLinearRegression.new([[1.0],[2.0],[3.0]], [1.0, 2.0, 3.0])
    assert_kind_of(Ml4r::LinearRegression, lr)

    #cum_sum = Ml4r.convertPdfToCumulativeSum([1.0, 2.0, 3.0])
    #assert_equal([1.0, 3.0, 6.0], cum_sum)
  end
end