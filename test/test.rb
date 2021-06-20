#!/usr/bin/env ruby

require "Gaussian"

# scipy.ndimage.gaussian_filter1d のサンプルで検証。

files = "scipy.ndimage.gaussian_filter1d.example.%s.bin"

open(files % "x", "rb") do |f|
    x = f.read.unpack("d*")
    y3, y6 = Gaussian.filter1d([x, x], [3, 6])

    open(files % "y3", "rb") do |fy3|
        sciy3 = fy3.read.unpack("d*")
        puts [y3, sciy3].transpose.map{|e| e.join " "}
    end
    
    open(files % "y6", "rb") do |fy6|
        sciy6 = fy6.read.unpack("d*")
        puts [y6, sciy6].transpose.map{|e| e.join " "}
    end
end