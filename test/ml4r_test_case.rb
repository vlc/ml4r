require 'test/unit'

class Ml4rTestCase < Test::Unit::TestCase
  def assert_in_delta(expected, actual, delta, message = nil)
    if expected.is_a?(Array)
      assert_kind_of(Array, actual)
      assert_equal(expected.size, actual.size)
      expected.zip(actual).each { |expected_element, actual_element|
        super(expected_element, actual_element, delta, message)
      }
    else
      super(expected, actual, delta, message)
    end
  end
end