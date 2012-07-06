require 'test/unit'
require 'ml4r/ml4r'

class Array
  def to_double_vector
  	v = Ml4r::DoubleVector.new
  	self.each { |e| v << e }
  end
end

class TestLinearRegression < Test::Unit::TestCase
  def test_getters_and_setters
  	lr = Ml4r::OLSLinearRegression.new
  	assert(lr.is_a? Ml4r::LinearRegression)
  	lr.setXs([[1.0],[2.0],[3.0]]);
  	lr.setYs([1.0, 2.0, 3.0])
  	lr.Execute
  	p lr.getPredictedYs
  	est = lr.getParameterEstimates
    p est.first
    p est.second
  	# assert_equal(42, )
  end
end
