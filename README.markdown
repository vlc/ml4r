# Machine Learning 4 Ruby #

## Description ##

A Ruby based library of Maching Learning algorithms.

Supports:

* Least Squares Linear Regression

## Installation ##
    gem install ml4r

## Requirements ##
    The entire library is built using SWIG and as such it requires the boost c++ library.
    To install on ubuntu / debian distros
        sudo apt-get install

## Usage ##
    
    require 'ml4r'
    regression = Ml4r::OLSLinearRegression.new([1.0, 2.0, 3.0], [1.0, 2.0, 3.0])

    puts regression.getParameterEstimates()     # => [[0.9999999999999993], 1.5543122344752192e-15]
    puts regression.getRSquared()               # => 1.0
    puts regression.getFittedYs()               # => [1.0000000000000009,2.0,3.0]
    puts regression.getPredictedYs()            # => [1.0000000000000053,2.0,3.0]
    puts regression.getSSquared()               # => 7.888609052210118e-31
    puts regression.getFstatistic()             # => 2535301200456456600000000000000.0
    puts regression.getTstatistics()            # => [1592262918131443.0, 1.1456439237389604]
    puts regression.getStandardErrors()         # => [6.280369834735097e-16, 1.3567149463006932e-15]
    puts regression.getPressStatistic()         # => 2.8398992587956324e-29
    puts regression.getPresarStatistic()        # => 0.0
    puts regression.getAdjustedRSquared()       # => 1.0
    puts regression.getRSquaredPrediction()     # => 1.0