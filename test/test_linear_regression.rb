require 'test/unit'
require 'ml4r/ml4r'

class Array
  def to_double_vector
  	v = Ml4r::DoubleVector.new
  	self.each { |e| v << e }
  end
end

class TestLinearRegression < Test::Unit::TestCase
  DELTA = 0.0001

  def test_getters_and_setters
  	lr = Ml4r::OLSLinearRegression.new
  	assert(lr.is_a? Ml4r::LinearRegression)
  	lr.setXs([[1.0],[2.0],[3.0]]);
  	lr.setYs([1.0, 2.0, 3.0])
  	lr.Execute
    expected_predictions = [1,2,3]
  	expected_predictions.zip(lr.getPredictedYs.to_a).each { |exp,pred| assert_in_delta(exp, pred, DELTA, "Predicted Ys") }
  	est = lr.getParameterEstimates
    assert_in_delta(1.0, est.first.first, DELTA, "Factors")
    assert_in_delta(0.0, est.second,      DELTA, "Constant")
  end
end
