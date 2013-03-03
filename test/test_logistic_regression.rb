require 'test/unit'
require 'ml4r/ml4r'

# class Array
#   def to_double_vector
#   	v = Ml4r::DoubleVector.new
#   	self.each { |e| v << e }
#   end
# end

class TestLogisticRegression < Test::Unit::TestCase
  DELTA = 0.0001

  def setup
	@data = Ml4r::MLData.new
	
	xs = [[1.0,2.0],[1.0,3.0],[1.0,4.0],[1.0,5.0],[1.0,6.0]]
	ys = [    0,        0,        1,        0,         1]
	ws = [    1,        1,        1,        1,         1]
	
	xs.zip(ys).each { |x,y|
	  @data.addObservation(x,y)
	}
  end
  
  def test_everything
    lr = Ml4r::LogisticRegressionRunner.new
  	assert(lr.is_a?(Ml4r::LogisticRegressionRunner))
  	assert(lr.is_a?(Ml4r::MLRunner))
  	lr.setData(@data)
  	lr.execute
  	predictions = lr.getMeanTrainingPredictions
  	p predictions
  end
end
