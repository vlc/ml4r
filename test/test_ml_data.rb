require 'test/unit'
require 'ml4r/ml4r'

class Array
  def to_double_vector
  	v = Ml4r::DoubleVector.new
  	self.each { |e| v << e }
  end
end

class TestMLData < Test::Unit::TestCase
  DELTA = 0.0001

  def test_everything
  	data = Ml4r::MLData.new
  	assert(data.is_a? Ml4r::MLData)
	
	xs = [[1.0,2.0],[2.0,3.0],[3.0,4.0]]
	data.setXs(xs);
	assert(xs == data.getXs(), "xs")
	
  	ys = [1,2,3]
	data.setYs(ys)
	assert(ys == data.getYs(), "ys")
	
  	#data.setXs([[1.0],[5.0]])
	#data.setYs([1,0])
	weights = [1,1,1]
	data.setWeights(weights)
	assert(weights == data.getWeights(), "weights")
	
	predictions = [0,0,0]
	data.setInitialPredictions(predictions)
	assert(predictions == data.getPredictions(), "predictions")
	
	names = ["a","b"]
	data.setFeatureNames(names)
	assert(names == data.getFeatureNames(), "feature names")
	
  end
end
