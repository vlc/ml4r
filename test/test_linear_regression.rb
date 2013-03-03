require 'ml4r_test_case'
require 'ml4r/ml4r'
require 'ml4r/linear_regression'

class TestLinearRegression < Ml4rTestCase
  DELTA = 0.0001

  def test_simple_regression
    lr = Ml4r::OLSLinearRegression.new([[1.0],[2.0],[3.0]], [1.0, 2.0, 3.0])
    assert_kind_of(Ml4r::LinearRegression, lr)

    parameters = lr.getParameterEstimates
    assert_in_delta(1.0, parameters.first.first, DELTA, "Factors")
    assert_in_delta(0.0, parameters.second,      DELTA, "Constant")

    assert_in_delta([1,2,3], lr.getPredictedYs.to_a, DELTA, "Predicted Ys")

    assert_in_delta(1.0, lr.getRSquared, DELTA)
  end

  def test_fixed_constant
    fixed_constant = 0.1
    lr = Ml4r::OLSLinearRegression.new([[1.0],[2.0],[3.0]], [1.0, 2.0, 3.0], fixed_constant)
    params = lr.getParameterEstimates
    assert_equal(fixed_constant, params.last)
    assert_operator params.first.first, :<, 1.0
  end

  def test_weighted_regression
    lr = Ml4r::OLSLinearRegression.new([[4.0],[2.0],[3.0], [4.0]], [1.0, 2.0, 3.0, 4.0], [0.0, 1.0, 1.0, 1.0])
    est = lr.getParameterEstimates
    assert_in_delta(1.0, est.first.first, DELTA, "Factors")
    assert_in_delta(0.0, est.second,      DELTA, "Constant")
  end

  def test_multi_dimensional_regression
    lr = Ml4r::OLSLinearRegression.new([[1.2, 1.0],[2.0, 1.8],[3.0, 3.2], [4.0, 4.4]], [1.1, 1.9, 3.1, 4.2])
    params = lr.getParameterEstimates
    assert_kind_of(Array, params)
    assert_equal(2, params.size)
    assert_kind_of(Array, params.first)
    assert_kind_of(Float, params.last)
    assert_in_delta([0.5, 0.5], params.first, DELTA)
  end

  def t_est_benchmark
    require "linefit"
    require 'benchmark'
    xs = (1..100).map(&:to_f).to_a
    ys = xs.map { |x| x + rand(3).to_f }

    Benchmark.bm do |x|
      n = 300
      line_fit_time = x.report('linefit') {
        lineFit = LineFit.new
        1.upto(n) { |i|
          lineFit.setData(xs,ys)
          intercept, slope = lineFit.coefficients
        }
      }

      ml4r_time = x.report('   ML4R') {
        1.upto(n) {
          lr = Ml4r::OLSLinearRegression.new(xs,ys)
          lr_slope, lr_intercept = lr.getParameterEstimates
        }
      }

      # Should be faster than the linefit gem :)
      assert_operator ml4r_time.utime, :<, line_fit_time.utime
    end
  end
end
