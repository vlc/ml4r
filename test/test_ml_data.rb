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
	
	observations = [
	  [[1.0,2.0],1.0],
	  [[2.0,3.0],2.0],
	  [[3.0,4.0],3.0]
	 ]
	
	observations.each { |obs|  
		p obs.first
		p obs.last
		data.addObservation(obs.first, obs.last)	
	}
	
	assert(data.getXs().size == 3, "array")
	assert(observations.map(&:first) == data.getXs(), "xs")
	
  	ys = [1,2,3]
	
	assert(observations.map(&:last) == data.getYs(), "ys")
	
	weights = [4,5,6]
	predictions = [2,2,2]
	weights.each_with_index { |w, index|
		observations[index] += [w, predictions[index]]
	}
	
	observations.each { |obs| data.addObservation(*obs) }
	
	
	assert([1,1,1] + weights == data.getWeights(), "weights")
	
	assert([0,0,0] + predictions == data.getPredictions(), "predictions")
	
	names = ["a","b"]
	data.setFeatureNames(names)
	assert(names == data.getFeatureNames(), "feature names")
	
  end
end
