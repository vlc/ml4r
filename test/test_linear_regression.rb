require 'test/unit'
require 'ml4r/ml4r'

class TestLinearRegression < Test::Unit::TestCase
  DELTA = 0.0001

  def test_simple_regression
    lr = Ml4r::OLSLinearRegression.new([[1.0],[2.0],[3.0]], [1.0, 2.0, 3.0])
    assert_kind_of(Ml4r::LinearRegression, lr)
    expected_predictions = [1,2,3]
  	expected_predictions.zip(lr.getPredictedYs.to_a).each { |exp,pred| assert_in_delta(exp, pred, DELTA, "Predicted Ys") }
  	est = lr.getParameterEstimates
    assert_in_delta(1.0, est.first.first, DELTA, "Factors")
    assert_in_delta(0.0, est.second,      DELTA, "Constant")
  end

  def test_weighted_regression
    lr = Ml4r::OLSLinearRegression.new([[4.0],[2.0],[3.0], [4.0]], [1.0, 2.0, 3.0, 4.0], [0.0, 1.0, 1.0, 1.0])
    est = lr.getParameterEstimates
    assert_in_delta(1.0, est.first.first, DELTA, "Factors")
    assert_in_delta(0.0, est.second,      DELTA, "Constant")
  end
end
